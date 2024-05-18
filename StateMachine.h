#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include"Snake.h"
#include"Event.h"
#include"TetrisBlock.h"
#include"GameTimer.h"
#include"GameStatistic.h"
#include"SFML/Graphics.hpp"
#include"SFML/Audio.hpp"
#include"FancyText.hpp"
#include<array>
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

    void* return_value = nullptr;
public:
	BaseStateMachine(void* return_value=nullptr)
    {
        font.loadFromFile("assets/ARCADECLASSIC.TTF");
        label_font.loadFromFile("assets/prstart.ttf");
        return_value = return_value;
    }
	virtual ~BaseStateMachine(){}
    
    virtual void render(sf::RenderWindow& window) = 0;
	bool should_move_to_next() { return move_to_next; }
    EventManager& get_event_manager() { return event_manager; }

    void* get_return_value() { return return_value; }
};



class Game:public BaseStateMachine
{
private:
    sf::RectangleShape apple = sf::RectangleShape(DRAWABLE_OBJECT_SIZE);
    sf::RectangleShape snake_head = sf::RectangleShape(DRAWABLE_OBJECT_SIZE);
    sf::RectangleShape border = sf::RectangleShape(sf::Vector2f(4.0f, CELL_SIZE*CELL_MAX));
    sf::RectangleShape block = sf::RectangleShape(DRAWABLE_OBJECT_SIZE);
    bool cleared = false;

    sf::Clock clock;
    sf::Clock block_generator_clock;
    sf::Clock block_movement_clock;
    sf::Clock color_changer_clock;

    Snake snake;
    stack<sf::Vector2u> snake_parts; //used for death
    vector<TetrisBLock*> tetris_blocks;
    
    Map* map = nullptr;
    Timer timer;
    sf::Text time, length, apples_to_grow, eat,move, score;
    sftk::FancyText title;

    bool dying = false;//snake eats itself
    bool loosing = false; //there is block that reached the top

    sf::Color color_to_eat;
    array<sf::Color, 3> colors_to_move;

    //represents color on left side panel
    sf::RectangleShape shape_to_eat; 
    array<sf::RectangleShape, 3> shapes_to_move;

    bool block_movement = false;
    vector<Direction> blocked_direction;

