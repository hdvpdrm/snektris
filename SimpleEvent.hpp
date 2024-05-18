#ifndef SIMPLE_EVENT_HPP
#define SIMPLE_EVENT_HPP
#include"BaseEvent.hpp"

//implementation of events, whose callback function doesn't require any additional information
class SimpleEvent: public BaseEvent
{
private:
	simple_callback fn;
public:
	SimpleEvent(Type type,
		        const predicat& pred,
		        const simple_callback& fn) :BaseEvent(type,pred)
	{
		this->fn = fn;
		c_type = ComplexityType::simple;
	}
	~SimpleEvent(){}
	void process()
	{
		if (pred())fn();
	}
	//this function isn't used by simple event class
	void process(size_t x, size_t y, Map* map) { return; }
};
#endif
