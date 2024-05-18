#ifndef BASE_EVENT_HPP
#define BASE_EVENT_HPP
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

#define ALWAYS_RET_T [](){ return true;}
#endif
