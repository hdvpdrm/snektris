#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include"BaseStateMachine.hpp"
#include"FancyText.hpp"
#include"SFML/Graphics/Text.hpp"
#include"SFML/Graphics.hpp"
class MainMenu : public BaseStateMachine
{
private:
	sftk::FancyText title, input_message;
	sf::RectangleShape cursor = sf::RectangleShape(sf::Vector2f(10.0f, 25.0f));
	sf::Clock anim_timer;
	sf::Text user_name;
public:
	MainMenu(void* return_value) :BaseStateMachine(return_value)
	{
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

		BaseEvent* check_start = new SimpleEvent(INDEP, [&]()
		{
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
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

	}

	string get_user_name()
	{
		return user_name.getString();
	}
	void update_user_name(char ch)
	{
		user_name.setString(user_name.getString() + ch);
		cursor.setPosition(cursor.getPosition() + sf::Vector2f(20.0f, 0.0f));
	}
	void render(sf::RenderWindow& window)
	{
		window.draw(title);
		window.draw(input_message);
		window.draw(cursor);
		window.draw(user_name);
	}

};
#endif