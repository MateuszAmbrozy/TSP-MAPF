#include "../lib/Task.h"

Task::Task(int capacity, map::Cell pickup, map::Cell dropoff)
    :   capacity(capacity), pickup(pickup), dropoff(dropoff) {}

// int Task::getId() const {
//     return id;
// }

int Task::getCapacity() const
{
    return capacity;
}

map::Cell Task::getPickupLocation() const {
    return pickup;
}

map::Cell Task::getDropoffLocation() const {
    return dropoff;
}
