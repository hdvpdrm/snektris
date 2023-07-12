#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#include"StateMachine.h"
#include<algorithm>
//use private inheritance, because RenderWindow's methods are used within this class only
class GameWindow: sf::RenderWindow
{
	StateMachineManager manager;

	//use static because it provides ability to make field constexpr
	static constexpr int WINDOW_WIDTH = (CELL_MAX * CELL_SIZE)+(int)delta*2;
	static constexpr int WINDOW_HEIGHT = CELL_MAX * CELL_SIZE;

public:
	GameWindow();
	~GameWindow();
	
	void run();
};

#endif //GAME_WINDOW_H