#ifndef HORIZONTAL_BLOCK_HPP
#define HORIZONTAL_BLOCK_HPP
#include"BaseTetrisBlock.hpp"
class HorizontalBlock : public TetrisBLock
{
private:
	virtual bool can_move(Map* map)
	{
		for (auto& pos : poses)
		{
			if (being_eaten) return false;

			auto down = sf::Vector2u(pos.x, pos.y + 1);
			if (down.y != CELL_MAX - 1)
			{
				if (map->is_apple(down.x, down.y) and win())
				{
					reached_the_top = true;
					return false;
				}
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
	HorizontalBlock(const sf::Color& color_to_eat) :TetrisBLock(color_to_eat)
	{
		generate();
	}
	~HorizontalBlock() {}
};


#endif
