#ifndef FALLING_BLOCK_H
#define FALLING_BLOCK_H
#include"GetRandomNumber.h"
#include<list>
#include"SFML/System/Vector2.hpp"
#include"constants.h"
#include<iostream>

static const sf::Vector2u DEAD_BLOCK = sf::Vector2u(-1, -1);
class FallingBlock
{
private:
	void generate_vertical_block();
	sf::Vector2u poses[4];
	short del_counter = 0;
public:
	FallingBlock();
	~FallingBlock();

	void fall();
	sf::Vector2u* get_poses() { return poses; }
	void del(int i)
	{
		poses[i] = DEAD_BLOCK;
		del_counter++;
	}
	bool time_to_die() { return del_counter == 3; }
};


#endif