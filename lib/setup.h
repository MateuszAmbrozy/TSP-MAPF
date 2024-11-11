/**
 * @file setup.h
 * @author Mateusz Ambroży
 * @brief constans variables 
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <vector>
#include <utility>
class setup
{
public:
    static const std::vector<std::pair<int, int>> moves;
    static const unsigned int max_time;
    static const unsigned int maxTasks;
    static const unsigned int maxCapacity;
    static const unsigned int maxStopTime;
};

enum class AlgType {
    A_STAR = 0,
    WHCA
};
