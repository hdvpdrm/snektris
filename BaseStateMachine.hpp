#ifndef BASE_STATE_MACHINE_HPP
#define BASE_STATE_MACHINE_HPP
#include"Event.hpp"
#include"SFML/Graphics.hpp"
/*
    StateMachine.
    Simply it's just combination of events and data that is related to these events.
    But there are many different data, so it's better to split them into different
    objects and switch between them when it's required.
    So There are 2 state machines: game and death.
*/

class BaseStateMachine
{
protected:
    EventManager event_manager;
    bool move_to_next = false;

    sf::Font font,label_font;

    void* return_value = nullptr;
    bool pause = false;
public:
    BaseStateMachine(void* return_value=nullptr)
    {
        font.loadFromFile("assets/ARCADECLASSIC.TTF");
        label_font.loadFromFile("assets/prstart.ttf");
        this->return_value = return_value;
    }
    virtual ~BaseStateMachine(){}
    
    virtual void render(sf::RenderWindow& window) = 0;
    bool should_move_to_next() { return move_to_next; }
    EventManager& get_event_manager() { return event_manager; }

    void* get_return_value() { return return_value; }
    bool is_paused() { return pause;}
    void set_pause(bool flag) { pause = flag;}

};
#endif //BASE_STATE_MACHINE_HPP
