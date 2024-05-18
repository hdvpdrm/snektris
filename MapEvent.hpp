#ifndef MAP_EVENT_HPP
#define MAP_EVENT_HPP
#include"BaseEvent.hpp"
#include"Map.h"
//implementation of events also known as complex
class MapEvent: public BaseEvent
{
private:
	map_callback fn;
public:
	MapEvent(Type type,
			 const predicat& pred,
			 const map_callback& fn) :BaseEvent(type, pred)
	{
		this->fn = fn;
		c_type = ComplexityType::map;
	}
	~MapEvent(){}
	void process() { return; }
	void process(size_t x, size_t y, Map* map)
	{
		if (pred())fn(x, y,map);
	}
};
#endif
