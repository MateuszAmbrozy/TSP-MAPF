/**
 * @file WHCA_Agent.h
 * @author Mateusz Ambroży
 * @brief Agent using in WHCA algorithm
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "../Agent.h"
#include "../Graph.h"
#include "../CA/Cell.h"
#include <vector>

class WCA_Agent{
private:
    std::vector<std::pair<map::Cell, int>> waypoints;
    int currentWaypointIndex;

public:
    WCA_Agent(const Agent& agent);
    int W;
    int K;
    Agent agent;
    std::vector<CA::Cell> getCurrentPath() const;
    int getCurrentWaypointIndex() const;
    bool atCurrentWaypoint() const;
    std::vector<std::pair<map::Cell, int>> getWaypoints();

    void incrementWaypointIndex();
    void clearWaypoints();
    void clearCurrentWayPointIndex();

    bool reachedCurrentWaypoint() const;

    void setWaypoints(const std::vector<std::pair<map::Cell, int> > &newGetWaypoints);

    bool finished();
    bool operator ==(const WCA_Agent& other) const;
};

