#ifndef VICTORY_SCREEN_H
#define VICTORY_SCREEN_H
#include"SFML/Graphics.hpp"
#include"BaseStateMachine.hpp"
#include"FancyText.hpp"
#include"GameStatistic.hpp"


class VictoryScreen: public BaseStateMachine
{
private:
  sf::Text label;
  sftk::FancyText snake_len, game_time, restart;
public:
  VictoryScreen(void* return_value):BaseStateMachine(return_value)
  {
    GameStatistic* stat = static_cast<GameStatistic*>(return_value);

    snake_len = sftk::TextBuilder{ label_font }
            << sftk::txt::size(24)
            << "snake's " << sf::Color::Yellow << "length " << sf::Color::White
            << "was " + to_string(stat->snake_len);
        snake_len.setPosition(170.0f, 160.0f);

        game_time = sftk::TextBuilder{ label_font }
            << sftk::txt::size(24)
            << "game time was " << sf::Color::Green << stat->game_time<<"m";
        game_time.setPosition(180.0f, 200.0f);

       restart = sftk::TextBuilder{ label_font }
            << sftk::txt::size(26)
            << "press " << sf::Color::Magenta << "R" << sf::Color::White << " to start again!";
        restart.setPosition(140.0f, 460.0f);	


	delete stat;
		
    label.setFont(font);
    label.setCharacterSize(60);
    label.setPosition(250.0f,40.0f);
    label.setFillColor(sf::Color::Magenta);
    label.setString("Victory!");

    BaseEvent* check_restart = new SimpleEvent(INDEP, [&]()
    {
      return sf::Keyboard::isKeyPressed(sf::Keyboard::R);
    },
      [&](){
	move_to_next = true;
      });
    event_manager.add(check_restart);
  }
  ~VictoryScreen(){}

  void render(sf::RenderWindow& window)
  {
    window.draw(label);
    window.draw(snake_len);
    window.draw(game_time);
    window.draw(restart);
  }
};

#endif
