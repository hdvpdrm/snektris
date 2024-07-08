#ifndef STATE_MACHINE_MANAGER_HPP
#define STATE_MACHINE_MANAGER_HPP
#include"GameStateMachine.hpp"
#include"DeathStateMachine.hpp"
#include"MainMenu.h"
#include"HelpMenu.hpp"
#include"HighScoreStateMachine.hpp"
#include"VictoryScreen.hpp"
class StateMachineManager
{
private:
    enum class StateMachineType
    {
      main_menu,
      game,
      death,
      help,
      high_score,
      victory,
      Count
    };
    int get_state_machine_types_number()
    {
        return static_cast<int>(StateMachineType::Count);
    }

    StateMachineType curr_type = StateMachineType::main_menu;
    BaseStateMachine* curr_state_machine = nullptr;
    string user_name;
public:
    StateMachineManager()
    {
        curr_state_machine = new MainMenu(nullptr);
    }
    ~StateMachineManager()
    {
        delete curr_state_machine;
    }

	bool is_allowed_to_accumulate_text_input()
	{
		return curr_type == StateMachineType::main_menu;
	}
    void update()
    {
      if(curr_type == StateMachineType::death)
	{
	  delete curr_state_machine;
	  curr_type = StateMachineType::game;
	  curr_state_machine = new Game(user_name);
	}
      else if(curr_type == StateMachineType::high_score)
	{
	  delete curr_state_machine;
	  curr_type = StateMachineType::main_menu;
	  curr_state_machine = new MainMenu(nullptr);
	}
      else if(curr_type == StateMachineType::help)
	{
	  user_name = "";
	  delete curr_state_machine;
	  curr_type = StateMachineType::main_menu;
	  curr_state_machine = new MainMenu(nullptr);
	}
      else if (curr_type == StateMachineType::game)
        {
	  if(static_cast<Game*>(curr_state_machine)->is_victory())
	    {
	      delete curr_state_machine;
	      curr_type = StateMachineType::victory;
	      curr_state_machine = new VictoryScreen(nullptr);
	    }
	  else
	    {
	      void* ret_value = curr_state_machine->get_return_value();
	      delete curr_state_machine;
	      curr_type = StateMachineType::death;
	      curr_state_machine = new Death(ret_value);
	    }
        }
        else
        {
	  if (curr_type == StateMachineType::main_menu/* and user_name.empty() */)
	    user_name = static_cast<MainMenu*>(curr_state_machine)->get_user_name();

	  if(curr_type == StateMachineType::main_menu)
	    {
	      if(user_name == "smhs")
		{
		  delete curr_state_machine;
		  curr_type = StateMachineType::high_score;
		  curr_state_machine = new HighScoreStateMachine(nullptr);
		}
	      else if(user_name == "wtf")
		{
		  delete curr_state_machine;
		  curr_type = StateMachineType::help;
		  curr_state_machine = new HelpMenu(nullptr);
		}
	    
	      else
		{
		  delete curr_state_machine;
		  curr_type = StateMachineType::game;
		  curr_state_machine = new Game(user_name);
		}
        
	    }
	}
    }
    BaseStateMachine* get_current_state_machine()
    { 
        return curr_state_machine;
    }
};
#endif
