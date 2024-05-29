#include"GameStateMachine.hpp"


void Game::set_text()
{
	time.setFont(label_font);
	time.setCharacterSize(18);
	time.setPosition(sf::Vector2f(10.0f, 125.0f));

	score.setFont(label_font);
	score.setCharacterSize(18);
	score.setPosition(sf::Vector2f(10.0f, 150.0f));

	length.setFont(label_font);
	length.setCharacterSize(18);
	length.setPosition(sf::Vector2f(10.0f, 180.0f));

	apples_to_grow.setFont(label_font);
	apples_to_grow.setCharacterSize(16);
	apples_to_grow.setPosition(sf::Vector2f(10.0f, 220.0f));

	eat.setFont(label_font);
	eat.setCharacterSize(16);
	eat.setString("eat:");
	eat.setPosition(sf::Vector2f(10.0f, 300.0f));

	move.setFont(label_font);
	move.setCharacterSize(16);
	move.setString("move:");
	move.setPosition(sf::Vector2f(10.0f, 340.0f));

	user_name.setFont(label_font);
	user_name.setCharacterSize(14);
	user_name.setPosition(sf::Vector2f(8, 410.0f));


	title = sftk::TextBuilder{ font }
		<< sftk::txt::size(40)
		<< sf::Color::Magenta << "Sn"
		<< sf::Color::Green << "ek" <<
		sf::Color::Yellow << "tr" <<
		sf::Color::Red << "is";
	title.setPosition(sf::Vector2f(10.0f, 10.0f));

	shape_to_eat.setSize(DRAWABLE_OBJECT_SIZE);
	shape_to_eat.setPosition(sf::Vector2f(80.0f, 301.0f));

	auto x = 90.0f;
	for (auto& s : shapes_to_move)
	{
		s.setSize(DRAWABLE_OBJECT_SIZE);
		s.setPosition(sf::Vector2f(x, 341.0f));
		x += 20.0f;
	}

	start_sound.play();
}
