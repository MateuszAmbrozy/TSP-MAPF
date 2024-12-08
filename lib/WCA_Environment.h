/**
 * @file WHCA_Environment.h
 * @author Mateusz Ambroży
 * @brief Implementation of Environment for WHCA* algorithm
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <vector>
#include <optional>


//#include <windows.h>
#include  <random>
#include  <iterator>

#include "Agent.h"

#include "Graph.h"

#include "WCA/WindowCooperativeA.h"
#include "CA/Reservation.h"

#include "WCA/WCA_Agent.h"
#include "BaseEnvironment.h"



class WCA_Environment: public BaseEnvironment
{
private:
    std::vector<WCA_Agent> agents;
    std::vector<WCA_Agent> vacant_agents;

    WindowCooperativeA whca;

    std::vector<WCA_Agent> capacity(const TaskGroup& task) const;
    std::optional<std::reference_wrapper<WCA_Agent>> random(std::vector<WCA_Agent>& capableAgents) const;



public:
    WCA_Environment(std::vector<Agent> agents, map::Graph graph, std::vector<std::pair<int, int>> avaliablePickups, std::vector<std::pair<int, int>> avaliableDropoffs);


    std::vector<Agent> getAgents() override{
        std::vector<Agent> buf;
        for (const WCA_Agent& a : agents) {
            buf.push_back(a.agent);
        }
        return buf;
    }

    std::vector<Agent> getVacantAgents() override
    {
        std::vector<Agent> buf;
        for(const auto &agent: vacant_agents)
            buf.push_back(agent.agent);
        return buf;
    }

    void assignVacantAgents() override;
    bool allTasksCompleted();
    void MOVEAGENTS(int timestep) override;
    void addAgent(Agent &newAgent) override;


    void runTimestep(int timestep, TaskGroup* task = nullptr) override;
};

