#pragma once
#include "Space-Time-AStar/Cell.h"
#include "TaskGroup.h"
#include "Graph.h"
#include <vector>


class Agent {
private:
    int id;
    int capacity;
    int accessibleCapacity;
    map::Cell position;  // Grid coordinates
    TaskGroup tasks;  // Current assigned tasks
    std::vector<SpaceTimeCell::Cell> path;  // Correct use of SpaceTimeCell namespace

public:
    Agent(int id, int capacity, map::Cell position);

    int getId() const;
    int getCapacity() const;
    int getAccessibleCapacity() const;
    std::vector<SpaceTimeCell::Cell> getPath() const;
    map::Cell getPosition() const;
    TaskGroup getTask() const;

    void assignTask(const TaskGroup taskGroup);

    void assignPath(const std::vector<SpaceTimeCell::Cell>& path);  // Pass by reference

    void move(map::Cell new_position);
    bool isIdle() const;
};
