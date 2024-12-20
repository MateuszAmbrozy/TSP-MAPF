/**
 * @file AStar.h
 * @author Mateusz Ambroży
 * @brief Typical A* algorithm
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "../Graph.h"
#include "../TaskGroup.h"
#include "../Agent.h"
#include "Node.h"
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>
#include <memory>
namespace A
{
    struct CompareCells {
        bool operator()(const A::Node a, const A::Node b) const {
            return a.fCost > b.fCost;
        }
    };

    class Astar
    {
        protected:
            map::Graph graph;
            std::vector<std::vector<Node>> aStarGraph;

            int lengthOfPath(std::shared_ptr<A::Node> node);
            void copyGraph();
            bool isValid(int x, int y);
            bool isDestination(int startX, int startY, int destX, int destY);
            

        public:
            Astar(map::Graph graph);
            int calculate(map::Cell start, map::Cell dest);
    };   
}
