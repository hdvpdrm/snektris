#ifndef HELP_MENU_HPP
#define HELP_MENU_HPP
#include"BaseStateMachine.hpp"
#include"FancyText.hpp"




class HelpMenu: public BaseStateMachine
{
private:
  sftk::FancyText help_label, movement_label, hint1, hint2, hint3,hint4,hint5, hint6;
public:
  HelpMenu(void* return_value):BaseStateMachine(return_value)
  {
    help_label =  sftk::TextBuilder{ font }
        << sftk::txt::size(60)
        << sf::Color::Magenta << "WTF       " << sf::Color::White << "page";
    help_label.setPosition(250.0f, 40.0f);


    
    movement_label =  sftk::TextBuilder{ font }
        << sftk::txt::size(40)
        << sf::Color::Magenta<<"Move  using  "
        <<sf::Color::White<<"W A S D   or   arrows";
    movement_label.setPosition(140.0f, 200.0f);

    
    hint1 =  sftk::TextBuilder{ font }
        << sftk::txt::size(40)
        << sf::Color::Magenta<<"You  need  "
        <<sf::Color::White<<"to  reach "
        <<sf::Color::Magenta<<"  66 "
        <<sf::Color::White<<" score points";
    hint1.setPosition(80.0f, 250.0f);

    hint2 = sftk::TextBuilder{ font }
 
    <<sftk::txt::size(40)
      <<sf::Color::Magenta << "Combine  "
      <<sf::Color::White<<" blocks of "
      <<sf::Color::Magenta<<" the same  "
      <<sf::Color::White<<" color";
    hint2.setPosition(80.0f, 300.0f);

    hint3 = sftk::TextBuilder{ font }
         << sftk::txt::size(40)
      << sf::Color::Magenta<< "Do  not  eat  "
      <<sf::Color::White<<" yourself";
    hint3.setPosition(210.0f,350.0f);

    hint4 = sftk::TextBuilder{ font }
         << sftk::txt::size(40)
	 << sf::Color::Magenta<< "Press  X  "
      <<sf::Color::White<<" to eat all colors temporaly";
    hint4.setPosition(80.0f,400.0f);

    hint5 = sftk::TextBuilder{ font }
      << sftk::txt::size(40)
      <<sf::Color::Magenta << "Press  Esc  "
      <<sf::Color::White<<" to pause game";
    hint5.setPosition(180.0f,450.0f);
      
    hint6 = sftk::TextBuilder{ font }
      << sftk::txt::size(40)
      << sf::Color::Magenta << "high fullness "
      <<sf::Color::White<<" sucks";
    hint6.setPosition(220.0f,500.0f);
    
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
  ~HelpMenu(){}

  void render(sf::RenderWindow& window)
  {
    window.draw(help_label);
    window.draw(movement_label);
    window.draw(hint1);
    window.draw(hint2);
    window.draw(hint3);
    window.draw(hint4);
    window.draw(hint5);
    window.draw(hint6);
  }

};
#endif
