#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <ctime>

#include "Agent.h"
#include "Task.h"
#include "Graph.h"
#include "TSP.h"


class Environment {
private:
    int width;
    int height;
    std::vector<Agent> agents;
    std::vector<Agent> vacant_agents;
    std::vector<std::vector<Task>> task_list; //in every task list is group of tasks
    map::Graph graph;
    TSP tsp;
    
public:
    Environment(std::vector<Agent> agents, int width, int height);
    
    void addAgent(const Agent& agent);

    
    std::vector<Agent> capacity(const Task& task) const; //wyszukuje agentow spelniajacych ograniczenie pojemnosci z agentow wakacyjnych
    std::optional<Agent> random() const;

    void addTaskGroup(const std::vector<Task>& taskGroup);
    //void taskGroupGenerator(); //automatyczne tworzenie taskGroup losowe

    

};

