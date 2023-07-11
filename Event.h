#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include<list>
#include"Map.h"

typedef std::function<bool()> predicat;
typedef std::function<void()> simple_callback;
typedef std::function<void(size_t x, size_t y,Map* map)> map_callback;

/*
	Events.
	Event is pair of two callback functions,
	where first function is always a logical predicat and 
	second one is callback function.

	Callback functions can be simple and it means that
	it does not require any specific arguments. Also 
	it can be complex, that means it require additional
	arguments.

	Since this is special implementation of game, there is two 
	types of callbacks in Tnake game. 
	
	One type of events represents
	independent pairs of functions, because to process them we don't 
	need to iterate over game field matrix. I call them independent, because
	they don't require information about map, current cell position e.t.c

	And the last type is dependent. To process them we need information about
	field map.
*/


//abstract class for base events
//it contains only type data and predicat
//because every event type has the same type of predicats
//simply it's just boolean lambda function
class BaseEvent
{
public:
	enum class Type
	{
		dependent,
		independent,
		after_stop
	};
	enum class ComplexityType
	{
		simple,
		map
	};
protected:
	 predicat pred;
	 Type type;
	 ComplexityType c_type;
public:
	BaseEvent(Type type,
		      const predicat& pred):pred(pred),type(type){}
	virtual ~BaseEvent(){}

	virtual void process() = 0;
	virtual void process(size_t x, size_t y,Map* map) = 0;

	Type get_type() { return type; }
	ComplexityType get_c_type() { return c_type; }
};
#define INDEP BaseEvent::Type::independent
#define DEP BaseEvent::Type::dependent
#define AS BaseEvent::Type::after_stop


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


//class stores all data about events and sorts them by type
class EventManager
{
private:
	list<BaseEvent*> dependent_events, independent_events, events_after_stop;
	bool stop_processing = false;
public:
	EventManager(){}
	~EventManager(){}
	
	void add(BaseEvent* event)
	{
		auto type = event->get_type();
		if (type == BaseEvent::Type::dependent)
			dependent_events.push_back(event);
		else if (type == BaseEvent::Type::independent)
			independent_events.push_back(event);
		else
			events_after_stop.push_back(event);
	}
	
	auto& get_dependent_events()const { return dependent_events; }
	auto& get_independent_events()const { return independent_events; }
	auto& get_events_after_stop()const { return events_after_stop; }

	void clear()
	{
		dependent_events.clear();
		independent_events.clear();
	}
	void stop()
	{
		stop_processing = true;
	}

	bool should_stop() { return stop_processing; }
};

#endif //EVENT_H