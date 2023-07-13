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
	if (can_move)
	{
		int i = 0;
		while (i < poses.size())
		{
			poses[i].y += 1;
			i++;
		}
	}
	else
	{
		int i = 0;
		while (i < poses.size())
		{
			if (i + 1 < poses.size())
			{
				int dist = (int)poses[i].y - (int)poses[i+1].y;
				if (dist > 1)
					poses[i + 1].y += 1;
			}
			i++;
		}
	}
}