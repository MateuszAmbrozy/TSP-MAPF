#pragma once
#include "../Graph.h"
#include "../TaskGroup.h"
#include "../Agent.h"
#include "Node.h"
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>

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

            int lengthOfPath(Node* node);
            void copyGraph();
            bool isValid(int x, int y);
            bool isDestination(int startX, int startY, int destX, int destY);
            

        public:
            Astar(map::Graph graph);
            int calculate(map::Cell start, map::Cell dest);
    };   
}
