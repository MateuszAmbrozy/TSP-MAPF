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
#include "../CA/Cell.h"
#include "../AStar/AStar.h"
#include "../Graph.h"
#include "../TaskGroup.h"
#include "../TSP.h"
#include "WCA_Agent.h"
#include "../CA/Reservation.h"
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

struct TupleEqual {
    bool operator()(const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs) const {
        return lhs == rhs;
    }
};
class WindowCooperativeA: public A::Astar
{
public:
private:
    std::vector<CA::Cell> getNeighbors(const WCA_Agent& agent, map::Cell target, CA::Node &current, const Reservation &table);

    void extracted();
    std::vector<CA::Cell> pathToTarget(WCA_Agent &unit, map::Cell start,
                                              map::Cell target, int currentTime,
                                              Reservation &table);
    std::vector<CA::Cell> reconstructPath(const CA::Node& node);
    bool recursiveWaitAndReturn(WCA_Agent& unit, const int waitTime, const map::Cell& target, CA::Cell* currentCell, CA::Cell* parentCell, int& waitT, std::vector<CA::Cell>& path, Reservation& table);
    bool isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table);
    CA::Cell findAlternativeWaitingCell(const WCA_Agent& agent, const CA::Cell& cell, int currentTime, Reservation& table);
public:
    WindowCooperativeA(map::Graph graph);
    std::vector<CA::Cell> findNextWSteps(WCA_Agent& unit, int currentTime, Reservation& table);
};


