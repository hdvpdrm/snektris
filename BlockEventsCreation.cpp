#include"GameStateMachine.hpp"

void Game::create_blocks_events()
{
	//events related to falling blocks
	BaseEvent* generate_falling_block = new SimpleEvent(INDEP,
		[&]()
	{
		if (block_generator_clock.getElapsedTime().asSeconds() > 5.0f)return true;
		else return false;
	},
		[&]()
	{
		tetris_blocks.push_back(generate_tetris_block(color_to_eat));
		block_generator_clock.restart();
	});
	event_manager.add(generate_falling_block);


	BaseEvent* _move_blocks = new SimpleEvent(INDEP, ALWAYS_RET_T,
		[&]() { return move_blocks(); });
	event_manager.add(_move_blocks);

	BaseEvent* update_blocks = new SimpleEvent(INDEP,
		[&]()
	{
		return block_movement_clock.getElapsedTime().asSeconds() > 1.0f;
	},
		[&]()
	{
		tetris_blocks.erase(
			std::remove_if(
				tetris_blocks.begin(),
				tetris_blocks.end(),
				[](TetrisBLock* b) { return b->should_die() /* and b->should_fall()*/; }
			),
			tetris_blocks.end()
		);
		for (auto& block : tetris_blocks)
		{
			if (block->should_fall())
				block->move(map);
		}
		block_movement_clock.restart();
	});
	event_manager.add(update_blocks);


	BaseEvent* change_color = new SimpleEvent(INDEP,
		[&]()
	{
		return color_changer_clock.getElapsedTime().asSeconds() > 30.0f;
	},
		[&]()
	{
		generate_color_to_eat();
		color_changer_clock.restart();
	});
	event_manager.add(change_color);


	BaseEvent* check_blocked_direction = new SimpleEvent(INDEP, ALWAYS_RET_T,
		[&]()
	{
		if (!blocked_direction.empty())
		{
			blocked_direction.erase(
				std::remove_if(blocked_direction.begin(),
					blocked_direction.end(),
					[&](Direction dir)
			{
				return dir != snake.get_dir();
			}),
				blocked_direction.end()
				);
		}
	});
	event_manager.add(check_blocked_direction);

	BaseEvent* _clear_ground = new SimpleEvent(INDEP, ALWAYS_RET_T,
		[&]() { clear_ground(); });
	event_manager.add(_clear_ground);

	BaseEvent* check_fullness = new SimpleEvent(INDEP,
						      [&](){ return false;},						    
						   [&]()
						   {
						     loosing = true;
						     //cout<<"fefe"<<endl;							
						     event_manager.stop();
						     snake_parts = map->get_snake();
						     clock.restart();
						       
						      });
	event_manager.add(check_fullness);
						      
						      
}
