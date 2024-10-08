#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include"BaseStateMachine.hpp"
#include"Snake.h"
#include"TetrisBlock.hpp"
#include"GameTimer.h"
#include"GameStatistic.hpp"
#include"SFML/Audio.hpp"
#include"FancyText.hpp"
#include"ColorManager.h"
#include"HighScoreManager.h"

class Game:public BaseStateMachine, public ColorManager
{
private:
    sf::RectangleShape apple = sf::RectangleShape(DRAWABLE_OBJECT_SIZE);
    sf::RectangleShape snake_head = sf::RectangleShape(DRAWABLE_OBJECT_SIZE);
    sf::RectangleShape border = sf::RectangleShape(sf::Vector2f(4.0f, CELL_SIZE*CELL_MAX));
    sf::RectangleShape block = sf::RectangleShape(DRAWABLE_OBJECT_SIZE);
    bool cleared = false;

    HighScoreManager hs_manager;

    sf::Clock clock;
    sf::Clock block_generator_clock;
    sf::Clock block_movement_clock;
    sf::Clock color_changer_clock;
    sf::Clock eat_all_label_clock;
  sf::Clock super_power_clock;

    Snake snake;
    stack<sf::Vector2u> snake_parts; //used for death
    vector<TetrisBLock*> tetris_blocks;
  
    Map* map = nullptr;
    Timer timer;
  sf::Text time, length, apples_to_grow, eat,move, score, user_name, fullness;
    sftk::FancyText title;
    sf::Text pause_label;

  sf::Text eat_all_label, eat_all_label_value;
  
    bool dying = false;//snake eats itself
    bool loosing = false; //there is block that reached the top

    bool pause_clicked = false;

  bool block_movement = false;
  vector<Direction> blocked_direction;

  sf::SoundBuffer eat_b, die_b, clear_b, start_b, end_b, x_b;
  sf::Sound _eat, die, clear, start_sound, end_sound, x_sound;
  sf::Music music;
  
  bool noneatable_exist = false;
  sf::Vector2u noneatable_snake_head;


  bool snake_direction_changed = false;
  sf::Vector2u snake_pos_before_direction_change;
  Direction old_direction;

  bool eat_all = false;
  
public:
	Game(const string& user_name)
	{
	  music.openFromFile("assets/game.ogg");
	  music.setVolume(35);
	  music.setLoop(true);
	  music.play();
	  this->user_name.setString(user_name);

	  eat_b.loadFromFile("assets/eat.wav");
	  _eat.setBuffer(eat_b);

	  die_b.loadFromFile("assets/die.wav");
	  die.setBuffer(die_b);

	  clear_b.loadFromFile("assets/clear.wav");
	  clear.setBuffer(clear_b);

	  start_b.loadFromFile("assets/start.wav");
	  start_sound.setBuffer(start_b);

	  x_b.loadFromFile("assets/x.wav");
	  x_sound.setBuffer(x_b);

	  color_changer_clock.restart();
	  block_generator_clock.restart();
	  block_movement_clock.restart();
	  clock.restart();
	  eat_all_label_clock.restart();
	  
	  apple.setFillColor(sf::Color::Red);
	  snake_head.setFillColor(sf::Color::Green);
	  border.setFillColor(border_color);
	  block.setFillColor(cell_color);

	  map = new Map(CELL_MAX, CELL_MAX, snake.get_head_pos());
	  tetris_blocks.push_back(generate_tetris_block(color_to_eat));
	  tetris_blocks[0]->set(map);//update first block at the beginning of game
   
	  set_text();

	  generate_color_to_eat();

	  create_key_events();
	  create_snake_events();
	  create_blocks_events();
	  create_text_events();
	}
	~Game()
	{
	  music.stop();
	  delete map;
	}

	//GameDrawingFunctions.cpp
	void render(sf::RenderWindow& window);

  bool is_victory()
  {
    return_value = new GameStatistic(snake.len(),
			timer.get_time(),
			snake.get_score(),
			!loosing);
    
    //if it's not victory, then it's death
    return snake.get_score() >= SCORE_TO_WIN;
  }
private:
	//KeyEventsCreation.cpp
	void create_key_events();

	//SnakeEventsCreation.cpp
	void create_snake_events();

	//BlocksEventsCreation.cpp
	void create_blocks_events();

	//TextEventsCreation.cpp
	void create_text_events();

	//GameDrawingFunctions.cpp
	void draw_border(sf::RenderWindow& window);
	void draw_text(sf::RenderWindow& window);
	void draw_shapes(sf::RenderWindow& window);
	//////

	//GameUtilFunctions.cpp
  bool does_new_dir_kills_snake(Direction dir);
  void move_blocks();
  bool can_snake_move();
  bool is_dir_blocked(Direction dir);
  void update_snake(size_t x, size_t y, bool should_decrement = true);
  bool is_pressed(sf::Keyboard::Key key);
  void clear_ground();
  
  short compute_fullness();
  std::string get_fullness_label(short fullness);
  /////


	//GameStateMachine.cpp
  void set_text();
};
#endif 