    sf::SoundBuffer eat_b, die_b, clear_b;
    sf::Sound _eat, die, clear;
public:
	Game()
	{
        eat_b.loadFromFile("assets/eat.wav");
        _eat.setBuffer(eat_b);

        die_b.loadFromFile("assets/die.wav");
        die.setBuffer(die_b);

        clear_b.loadFromFile("assets/clear.wav");
        clear.setBuffer(clear_b);

        color_changer_clock.restart();
        block_generator_clock.restart();
        block_movement_clock.restart();
        clock.restart();

        apple.setFillColor(sf::Color::Red);
        snake_head.setFillColor(sf::Color::Green);
        border.setFillColor(sf::Color(229,185,242,255));
        block.setFillColor(sf::Color(68,55,72,255));

	map = new Map(CELL_MAX, CELL_MAX, snake.get_head_pos());
        tetris_blocks.push_back(generate_tetris_block(color_to_eat));
        tetris_blocks[0]->set(map);//update first block at the beginning of game
   
        time.setFont(label_font);
        time.setCharacterSize(18);
        time.setPosition(sf::Vector2f(10.0f, 125.0f));

        score.setFont(label_font);
        score.setCharacterSize(18);
        score.setPosition(sf::Vector2f(10.0f, 150.0f));

        length.setFont(label_font);
        length.setCharacterSize(18);
        length.setPosition(sf::Vector2f(10.0f, 180.0f));

        apples_to_grow.setFont(label_font);
        apples_to_grow.setCharacterSize(16);
        apples_to_grow.setPosition(sf::Vector2f(10.0f, 220.0f));

        eat.setFont(label_font);
        eat.setCharacterSize(16);
        eat.setString("eat:");
        eat.setPosition(sf::Vector2f(10.0f, 300.0f));

        move.setFont(label_font);
        move.setCharacterSize(16);
        move.setString("move:");
        move.setPosition(sf::Vector2f(10.0f, 340.0f));
	

        title = sftk::TextBuilder{ font }
            << sftk::txt::size(40)
            << sf::Color::Magenta << "Sn"
            <<sf::Color::Green<<"ek"<<
            sf::Color::Yellow<<"tr"<<
            sf::Color::Red<<"is";
        title.setPosition(sf::Vector2f(10.0f, 10.0f));

        shape_to_eat.setSize(DRAWABLE_OBJECT_SIZE);
        shape_to_eat.setPosition(sf::Vector2f(80.0f, 301.0f));

        auto x = 90.0f;
        for (auto& s : shapes_to_move)
        {
            s.setSize(DRAWABLE_OBJECT_SIZE);
            s.setPosition(sf::Vector2f(x, 341.0f));
            x += 20.0f;
        }
	
        generate_color_to_eat();


        //key processing
		BaseEvent* move_left = new SimpleEvent(INDEP, [&]() {
			return is_pressed(sf::Keyboard::Left) and
				!is_dir_blocked(Direction::Left); },
            [&]() {
                snake.change_dir(Direction::Left); });
        event_manager.add(move_left);

		BaseEvent* move_right = new SimpleEvent(INDEP, [&]() {
			return is_pressed(sf::Keyboard::Right) and
				!is_dir_blocked(Direction::Right); },
            [&]() {
                snake.change_dir(Direction::Right); });
        event_manager.add(move_right);


		BaseEvent* move_up = new SimpleEvent(INDEP, [&]() {
			return is_pressed(sf::Keyboard::Up) and
				!is_dir_blocked(Direction::Up);   },
            [&]() {
                snake.change_dir(Direction::Up); });
        event_manager.add(move_up);

		BaseEvent* move_down = new SimpleEvent(INDEP, [&]() {
			return is_pressed(sf::Keyboard::Down) and
				!is_dir_blocked(Direction::Down);   },
            [&]() {
                snake.change_dir(Direction::Down); });
        event_manager.add(move_down);
        ///


		

        //this is nonconditional event
   //because snake is moving all time
        BaseEvent* process_movement = new SimpleEvent(INDEP, 
            [&]() { return can_snake_move() and !is_dir_blocked(snake.get_dir()); },
            [&]() {snake.update_head_pos(); });
        event_manager.add(process_movement);


        BaseEvent* update = new MapEvent(DEP,
            [&]() {return !is_dir_blocked(snake.get_dir()); },
            [&](size_t x, size_t y, Map* map)
            {
                update_snake(x, y);
            });
        event_manager.add(update);

        BaseEvent* set_snake = new SimpleEvent(INDEP, [&]() { return !dying; },
            [&]()
            {
                (*map).set_element(snake.get_head_pos().x, snake.get_head_pos().y,
                GameState(new SnakePiece(snake.len())));
            });
        event_manager.add(set_snake);

        BaseEvent* eat_apple = new SimpleEvent(INDEP, [&]()
            {
                for (auto& block:tetris_blocks)
                {
                    if (choose_color(block->get_state()) == color_to_eat)
                    {
                        if (block->is_eaten(snake.get_head_pos()))
                            return true;
                    }
                }
                return false;
            },
            [&]()
            {
                snake.grow();
                _eat.play();
            });
        event_manager.add(eat_apple);
        

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
                       
                        if (curr == move_point(snake.get_head_pos(),snake.get_dir()))
                        {
                            event_manager.stop();
                            snake_parts = map->get_snake();
                            clock.restart();
                        }
                    }
                }
            });
        event_manager.add(check_death);

	/*!@@ DEPRECATED FOR SOME TIME...
	//TODO:::!
        BaseEvent* check_blocks_reach_top = new MapEvent(DEP, ALWAYS_RET_T,
            [&](size_t x, size_t y, Map* map)
            {
	      if(map->is_column_filled_with_blocks(x))
		{
		  event_manager.stop();
		  snake_parts = map->get_snake();
		  clock.restart();
		  loosing = true;
		}
		/*
                for (auto& block : tetris_blocks)
                {
                    if (block->did_reach_the_top())
                    {
                        event_manager.stop();
                        snake_parts = map->get_snake();
                        clock.restart();
                        loosing = true;
                    }
		    /*    }*/
		    //        });*/
    //event_manager.add(check_blocks_reach_top);



        BaseEvent* process_snake_fading = new SimpleEvent(AS, ALWAYS_RET_T,
            [&]() {
                dying = true;
                if (clock.getElapsedTime().asSeconds() > 0.1f)
                {
                    if (!snake_parts.empty())
                    {
                        auto pos = snake_parts.top();
                        snake_parts.pop();
                        die.play();

                        (*map).set_element(pos.x, pos.y, GameState(State::none));
                    }
                    else move_to_next = true;
                    clock.restart();
                }
            });
        event_manager.add(process_snake_fading);

        BaseEvent* set_game_stat = new SimpleEvent(AS, ALWAYS_RET_T,
            [&]()
            {
                return_value = new GameStatistic(snake.len(), 
                                                 timer.get_time(),
                                                 snake.get_score(),
                                                 !loosing);
            });
        event_manager.add(set_game_stat);

        //events related to falling blocks
        BaseEvent* generate_falling_block = new SimpleEvent(INDEP, 
            [&]()
            {
                if (block_generator_clock.getElapsedTime().asSeconds() > 5.0f)return true;
                else return false;
            },
            [&]()
            {
                tetris_blocks.push_back(generate_tetris_block(color_to_eat));
                block_generator_clock.restart();
            });
        event_manager.add(generate_falling_block);


        BaseEvent* _move_blocks = new SimpleEvent(INDEP, ALWAYS_RET_T,
            [&]() { return move_blocks(); });
        event_manager.add(_move_blocks);

        BaseEvent* update_blocks = new SimpleEvent(INDEP,
            [&]()
            {
                return block_movement_clock.getElapsedTime().asSeconds() > 1.0f;
            },
            [&]()
            {
                tetris_blocks.erase(
                    std::remove_if(
                        tetris_blocks.begin(),
                        tetris_blocks.end(),
                        [](TetrisBLock* b) { return b->should_die() /* and b->should_fall()*/; }
                    ),
                    tetris_blocks.end()
                );
                for (auto& block : tetris_blocks)
                {
                    if(block->should_fall())
                        block->move(map);
                }
                block_movement_clock.restart();
            });
        event_manager.add(update_blocks);


        BaseEvent* change_color = new SimpleEvent(INDEP,
            [&]()
            {
                return color_changer_clock.getElapsedTime().asSeconds() > 30.0f;
            },
            [&]()
            {
                generate_color_to_eat();
                color_changer_clock.restart();
            });
        event_manager.add(change_color);


        BaseEvent* check_blocked_direction = new SimpleEvent(INDEP, ALWAYS_RET_T,
            [&]()
            {
                if (!blocked_direction.empty())
                {
                    blocked_direction.erase(
                        std::remove_if(blocked_direction.begin(),
                            blocked_direction.end(),
                            [&](Direction dir) 
							{ 
								return dir != snake.get_dir(); 
							}),
                        blocked_direction.end()
                    );
                }
            });
        event_manager.add(check_blocked_direction);

        ///////////TEXT EVENTS /////////////////////////////
        BaseEvent* update_text = new SimpleEvent(INDEP, ALWAYS_RET_T,
            [&]()
            {
                score.setString("score:" + to_string(snake.get_score()));
                time.setString("time:"+timer.get_time());
                auto len = snake.len();
                if (dying)len = snake_parts.size();
                length.setString("length:" + to_string(len));

                apples_to_grow.setString("till grow:" + to_string(snake.get_apples_till_grow()));
            });
        event_manager.add(update_text);


        BaseEvent* _clear_ground = new SimpleEvent(INDEP, ALWAYS_RET_T,
            [&]() { clear_ground(); });
        event_manager.add(_clear_ground);

        ////////////////////////////////////////////
	}
	~Game()
	{
		delete map;
	}

    void render(sf::RenderWindow& window)
    {
        for (int y = 0; y < CELL_MAX; ++y)
            for (int x = 0; x < CELL_MAX; ++x)
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

                block.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + DELTA * 2, (float)y * CELL_SIZE));
                window.draw(block);


                if (map->is_apple(x, y))
                {               
                    auto type = map->get_apple_type(x, y);
                    apple.setFillColor(choose_color(type));
                    apple.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + DELTA * 2, (float)y * CELL_SIZE));
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
                    snake_head.setPosition(sf::Vector2f(((float)x * CELL_SIZE) + DELTA * 2, (float)y * CELL_SIZE));
                    window.draw(snake_head);
                }
            }

        draw_border(window);
        draw_text(window);
        draw_shapes(window);
    }
