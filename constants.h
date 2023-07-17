#ifndef CONSTANTS_H
#define CONSTANTS_H
#include<array>
#include"SFML/Graphics/Color.hpp"

static const int CELL_SIZE = 20;
static const int CELL_MAX = 30; //max number of cells

static const float delta = 100.0f;

static const std::array<sf::Color, 4> colors = {sf::Color::Red,
                                                sf::Color::Yellow,
                                                sf::Color::Green,
                                                sf::Color::Magenta };
#endif