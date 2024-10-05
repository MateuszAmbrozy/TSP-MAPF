#pragma once
#include <vector>
#include <utility>
class setup
{
public:
    // Deklaracja statycznych zmiennych członkowskich
    static const std::vector<std::pair<int, int>> moves;
    static const unsigned int max_time;
    static const unsigned int maxTasks;
    static const unsigned int maxCapacity;
    static const unsigned int maxStopTime;
};


//const unsigned int setup::max_time = 100;