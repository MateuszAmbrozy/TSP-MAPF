/**
 * @file TaskGroup.h
 * @author Mateusz Ambroży
 * @brief Implementation of Task class
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <iostream>
#include <vector>
#include "Graph.h"


class TaskGroup {
private:
    int capacity;
    std::vector<map::Cell> pickup_points; 
    std::vector<int> stop_times;
    map::Cell dropoff; 
    int dropoff_stop_time;  

public:
    TaskGroup() = default;
    TaskGroup(int capacity, std::vector<map::Cell> pickup_points, map::Cell dropoff, std::vector<int> stop_times, int dropoff_stop_time);


    int getCapacity() const;
    map::Cell getPickupLocation(int index) const; 
    map::Cell getDropoffLocation() const;  

    std::vector<map::Cell> getPickupPoints() const;
    int getPickupStopTime(int index) const;  
    int getDropoffStopTime() const; 


    void setPickupPoints(std::vector<map::Cell> pickup_points);
    void setStopTimes(std::vector<int> stop_times, int dropoff_stop_time);
    int getNumTasks() const;  

    map::Cell& operator [](int idx);
    bool operator==(const TaskGroup& other) const;

    friend  std::ostream& operator<<(std::ostream& os, const TaskGroup& task);  
};
