#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include"Snake.h"
#include"Map.h"
#include"Event.h"
#include"SFML/Graphics.hpp"
#include"FallingBlock.h"
#include"GameTimer.h"
/*
    StateMachine.
    Simply it's just combination of events and data that is related to these events.
    But there are many different data, so it's better to split them into different
    objects and switch between them when it's required.
    So There are 3 state machines: main menu, game and death.
*/

class BaseStateMachine
{
protected:
	EventManager event_manager;
	bool move_to_next = false;

    sf::Font font,label_font;
public:
	BaseStateMachine()
    {
        font.loadFromFile("assets/ARCADECLASSIC.TTF");
        label_font.loadFromFile("assets/prstart.ttf");
    }
	virtual ~BaseStateMachine(){}
    
    virtual void render(sf::RenderWindow& window) = 0;
	bool should_move_to_next() { return move_to_next; }
    EventManager& get_event_manager() { return event_manager; }
};

class Game:public BaseStateMachine
{
private:
    sf::RectangleShape apple = sf::RectangleShape(sf::Vector2f(16.0f, 16.0f));
    sf::RectangleShape snake_head = sf::RectangleShape(sf::Vector2f(16.0f, 16.0f));
    sf::RectangleShape border = sf::RectangleShape(sf::Vector2f(4.0f, CELL_SIZE*CELL_MAX));
    sf::RectangleShape block = sf::RectangleShape(sf::Vector2f(16.0f, 16.0f));
    bool cleared = false;

    sf::Clock clock;
    sf::Clock block_generator_clock;
    sf::Clock block_movement_clock;

	Snake snake;
    stack<sf::Vector2u> snake_parts; //used for death

	Map* map = nullptr;
    Timer timer;
    sf::Text time, length;
public:
	Game()
	{
        apple.setFillColor(sf::Color::Red);
        snake_head.setFillColor(sf::Color::Green);
        border.setFillColor(sf::Color(229,185,242,255));
        block.setFillColor(sf::Color(68,55,72,255));

		map = new Map(CELL_MAX, CELL_MAX, snake.get_head_pos());
        block_generator_clock.restart();
        FallingBlock::generate(map);
   
        time.setFont(label_font);
        time.setCharacterSize(18);
        time.setPosition(sf::Vector2f(10.0f, 150.0f));

        length.setFont(label_font);
        length.setCharacterSize(18);
        length.setPosition(sf::Vector2f(10.0f, 180.0f));


        //key processing
        BaseEvent* move_left = new SimpleEvent(INDEP, []() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                return true;
            else return false; },
            [&]() {
                snake.change_dir(Snake::Direction::Left); });
        event_manager.add(move_left);

