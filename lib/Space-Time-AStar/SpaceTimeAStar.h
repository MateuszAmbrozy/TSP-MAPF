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
    std::vector<SpaceTimeCell::Cell> getNeighbors(SpaceTimeCell::Node &current, const Reservation &table);
    std::vector<SpaceTimeCell::Cell> pathToTarget(const map::Cell& start, int currentTime, const map::Cell& target, Reservation& table);
    std::vector<SpaceTimeCell::Cell> solveCollision(std::vector<SpaceTimeCell::Cell>& path, Reservation& table, const std::vector<map::Cell>& waypoints);
    std::vector<SpaceTimeCell::Cell> pathFromWaypoints(const SpaceTimeCell::Cell& start, int startTime, const std::vector<map::Cell>& waypoints, size_t waypointIndex, Reservation& table);
public:
    SpaceTimeAStar(map::Graph graph);

    std::vector<SpaceTimeCell::Cell> findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table);
};