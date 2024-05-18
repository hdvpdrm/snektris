#ifndef DEATH_STATE_MACHINE_MANAGER_HPP
#define DEATH_STATE_MACHINE_MANAGER_HPP
#include"BaseStateMachine.hpp"
#include"GameStatistic.hpp"
#include"FancyText.hpp"

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
#endif //DEATH_STATE_MACHINE_MANAGER_HPP