        BaseEvent* move_right = new SimpleEvent(INDEP, []() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                return true;
            else return false; },
            [&]() {
                snake.change_dir(Snake::Direction::Right); });
        event_manager.add(move_right);


        BaseEvent* move_up = new SimpleEvent(INDEP, []() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                return true;
            else return false; },
            [&]() {
                snake.change_dir(Snake::Direction::Up); });
        event_manager.add(move_up);

        BaseEvent* move_down = new SimpleEvent(INDEP, []() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                return true;
            else return false; },
            [&]() {
                snake.change_dir(Snake::Direction::Down); });
        event_manager.add(move_down);
        ///

        //this is nonconditional event
   //because snake is moving all time
        BaseEvent* process_movement = new SimpleEvent(INDEP, ALWAYS_RET_T,
            [&]() {snake.move(); });
        event_manager.add(process_movement);


        BaseEvent* update = new MapEvent(DEP, ALWAYS_RET_T,
            [&](size_t x, size_t y, Map* map)
            {
                auto cell = map->get_cell(x, y);
                if (holds_alternative<GameState>(cell))
                {
                    auto obj = get<0>(cell);
                    if (holds_alternative<SnakePiece*>(obj))
                    {
                        auto part = get<1>(obj);
                        if (part->should_terminate())
                            map->set_element(x, y, GameState(State::none));
                        else part->decrement();

                    }
                }


                map->set_element(snake.get_head_pos().x, snake.get_head_pos().y,
                    GameState(new SnakePiece(snake.len())));
            });
        event_manager.add(update);

        BaseEvent* eat_apple = new SimpleEvent(INDEP, [&]()
            { return does_snake_eat_apple(); },
            [&]()
            {
                snake.grow();
            });
        event_manager.add(eat_apple);
        
        auto is_dead = [&](const sf::Vector2u& a, const sf::Vector2u& b)
        {return a == b; };
        BaseEvent* check_death = new MapEvent(DEP, ALWAYS_RET_T,
            [&](size_t x, size_t y, Map* map)
            {
                auto cell = map->get_cell(x, y);
                if (holds_alternative<GameState>(cell))
                {
                    auto obj = get<0>(cell);
                    if (holds_alternative<SnakePiece*>(obj))
                    {
                        auto curr = sf::Vector2u(x, y);
                        auto head_pos = snake.get_head_pos();
                        switch (snake.get_dir())
                        {
                        case Snake::Direction::Down:
                            head_pos.y += 1;
                            break;
                        case Snake::Direction::Up:
                            head_pos.y -= 1;
                            break;
                        case Snake::Direction::Left:
                            head_pos.x -= 1;
                            break;
                        case Snake::Direction::Right:
                            head_pos.x += 1;
                            break;
                        }
                        if (is_dead(curr, head_pos))
                        {
                            event_manager.stop();
                            snake_parts = map->get_snake();
                            clock.restart();
                        }
                    }
                }
            });
        event_manager.add(check_death);


        BaseEvent* process_snake_fading = new SimpleEvent(AS, ALWAYS_RET_T,
            [&]() {
                if (clock.getElapsedTime().asSeconds() > 0.1f)
                {
                    if (!snake_parts.empty())
                    {
                        auto pos = snake_parts.top();
                        snake_parts.pop();
                        snake.decrease_length();

                        map->set_element(pos.x, pos.y, GameState(State::none));
                    }
                    else move_to_next = true;
                    clock.restart();
                }
            });
        event_manager.add(process_snake_fading);


        //events related to falling blocks
        BaseEvent* generate_falling_block = new SimpleEvent(INDEP, 
            [&]()
            {
                if (block_generator_clock.getElapsedTime().asSeconds() > 5.0f)return true;
                else return false;
            },
            [&]()
            {
                FallingBlock::generate(map);
                block_generator_clock.restart();
            });
        event_manager.add(generate_falling_block);

        BaseEvent* move_blocks = new SimpleEvent(INDEP,
            [&]() { return  block_movement_clock.getElapsedTime().asSeconds() > 1.0f; },
            [&]()
            {
                vector<sf::Vector2u> moved;
                for (int y : views::iota(0, CELL_MAX))
                    for (int x : views::iota(0, CELL_MAX))
                    {
                        if (map->is_apple(x, y)       and
                            std::find(moved.begin(),moved.end(),sf::Vector2u(x,y)) == moved.end())
                        {
                            if (map->is_empty(x, y + 1))
                            {
                                if (y+1 != CELL_MAX-1)
                                {
                                    map->set_element(x, y, GameState(State::none));
                                    map->set_element(x, y + 1, GameState(State::apple));
                                    moved.push_back(sf::Vector2u(x, y + 1));
                                }
                            }
                            else
                            {
                                auto free_y = find_empty_space(x, y + 1);
                                if (free_y != -1)
                                {
                                    if (free_y != CELL_MAX-1)
                                    {
                                        map->set_element(x, y, GameState(State::none));
                                        map->set_element(x, free_y, GameState(State::apple));
                                        moved.push_back(sf::Vector2u(x, free_y));
                                    }
                                }
                            }
                            
                        }
                    }
                block_movement_clock.restart();
            });
        event_manager.add(move_blocks);

        ///////////TEXT EVENTS /////////////////////////////
        BaseEvent* update_text = new SimpleEvent(INDEP, ALWAYS_RET_T,
            [&]()
            {
                time.setString("time:"+timer.get_time());
                length.setString("length:" + to_string(snake.len()+1));
            });
        event_manager.add(update_text);

        ////////////////////////////////////////////
	}
	~Game()
	{
		delete map;
	}

    void render(sf::RenderWindow& window)
    {
        for (int y : views::iota(0, CELL_MAX))
            for (int x : views::iota(0, CELL_MAX))
            {
                if (!event_manager.should_stop())
                {
                    for (auto& e : event_manager.get_dependent_events())
                    {
                        static_cast<MapEvent*>(e)->process(x, y, map);
                    }
                }
                else
                {
                    //clear event manager once only
                    if (!cleared)
                    {
                        event_manager.clear();
                        cleared = true;
                    }

                    for (auto& ev : event_manager.get_events_after_stop())
                        ev->process();
                }

                block.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + delta * 2, (float)y * CELL_SIZE));
                window.draw(block);


                if (map->is_apple(x, y))
                {
                    apple.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + delta * 2, (float)y * CELL_SIZE));
                    window.draw(apple);
                }
                if (map->is_snake_at_pos(x, y))
                {
                    auto piece = get<1>(get<0>(map->get_cell(x, y)));
                    auto diff = (snake.len() - piece->get_val()) * 10;

                    auto color = sf::Color();
                    if (diff < 220)color = sf::Color(255 - diff, 255 - diff, 255 - diff, 255);
                    else
                    {
                        color = sf::Color(25, 25, 25, 255);
                    }
                    snake_head.setFillColor(color);
                    snake_head.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + delta * 2, (float)y * CELL_SIZE));
                    window.draw(snake_head);
                }
            }
        draw_border(window);
        draw_text(window);
    }
private:
    void draw_border(sf::RenderWindow& window)
    {
        border.setPosition(sf::Vector2f((2*delta)-8.0f, 0.0f));
        window.draw(border);
    }
    void draw_text(sf::RenderWindow& window)
    {
        window.draw(time);
        window.draw(length);
    }

    int find_empty_space(int x,int begin)
    {
        int y = begin + 1;
        while (y != CELL_MAX-1)
        {
            if (map->is_empty(x, y)) return y;
            else y += 1;
        }
        return -1;
    }

    bool does_snake_eat_apple()
    {
        return map->is_apple(snake.get_head_pos().x, snake.get_head_pos().y);
    }
};
class Death:public BaseStateMachine
{
private:
    sf::Text title;
public:
    Death()
    {
        title.setString("ATE    YOURSELF");
        title.setCharacterSize(64);
        title.setFillColor(sf::Color::White);
        title.setPosition(200.0f, 40.0f);
        title.setFont(font);
    }
    ~Death(){}

    void render(sf::RenderWindow& window)
    {
        window.draw(title);
    }
};


class StateMachineManager
{
private:
    enum class StateMachineType
    {
        main_menu,
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
            delete curr_state_machine;
            curr_type = StateMachineType::death;
            curr_state_machine = new Death;
        }
    }
    BaseStateMachine* get_current_state_machine()
    { 
        return curr_state_machine;
    }
};

#endif 