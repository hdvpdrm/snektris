#ifndef GET_RANDOM_NUMBER_H
#define GET_RANDOM_NUMBER_H
#include<random>

static int get_random_int(int min, int max)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> gen(min, max);
    return gen(rng);
}
#endif