#include"Map.h"

Map::Map(size_t width, size_t height,const sf::Vector2u& snake_head_pos)
{
	//fill map with empty cells
	map = new Matrix<GameState>(width, height);
	for (int y : views::iota(0, (int)height))
		for (int x : view::iota(0, (int)width))
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
	for (int y : views::iota(0, (int)map->get_height()))
		for (int x : view::iota(0, (int)map->get_width()))
		{
			auto el = map->get_elem(x, y);
			if (holds_alternative<GameState>(el))
			{
				auto obj = get<0>(el);
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