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
			map->set_element(pos.x, pos.y, GameState(cur_state));
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
	bool eatable = false;
private:
	State cur_state;
	State choose_type(const sf::Color& color)
	{
		if (color == sf::Color::Green)   return State::green_apple;
		if (color == sf::Color::Yellow)  return State::yellow_apple;
		if (color == sf::Color::Magenta) return State::magenta_apple;
		if (color == sf::Color::Red)     return State::red_apple;
	}
public:
	TetrisBLock(const sf::Color& color_to_eat)
	{
		int rand_n = get_random_int(0, 6);
		eatable = true ? rand_n > 3: false;
		if (eatable)
		{
			cur_state = choose_type(color_to_eat);
		}
		else
		{
			//exlude color_to_eat
			vector<sf::Color> colors_to_move;
			std::copy_if(colors.begin(), colors.end(), back_inserter(colors_to_move),
				[&](sf::Color color) { return color != color_to_eat; });

			int rand_n = get_random_int(0, colors_to_move.size() - 1);
			cur_state = choose_type(colors_to_move[rand_n]);
		}
		
	}
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
	bool is_eatable() { return eatable; }
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
	HorizontalBlock(const sf::Color& color_to_eat):TetrisBLock(color_to_eat)
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
	VerticalBlock(const sf::Color& color_to_eat) :TetrisBLock(color_to_eat)
	{ 
		generate(); 
	}
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
		{
			left_last = sf::Vector2u(right_last.x - 1, right_last.y);
			left = std::find(poses.begin(), poses.end(), left_last);
			if (left == poses.end())
				return sf::Vector2u(-1, -1);
		}
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
	ZigZagBlock(const sf::Color& color_to_eat) :TetrisBLock(color_to_eat)
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
	SquareBlock(const sf::Color& color_to_eat):TetrisBLock(color_to_eat)
	{
		generate();
	}
	~SquareBlock()
	{}
};

static const std::vector<std::function<TetrisBLock*(const sf::Color& color_to_eat)>> generators =
{
	[](const sf::Color& color_to_eat) { return new HorizontalBlock(color_to_eat); },
	[](const sf::Color& color_to_eat) { return new VerticalBlock(color_to_eat); },
	[](const sf::Color& color_to_eat) { return new ZigZagBlock(color_to_eat); },
	[](const sf::Color& color_to_eat) { return new SquareBlock(color_to_eat); }
};
static TetrisBLock* generate_tetris_block(const sf::Color& color_to_eat)
{
	int type = get_random_int(0, generators.size()-1);
	return generators[type](color_to_eat);
}
#endif