#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include"BaseStateMachine.hpp"
#include"FancyText.hpp"
#include"SFML/Graphics/Text.hpp"
#include"SFML/Graphics.hpp"
#include"SFML/Audio.hpp"

class MainMenu : public BaseStateMachine
{
private:
  sftk::FancyText title, input_message, help_message;
  sf::RectangleShape cursor = sf::RectangleShape(sf::Vector2f(10.0f, 25.0f));
  sf::Clock anim_timer;
  sf::Text user_name;

  sf::Music music;
public:
	MainMenu(void* return_value) :BaseStateMachine(return_value)
	{
	  music.openFromFile("assets/startmenu.wav");
	  music.setLoop(true);
	  music.setVolume(35);
	  music.play();
		user_name.setCharacterSize(32);
		user_name.setFont(font);
		user_name.setPosition(sf::Vector2f(100.0f, 200.0f));
		cursor.setPosition(sf::Vector2f(100.0f, 210.0f));

		anim_timer.restart();
		title = sftk::TextBuilder{ font }
			<< sftk::txt::size(64)
			<< sf::Color::Magenta << "Sn"
			<< sf::Color::Green << "ek" <<
			sf::Color::Yellow << "tr" <<
			sf::Color::Red << "is";
		title.setPosition(sf::Vector2f(240.0f, 10.0f));


		input_message = sftk::TextBuilder{ font }
			<< sftk::txt::size(34)
			<<sftk::txt::spacing(2.0f)
			<< sf::Color::White
			<< "Who are you master of "
			<< sf::Color::Green
			<< "serpent" 
			<<sf::Color::White
			<<"and "
			<<sf::Color::Magenta
			<<"block";
		input_message.setPosition(sf::Vector2f(100.0f, 150.0f));

		help_message = sftk::TextBuilder{ font }
		  << sftk::txt::size(24)
		  << sftk::txt::spacing(4.0f)
		  <<sf::Color::White
		  <<" Type "
		  <<sf::Color::Magenta
		  <<" smhs "
		  <<sf::Color::White
		  <<" to see high scores or "
		  <<sf::Color::Magenta
		  <<" wtf "
		  <<sf::Color::White
		  <<" to see help ";
		help_message.setPosition(sf::Vector2f(120.0f,400.0f));
		  
		
		BaseEvent* check_start = new SimpleEvent(INDEP, [&]()
		{
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
				   sf::Keyboard::isKeyPressed(sf::Keyboard::Tab);
		},
			[&]() {
			move_to_next = true;
		});
		event_manager.add(check_start);

		BaseEvent* update_cursor_view = new SimpleEvent(INDEP, [&]()
		{
			return anim_timer.getElapsedTime().asSeconds() >  0.5f;
		},
			[&]()
			{
			if (cursor.getFillColor().r == 155)
			{
				auto color = sf::Color(255, 255, 255, 255);
				cursor.setFillColor(color);
			}
			else
			{
				auto color = sf::Color(155, 255, 255, 0);
				cursor.setFillColor(color);
			}
			anim_timer.restart();
			});
		event_manager.add(update_cursor_view);
	}
	~MainMenu()
        {
	  music.stop();
	}

	string get_user_name()
	{
		return user_name.getString().isEmpty()? "unknown":user_name.getString();
	}
        void move_cursor(const sf::Vector2f& pos)
        {
	  auto bounds = user_name.getGlobalBounds();
	  auto _pos = sf::Vector2f(bounds.left+bounds.width,cursor.getPosition().y);
	  cursor.setPosition(_pos + pos);
        }
	void update_user_name(char ch)
	{
	  user_name.setString(user_name.getString() + ch);
	  move_cursor(sf::Vector2f(1.0f,0.0f));
	}
        void remove_last_char()
        {
	  auto str = user_name.getString().toAnsiString();
	  if(str.empty()) return;

	  
	  str.pop_back();
	  user_name.setString(str);
	  move_cursor(sf::Vector2f(-1.0f,0.0f));
        }
	void render(sf::RenderWindow& window)
	{
	  window.draw(title);
	  window.draw(input_message);
	  window.draw(cursor);
	  window.draw(user_name);
	  window.draw(help_message);
	}

};
#endif
