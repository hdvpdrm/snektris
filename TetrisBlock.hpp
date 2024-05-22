#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H
#include"BaseTetrisBlock.hpp"
#include"SquareBlock.hpp"
#include"VerticalBlock.hpp"
#include"HorizontalBlock.hpp"
#include"ZigZagBlock.hpp"

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
