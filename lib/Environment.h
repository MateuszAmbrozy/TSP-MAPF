#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <ctime>
#include <unordered_map>
//#include <chrono>


#include "Agent.h"

#include "Graph.h"
#include "TSP.h"
#include "setup.h"
#include "Space-Time-AStar/Reservation.h"
#include "Space-Time-AStar/SpaceTimeAStar.h"
#include "BaseEnvironment.h"


class Environment : public BaseEnvironment
{
private:
    std::vector<Agent> agents;
    std::vector<Agent> vacant_agents;


    SpaceTimeAStar sta;


    std::vector<Agent> capacity(const TaskGroup& task) const; //wyszukuje agentow spelniajacych ograniczenie pojemnosci z agentow wakacyjnych
    std::optional<Agent> random(std::vector<Agent>& capableAgents) const;

public:
    Environment(std::vector<Agent> agents, map::Graph graph);


    std::vector<Agent> getAgents() {return this->agents;}
    std::vector<Agent> getVacantAgents() {return this->vacant_agents;}

   void assignVacantAgents();

    void MOVEAGENTS(int timestep);
    void addAgent(Agent& newAgent);
    void mainAlgorithm();
    void runTimestep(int timestep, TaskGroup* task = nullptr);
};
