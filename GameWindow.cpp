#include"GameWindow.h"


GameWindow::GameWindow():
    sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),
                     "Snektris",
                     sf::Style::Close | sf::Style::Titlebar)
{
    setFramerateLimit(10);//otherwise game will work with the light speed
    
    sf::Image icon;
    icon.loadFromFile("assets/icon.png");
    setIcon(32, 32, icon.getPixelsPtr());
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
    while (isOpen())
    {
        sf::Event event;
        while (pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                close();
			if (event.type == sf::Event::TextEntered and 
				manager.is_allowed_to_accumulate_text_input() and
				input_counter < 16)
			{
				char input_val = event.text.unicode;
				if(isalpha(input_val))
					static_cast<MainMenu*>(current_state_machine)->update_user_name(event.text.unicode);
				input_counter++;
			}
        }

        //if it's time to switch to other state machine
        //then update it and change state machine
        if (current_state_machine->should_move_to_next())
        {
            manager.update();
            current_state_machine = manager.get_current_state_machine();
            event_manager = current_state_machine->get_event_manager();
        }

        //process events, that doesn't require special and additional data
        if (!event_manager.should_stop())
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
