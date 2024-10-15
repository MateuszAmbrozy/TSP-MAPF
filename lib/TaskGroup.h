#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include "Graph.h"


class TaskGroup {
private:
    int capacity;
    std::vector<map::Cell> pickup_points;  // Pickup locations for tasks
    std::vector<int> stop_times;
    map::Cell dropoff;  // Fixed drop-off location for all agents
    int dropoff_stop_time;  // Stop time at the dropoff point

public:
    TaskGroup() = default;
    TaskGroup(int capacity, std::vector<map::Cell> pickup_points, map::Cell dropoff, std::vector<int> stop_times, int dropoff_stop_time);


    int getCapacity() const;
    map::Cell getPickupLocation(int index) const;  // Get a specific pickup point by index
    map::Cell getDropoffLocation() const;  // Get the fixed dropoff location

    int getPickupStopTime(int index) const;  // Get the stop time for a specific pickup point
    int getDropoffStopTime() const;  // Get the stop time for the dropoff point


    void setPickupPoints(std::vector<map::Cell> pickup_points);
    void setStopTimes(std::vector<int> stop_times, int dropoff_stop_time);
    int getNumTasks() const;  // Return the number of pickup points

    map::Cell& operator [](int idx);
    bool operator==(const TaskGroup& other) const;

    friend  std::ostream& operator<<(std::ostream& os, const TaskGroup& task);  
};
