// #include "../lib/Environment.h"



// Environment::Environment(std::vector<Agent> agents, map::Graph graph)
//     : graph(graph), agents(agents),
//      tsp(graph), sta(graph)
//     {
//        srand(time(0));
//     }



// void Environment::addAgent(const Agent& agent) {
//     agents.push_back(agent);
// }

// void Environment::assignVacanAgents()
// {
//     vacant_agents.clear();
//     for(const Agent& agent : agents)
//     {
//         if(agent.isIdle())
//             vacant_agents.push_back(agent);
//     }   
// }


// std::vector<Agent> Environment::capacity(const TaskGroup& task) const
// {
//     std::vector<Agent> buf;
//     for(const auto& agent :vacant_agents)
//     {
//         if(agent.getAccessibleCapacity() >= task.getCapacity())
//         {
//             buf.push_back(agent);
//         }
//     }
//     return buf;
// }

// std::optional<Agent> Environment::random(const std::vector<Agent>& capableAgents) const
// {
//     if(capableAgents.size() > 0)
//     {
//         int randomNum = rand() % capableAgents.size();
//         return capableAgents.at(randomNum);
//     }
//      return std::nullopt; // Brak agenta
        
// }

// void Environment::addTaskGroup(const TaskGroup& taskGroup) 
// {
//     task_list.push_back(taskGroup);
// }
// void Environment::MOVEAGENTS(int timestep)
// {
//     for(auto agent: agents)
//     {
//         if (!agent.isIdle()) {

//             std::vector<SpaceTimeCell::Cell> path = agent.getPath();

//             if (timestep < path.size()) {

//                 SpaceTimeCell::Cell nextPosition = path[timestep];
                
//                 map::Cell newPosition(nextPosition.x, nextPosition.y);
//                 agent.move(newPosition);
//             } else 
//             {
//                 std::cout << "Agent " << agent.getId() << " has completed its path." << std::endl;
//                 task_list.erase(std::remove(task_list.begin(), task_list.end(), agent.getTask()), task_list.end());
                


//                 agent.assignPath({});
//                 agent.assignTask({});

//                 for (const auto& cell : agent.getPath()) {
//                     table.removeReservation(cell.x, cell.y, cell.t);  // Remove cell reservations

//                     // If the path contains more than one point, remove edge reservations too
//                     const auto& nextCell = agent.getPath().back(); 
//                     if (&cell != &nextCell) {
//                         table.removeEdgeReservation(cell.x, cell.y, nextCell.x, nextCell.y, cell.t);
//                     }
//                 }
                
//             }
            
//         }
//     }
    
// }

// TaskGroup Environment::TASKGROUPGENERATOR() {
//     int numTasks = rand() % setup::maxTasks + 1;
    

//     std::vector<map::Cell> pickupPoints;
    
//     for (int i = 0; i < numTasks; ++i) {
//         int x = rand() % graph.width;
//         int y = rand() % graph.height;
//         pickupPoints.push_back(map::Cell(x, y));
//     }

    
//     int dropoffX = rand() % graph.width;
//     int dropoffY = rand() % graph.height;
//     map::Cell dropoffPoint(dropoffX, dropoffY);


//     int taskCapacity = rand() % setup::maxCapacity + 1;

//     std::vector<int> stopTimes(numTasks, rand() % setup::maxStopTime);

//     TaskGroup taskGroup(numTasks, pickupPoints, dropoffPoint, stopTimes, taskCapacity);
//     return taskGroup;
// }

// void Environment::mainAlgorithm() {

//     assignVacanAgents();  // Step 3
    
//     for (int timestep = 0; timestep <= setup::max_time; ++timestep) {  // Step 5
//         task_list.push_back(TASKGROUPGENERATOR());  // Step 6
        
//         for (int l = 0; l < task_list.size(); ++l) {  // Step 7
//             const TaskGroup& taskGroup = task_list[l];
            
//             std::vector<Agent> capableAgents = capacity(taskGroup);  // Step 8
            
//             if (capableAgents.size() > 0) 
//             {

//                 std::optional<Agent> selectedAgentOpt = random(capableAgents);  // Step 10
//                 if (!selectedAgentOpt) {
//                     continue;
//                 }
//                 Agent& selectedAgent = *selectedAgentOpt;
                
//                 selectedAgent.assignTask(taskGroup);  // Step 11
                
//                 std::vector<int> order = tsp.solveTSP(selectedAgent, taskGroup);  // Step 12, 13

                

//                 selectedAgent.assignPath(sta.findPath(selectedAgent, timestep, taskGroup, order, table)); //step 14, 15
                
//                 vacant_agents.erase(std::remove(vacant_agents.begin(), vacant_agents.end(), selectedAgent), vacant_agents.end());  // Step 16
//             }
//         }
//         Sleep(1000);

//         MOVEAGENTS(timestep);  // Step 20
//     }
// }
