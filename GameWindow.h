#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#include"StateMachine.h"
#include"constants.h"
#include<algorithm>


//use private inheritance, because RenderWindow's methods are used within this class only
class GameWindow: sf::RenderWindow
{
	StateMachineManager manager;


public:
	GameWindow();
	~GameWindow();
	
	void run();
};

#endif //GAME_WINDOW_H