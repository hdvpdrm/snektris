#ifndef CONSTANTS_H
#define CONSTANTS_H
#include<array>
#include"SFML/Graphics/Color.hpp"
#include"SFML/System/Vector2.hpp"

static const int CELL_SIZE = 20;
static const int CELL_MAX = 30; //max number of cells
static const sf::Vector2f DRAWABLE_OBJECT_SIZE = sf::Vector2f(16.0f,16.0f);
static constexpr float DELTA = 100.0f;


//colors of blocks
static const std::array<sf::Color, 4> colors = {sf::Color::Red,
                                                sf::Color::Yellow,
                                                sf::Color::Green,
                                                sf::Color::Magenta };

static const sf::Color border_color = sf::Color(229, 185, 242, 255);
static const sf::Color cell_color   = sf::Color(68, 55, 72, 255);

static constexpr int WINDOW_WIDTH = (CELL_MAX * CELL_SIZE) + (int)DELTA * 2;
static constexpr int WINDOW_HEIGHT = (CELL_MAX * CELL_SIZE) - CELL_SIZE;

static const int USER_NAME_MAX_LEN = 13;

#endif
