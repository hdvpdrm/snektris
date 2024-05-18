#ifndef STATE_MACHINE_MANAGER_HPP
#define STATE_MACHINE_MANAGER_HPP
#include"GameStateMachine.hpp"
#include"DeathStateMachine.hpp"
class StateMachineManager
{
private:
    enum class StateMachineType
    {
        game,
        death,
        Count
    };
    int get_state_machine_types_number()
    {
        return static_cast<int>(StateMachineType::Count);
    }

    StateMachineType curr_type = StateMachineType::game;
    BaseStateMachine* curr_state_machine = nullptr;
public:
    StateMachineManager()
    {
        curr_state_machine = new Game();
    }
    ~StateMachineManager()
    {
        delete curr_state_machine;
    }

    void update()
    {
        if (curr_type == StateMachineType::game)
        {
            void* ret_value = curr_state_machine->get_return_value();
            delete curr_state_machine;
            curr_type = StateMachineType::death;
            curr_state_machine = new Death(ret_value);
        }
        else
        {
            delete curr_state_machine;
            curr_type = StateMachineType::game;
            curr_state_machine = new Game;
        }
    }
    BaseStateMachine* get_current_state_machine()
    { 
        return curr_state_machine;
    }
};
#endif