private:
    void draw_border(sf::RenderWindow& window)
    {
        border.setPosition(sf::Vector2f((2*DELTA)-8.0f, 0.0f));
        window.draw(border);
    }
    void draw_text(sf::RenderWindow& window)
    {
        window.draw(time);
        window.draw(length);
        window.draw(apples_to_grow);
        window.draw(eat);
        window.draw(move);
        window.draw(title);
        window.draw(score);
    }
    void draw_shapes(sf::RenderWindow& window)
    {
        window.draw(shape_to_eat);
        for (auto& s : shapes_to_move)window.draw(s);
    }

    void generate_color_to_eat()
    {
        int color = get_random_int(0, colors.size()-1);
        color_to_eat = colors[color];
        shape_to_eat.setFillColor(color_to_eat);

        int n = 0;
        for (int i = 0; i < 4; i++)
        {
            if (i != color)
            {
                colors_to_move[n] = colors[i];
                shapes_to_move[n].setFillColor(colors_to_move[n]);
                n++;
            }
        }
    }
    sf::Color choose_color(State s)
    {
        switch (s)
        {
        case State::green_apple: return sf::Color::Green; break;
        case State::magenta_apple: return sf::Color::Magenta; break;
        case State::yellow_apple: return sf::Color::Yellow; break;
        case State::red_apple: return sf::Color::Red; break;
        }
    }
    State choose_color(sf::Color c)
    {
        if (c == sf::Color::Green) return State::green_apple;
        if (c == sf::Color::Magenta) return State::magenta_apple;
        if (c == sf::Color::Yellow) return State::yellow_apple;
        if (c == sf::Color::Red) return State::red_apple;
    }

  //@!1
    void move_blocks()
    {
        for (auto& block : tetris_blocks)
        {
            if (block->does_intersect_snake(snake.get_next_pos()))
            {  
                auto block_color = choose_color(block->get_state());
                if (block_color != color_to_eat)
                {
                    if (!block->is_going_out_of_map(snake.get_dir()))
                    {
                        
                        block->set_attaching_flag(true);

                        if (block->can_move_with_dir(map,snake.get_dir()))
                        {
                            block_movement = false;			   
			    block->move(map,snake.get_dir());
                        }
                        else
                        {
                            block->set(map);
                            blocked_direction.push_back(snake.get_dir());
                            map->apply_procedure([&](size_t x, size_t y)
                                {
                                    update_snake(x, y,false);
                                });
                        }
                    }
                }
            }
            else
            {
                block->set_attaching_flag(false);
            }
        }
    }
    bool can_snake_move()
    {
        auto next_pos = snake.get_next_pos();
        if (map->is_apple(next_pos.x, next_pos.y))
        {
            auto apple_type = map->get_apple_type(next_pos.x, next_pos.y);
            if (choose_color(apple_type) == color_to_eat)
                return true;
            else return false;
        }
        return true;
    }
    bool is_dir_blocked(Direction dir)
    {
        return std::find(blocked_direction.begin(), blocked_direction.end(), dir) != blocked_direction.end();
    }
    void update_snake(size_t x, size_t y, bool should_decrement=true)
    {
        //last argument is just hack

        auto cell = map->get_cell(x, y);
        if (holds_alternative<GameState>(cell))
        {
            auto obj = get<0>(cell);
            if (holds_alternative<SnakePiece*>(obj))
            {
                auto part = get<1>(obj);
                if (part->should_terminate())
		  {	
                    map->set_element(x, y, GameState(State::none));
		  }
                else if(should_decrement) part->decrement();
            }
        }
    }

    bool is_pressed(sf::Keyboard::Key key)
    {
        vector<sf::Keyboard::Key> keys = {sf::Keyboard::Left, 
                                     sf::Keyboard::Right,
                                     sf::Keyboard::Up,
                                     sf::Keyboard::Down};


        bool pressed = false;
        if (sf::Keyboard::isKeyPressed(key))pressed = true;
        for (auto& k : keys)
        {
            if (k != key and sf::Keyboard::isKeyPressed(k))return false;
        }
        
        return pressed;
    }

    void clear_ground()
    {
        vector<vector<sf::Vector2u>> red, green, magenta, yellow;

        auto add_pos = [&](vector<vector<sf::Vector2u>>& poses, sf::Vector2u pos)
        {
            if (poses.empty())
            {
                vector<sf::Vector2u> line = { pos };
                poses.push_back(line);
            }
            else
            {
                bool added = false;
                for (auto& line : poses)
                {
                    auto last = *(--line.end());
                    auto diff = abs((int)last.x - (int)pos.x);
                    if (diff == 1)
                    {
                        line.push_back(pos);
                        added = true;
                        break;
                    }
                }
                if (!added)
                {
                    vector<sf::Vector2u> line = { pos };
                    poses.push_back(line);
                }
            }
        };
        auto check_poses = [&](vector<vector<sf::Vector2u>>& poses)
        {
            for (auto& line : poses)
            {
                if (line.size() > 4)
                {
                    clear.play();
                    for (auto& block : tetris_blocks)
                    {
                        block->erase(line, map);
                    }
                    snake.add_score();
                }
            }
        };
        for (size_t x = 0; x < CELL_MAX - 1; x++)
        {
            if (map->is_apple(x, CELL_MAX - 2))
            {
                auto type = map->get_apple_type(x, CELL_MAX - 2);
                switch (type)
                {
                case State::green_apple:
                    add_pos(green, sf::Vector2u(x, CELL_MAX - 2));
                    break;
                case State::red_apple:
                    add_pos(red, sf::Vector2u(x, CELL_MAX - 2));
                    break;
                case State::magenta_apple:
                    add_pos(magenta, sf::Vector2u(x, CELL_MAX - 2));
                    break;
                case State::yellow_apple:
                    add_pos(yellow, sf::Vector2u(x, CELL_MAX - 2));
                    break;
                }
            }
        }

        check_poses(red);
        check_poses(green);
        check_poses(magenta);
        check_poses(yellow);

    }
};
class Death:public BaseStateMachine
{
private:
    sftk::FancyText title,snake_len, game_time, score, restart;
public:
    Death(void* return_value):BaseStateMachine(return_value)
    {
        GameStatistic* stat = static_cast<GameStatistic*>(return_value);

        string first_word = stat->eat_itself ? "ATE" : "GAME";
        string second_word = stat->eat_itself ? " YOURSELF" : "          OVER";
        title = sftk::TextBuilder{ font }
        << sftk::txt::size(60)
        << first_word << sf::Color::Magenta << second_word;
        title.setPosition(200.0f, 40.0f);

        

        snake_len = sftk::TextBuilder{ label_font }
            << sftk::txt::size(24)
            << "snake's " << sf::Color::Yellow << "length " << sf::Color::White
            << "was " + to_string(stat->snake_len);
        snake_len.setPosition(170.0f, 140.0f);

        game_time = sftk::TextBuilder{ label_font }
            << sftk::txt::size(24)
            << "game time was " << sf::Color::Green << stat->game_time<<"m";
        game_time.setPosition(180.0f, 180.0f);

        score = sftk::TextBuilder{ label_font }
            << sftk::txt::size(24)
            << "your score is " << sf::Color::Red << to_string(stat->score);
        score.setPosition(190.0f, 220.0f);


        restart = sftk::TextBuilder{ label_font }
            << sftk::txt::size(26)
            << "press " << sf::Color::Magenta << "R" << sf::Color::White << " to restart.";
        restart.setPosition(175.0f, 320.0f);
        delete stat;


        BaseEvent* check_restart = new SimpleEvent(INDEP, [&]()
            {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::R);
            },
            [&](){
                move_to_next = true;
            });
        event_manager.add(check_restart);
    }
    ~Death(){}

    void render(sf::RenderWindow& window)
    {
        window.draw(title);
        window.draw(snake_len);
        window.draw(game_time);
        window.draw(score);
        window.draw(restart);
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
