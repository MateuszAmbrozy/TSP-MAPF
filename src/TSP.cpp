#include "../lib/TSP.h"
//TSP
TSP::TSP(map::Graph graph)
    :astar(graph), graph(graph)
{}


double TSP::calculateTotalDistance(const std::vector<int>& path, const std::vector<std::vector<double>>& distanceMatrix) {
    double totalDistance = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalDistance += distanceMatrix[path[i]][path[i + 1]];
    }
    return totalDistance;
}

std::vector<std::vector<double>> TSP::calcDistanceMatrix(const Agent& agent, const TaskGroup& taskGroup) {
    int n = taskGroup.getNumTasks();
    std::vector<std::vector<double>> distanceMatrix(n + 2, std::vector<double>(n + 2, 0));  // Extra slots for agent's position and dropoff

    // Calculate distances between agent's position and pickup points
    for (int i = 0; i < n; ++i) {
        distanceMatrix[0][i + 1] = astar.calculate(agent.getPosition(), taskGroup.getPickupLocation(i));
        distanceMatrix[i + 1][0] = distanceMatrix[0][i + 1];  // Symmetric distances
    }

    // Calculate distances between pickup points
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            distanceMatrix[i + 1][j + 1] = astar.calculate(taskGroup.getPickupLocation(i), taskGroup.getPickupLocation(j));
            distanceMatrix[j + 1][i + 1] = distanceMatrix[i + 1][j + 1];  // Symmetric distances
        }
    }

    // Calculate distances between pickup points and the drop-off point
    map::Cell dropoff = taskGroup.getDropoffLocation();
    for (int i = 0; i < n; ++i) {
        distanceMatrix[i + 1][n + 1] = astar.calculate(taskGroup.getPickupLocation(i), dropoff);
        distanceMatrix[n + 1][i + 1] = distanceMatrix[i + 1][n + 1];  // Symmetric distances
    }

    return distanceMatrix;
}

std::vector<int> TSP::twoOptSwap(const std::vector<int>& path, int i, int j) {
    std::vector<int> newPath = path;
    std::reverse(newPath.begin() + i, newPath.begin() + j + 1);
    return newPath;
}

std::vector<int> TSP::solveTSP(Agent agent, const TaskGroup& taskGroup) {
    std::vector<std::vector<double>> distanceMatrix = this->calcDistanceMatrix(agent, taskGroup);
    int n = taskGroup.getNumTasks();
    std::vector<int> path(n + 2);  // Start from agent's position (index 0) and end at the drop-off point (index n+1)

    // Initialize path
    for (int i = 1; i <= n; ++i) {
        path[i] = i;
    }
    path[0] = 0;  // Start from agent's position
    path[n + 1] = n + 1;  // End at drop-off point

    // 2-opt algorithm to optimize the path
    bool improvement = true;
    while (improvement) {
        improvement = false;
        double bestDistance = calculateTotalDistance(path, distanceMatrix);

        for (int i = 1; i < n; ++i) {  // Skip start (agent) and end (dropoff)
            for (int j = i + 1; j <= n; ++j) {
                std::vector<int> newPath = twoOptSwap(path, i, j);
                double newDistance = calculateTotalDistance(newPath, distanceMatrix);

                if (newDistance < bestDistance) {
                    path = newPath;
                    bestDistance = newDistance;
                    improvement = true;
                }
            }
        }
    }

    // Return optimized task order (ignoring start and end points)
    std::vector<int> optimizedTaskOrder(path.begin() + 1, path.end() - 1);
    std::transform(optimizedTaskOrder.begin(), optimizedTaskOrder.end(), optimizedTaskOrder.begin(), [](int i) { return --i; });

    return optimizedTaskOrder;
}
