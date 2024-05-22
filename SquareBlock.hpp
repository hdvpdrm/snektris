#ifndef SQUARE_BLOCK_HPP
#define SQUARE_BLOCK_HPP
#include"BaseTetrisBlock.hpp"

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

		auto last = find_last();

		for (auto& pos : last)
		{
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
		auto x = get_random_int(0, CELL_MAX - 4);
		for (int y = 0; y < 2; y++)
		{
			poses.push_back(sf::Vector2u(x, y));
			poses.push_back(sf::Vector2u(x + 1, y));
		}
	}
public:
	SquareBlock(const sf::Color& color_to_eat) :TetrisBLock(color_to_eat)
	{
		generate();
	}
	~SquareBlock()
	{}
};
#endif