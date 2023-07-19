#ifndef SNAKE_H
#define SNAKE_H
#include"SFML/System/Vector2.hpp"
#include"GetRandomNumber.h"
#include"MovebaleObject.h"

/*
	Snake is discrete object that is represented by
	different cells. Each cell has a life time and 
	it decreases every iteration of game cycle,
	so when it's life time reachs 0 it should be terminated.

	This is the mechanism of snake.

	Everytime when snake eats apple, it grows, so
	its length increases by 1 and life time increases with 1.
*/
class SnakePiece
{
private:
	unsigned short s; //piece's life time
public:
	SnakePiece(unsigned short s):s(s){}
	auto get_val() { return s; }
	bool should_terminate() { return s == 0; }
	void decrement() { --s; }
};

//this class implements functionality of snake
class Snake
{
private:
	int length = 2;
	sf::Vector2u head_pos;
	Direction dir;
	int score = 0;

	int apples_till_grow = 4;
public:
	Snake()
	{
		auto x = get_random_int(0, CELL_MAX - 1);
		auto y = get_random_int(0, CELL_MAX - 1);
		head_pos = sf::Vector2u(x, y);

		dir = (Direction)get_random_int((int)Direction::Up, (int)Direction::Down);
	}
	~Snake() {}

	sf::Vector2u get_head_pos() { return head_pos; }
	int len() { return length; }
	void grow() { 
		apples_till_grow--;
		if (apples_till_grow == 0)
		{
			length += 1;
			score += 1;
			apples_till_grow = 4;
		}
	}
	Direction get_dir() { return dir; }

	void change_dir(Direction new_dir)
	{
		//snake can't move in opposite direction to current direction
		//otherwise it will kill itself
		bool check1 = new_dir == Direction::Right and dir != Direction::Left;
		bool check2 = new_dir == Direction::Left  and dir != Direction::Right;
		bool check3 = new_dir == Direction::Up    and dir != Direction::Down;
		bool check4 = new_dir == Direction::Down  and dir != Direction::Up;

		if (check1 or check2 or check3 or check4)
			dir = new_dir;	
	}
	sf::Vector2u move()
	{
		return move_point(head_pos, dir);
	}
	void update_head_pos()
	{
		head_pos = move();
	}
	void add_score() { score+=2; }
	int get_score() { return score; }
	int get_apples_till_grow() { return apples_till_grow; }

	sf::Vector2u get_next_pos()
	{
		return move_point(head_pos, dir);
	}
};

#endif //SNAKE_H