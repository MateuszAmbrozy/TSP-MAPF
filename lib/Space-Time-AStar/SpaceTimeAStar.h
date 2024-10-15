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


class SpaceTimeAStar :public A::Astar
{
private:
    std::vector<SpaceTimeCell::Cell> getNeighbors(const Agent& agent, SpaceTimeCell::Node &current, const Reservation &table);
    std::vector<SpaceTimeCell::Cell> pathToTarget(const Agent& unit, const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table);
    //std::vector<SpaceTimeCell::Cell> solveCollision(std::vector<SpaceTimeCell::Cell>& path, Reservation& table, const std::vector<map::Cell>& waypoints);
    //std::vector<SpaceTimeCell::Cell> pathFromWaypoints(const SpaceTimeCell::Cell& start, int startTime, const std::vector<map::Cell>& waypoints, size_t waypointIndex, Reservation& table);
    bool recursiveWaitAndReturn(SpaceTimeCell::Cell* currentCell, SpaceTimeCell::Cell* parentCell, int& waitT, std::vector<SpaceTimeCell::Cell>& path, Reservation& table);
    bool isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table);
    SpaceTimeCell::Cell findAlternativeWaitingCell(const SpaceTimeCell::Cell& cell, int currentTime, Reservation& table);
public:
    SpaceTimeAStar(map::Graph graph);

    std::vector<SpaceTimeCell::Cell> findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table);
};