#include "../lib/TaskGroup.h"


TaskGroup::TaskGroup(int capacity, std::vector<map::Cell> pickup_points, map::Cell dropoff, std::vector<int> stop_times, int dropoff_stop_time)
    : capacity(capacity), pickup_points(pickup_points), dropoff(dropoff), stop_times(stop_times), dropoff_stop_time(dropoff_stop_time) {}

int TaskGroup::getCapacity() const {
    return capacity;
}

map::Cell TaskGroup::getPickupLocation(int index) const {
    if (index >= 0 && index < pickup_points.size()) {
        return pickup_points[index];
    }
    throw std::out_of_range("Invalid pickup point index");
}

map::Cell TaskGroup::getDropoffLocation() const {
    return dropoff;
}
int TaskGroup::getPickupStopTime(int index) const  // Get the stop time for a specific pickup point
{
    return stop_times.at(index);
}
int TaskGroup::getDropoffStopTime() const  // Get the stop time for the dropoff point
{
return dropoff_stop_time;
}

int TaskGroup::getNumTasks() const {
    return pickup_points.size();
}

void TaskGroup::setPickupPoints(std::vector<map::Cell> pickup_points)
{
    this->pickup_points = pickup_points;
}
void TaskGroup::setStopTimes(std::vector<int> stop_times, int dropoff_stop_time)
{
    this->stop_times = stop_times;
    this->dropoff_stop_time = dropoff_stop_time;
}
map::Cell &TaskGroup::operator[](int idx)
{
    if (idx >= 0 && idx < pickup_points.size()) {
        return pickup_points[idx];
    }
    throw std::out_of_range("Invalid pickup point index");
}
std::ostream& operator<<(std::ostream& os, const TaskGroup& task)
{
    for(int i=0; i<task.pickup_points.size(); i++)
    {
        os << "(" << task.pickup_points[i].x << ", " << task.pickup_points[i].y << ") -> "; 
    }
    os << std::endl;
    return os;
    
    
}