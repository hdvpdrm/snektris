#ifndef HIGH_SCORE_STATE_MACHINE_HPP
#define HIGH_SCORE_STATE_MACHINE_HP
#include"BaseStateMachine.hpp"
#include"FancyText.hpp"
#include"HighScoreManager.h"
#include<string>
class HighScoreStateMachine:public BaseStateMachine
{

private:
  sftk::FancyText label, list;
  HighScoreManager hs_manager;
public:
  HighScoreStateMachine(void* return_value):BaseStateMachine(return_value)
  {
    label = sftk::TextBuilder{ font }
        << sftk::txt::size(60)
        << sf::Color::Magenta << "High scores";
        label.setPosition(200.0f, 40.0f);

	sftk::TextBuilder lst{font};
	lst<<sftk::txt::size(30);
	int id = 1;
    for(auto& record: hs_manager.get_data())
       {
	 auto score_value = record.second;
	 auto player_name = record.first;

	 lst << sf::Color::White<<std::to_string(id)
	 <<"      "
	 << sf::Color::Magenta<<std::to_string(score_value)
	 <<"   "
	 <<sf::Color::Green<<player_name
	 <<"\n";
	 
	 ++id;
       }
    list.set_text(lst);
    list.setPosition(250.0f,160.0f);
    
    BaseEvent* check_restart = new SimpleEvent(INDEP, [&]()
    {
      return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) or
	sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    },
      [&](){
	move_to_next = true;
      });
    event_manager.add(check_restart);
    
  }
  ~HighScoreStateMachine()
  {
  }

  void render(sf::RenderWindow& window)
  {
    window.draw(label);
    window.draw(list);
  }

};
#endif 
