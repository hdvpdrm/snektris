#ifndef MOVEBABLE_OBJECT_H
#define MOVEBABLE_OBJECT_H
#include"SFML/System/Vector2.hpp"
#include"constants.h"
enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

/*
		use step only when you move blocks(!!!)
*/
static sf::Vector2u move_point(const sf::Vector2u& point,
							   Direction dir,
							   int step=1)
{
	switch (dir)
	{
	case Direction::Down:
		//if it reaches the bottom, then it teleports to the top
		//otherwise it just moves down
		if (point.y == CELL_MAX - 2) return sf::Vector2u(point.x, 0);
		else return sf::Vector2u(point.x, point.y + step);
		break;
	case Direction::Up:
		//if it reaches the top, then it teleports to the bottom
		//otherswise it moves up
		if (point.y == 0) return sf::Vector2u(point.x, CELL_MAX - 2);
		else return sf::Vector2u(point.x, point.y - step);
		break;
	case Direction::Left:
		//similar conception of movement
		if (point.x == 0) return sf::Vector2u(CELL_MAX - 1, point.y);
		else return sf::Vector2u(point.x - step, point.y);
		break;
	case Direction::Right:
		if (point.x == CELL_MAX - 1) return sf::Vector2u(0, point.y);
		else return sf::Vector2u(point.x + step, point.y);
		break;
	}
}
static Direction get_opposite_direction(Direction dir)
{
	switch (dir)
	{
	case Direction::Up: return Direction::Down;
	case Direction::Down: return Direction::Up;
	case Direction::Left: return Direction::Right;
	case Direction::Right: return Direction::Left;
	}
}
#endif