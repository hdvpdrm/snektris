#ifndef ZIG_ZAG_BLOCK_HPP
#define ZIG_ZAG_BLOCK_HPP
#include"BaseTetrisBlock.hpp"
class ZigZagBlock : public TetrisBLock
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
			[](sf::Vector2u pos1,
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
			if (left == poses.end()) //strange
				return sf::Vector2u(-1, -1);
		}

		return *left;
	}
	virtual bool can_move(Map* map)
	{
		if (being_eaten) return false;

		vector<sf::Vector2u> last_els = { find_last(),find_lefter_last() };

		for (auto& last : last_els)
		{
			if (last == sf::Vector2u(-1, -1))break;

			auto down = sf::Vector2u(last.x, last.y + 1);
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



#endif