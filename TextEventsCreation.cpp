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

	
	BaseEvent* update_eat_all_label = new SimpleEvent(INDEP,
	[&](){ return eat_all;},
	[&]()
	{
	  auto time = (int)eat_all_label_clock.getElapsedTime().asSeconds();
	  if(time >= 5)
	    {
	      eat_all_label_clock.restart();
	    }
	  else if(time == 1)
	    {
	      eat_all_label_value.setFillColor(sf::Color::Magenta);
	    }
	  else if(time == 2)
	    {
	      eat_all_label_value.setFillColor(sf::Color::Green);
	    }
	  else if(time == 3)
	    {
	      eat_all_label_value.setFillColor(sf::Color::Red);
	    }
	  else if(time == 4)
	    {
	      eat_all_label_value.setFillColor(sf::Color::Yellow);
	    }
	});
	event_manager.add(update_eat_all_label);

}
