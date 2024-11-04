#pragma once
#include <vector>
#include <optional>


#include <windows.h>
#include  <random>
#include  <iterator>

#include "Agent.h"

#include "Graph.h"

#include "WHCA/whca.h"
#include "Space-Time-AStar/Reservation.h"

#include "WHCA/WHCA_Agent.h"
#include "BaseEnvironment.h"



class WHCA_Environment: public BaseEnvironment
{
private:
    std::vector<WHCA_Agent> agents;
    std::vector<WHCA_Agent> vacant_agents;

    WHCA whca;

    std::vector<WHCA_Agent> capacity(const TaskGroup& task) const;
    std::optional<WHCA_Agent> random(std::vector<WHCA_Agent>& capableAgents) const;



public:
    WHCA_Environment(std::vector<Agent> agents, map::Graph graph);


    std::vector<Agent> getAgents() {
        std::vector<Agent> buf;
        for (const WHCA_Agent& a : agents) {
            buf.push_back(a.agent);  // Dostęp do agent w środku AgentWrapper
        }
        return buf;
    }

    std::vector<Agent> getVacantAgents()
    {
        std::vector<Agent> buf;
        for(const auto &agent: vacant_agents)
            buf.push_back(agent.agent);
        return buf;
    }

    void assignVacantAgents();

    void MOVEAGENTS(int timestep);
    void addAgent(Agent &newAgent);


    void runTimestep(int timestep, TaskGroup* task = nullptr);
};

