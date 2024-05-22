#ifndef VERTICAL_BLOCK_HPP
#define VERTICAL_BLOCK_HPP
#include"BaseTetrisBlock.hpp"
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

		auto last = find_last();
		auto down = sf::Vector2u(last.x, last.y + 1);
		if (down.y != CELL_MAX - 1)
		{
			if (map->is_apple(down.x, down.y) and win() and !map->is_snake_at_pos(down.x, down.y))
			{
				reached_the_top = true;
				return false;
			}

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
		auto x = get_random_int(0, CELL_MAX - 1);

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
	~VerticalBlock() {}

};


#endif