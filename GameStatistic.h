#ifndef GAME_STATISTIC_H
#define GAME_STATISTIC_H
#include<string>
/*
  Structure represents information shown after death.
*/
struct GameStatistic
{
    int snake_len, score;
    std::string game_time;
    bool eat_itself;
    GameStatistic(int snake_len,
                  std::string game_time,
                  int score,
                  bool eat_itself):snake_len(snake_len),
                             game_time(game_time),
                             score(score),
                             eat_itself(eat_itself)
    {}
    ~GameStatistic(){}
};
#endif //GAME_STATISTIC_H
