#include"FallingBlock.h"

FallingBlock::FallingBlock()
{

}
FallingBlock::~FallingBlock()
{
}
void FallingBlock::generate_vertical_block(Map* map)
{
	//first of all choose start x position, meanwhile y is always out of matrix
	auto x = get_random_int(0, CELL_MAX);

	for (int y = 0; y < 2; y++)
	{
		map->set_element(x,y,GameState(State::apple));
	}	
}
void FallingBlock::generate_horizontal_block(Map* map)
{
	auto start_x = get_random_int(0, CELL_MAX-5);

	for (int x = 0; x < 4; x++)
	{
		map->set_element(x + start_x, 0, GameState(State::apple));
	}
}
void FallingBlock::generate(Map* map)
{
	std::vector < std::function<void(Map* map)>> generators =
	{
		[](Map* map) { generate_vertical_block(map); },
		[](Map* map) {generate_horizontal_block(map); }
	};

	auto i = get_random_int(0, generators.size() - 1);
	generators[i](map);
}