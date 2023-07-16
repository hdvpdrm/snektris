#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H
#include<vector>
#include"SFML/System/Vector2.hpp"
#include"Map.h"
#include"GetRandomNumber.h"
#include<functional>
#include<iostream>
#include<algorithm>
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

	bool being_eaten = false;
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
				being_eaten = true;
				it = poses.erase(it);
				return true;
			}
		}

		being_eaten = false;
		return false;
	}
	bool should_die() { return poses.empty(); }
};

class HorizontalBlock : public TetrisBLock
{
private:
	virtual bool can_move(Map* map)
	{
		for (auto& pos : poses)
		{
			if (being_eaten) return false;
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
		if (being_eaten) return false;
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
class ZigZagBlock: public TetrisBLock
{
protected:
	bool is_last(const sf::Vector2u& pos1,
		const sf::Vector2u& pos2)
	{
		return pos1.y > pos2.y;
	}
	sf::Vector2u find_last()
	{
		return  *std::max_element(poses.begin(), poses.end(),
			[]( sf::Vector2u pos1,
				sf::Vector2u pos2)
			{
				return pos1.y < pos2.y;
			});
	}
	sf::Vector2u find_lefter_last()
	{
		auto right_last = find_last();
		auto left_last = sf::Vector2u(right_last.x - 1, right_last.y - 1);

		auto left = std::find(poses.begin(), poses.end(), left_last);
		if (left == poses.end())
			return sf::Vector2u(-1, -1);
		else return *left;
	}
	virtual bool can_move(Map* map)
	{
		if (being_eaten) return false;
		if (check_intersects_snake(map))return false;

		vector<sf::Vector2u> last_els = { find_last(),find_lefter_last()};

		for (auto& last : last_els)
		{
			if (last == sf::Vector2u(-1, -1))break;

			auto down = sf::Vector2u(last.x, last.y + 1);
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
		auto x1 = get_random_int(0, CELL_MAX - 4);
		auto x2 = x1 + 1;

		for (int y = 0; y < 2; y++)
		{
			poses.push_back(sf::Vector2u(x1, y));
		}

		for (int y = 0; y < 2; y++)
		{
			poses.push_back(sf::Vector2u(x2, poses[0].y + 1 + y));
		}
	}
public:
	ZigZagBlock()
	{
		generate();
	}
	virtual ~ZigZagBlock()
	{
	}
};
class SquareBlock : public TetrisBLock
{
private:
	vector<sf::Vector2u> find_last()
	{
		vector<sf::Vector2u> sorted(poses);
		std::sort(sorted.begin(), sorted.end(), [](sf::Vector2u pos1,
			sf::Vector2u pos2) {return pos1.y > pos2.y; });

		vector<sf::Vector2u> last;
		last.push_back(sorted[0]);
		if (sorted.size() > 1)
		{
			if (sorted[1].y == sorted[0].y)last.push_back(sorted[1]);
		}
		return last;
	}
	bool can_move(Map* map)
	{
		if (being_eaten) return false;
		if (check_intersects_snake(map))return false;

		auto last = find_last();
		
		for (auto& pos : last)
		{
			auto down = sf::Vector2u(pos.x, pos.y + 1);
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
		auto x = get_random_int(0, CELL_MAX - 4);
		for (int y = 0; y < 2; y++)
		{
			poses.push_back(sf::Vector2u(x, y));
			poses.push_back(sf::Vector2u(x+1, y));
		}
	}
public:
	SquareBlock() 
	{
		generate();
	}
	~SquareBlock()
	{}
};

static const std::vector<std::function<TetrisBLock* ()>> generators =
{
	[]() { return new HorizontalBlock; },
	[]() { return new VerticalBlock; },
	[]() { return new ZigZagBlock; },
	[]() { return new SquareBlock; }
};
static TetrisBLock* generate_tetris_block()
{
	int type = get_random_int(0, generators.size()-1);
	return generators[type]();
}
#endif