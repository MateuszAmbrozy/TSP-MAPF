#include "../lib/Agent.h"

Agent::Agent(int id, int capacity, map::Cell position)
    : id(id), capacity(capacity), accessibleCapacity(capacity),
         position(position), startPos(position) , idle(true)
         {
            
         }

// Agent::Agent(const Agent &other)
//  :  id(other.id),
//     capacity(other.capacity),
//     accessibleCapacity(other.accessibleCapacity),
//     position(other.position),
//     tasks(other.tasks),
//     startPos(other.startPos),
//     path(other.path),
//     illicits(other.illicits),
//     idle(other.idle)
//          {}

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

std::vector<SpaceTime::Cell> Agent::getPath() const
{
    return this->path;
}

map::Cell Agent::getPosition() const {
    return position;
}
map::Cell Agent::getStartPosition() const {
    return startPos;
}
TaskGroup Agent::getTask() const
{
    return this->tasks;
}

std::vector<map::Cell> Agent::getIllicits() const
{
    return illicits;
}
void Agent::assignTask(TaskGroup taskGroup) {
        tasks = taskGroup;
}
void Agent::clearTask()
{
    assignTask(TaskGroup{});
    setIdle(true);
}

void Agent::assignPath(std::vector<SpaceTime::Cell> path)
{
    this->path = path;
}
void Agent::clearPath()
{
    assignPath(std::vector<SpaceTime::Cell>{});
}

void Agent::setIdle(bool state)
{
    this->idle = state;
}
void Agent::move(map::Cell new_position) {
    position = new_position;
}

bool Agent::isIdle() const {
    return tasks.getNumTasks() == 0;
}

void Agent::addIllicitCell(const map::Cell& cell)
{
    illicits.push_back(cell);
}

bool Agent::operator ==(const Agent& other) const
{
    return id==other.id &&
           capacity == other.capacity &&
           accessibleCapacity == other.accessibleCapacity &&
           position == other.position &&
           tasks == other.tasks &&
           startPos == other.startPos && 
           path == other.path&&
           illicits == other.illicits;
}
