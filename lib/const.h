#pragma once
#include <vector>
#include <utility>
class setup
{
public:
    // Deklaracja statycznych zmiennych członkowskich
    static const std::vector<std::pair<int, int>> moves;
    static const unsigned int max_time;
    

    // Konstruktor
    setup()
    {
        // Statyczne zmienne nie mogą być inicjalizowane tutaj
    }
};

const std::vector<std::pair<int, int>> setup::moves{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
const unsigned int setup::max_time = 100;