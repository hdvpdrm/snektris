cmake_minimum_required(VERSION 3.10)
project(snektris)
set (CMAKE_CXX_STANDARD 17)

set(SOURCE main.cpp)
set(HEADERS 
DeathStateMachine.hpp
EventManager.hpp
VictoryScreen.hpp
MainMenu.h
HorizontalBlock.hpp
HelpMenu.hpp
BaseTetrisBlock.hpp
ColorManager.h
GameWindow.h
HighScoreStateMachine.hpp
GameUtilFunctions.cpp
BaseEvent.hpp
MovebaleObject.h
GameDrawingFunctions.cpp
GameStatistic.hpp
GetRandomNumber.h
resource.h
HighScoreManager.cpp
Map.cpp
GameTimer.h
BlockEventsCreation.cpp
HighScoreManager.h
GameStateMachine.cpp
Map.h
GameStateMachine.hpp
VerticalBlock.hpp
ZigZagBlock.hpp
Snake.h
TextEventsCreation.cpp
TetrisBlock.hpp
GameWindow.cpp
KeyEventsCreation.cpp
main.cpp
ColorManager.cpp
SquareBlock.hpp
constants.h
BaseStateMachine.hpp
SnakeEventsCreation.cpp
FancyText.hpp
MapEvent.hpp
SimpleEvent.hpp
FancyText.cpp
Matrix.hpp
StateMachineManager.hpp
Event.hpp)

add_link_options(-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio)
add_executable(snektris ${SOURCE} ${HEADERS}) 
