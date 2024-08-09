#include"GameStateMachine.hpp"

void Game::move_blocks()
{
  if(eat_all) return;
  
	for (auto& block : tetris_blocks)
	{
		if (block->does_intersect_snake(snake.get_next_pos()))
		{
			auto block_color = choose_color(block->get_state());
			if (block_color != color_to_eat)
			{
				if (!block->is_going_out_of_map(snake.get_dir()))
				{

					block->set_attaching_flag(true);

					if (block->can_move_with_dir(map, snake.get_dir()))
					{
						block_movement = false;
						if (block->does_intersect_snake(move_point(snake.get_head_pos(), snake.get_dir())))
							block->move(map, snake.get_dir());

						auto head_pos = move_point(snake.get_head_pos(), snake.get_dir(), 1);
						map->set_element(head_pos.x, head_pos.y, GameState(new SnakePiece(snake.len(), false)));
						noneatable_snake_head = head_pos;
						noneatable_exist = true;
					}
					else
					{
						block->set(map);
						blocked_direction.push_back(snake.get_dir());
						map->apply_procedure([&](size_t x, size_t y)
						{
							update_snake(x, y, false);
						});
					}
				}
			}
		}
		else
		{
			block->set_attaching_flag(false);
		}
	}
}
bool Game::can_snake_move()
{ 
	auto next_pos = snake.get_next_pos();
	if (map->is_apple(next_pos.x, next_pos.y))
	{
		auto apple_type = map->get_apple_type(next_pos.x, next_pos.y);
		if (choose_color(apple_type) == color_to_eat || eat_all)
			return true;
		else return false;
	}
	return true;
}
bool Game::is_dir_blocked(Direction dir)
{
  return std::find(blocked_direction.begin(), blocked_direction.end(), dir) != blocked_direction.end();
}
void Game::update_snake(size_t x, size_t y, bool should_decrement)
{
	//last argument is just hack

	auto cell = map->get_cell(x, y);
	if (holds_alternative<GameState>(cell))
	{
		auto obj = get<0>(cell);
		if (holds_alternative<SnakePiece*>(obj))
		{
			auto part = get<1>(obj);
			if (part->should_terminate())
			{
				map->set_element(x, y, GameState(State::none));
			}
			else if (should_decrement) part->decrement();
		}
	}
}

bool Game::is_pressed(sf::Keyboard::Key key)
{
	vector<sf::Keyboard::Key> keys = { sf::Keyboard::Left,
								 sf::Keyboard::Right,
								 sf::Keyboard::Up,
								 sf::Keyboard::Down };


	bool pressed = false;
	if (sf::Keyboard::isKeyPressed(key))pressed = true;
	for (auto& k : keys)
	{
		if (k != key and sf::Keyboard::isKeyPressed(k))return false;
	}

	return pressed;
}

void Game::clear_ground()
{
	vector<vector<sf::Vector2u>> red, green, magenta, yellow;

	auto add_pos = [&](vector<vector<sf::Vector2u>>& poses, sf::Vector2u pos)
	{
		if (poses.empty())
		{
			vector<sf::Vector2u> line = { pos };
			poses.push_back(line);
		}
		else
		{
			bool added = false;
			for (auto& line : poses)
			{
				auto last = *(--line.end());
				auto diff = abs((int)last.x - (int)pos.x);
				if (diff == 1)
				{
					line.push_back(pos);
					added = true;
					break;
				}
			}
			if (!added)
			{
				vector<sf::Vector2u> line = { pos };
				poses.push_back(line);
			}
		}
	};
	auto check_poses = [&](vector<vector<sf::Vector2u>>& poses)
	{
		for (auto& line : poses)
		{
			if (line.size() > 4)
			{
				clear.play();
				for (auto& block : tetris_blocks)
				{
					block->erase(line, map);
				}
				snake.add_score();
			}
		}
	};
	for (size_t x = 0; x < CELL_MAX - 1; x++)
	{
		if (map->is_apple(x, CELL_MAX - 2))
		{
			auto type = map->get_apple_type(x, CELL_MAX - 2);
			switch (type)
			{
			case State::green_apple:
				add_pos(green, sf::Vector2u(x, CELL_MAX - 2));
				break;
			case State::red_apple:
				add_pos(red, sf::Vector2u(x, CELL_MAX - 2));
				break;
			case State::magenta_apple:
				add_pos(magenta, sf::Vector2u(x, CELL_MAX - 2));
				break;
			case State::yellow_apple:
				add_pos(yellow, sf::Vector2u(x, CELL_MAX - 2));
				break;
			}
		}
	}

	check_poses(red);
	check_poses(green);
	check_poses(magenta);
	check_poses(yellow);
}
bool Game::does_new_dir_kills_snake(Direction dir)
{
	auto pos = move_point(snake.get_head_pos(), dir);
	return map->is_snake_at_pos(pos.x, pos.y);
}
short Game::compute_fullness()
{
  int counter = 0;
  for(int y = 0;y<map->get_height();++y)
    {
      for(int x = 0;x<map->get_width();++x)
	{
	  if(map->is_apple(x,y))++counter;
	}

    }

  float square = map->get_height()*map->get_width();
  std::cout<<counter<<std::endl;
  float val = (float)counter/square;
  if(val < 0.2) return 0;
  else if(val >= 0.2 and val < 0.4) return 1;
  else if(val >= 0.4 and val < 0.6) return 2;
  else return 3;

}
std::string Game::get_fullness_label(short fullness)
{
  if(fullness == 0) return "low";
  else if(fullness == 1) return "mid";
  else if(fullness == 2) return "high";
  else return "fuck";
}
