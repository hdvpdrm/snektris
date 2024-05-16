#include"Map.h"

Map::Map(size_t width, size_t height,const sf::Vector2u& snake_head_pos)
{
	//fill map with empty cells
	map = new Matrix<GameState>(width, height);
	for (int y  = 0; y<height;++y)
		for (int x = 0; x< width; ++x)
			map->set_element(GameState(State::none), x, y);

	//set start position of snake
	map->set_element(GameState(new SnakePiece(1)), snake_head_pos.x, snake_head_pos.y);
}
Map::~Map()
{
	delete map;
}
stack<sf::Vector2u> Map::get_snake()
{
	vector< tuple<sf::Vector2u, SnakePiece*>> snake;
	for (int y = 0; y < map->get_height();++y)
		for (int x = 0; x < map->get_width(); ++x)
		{
			auto el = map->get_elem(x, y);
			if (holds_alternative<GameState>(el))
			{
				auto obj = get<0>(el);
				auto type = static_cast<GameState>(obj);
				
				if (holds_alternative<SnakePiece*>(obj))
				{
					snake.push_back(make_tuple(sf::Vector2u(x, y), get<1>(obj)));
				}
			}
		}

	typedef tuple<sf::Vector2u, SnakePiece*> s;
	sort(snake.begin(), snake.end(),[](s a, s b) 
		{
			return get<1>(a)->get_val() > get<1>(b)->get_val();
		});
	
	stack<sf::Vector2u> snake_pieces_poses;
	for (auto& el : snake)
	{
		snake_pieces_poses.push(get<0>(el));
	}
	return snake_pieces_poses;
}
bool Map::is_snake_at_pos(size_t x, size_t y)
{
	auto cell = map->get_elem(x, y);
	if (holds_alternative<GameState>(cell))
	{
		auto obj = get<0>(cell);
		if (holds_alternative<SnakePiece*>(obj))
			return true;
	}
	return false;
}
bool Map::is_empty(size_t x, size_t y)
{
	auto cell = map->get_elem(x, y);
	if (holds_alternative<GameState>(cell))
	{
		auto obj = get<0>(cell);
		if (holds_alternative<State>(obj))
		{
			auto s = get<0>(obj);
			if (s == State::none)return true;
		}
	}
	return false;
}
bool Map::is_apple(size_t x, size_t y)
{
	auto cell = map->get_elem(x, y);
	if (holds_alternative<GameState>(cell))
	{
		auto obj = get<0>(cell);
		if (holds_alternative<State>(obj))
		{
			auto s = get<0>(obj);
			if (s != State::none)return true;
		}
	}
	return false;
}
State Map::get_apple_type(size_t x, size_t y)
{
	auto cell = map->get_elem(x, y);
	if (holds_alternative<GameState>(cell))
	{
		auto obj = get<0>(cell);
		if (holds_alternative<State>(obj))
		{
			auto s = get<0>(obj);
			if (s != State::none)return s;
		}
	}
	return State::none;
}
void Map::apply_procedure(const function<void(size_t x, size_t y)>& proc)
{
	for (int y = 0; y < map->get_height(); ++y)
		for (int x = 0; x < map->get_height(); ++x)
		{
			proc(x, y);
		}
}
bool Map::is_position_eatable(size_t x, size_t y, const State color_to_eat)
{
	if (!is_apple(x, y)) return true;
	return get_apple_type(x, y) == color_to_eat;
}