#include"GameWindow.h"


GameWindow::GameWindow():
    sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),
                     "Snektris",
                     sf::Style::Close | sf::Style::Titlebar)
{
    setFramerateLimit(10);//otherwise game will work with the light speed
    setKeyRepeatEnabled(false);
    
    sf::Image icon;
    icon.loadFromFile("assets/icon.png");
    setIcon(32, 32, icon.getPixelsPtr());

    type_b.loadFromFile("assets/type.wav");
    type_sound.setBuffer(type_b);
}
GameWindow::~GameWindow()
{
}
void GameWindow::run()
{
    //start with default state machine
    auto* current_state_machine = manager.get_current_state_machine();
    auto event_manager = current_state_machine->get_event_manager();

    int input_counter = 0;
    int space_counter = 0;
    while (isOpen())
    {
        sf::Event event;
        while (pollEvent(event))
        {
	  
            if (event.type == sf::Event::Closed)
                close();
	    if(event.type == sf::Event::KeyPressed)
	      {
		if(event.key.scancode == sf::Keyboard::Scan::Escape)
		current_state_machine->set_pause(!current_state_machine->is_paused());
	      }
	    if(current_state_machine->is_paused())
	      continue;
	    
	    bool input = manager.is_allowed_to_accumulate_text_input() and
	      input_counter < USER_NAME_MAX_LEN;

	    if (event.type == sf::Event::TextEntered and 
		input)
	      {
		char input_val = event.text.unicode;
		if(isalpha(input_val))
		  static_cast<MainMenu*>(current_state_machine)->update_user_name(event.text.unicode);

		if((int)input_val != 8)
		input_counter++;
		type_sound.play();
	      }
	    if(event.type == sf::Event::KeyPressed and
	       input)
	      {
		if (event.key.scancode == sf::Keyboard::Scan::Space)
		  {
		    static_cast<MainMenu*>(current_state_machine)->update_user_name(' ');
		    input_counter++;
		    type_sound.play();
		  }
	      }
	    if(event.type == sf::Event::KeyPressed)
	      {
		if(event.key.scancode == sf::Keyboard::Scan::Backspace)
		  {
		    if(static_cast<MainMenu*>(current_state_machine)->get_name_len() == 0)
		      {}
		    else{
		    static_cast<MainMenu*>(current_state_machine)->remove_last_char();
		    input_counter--;
		    type_sound.play();
		    }
		  }
	      }
	    
        }

        //if it's time to switch to other state machine
        //then update it and change state machine
        if (current_state_machine->should_move_to_next())
        {
            manager.update();
            current_state_machine = manager.get_current_state_machine();
            event_manager = current_state_machine->get_event_manager();
	    input_counter = 0;
	    space_counter = 0;
        }

	
        //process events, that doesn't require special and additional data
        if (!event_manager.should_stop() and !current_state_machine->is_paused())
            for (auto& e : event_manager.get_independent_events())
            {
                if (e->get_c_type() == BaseEvent::ComplexityType::simple)
                    static_cast<SimpleEvent*>(e)->process();
            }
        
        clear();
        //dependent events are part of game state machine
        //so they are bound to render
        current_state_machine->render(*this); 
        display();
    }
}
