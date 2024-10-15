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
    map::Cell position; 
    TaskGroup tasks;  
    map::Cell startPos;
    std::vector<SpaceTimeCell::Cell> path;
    std::vector<map::Cell> illicits; //cells that agent cant enter becauese they are 
    bool idle;

public:
    Agent(int id, int capacity, map::Cell position);

    int getId() const;
    
    int getCapacity() const;
    int getAccessibleCapacity() const;
    std::vector<SpaceTimeCell::Cell> getPath() const;
    map::Cell getPosition() const;
    TaskGroup getTask() const;
    std::vector<map::Cell> getIllicits() const;

    void assignTask(const TaskGroup taskGroup);
    void clearTask();
    void assignPath(std::vector<SpaceTimeCell::Cell> path);  // Pass by reference
    void clearPath();

    void setIdle(bool state);
    bool isIdle() const;

    void addIllicitCell(const map::Cell& cell);
    bool operator ==(const Agent& other) const; 
    
    void move(map::Cell new_position);
};
