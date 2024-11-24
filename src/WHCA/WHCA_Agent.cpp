#include "../../lib/WHCA/WHCA_Agent.h"
WHCA_Agent::WHCA_Agent(const Agent &agent)
    : currentWaypointIndex(0), W(10), K(5), agent(agent)
    {}


int WHCA_Agent::getCurrentWaypointIndex() const {
    return currentWaypointIndex;
}
bool WHCA_Agent::atCurrentWaypoint() const
{
    return currentWaypointIndex < static_cast<int>(waypoints.size()) && reachedCurrentWaypoint();
}


std::vector<std::pair<map::Cell, int> > WHCA_Agent::getWaypoints()
{
    return this->waypoints;
}

void WHCA_Agent::incrementWaypointIndex() {
    if(currentWaypointIndex < static_cast<int>(waypoints.size()) - 1)
        currentWaypointIndex++;
}

void WHCA_Agent::clearWaypoints()
{
    this->waypoints.clear();
}

void WHCA_Agent::clearCurrentWayPointIndex()
{
    this->currentWaypointIndex=0;
}

bool WHCA_Agent::reachedCurrentWaypoint() const
{
    const auto& waypoint = waypoints[currentWaypointIndex].first;
    return agent.getPosition() == waypoint;
}

void WHCA_Agent::setWaypoints(const std::vector<std::pair<map::Cell, int> > &newGetWaypoints)
{
    this->waypoints = newGetWaypoints;
}

bool WHCA_Agent::finished()
{
    return currentWaypointIndex >= static_cast<int>(waypoints.size()) - 1 && reachedCurrentWaypoint();
}
bool WHCA_Agent::operator ==(const WHCA_Agent& other) const
{
    return agent.getId() == other.agent.getId();
}
