#ifndef MAP_H
#define MAP_H
#include"Matrix.hpp"
#include"Snake.h"
#include<stack>
#include <algorithm>
#include<functional>

/*
About game state:
  game field is matrix, where each element can be:
  1) empty cell
  2) apple
  3) part of snake

  empty cells and apples are simple objects that can be represented as constants,
  so it's obvious to use enum clas, but snake part is more diffucult, so it's represented
  as class.
*/
enum class State
{
	none,
	red_apple,
	yellow_apple,
	green_apple,
	magenta_apple
};
typedef variant<State,SnakePiece*> GameState;
typedef variant<GameState, void*> cell;
//////////////
class Map
{
private:
	Matrix<GameState>* map = nullptr;
public:
	Map(size_t width, size_t height, const sf::Vector2u& snake_head_pos);
	~Map();

	//since matrix is incapsulated we provided only special and required methods
	//to get and set cells of matrix
	auto get_cell(size_t x, size_t y)
	{
		return map->get_elem(x, y);
	}
	void set_element(size_t x, size_t y, const GameState& state)
	{
		map->set_element(state, x, y);
	}    
        
	stack<sf::Vector2u> get_snake();
	bool is_snake_at_pos(size_t x, size_t y);
	bool is_empty(size_t x, size_t y);
	bool is_apple(size_t x, size_t y);
	State get_apple_type(size_t x, size_t y);

	void apply_procedure(const function<void(size_t x, size_t y)>& proc);
};

#endif //MAP_H
