/**
 * @file Agent.h
 * @author Mateusz Ambroży
 * @brief Agent background class storing all necessery variables
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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
    std::vector<SpaceTime::Cell> path;
    std::vector<map::Cell> illicits; //cells that agent cant enter becauese they are 
    bool idle;


public:
    Agent(int id, int capacity, map::Cell position);
    Agent(const Agent& other) = default;
    ~Agent() = default;

    int getId() const;
    
    int getCapacity() const;
    int getAccessibleCapacity() const;
    std::vector<SpaceTime::Cell> getPath() const;
    map::Cell getPosition() const;
    map::Cell getStartPosition() const;
    TaskGroup getTask() const;

    std::vector<map::Cell> getIllicits() const;

    void assignTask(const TaskGroup taskGroup);
    void clearTask();
    void assignPath(std::vector<SpaceTime::Cell> path);  // Pass by reference
    void clearPath();

    void setIdle(bool state);
    bool isIdle() const;

    void addIllicitCell(const map::Cell& cell);
    bool operator ==(const Agent& other) const; 
    
    void move(map::Cell new_position);
};
