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
#include "../Space-Time-AStar/Cell.h"
#include <vector>

class WHCA_Agent{
private:
    std::vector<std::pair<map::Cell, int>> waypoints; //waypints key-position, value-stoptime
    int currentWaypointIndex;

public:
    WHCA_Agent(const Agent& agent);
    int W;
    int K;
    Agent agent;
    // Metody dostępu
    std::vector<SpaceTime::Cell> getCurrentPath() const;
    int getCurrentWaypointIndex() const;
    bool atCurrentWaypoint() const;
    std::vector<std::pair<map::Cell, int>> getWaypoints();

    void incrementWaypointIndex();
    void clearWaypoints();
    void clearCurrentWayPointIndex();

    // Sprawdza, czy agent osiągnął bieżący punkt docelowy
    bool reachedCurrentWaypoint() const;

    void setWaypoints(const std::vector<std::pair<map::Cell, int> > &newGetWaypoints);

    bool finished();
    bool operator ==(const WHCA_Agent& other) const;
};

