#ifndef GAME_TIMER_H
#define GAME_TIMER_H
#include"SFML/System/Clock.hpp"
#include<string>
#include<iostream>
class Timer
{
private:
	sf::Clock clock;
	int sec = 0, min = 0;
public:
	Timer()
	{
		clock.restart();
	}
	~Timer()
	{

	}
	std::string get_time()
	{
		auto t = (int)clock.getElapsedTime().asSeconds();
		if (t == 1)
		{
			sec++;
			clock.restart();
		}
		if (sec == 60)
		{
			min++;
			sec = 0;
		}
		return  std::to_string(min) + "." + std::to_string(sec);
	}
};

#endif 