#ifndef FALLING_BLOCK_H
#define FALLING_BLOCK_H
#include"GetRandomNumber.h"
#include"SFML/System/Vector2.hpp"
#include"constants.h"
#include<functional>
#include"Map.h"

class FallingBlock
{
	static void generate_vertical_block(Map* map);
	static void generate_horizontal_block(Map* map);
public:
	FallingBlock();
	~FallingBlock();

	static void generate(Map* map);
};


#endif