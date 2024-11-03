#pragma once
#include <vector>
#include <optional>


#include <windows.h>
#include  <random>
#include  <iterator>

#include "Agent.h"
#include "TaskGroup.h"
#include "Graph.h"
#include "TSP.h"
#include "WHCA/whca.h"
#include "Space-Time-AStar/Reservation.h"

#include "WHCA/WHCA_Agent.h"




class WHCA_Environment {
private:
    std::vector<WHCA_Agent> agents;
    std::vector<WHCA_Agent> vacant_agents;

    std::vector<TaskGroup> task_list; //in every task list is group of tasks
    map::Graph graph;
    TSP tsp;

    WHCA whca;

    Reservation table;
    template<typename Iter, typename RandomGenerator>
    Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    }

    template<typename Iter>
    Iter select_randomly(Iter start, Iter end) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return select_randomly(start, end, gen);
    }




    std::vector<WHCA_Agent> capacity(const TaskGroup& task) const;
    std::optional<WHCA_Agent> random(std::vector<WHCA_Agent>& capableAgents) const;

    void addTaskGroup(const TaskGroup& taskGroup);
    //void taskGroupGenerator(); //automatyczne tworzenie taskGroup losowe


public:
    WHCA_Environment(std::vector<Agent> agents, map::Graph graph);

    inline map::Graph getGraph()
    {
        return this->graph;
    }
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

    void assignVacanAgents();

    TaskGroup TASKGROUPGENERATOR(std::vector<int> avaliablePickupX, std::vector<int> avaliablePickupY, std::vector<int> avaliableDropofX, std::vector<int> avaliableDropofY );

    void MOVEAGENTS(int timestep);
    void addAgent(Agent &newAgent);


    void runTimestep(int timestep, TaskGroup* task = nullptr);
};

