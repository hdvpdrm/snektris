#include"FallingBlock.h"

FallingBlock::FallingBlock()
{
	generate_vertical_block();
}
FallingBlock::~FallingBlock()
{
}
void FallingBlock::generate_vertical_block()
{
	//first of all choose start x position, meanwhile y is always out of matrix
	auto x = get_random_int(0, CELL_MAX);

	int i = 0;
	for (int y = 0; y > -4; --y)
	{
		poses.push_back(sf::Vector2u(x, y));
		++i;
	}
	
}
void FallingBlock::fall()
{
	for (int i = 0;i<poses.size();++i)
	{
		if ((*(--poses.end())).y + 1 != 27)
		{
			poses[i].y += 1;
		}
	}
}
