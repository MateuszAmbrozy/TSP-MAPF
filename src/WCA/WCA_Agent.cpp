#include "../../lib/WCA/WCA_Agent.h"
WCA_Agent::WCA_Agent(const Agent &agent)
    : currentWaypointIndex(0), W(5), K(5), agent(agent)
    {}


int WCA_Agent::getCurrentWaypointIndex() const {
    return currentWaypointIndex;
}
bool WCA_Agent::atCurrentWaypoint() const
{
    return currentWaypointIndex < static_cast<int>(waypoints.size()) && reachedCurrentWaypoint();
}


std::vector<std::pair<map::Cell, int> > WCA_Agent::getWaypoints()
{
    return this->waypoints;
}

void WCA_Agent::incrementWaypointIndex() {
    if(currentWaypointIndex < static_cast<int>(waypoints.size()) - 1)
        currentWaypointIndex++;
}

void WCA_Agent::clearWaypoints()
{
    this->waypoints.clear();
}

void WCA_Agent::clearCurrentWayPointIndex()
{
    this->currentWaypointIndex=0;
}

bool WCA_Agent::reachedCurrentWaypoint() const
{
    const auto& waypoint = waypoints[currentWaypointIndex].first;
    return agent.getPosition() == waypoint;
}

void WCA_Agent::setWaypoints(const std::vector<std::pair<map::Cell, int> > &newGetWaypoints)
{
    this->waypoints = newGetWaypoints;
}

bool WCA_Agent::finished()
{
    return currentWaypointIndex >= static_cast<int>(waypoints.size()) - 1 && reachedCurrentWaypoint();
}
bool WCA_Agent::operator ==(const WCA_Agent& other) const
{
    return agent.getId() == other.agent.getId();
}
