#ifndef FALLING_BLOCK_H
#define FALLING_BLOCK_H
#include"GetRandomNumber.h"
#include"SFML/System/Vector2.hpp"
#include"constants.h"
#include<iostream>
#include<vector>

static const sf::Vector2i DEAD_BLOCK = sf::Vector2i(-1, -1);
class FallingBlock
{
private:
	void generate_vertical_block();
	std::vector<sf::Vector2u> poses;
	short del_counter = 0;

	bool can_move = true;
public:
	FallingBlock();
	~FallingBlock();

	void fall();
	std::vector<sf::Vector2u> get_poses() { return poses; }
	void del(int i)
	{
		poses.erase(poses.begin() + i);
	}
	bool time_to_die() { return del_counter == 3; }
	void set_move_flag(bool flag) { can_move = flag; }
};


#endif