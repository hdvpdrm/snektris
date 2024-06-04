#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#include"StateMachineManager.hpp"
#include"constants.h"
#include<algorithm>
#include<string>
#include"SFML/Audio.hpp"

//use private inheritance, because RenderWindow's methods are used within this class only
class GameWindow: sf::RenderWindow
{
  StateMachineManager manager;

  sf::SoundBuffer type_b;
  sf::Sound type_sound;
public:
	GameWindow();
	~GameWindow();
	
	void run();
};

#endif //GAME_WINDOW_H
