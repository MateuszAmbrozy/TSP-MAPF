// #pragma once
// #include <iostream>
// #include <vector>
// #include <optional>
// #include <ctime>
// #include <unordered_map>
// //#include <chrono>
// #include <windows.h>

// #include "Agent.h"
// #include "TaskGroup.h"
// #include "Graph.h"
// #include "TSP.h"
// #include "setup.h"
// #include "Space-Time-AStar/Reservation.h"
// #include "Space-Time-AStar/SpaceTimeAStar.h"



// class Environment {
// private:
//     std::vector<Agent> agents;
//     std::vector<Agent> vacant_agents;
//     std::vector<TaskGroup> task_list; //in every task list is group of tasks
//     map::Graph graph;
//     TSP tsp;
//     SpaceTimeAStar sta;
//     Reservation table;
    
//     void addAgent(const Agent& agent);
//     void assignVacanAgents();
    
//     std::vector<Agent> capacity(const TaskGroup& task) const; //wyszukuje agentow spelniajacych ograniczenie pojemnosci z agentow wakacyjnych
//     std::optional<Agent> random(const std::vector<Agent>& capableAgents) const;

//     void addTaskGroup(const TaskGroup& taskGroup);
//     //void taskGroupGenerator(); //automatyczne tworzenie taskGroup losowe

//     void MOVEAGENTS(int timestep);
//     TaskGroup TASKGROUPGENERATOR();
    
// public:
//     Environment(std::vector<Agent> agents, map::Graph graph);


//     void mainAlgorithm();

    

// };

