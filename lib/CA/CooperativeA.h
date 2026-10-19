/**
 * @file SpaceTimeAStar.h
 * @author Mateusz Ambroży
 * @brief Implementation of Space Time A* 
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <vector>

#include "Cell.h"
#include "../AStar/AStar.h"
#include "../Graph.h"
#include "../TaskGroup.h"
#include "../TSP.h"
#include "../Agent.h"
#include "Reservation.h"
#include "../setup.h"
#include <unordered_set>
#include <chrono>
class CooperativeA :public A::Astar
{
private:
    std::vector<CA::Cell> getNeighbors(const Agent& agent, map::Cell target, CA::Node &current, const Reservation &table, std::vector<std::vector<bool>> closedList);
    std::vector<CA::Cell> pathToTarget(const Agent& unit, const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table);
    bool recursiveWaitAndReturn(const Agent& unit, const int waitTime, const map::Cell& target, CA::Cell* currentCell, CA::Cell* parentCell, int& waitT, std::vector<CA::Cell>& path, Reservation& table);
    bool isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table);
    CA::Cell findAlternativeWaitingCell(const Agent& unit, const CA::Cell& cell, int currentTime, Reservation& table);
public:
    CooperativeA(map::Graph graph);
    std::vector<CA::Cell> findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table);
};
