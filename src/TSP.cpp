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
double TSP::euclideanDistance(const map::Cell& a, const map::Cell& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}
std::vector<std::vector<double>> TSP::calcDistanceMatrix(const Agent& agent, const TaskGroup& taskGroup) {
    int n = taskGroup.getNumTasks();
    std::vector<std::vector<double>> distanceMatrix(n + 2, std::vector<double>(n + 2, 0));

    map::Cell agentPos = agent.getPosition();
    map::Cell dropoff = taskGroup.getDropoffLocation();

    for (int i = 0; i < n; ++i) {
        map::Cell pickup = taskGroup.getPickupLocation(i);

        distanceMatrix[0][i + 1] = euclideanDistance(agentPos, pickup);
        distanceMatrix[i + 1][0] = distanceMatrix[0][i + 1];

        distanceMatrix[i + 1][n + 1] = euclideanDistance(pickup, dropoff);
        distanceMatrix[n + 1][i + 1] = distanceMatrix[i + 1][n + 1];
    }


    for (int i = 0; i < n; ++i) {
        map::Cell locI = taskGroup.getPickupLocation(i);
        for (int j = i + 1; j < n; ++j) {
            map::Cell locJ = taskGroup.getPickupLocation(j);
            distanceMatrix[i + 1][j + 1] = euclideanDistance(locI, locJ);
            distanceMatrix[j + 1][i + 1] = distanceMatrix[i + 1][j + 1];
        }
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
    std::vector<int> path(n + 2); 


    for (int i = 1; i <= n; ++i) {
        path[i] = i;
    }
    path[0] = 0;  
    path[n + 1] = n + 1;  

    bool improvement = true;
    while (improvement) {
        improvement = false;
        double bestDistance = calculateTotalDistance(path, distanceMatrix);

        for (int i = 1; i < n; ++i) { 
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

    std::vector<int> optimizedTaskOrder(path.begin() + 1, path.end() - 1);
    std::transform(optimizedTaskOrder.begin(), optimizedTaskOrder.end(), optimizedTaskOrder.begin(), [](int i) { return --i; });

    return optimizedTaskOrder;
}
