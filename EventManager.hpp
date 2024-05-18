#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP
#include"BaseEvent.hpp"

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
#endif
