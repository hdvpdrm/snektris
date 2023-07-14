#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H
#include<vector>
#include"SFML/System/Vector2.hpp"
#include"Map.h"
#include"GetRandomNumber.h"
#include<functional>
#include<iostream>
class TetrisBLock
{
protected:
	std::vector<sf::Vector2u> poses;
	virtual bool can_move(Map* map) = 0;
	
	virtual void update()
	{
		for (int i = 0; i < poses.size(); i++)
			poses[i].y += 1;
	}
	virtual void clear(Map* map)
	{
		for (auto& pos : poses)
			map->set_element(pos.x, pos.y, GameState(State::none));
	}
	virtual void set(Map* map)
	{
		for (auto& pos : poses)
			map->set_element(pos.x, pos.y, GameState(State::apple));
	}

	bool check_intersects_snake(Map* map)
	{
		for (auto& pos : poses)
		{
			if (map->is_snake_at_pos(pos.x, pos.y))return true;
		}
		return false;
	}
public:
	TetrisBLock(){}
	virtual ~TetrisBLock(){}

	void move(Map* map)
	{
		if (can_move(map))
		{
			clear(map);
			update();
			set(map);
		}
	}
	virtual void generate() = 0;

	bool is_eaten(const sf::Vector2u& snake_head)
	{
		for (auto it = poses.begin(); it != poses.end(); ++it)
		{
			if ((*it) == snake_head)
			{
				it = poses.erase(it);
				return true;
			}
		}
		return false;
	}
	bool should_die() { return poses.empty(); }
};

class HorizontalBlock : public TetrisBLock
{
private:
	bool can_move(Map* map)
	{
		for (auto& pos : poses)
		{
			if (check_intersects_snake(map))return false;

			auto down = sf::Vector2u(pos.x,pos.y+1);
			if (down.y != CELL_MAX - 1)
			{
				if (!map->is_empty(down.x, down.y))
				{
					return false;
				}
			}
			else return false;
		}
		return true;
	}
	void generate()
	{
		auto start_x = get_random_int(0, CELL_MAX - 8);

		for (int x = 0; x < 4; x++)
		{
			poses.push_back(sf::Vector2u(x + start_x, 0));
		}
	}
public:
	HorizontalBlock()
	{
		generate();
	}
	~HorizontalBlock(){}
};
class VerticalBlock : public TetrisBLock
{
private:

	sf::Vector2u find_last()
	{
		if (poses.size() == 1)return poses[0];
		else
		{
			if (poses[0].y > poses[1].y)return poses[0];
			else return poses[1];
		}
	}
	bool can_move(Map* map)
	{
		if (check_intersects_snake(map))return false;

		auto last = find_last();
		auto down = sf::Vector2u(last.x, last.y + 1);
		if (down.y != CELL_MAX - 1)
		{
			if (!map->is_empty(down.x, down.y))
			{
				return false;
			}
			else return true;
		}
		else return false;
	}
	void generate()
	{
		auto x = get_random_int(0, CELL_MAX-1);

		for (int y = 0; y < 2; y++)
		{
			poses.push_back(sf::Vector2u(x, y));
		}
	}
public:
	VerticalBlock() { generate(); }
	~VerticalBlock(){}

};

static const std::vector<std::function<TetrisBLock* ()>> generators =
{
	[]() { return new HorizontalBlock; },
	[]() { return new VerticalBlock; }
};
static TetrisBLock* generate_tetris_block()
{
	int type = get_random_int(0, generators.size()-1);
	return generators[type]();
}
#endif