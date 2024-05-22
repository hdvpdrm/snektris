#include"ColorManager.h"


void ColorManager::generate_color_to_eat()
{
	int color = get_random_int(0, colors.size() - 1);
	color_to_eat = colors[color];
	shape_to_eat.setFillColor(color_to_eat);

	int n = 0;
	for (int i = 0; i < 4; i++)
	{
		if (i != color)
		{
			colors_to_move[n] = colors[i];
			shapes_to_move[n].setFillColor(colors_to_move[n]);
			n++;
		}
	}
}
sf::Color ColorManager::choose_color(State s)
{
	switch (s)
	{
	case State::green_apple: return sf::Color::Green; break;
	case State::magenta_apple: return sf::Color::Magenta; break;
	case State::yellow_apple: return sf::Color::Yellow; break;
	default: break;
	}

	return sf::Color::Red;
}
State ColorManager::choose_color(sf::Color c)
{
	if (c == sf::Color::Green) return State::green_apple;
	if (c == sf::Color::Magenta) return State::magenta_apple;
	if (c == sf::Color::Yellow) return State::yellow_apple;

	return State::red_apple;
}