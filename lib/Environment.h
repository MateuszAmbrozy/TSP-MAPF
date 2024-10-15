#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <ctime>
#include <unordered_map>
//#include <chrono>
#include <windows.h>
#include  <random>
#include  <iterator>

#include "Agent.h"
#include "TaskGroup.h"
#include "Graph.h"
#include "TSP.h"
#include "setup.h"
#include "Space-Time-AStar/Reservation.h"
#include "Space-Time-AStar/SpaceTimeAStar.h"



class Environment {
private:
    std::vector<Agent> agents;
    std::vector<Agent> vacant_agents;
    std::vector<TaskGroup> task_list; //in every task list is group of tasks
    map::Graph graph;
    TSP tsp;
    SpaceTimeAStar sta;
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
    void assignVacanAgents();
    
    std::vector<Agent> capacity(const TaskGroup& task) const; //wyszukuje agentow spelniajacych ograniczenie pojemnosci z agentow wakacyjnych
    std::optional<Agent> random(std::vector<Agent>& capableAgents) const;

    void addTaskGroup(const TaskGroup& taskGroup);
    //void taskGroupGenerator(); //automatyczne tworzenie taskGroup losowe

    void MOVEAGENTS(int timestep);
    TaskGroup TASKGROUPGENERATOR(std::vector<int> avaliablePickupX, std::vector<int> avaliablePickupY, std::vector<int> avaliableDropofX, std::vector<int> avaliableDropofY );
    
public:
    Environment(std::vector<Agent> agents, map::Graph graph);

    void addAgent(Agent&& newAgent);
    void mainAlgorithm();

    

};

