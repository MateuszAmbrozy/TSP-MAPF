/**
 * @file TSP.h
 * @author Mateusz Ambroży
 * @brief Implementation of TSP(Traveling Salesman Problem)
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "AStar/AStar.h"
#include "Graph.h"
#include "TaskGroup.h"
#include "Agent.h"

#include <cmath>
#include <iterator>
#include <iostream>
#include <numeric>
#include <queue>
#include <algorithm>

class Agent;

class TSP {
private:
    A::Astar astar;

    map::Graph graph;

    double calculateTotalDistance(const std::vector<int>& path, const std::vector<std::vector<double>>& distanceMatrix);
    std::vector<std::vector<double>> calcDistanceMatrix(const Agent& agent, const TaskGroup& taskGroup);
    std::vector<int> twoOptSwap(const std::vector<int>& path, int i, int j);
    double euclideanDistance(const map::Cell& a, const map::Cell& b);
public:
    TSP(map::Graph graph);

    std::vector<int> solveTSP(Agent agent, const TaskGroup& taskGroup); // Returns the indices of taskGroup in the optimized order
};
