#include "../lib/Agent.h"
#include <iostream>

Agent::Agent(int id, int capacity, map::Cell position)
    : id(id), capacity(capacity), accessibleCapacity(capacity),
         position(position) {}

int Agent::getId() const {
    return id;
}

int Agent::getCapacity() const {
    return capacity;
}

int Agent::getAccessibleCapacity() const
{
    return accessibleCapacity;
}

std::vector<SpaceTimeCell::Cell> Agent::getPath() const
{
    return this->path;
}

map::Cell Agent::getPosition() const {
    return position;
}
TaskGroup Agent::getTask() const
{
    return this->tasks;
}


void Agent::assignTask(TaskGroup taskGroup) {
        tasks = taskGroup;
}

void Agent::assignPath(const std::vector<SpaceTimeCell::Cell>& path)
{
    this->path = path;
}

void Agent::move(map::Cell new_position) {
    position = new_position;
}

bool Agent::isIdle() const {
    return tasks.getNumTasks() == 0;
}

