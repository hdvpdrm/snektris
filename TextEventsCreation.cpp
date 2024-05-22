#include"GameStateMachine.hpp"


void Game::create_text_events()
{
	BaseEvent* update_text = new SimpleEvent(INDEP, ALWAYS_RET_T,
		[&]()
	{
		score.setString("score:" + to_string(snake.get_score()));
		time.setString("time:" + timer.get_time());
		auto len = snake.len();
		if (dying)len = snake_parts.size();
		length.setString("length:" + to_string(len));

		apples_to_grow.setString("till grow:" + to_string(snake.get_apples_till_grow()));
	});
	event_manager.add(update_text);


	BaseEvent* set_game_stat = new SimpleEvent(AS, ALWAYS_RET_T,
		[&]()
	{
		return_value = new GameStatistic(snake.len(),
			timer.get_time(),
			snake.get_score(),
			!loosing);
	});
	event_manager.add(set_game_stat);

}