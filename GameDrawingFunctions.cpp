#include"GameStateMachine.hpp"


void Game::render(sf::RenderWindow& window)
{
	for (int y = 0; y < CELL_MAX; ++y)
		for (int x = 0; x < CELL_MAX; ++x)
		{
			if (!event_manager.should_stop())
			{
				for (auto& e : event_manager.get_dependent_events())
				{
					static_cast<MapEvent*>(e)->process(x, y, map);
				}
			}
			else
			{
				//clear event manager once only
				if (!cleared)
				{
					event_manager.clear();
					cleared = true;
				}

				for (auto& ev : event_manager.get_events_after_stop())
					ev->process();
			}

			block.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + DELTA * 2, (float)y * CELL_SIZE));
			window.draw(block);


			if (map->is_apple(x, y))
			{
				auto type = map->get_apple_type(x, y);
				apple.setFillColor(choose_color(type));
				apple.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + DELTA * 2, (float)y * CELL_SIZE));
				window.draw(apple);
			}
			if (map->is_snake_at_pos(x, y))
			{
				auto piece = get<1>(get<0>(map->get_cell(x, y)));
				auto diff = (snake.len() - piece->get_val()) * 10;

				auto color = sf::Color();
				if (diff < 220)color = sf::Color(255 - diff, 255 - diff, 255 - diff, 255);
				else
				{
					color = sf::Color(25, 25, 25, 255);
				}
				snake_head.setFillColor(color);
				snake_head.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + DELTA * 2, (float)y * CELL_SIZE));
				window.draw(snake_head);
			}
		}

	draw_border(window);
	draw_text(window);
	draw_shapes(window);
}
void Game::draw_border(sf::RenderWindow& window)
{
	border.setPosition(sf::Vector2f((2 * DELTA) - 8.0f, 0.0f));
	window.draw(border);
}
void Game::draw_text(sf::RenderWindow& window)
{
	window.draw(time);
	window.draw(length);
	window.draw(apples_to_grow);

	if(!eat_all)
	  {
	    window.draw(eat);
	    window.draw(move);
	  }
	else
	  {
	    window.draw(eat_all_label);
	    window.draw(eat_all_label_value);
	  }
	window.draw(title);
	window.draw(score);
	window.draw(user_name);

	if(pause)window.draw(pause_label);
}
void Game::draw_shapes(sf::RenderWindow& window)
{
  if(eat_all) return;
  
	window.draw(shape_to_eat);
	for (auto& s : shapes_to_move)window.draw(s);
}
