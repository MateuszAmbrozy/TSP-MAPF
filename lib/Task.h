#pragma once

#include <utility>
#include <vector>
#include "Graph.h"

class Task {
private:
//int id;       //chyba nie jest potrzebne tutaj id dla zadań
    int capacity;
    map::Cell pickup;
    map::Cell dropoff;
public:
    Task(int capacity, map::Cell pickup, map::Cell dropoff);

    // int getId() const;
    int getCapacity() const;
    map::Cell getPickupLocation() const;
    map::Cell getDropoffLocation() const;
};
