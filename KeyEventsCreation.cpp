#include"GameStateMachine.hpp"

void Game::create_key_events()
{
	//key processing
	BaseEvent* move_left = new SimpleEvent(INDEP, [&]() {
		return is_pressed(sf::Keyboard::Left) and
			!is_dir_blocked(Direction::Left); },
		[&]() {
			if (does_new_dir_kills_snake(Direction::Left)) return; //break to prevent suicide

			old_direction = snake.get_dir();
			snake.change_dir(Direction::Left);

			if (noneatable_exist)
			{
				snake.update_head_pos(noneatable_snake_head);
				noneatable_exist = false;
			}
			snake_direction_changed = true;
			snake_pos_before_direction_change = snake.get_head_pos();
		});
	event_manager.add(move_left);

	BaseEvent* move_right = new SimpleEvent(INDEP, [&]() {
		return is_pressed(sf::Keyboard::Right) and
			!is_dir_blocked(Direction::Right); },
		[&]() {
			if (does_new_dir_kills_snake(Direction::Right)) return; //break to prevent suicide

			old_direction = snake.get_dir();
			snake.change_dir(Direction::Right);
			if (noneatable_exist)
			{
				snake.update_head_pos(noneatable_snake_head);
				noneatable_exist = false;
			}
			snake_direction_changed = true;
			snake_pos_before_direction_change = snake.get_head_pos();
		});
	event_manager.add(move_right);


	BaseEvent* move_up = new SimpleEvent(INDEP, [&]() {
		return is_pressed(sf::Keyboard::Up) and
			!is_dir_blocked(Direction::Up);   },
		[&]() {
			if (does_new_dir_kills_snake(Direction::Up)) return; //break to prevent suicide
			old_direction = snake.get_dir();
			snake.change_dir(Direction::Up);
			if (noneatable_exist)
			{
				snake.update_head_pos(noneatable_snake_head);
				noneatable_exist = false;
			}
			snake_direction_changed = true;
			snake_pos_before_direction_change = snake.get_head_pos();
		});
	event_manager.add(move_up);

	BaseEvent* move_down = new SimpleEvent(INDEP, [&]() {
		return is_pressed(sf::Keyboard::Down) and
			!is_dir_blocked(Direction::Down);   },
		[&]() {
			if (does_new_dir_kills_snake(Direction::Down)) return; //break to prevent suicide
			old_direction = snake.get_dir();
			snake.change_dir(Direction::Down);
			if (noneatable_exist)
			{
				snake.update_head_pos(noneatable_snake_head);
				noneatable_exist = false;
			}
			snake_direction_changed = true;
			snake_pos_before_direction_change = snake.get_head_pos();
		});
	event_manager.add(move_down);

	//direction can be changed, but snake won't move because the way is blocked by block
	//this event prevents snake's ability to kill itself due to this possibility 
	BaseEvent* check_was_direction_really_changed = new SimpleEvent(INDEP, [&]() { return snake_direction_changed; },
		[&]()
	{
		auto pos = move_point(snake.get_head_pos(), snake.get_dir());
		if (move_point(snake.get_head_pos(), snake.get_dir()) == snake_pos_before_direction_change)
		{
			snake.change_dir(old_direction);
			
		}
		snake_direction_changed = false;
	});
	event_manager.add(check_was_direction_really_changed);
}