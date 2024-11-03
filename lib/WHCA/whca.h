#pragma once
#include "../Space-Time-AStar/Cell.h"
#include "../AStar/AStar.h"
#include "../Graph.h"
#include "../TaskGroup.h"
#include "../TSP.h"
#include "WHCA_Agent.h"
#include "../Space-Time-AStar/Reservation.h"
class WHCA: public A::Astar
{
public:
private:
    std::vector<SpaceTimeCell::Cell> getNeighbors(const WHCA_Agent& agent, SpaceTimeCell::Node &current, const Reservation &table);

    std::vector<SpaceTimeCell::Cell> pathToTarget(WHCA_Agent &unit, int currentTime, Reservation& table);

    bool recursiveWaitAndReturn(SpaceTimeCell::Cell* currentCell, SpaceTimeCell::Cell* parentCell, int& waitT, std::vector<SpaceTimeCell::Cell>& path, Reservation& table);
    bool isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table);
    SpaceTimeCell::Cell findAlternativeWaitingCell(const SpaceTimeCell::Cell& cell, int currentTime, Reservation& table);
public:
    WHCA(map::Graph graph);
    std::vector<SpaceTimeCell::Cell> findNextWSteps(WHCA_Agent& unit, int currentTime, Reservation& table);
};


