#pragma once
#include "Environment.h"
#include "Reserve.h"

class TSP_MAPD
{
private:
    Environment environment;
    
public:
    TSP_MAPD(); //konstruktor 

    Reserve algorithm(); //głowny algorytm

    
};