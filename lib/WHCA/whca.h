/**
 * @file whca.h
 * @author Mateusz Ambroży
 * @brief Implementation of WHCA algorithm
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "../Space-Time-AStar/Cell.h"
#include "../AStar/AStar.h"
#include "../Graph.h"
#include "../TaskGroup.h"
#include "../TSP.h"
#include "WHCA_Agent.h"
#include "../Space-Time-AStar/Reservation.h"
#include <chrono>
#include <unordered_set>
#include <set>
struct TupleHash {
    std::size_t operator()(const std::tuple<int, int, int>& t) const {
        return std::hash<int>()(std::get<0>(t)) ^
               (std::hash<int>()(std::get<1>(t)) << 1) ^
               (std::hash<int>()(std::get<2>(t)) << 2);
    }
};

// Operator porównania (domyślnie porównuje std::tuple)
struct TupleEqual {
    bool operator()(const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs) const {
        return lhs == rhs;
    }
};
class WHCA: public A::Astar
{
public:
private:
    std::vector<SpaceTime::Cell> getNeighbors(const WHCA_Agent& agent, map::Cell target, SpaceTime::Node &current, const Reservation &table, std::vector<std::vector<bool>> closedList);

    void extracted();
    std::vector<SpaceTime::Cell> pathToTarget(WHCA_Agent &unit, map::Cell start,
                                              map::Cell target, int currentTime,
                                              Reservation &table);
    std::vector<SpaceTime::Cell> reconstructPath(const SpaceTime::Node& node);
    bool recursiveWaitAndReturn(WHCA_Agent& unit, const int waitTime, const map::Cell& target, SpaceTime::Cell* currentCell, SpaceTime::Cell* parentCell, int& waitT, std::vector<SpaceTime::Cell>& path, Reservation& table);
    bool isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table);
    SpaceTime::Cell findAlternativeWaitingCell(const WHCA_Agent& agent, const SpaceTime::Cell& cell, int currentTime, Reservation& table);
public:
    WHCA(map::Graph graph);
    std::vector<SpaceTime::Cell> findNextWSteps(WHCA_Agent& unit, int currentTime, Reservation& table);
};


