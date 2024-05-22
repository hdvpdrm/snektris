#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H
#include<array>
#include"SFML/Graphics.hpp"
#include"Map.h"
class ColorManager
{
protected:
	ColorManager(){}

	sf::Color color_to_eat;
	array<sf::Color, 3> colors_to_move;

	//represents color on left side panel
	sf::RectangleShape shape_to_eat;
	array<sf::RectangleShape, 3> shapes_to_move;

	void generate_color_to_eat();
	sf::Color choose_color(State s);
	State choose_color(sf::Color c);

};
#endif
