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

static sf::Vector2u move_point(const sf::Vector2u& point,
							   Direction dir)
{
	switch (dir)
	{
	case Direction::Down:
		//if it reaches the bottom, then it teleports to the top
		//otherwise it just moves down
		if (point.y == CELL_MAX - 1) return sf::Vector2u(point.x, 0);
		else return sf::Vector2u(point.x, point.y + 1);
		break;
	case Direction::Up:
		//if it reaches the top, then it teleports to the bottom
		//otherswise it moves up
		if (point.y == 0) return sf::Vector2u(point.x, CELL_MAX - 1);
		else return sf::Vector2u(point.x, point.y - 1);
		break;
	case Direction::Left:
		//similar conception of movement
		if (point.x == 0) return sf::Vector2u(CELL_MAX - 1, point.y);
		else return sf::Vector2u(point.x - 1, point.y);
		break;
	case Direction::Right:
		if (point.x == CELL_MAX - 1) return sf::Vector2u(0, point.y);
		else return sf::Vector2u(point.x + 1, point.y);
		break;
	}
}
#endif