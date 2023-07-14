#ifndef SNAKE_H
#define SNAKE_H
#include"SFML/System/Vector2.hpp"
#include"GetRandomNumber.h"
#include"constants.h"

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
public:
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};
private:
	int length = 1;
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
	void move()
	{
		switch (dir)
		{
		case Direction::Down:
			//if it reaches the bottom, then it teleports to the top
			//otherwise it just moves down
			if (head_pos.y == CELL_MAX-1) head_pos = sf::Vector2u(head_pos.x, 0);
			else head_pos = sf::Vector2u(head_pos.x, head_pos.y + 1);
			break;
		case Direction::Up:
			//if it reaches the top, then it teleports to the bottom
			//otherswise it moves up
			if (head_pos.y == 0) head_pos = sf::Vector2u(head_pos.x, CELL_MAX-1);
			else head_pos = sf::Vector2u(head_pos.x, head_pos.y - 1);
			break;
		case Direction::Left:
			//similar conception of movement
			if (head_pos.x == 0) head_pos = sf::Vector2u(CELL_MAX-1, head_pos.y);
			else head_pos = sf::Vector2u(head_pos.x - 1, head_pos.y);
			break;
		case Direction::Right:
			if (head_pos.x == CELL_MAX-1) head_pos = sf::Vector2u(0, head_pos.y);
			else head_pos = sf::Vector2u(head_pos.x + 1, head_pos.y);
			break;
		}
	}

	int get_score() { return score; }
	int get_apples_till_grow() { return apples_till_grow; }
};

#endif //SNAKE_H