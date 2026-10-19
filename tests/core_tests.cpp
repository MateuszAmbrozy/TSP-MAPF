#include "AStar/AStar.h"
#include "CA/Reservation.h"
#include "TSP.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

int main() {
    const int unreachable = std::numeric_limits<int>::max();
    map::Graph graph(5, 5, {{2, 0}, {2, 1}, {2, 2}, {2, 3}});
    A::Astar astar(graph);
    check(astar.calculate({0, 0}, {4, 0}) == 12, "A* detours around obstacles");
    check(astar.calculate({1, 1}, {1, 1}) == 0, "A* identical endpoints");
    check(astar.calculate({-1, 0}, {4, 0}) == unreachable, "invalid start");
    check(astar.calculate({2, 0}, {2, 0}) == unreachable, "blocked start and goal");
    check(astar.calculate({0, 0}, {5, 0}) == unreachable, "invalid destination");
    A::Astar empty(map::Graph{});
    check(empty.calculate({0, 0}, {0, 0}) == unreachable, "empty graph");
    A::Astar disconnected(map::Graph(3, 1, {{1, 0}}));
    check(disconnected.calculate({0, 0}, {2, 0}) == unreachable, "unreachable goal");

    Reservation reservations;
    reservations.reserve(1, 2, 3);
    check(reservations.isReserved(1, 2, 3), "vertex reservation");
    check(!reservations.isReserved(1, 2, 4), "reservations are time-dependent");
    reservations.removeReservation(1, 2, 3);
    check(!reservations.isReserved(1, 2, 3), "vertex release");
    reservations.reserveEdge(0, 0, 1, 0, 2);
    check(reservations.wouldCauseEdgeCollision(1, 0, 2, 0, 0), "reverse edge conflict");
    check(!reservations.wouldCauseEdgeCollision(1, 0, 3, 0, 0), "edge conflict time");
    reservations.removeEdgeReservation(0, 0, 2);
    check(!reservations.wouldCauseEdgeCollision(1, 0, 2, 0, 0), "edge release");

    TSP tsp(graph);
    Agent agent(0, 10, {0, 0});
    TaskGroup task(1, {{4, 0}, {1, 0}, {3, 0}}, {4, 1}, {0, 0, 0}, 0);
    auto order = tsp.solveTSP(agent, task);
    check(order == std::vector<int>({1, 2, 0}), "2-opt improves pickup order");
    check(tsp.solveTSP(agent, TaskGroup{}).empty(), "empty task order");
    check(TaskGroup{}.getCapacity() == 0, "default task is initialized");
    std::ostringstream output;
    output << task;
    check(output.str().find("dropoffPoint") != std::string::npos, "task uses supplied stream");
    std::cout << "All core checks passed.\n";
}
