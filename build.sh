#/bin/sh
g++ -std=c++17 -I. *.cpp *.hpp *.h -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o snektris
