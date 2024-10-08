#include"GameStateMachine.hpp"


void Game::create_snake_events()
{
	//this is nonconditional event
//because snake is moving all time
	BaseEvent* process_movement = new SimpleEvent(INDEP,
		[&]() { return can_snake_move() and !is_dir_blocked(snake.get_dir()); },
		[&]() {snake.update_head_pos(); });
	event_manager.add(process_movement);

	BaseEvent* start_eating_any_block = new SimpleEvent(INDEP,
		 [&]() { return sf::Keyboard::isKeyPressed(sf::Keyboard::X) and !eat_all;},
		 [&]()
		 {
		   if(snake.get_score() - 10 >= 0)
		     {
		       x_sound.play();
		       snake.spend_score();
		       eat_all = true;
		       super_power_clock.restart();
		     }
		   
		 });
	event_manager.add(start_eating_any_block);
	BaseEvent* check_should_stop_super_power = new SimpleEvent(INDEP,
								   [&]() { return eat_all and super_power_clock.getElapsedTime().asSeconds() > 30.0f;},
								   [&](){
								     eat_all = false;
								     super_power_clock.restart();
								   });
	event_manager.add(check_should_stop_super_power);
	

	BaseEvent* update = new MapEvent(DEP,
		[&]() {return !is_dir_blocked(snake.get_dir()); },
		[&](size_t x, size_t y, Map* map)
	{
	  if(!pause)
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
			if (choose_color(block->get_state()) == color_to_eat || eat_all)
			{
				if (block->is_eaten(snake.get_head_pos()))
					return true;
			}
		}
		return false;
	},
		[&]()
	{ 
		snake.grow(start_sound);
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
			else
			  {
				//add high score value
				hs_manager.add(user_name.getString().toAnsiString(), snake.get_score());
			    move_to_next = true;
			  }
			clock.restart();
		}
	});
	event_manager.add(process_snake_fading);	
	BaseEvent* check_victory = new SimpleEvent(INDEP,
						   ALWAYS_RET_T,
						   [&](){
						     if(snake.get_score() == SCORE_TO_WIN)
						       move_to_next = true;						     
						   });
	event_manager.add(check_victory);
}
