#include "AStar/AStar.h"
#include "TSP.h"
#include <iostream>

int main() {
    map::Graph graph(8, 6, {{3, 1}, {3, 2}, {3, 3}});
    Agent agent(0, 10, {0, 0});
    TaskGroup task(3, {{6, 4}, {1, 2}, {5, 0}}, {7, 5}, {0, 0, 0}, 0);
    TSP optimizer(graph);
    const auto order = optimizer.solveTSP(agent, task);
    std::cout << "Pickup order (zero-based indices):";
    for (int index : order) std::cout << ' ' << index;
    A::Astar astar(graph);
    std::cout << "\nShortest grid distance from start to dropoff: "
              << astar.calculate(agent.getPosition(), task.getDropoffLocation())
              << "\nRun tsp_mapf_gui for the multi-agent simulation.\n";
}
