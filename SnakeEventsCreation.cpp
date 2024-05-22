#include"GameStateMachine.hpp"


void Game::create_snake_events()
{
	//this is nonconditional event
//because snake is moving all time
	BaseEvent* process_movement = new SimpleEvent(INDEP,
		[&]() { return can_snake_move() and !is_dir_blocked(snake.get_dir()); },
		[&]() {snake.update_head_pos(); });
	event_manager.add(process_movement);


	BaseEvent* update = new MapEvent(DEP,
		[&]() {return !is_dir_blocked(snake.get_dir()); },
		[&](size_t x, size_t y, Map* map)
	{
		update_snake(x, y);
	});
	event_manager.add(update);

	BaseEvent* set_snake = new SimpleEvent(INDEP, [&]() { return !dying; },
		[&]()
	{
		(*map).set_element(snake.get_head_pos().x, snake.get_head_pos().y,
			GameState(new SnakePiece(snake.len())));
	});
	event_manager.add(set_snake);

	BaseEvent* eat_apple = new SimpleEvent(INDEP, [&]()
	{
		for (auto& block : tetris_blocks)
		{
			if (choose_color(block->get_state()) == color_to_eat)
			{
				if (block->is_eaten(snake.get_head_pos()))
					return true;
			}
		}
		return false;
	},
		[&]()
	{
		snake.grow();
		_eat.play();
	});
	event_manager.add(eat_apple);


	BaseEvent* check_death = new MapEvent(DEP, ALWAYS_RET_T,
		[&](size_t x, size_t y, Map* map)
	{
		auto cell = map->get_cell(x, y);
		if (holds_alternative<GameState>(cell))
		{
			auto obj = get<0>(cell);
			if (holds_alternative<SnakePiece*>(obj))
			{
				if (get<1>(obj)->is_eatable())
				{
					auto curr = sf::Vector2u(x, y);
					auto head_pos = snake.get_head_pos();

					if (curr == move_point(snake.get_head_pos(), snake.get_dir()))
					{
						event_manager.stop();
						snake_parts = map->get_snake();
						clock.restart();
					}
				}
			}
		}
	});
	event_manager.add(check_death);

	/*!@@ DEPRECATED FOR SOME TIME...
	//TODO:::!
		BaseEvent* check_blocks_reach_top = new MapEvent(DEP, ALWAYS_RET_T,
			[&](size_t x, size_t y, Map* map)
			{
		  if(map->is_column_filled_with_blocks(x))
		{
		  event_manager.stop();
		  snake_parts = map->get_snake();
		  clock.restart();
		  loosing = true;
		}
		/*
				for (auto& block : tetris_blocks)
				{
					if (block->did_reach_the_top())
					{
						event_manager.stop();
						snake_parts = map->get_snake();
						clock.restart();
						loosing = true;
					}
			/*    }*/
			//        });*/
	//event_manager.add(check_blocks_reach_top);



	BaseEvent* process_snake_fading = new SimpleEvent(AS, ALWAYS_RET_T,
		[&]() {
		dying = true;
		if (clock.getElapsedTime().asSeconds() > 0.1f)
		{
			if (!snake_parts.empty())
			{
				auto pos = snake_parts.top();
				snake_parts.pop();
				die.play();

				(*map).set_element(pos.x, pos.y, GameState(State::none));
			}
			else move_to_next = true;
			clock.restart();
		}
	});
	event_manager.add(process_snake_fading);
}