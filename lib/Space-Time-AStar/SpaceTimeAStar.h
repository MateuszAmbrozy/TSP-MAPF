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
class SpaceTimeAStar :public A::Astar
{
private:
    std::vector<SpaceTime::Cell> getNeighbors(const Agent& agent, map::Cell target, SpaceTime::Node &current, const Reservation &table, std::vector<std::vector<bool>> closedList);
    std::vector<SpaceTime::Cell> pathToTarget(const Agent& unit, const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table);
    bool recursiveWaitAndReturn(const Agent& unit, const int waitTime, const map::Cell& target, SpaceTime::Cell* currentCell, SpaceTime::Cell* parentCell, int& waitT, std::vector<SpaceTime::Cell>& path, Reservation& table);
    bool isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table);
    SpaceTime::Cell findAlternativeWaitingCell(const Agent& unit, const SpaceTime::Cell& cell, int currentTime, Reservation& table);
public:
    SpaceTimeAStar(map::Graph graph);
    std::vector<SpaceTime::Cell> findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table);
};
