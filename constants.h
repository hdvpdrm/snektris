#ifndef CONSTANTS_H
#define CONSTANTS_H
#include<array>
#include"SFML/Graphics/Color.hpp"

static const int CELL_SIZE = 20;
static const int CELL_MAX = 30; //max number of cells
static const sf::Vector2f DRAWABLE_OBJECT_SIZE = sf::Vector2f(16.0f,16.0f);
static constexpr float DELTA = 100.0f;

static const std::array<sf::Color, 4> colors = {sf::Color::Red,
                                                sf::Color::Yellow,
                                                sf::Color::Green,
                                                sf::Color::Magenta };

static constexpr int WINDOW_WIDTH = (CELL_MAX * CELL_SIZE) + (int)DELTA * 2;
static constexpr int WINDOW_HEIGHT = (CELL_MAX * CELL_SIZE) - CELL_SIZE;

#endif
