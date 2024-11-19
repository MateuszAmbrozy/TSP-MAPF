/**
 * @file Environment.h
 * @author Mateusz Ambroży
 * @brief Cooperative A* Environment 
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <ctime>
#include <unordered_map>
#include "Agent.h"

#include "Graph.h"
#include "TSP.h"
#include "setup.h"
#include "Space-Time-AStar/Reservation.h"
#include "Space-Time-AStar/SpaceTimeAStar.h"
#include "BaseEnvironment.h"


class CA_Environment : public BaseEnvironment
{
private:
    std::vector<Agent> agents;
    std::vector<Agent> vacant_agents;

    SpaceTimeAStar sta;


    std::vector<Agent> capacity(const TaskGroup& task) const; //wyszukuje agentow spelniajacych ograniczenie pojemnosci z agentow wakacyjnych
    std::optional<Agent> random(std::vector<Agent>& capableAgents) const;

public:
    CA_Environment(std::vector<Agent> agents, map::Graph graph, std::vector<std::pair<int, int>> avaliablePickups, std::vector<std::pair<int, int>> avaliableDropoffs);


    std::vector<Agent> getAgents() {return this->agents;}
    std::vector<Agent> getVacantAgents() {return this->vacant_agents;}

   void assignVacantAgents();
    bool allTasksCompleted(); 

    void MOVEAGENTS(int timestep);
    void addAgent(Agent& newAgent);
    void mainAlgorithm();
    void runTimestep(int timestep, TaskGroup* task = nullptr);
};
