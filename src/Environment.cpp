#include "../lib/Environment.h"



Environment::Environment(std::vector<Agent> agents, map::Graph graph)
    : graph(graph), agents(agents),
     tsp(graph), sta(graph)
    {
       srand(time(0));
    }



void Environment::addAgent(Agent&& newAgent) {
    // Add the new agent to the list of agents
    agents.push_back(newAgent);

    // Notify all other agents to add the new agent's startPos to their illicit cells
    for (Agent& agent : agents) {
        if (agent.getId() != newAgent.getId()) {
            agent.addIllicitCell(newAgent.getPosition());  // Add newAgent's startPos to other agents
        }
    }

    // Also make sure the new agent knows about all the other agents' start positions
    for (const Agent& agent : agents) {
        if (agent.getId() != newAgent.getId()) {
            const map::Cell& otherAgentStartPos = agent.getPosition();
            const_cast<Agent&>(newAgent).addIllicitCell(otherAgentStartPos);  // Add existing agents' start positions to newAgent
        }
    }
}

void Environment::assignVacanAgents()
{
    vacant_agents.clear();
    for(const Agent& agent : agents)
    {
        if(agent.isIdle())
            vacant_agents.push_back(agent);
    }
}


std::vector<Agent> Environment::capacity(const TaskGroup& task) const
{
    std::vector<Agent> buf;
    for(const auto& agent :vacant_agents)
    {
        if(agent.getAccessibleCapacity() >= task.getCapacity())
        {
            buf.push_back(agent);
        }
    }
    return buf;

}

std::optional<Agent> Environment::random(std::vector<Agent>& capableAgents) const {
    if (!capableAgents.empty()) {
        int randomNum = rand() % capableAgents.size();
        return capableAgents.at(randomNum); // Return a reference
    }
    return std::nullopt; // No agent available
}


void Environment::addTaskGroup(const TaskGroup& taskGroup)
{
    task_list.push_back(taskGroup);
}
void Environment::MOVEAGENTS(int timestep)
{
    for(auto& agent: agents)
    {
        if (!agent.isIdle()) {

            std::vector<SpaceTimeCell::Cell> path = agent.getPath();

            if (timestep < path.size() + path.front().t) 
            {

                SpaceTimeCell::Cell nextPosition;

                for (const auto& cell : path) 
                {
                    if (cell.t == timestep) 
                    {
                        nextPosition = cell;
                        break;
                    }
                }
                map::Cell newPosition(nextPosition.x, nextPosition.y);
                agent.move(newPosition);

                std::cout<<agent.getId() << ": (" << agent.getPosition().x << ", " << agent.getPosition().y << "), t = " << timestep << "\n";
            } 
            else
            {
                std::cout << "Agent " << agent.getId() << " has completed its path." << std::endl;
                for (const auto& cell : agent.getPath()) 
                {
                    table.removeReservation(cell.x, cell.y, cell.t);  // Remove cell reservations

                    const auto& nextCell = agent.getPath().back();
                    if (&cell != &nextCell) 
                    {
                        table.removeEdgeReservation(cell.x, cell.y, nextCell.x, nextCell.y, cell.t);
                    }
                }
                agent.clearPath();
                agent.clearTask();

                vacant_agents.push_back(agent);
                
            }
            
        }
    }
    
}

TaskGroup Environment::TASKGROUPGENERATOR(std::vector<int> avaliablePickupX, std::vector<int> avaliablePickupY, std::vector<int> avaliableDropofX, std::vector<int> avaliableDropofY ) {
    int numTasks = rand() % setup::maxTasks + 1;
    
    std::vector<map::Cell> pickupPoints;
    
    for (int i = 0; i < numTasks; ++i) {
        int x = *select_randomly(avaliablePickupX.begin(), avaliablePickupX.end());
        int y = *select_randomly(avaliablePickupY.begin(), avaliablePickupY.end());
        pickupPoints.push_back(map::Cell(x, y));
    }

    
    int dropoffX = *select_randomly(avaliableDropofX.begin(), avaliableDropofX.end());
    int dropoffY = *select_randomly(avaliableDropofY.begin(), avaliableDropofY.end());
    map::Cell dropoffPoint(dropoffX, dropoffY);


    int taskCapacity = rand() % setup::maxCapacity + 1;
    int dropoffTime;
    std::vector<int> stopTimes;
    if (setup::maxStopTime > 0) {
        for(int i=0; i<numTasks; i++)
        {
            stopTimes.push_back(rand() % setup::maxStopTime);
        }
        dropoffTime = rand() % setup::maxStopTime;
    } 
    else 
    {
        stopTimes = std::vector<int>(numTasks, 0);  // Default to 0 if maxStopTime is 0
        dropoffTime = 0;
    }

    TaskGroup taskGroup(taskCapacity, pickupPoints, dropoffPoint, stopTimes, dropoffTime);

    return taskGroup;
}

void Environment::mainAlgorithm() {

    assignVacanAgents();  // Step 3
    std::vector<int> avaliablePickupX={1, 2};
    std::vector<int> avaliablePickupY={1, 2};
    std::vector<int> avaliableDropofX={1, 2, 3};
    std::vector<int> avaliableDropofY={3};

    for (int timestep = 0; timestep <= setup::max_time; ++timestep) {  // Step 5
        if(timestep%7==0)
            task_list.push_back(TASKGROUPGENERATOR(avaliablePickupX, avaliablePickupY, avaliableDropofX, avaliableDropofY));  // Step 6
        
        for (int l = 0; l < task_list.size(); ++l) {  // Step 7
            const TaskGroup taskGroup = task_list[l];
            
            std::vector<Agent> capableAgents = capacity(taskGroup);  // Step 8
            
        if (capableAgents.size() > 0)
        {
            auto selectedAgentOpt = random(capableAgents);  // Step 10
            if (!selectedAgentOpt.has_value()) 
            {
                continue;
            }
            Agent selectedAgent = *selectedAgentOpt;

            std::cout << "agent: (" << selectedAgent.getPosition().x << ", " << selectedAgent.getPosition().y << ")\n";
            std::cout << taskGroup << std::endl;
            
            std::vector<int> order = tsp.solveTSP(selectedAgent, taskGroup);  // Step 12, 13
            std::vector<SpaceTimeCell::Cell> path = sta.findPath(selectedAgent, timestep, taskGroup, order, table);
            auto it = std::find(agents.begin(), agents.end(), selectedAgent);

            it->assignPath(path);
            it->assignTask(taskGroup);
            it->setIdle(false);

            
            // Usuwanie agenta z vacant_agents
            vacant_agents.erase(
                std::remove(vacant_agents.begin(), vacant_agents.end(), selectedAgent), 
                vacant_agents.end()
            );  // Step 16
            
            // Usuwanie zadania z task_list
            task_list.erase(
                std::remove(task_list.begin(), task_list.end(), taskGroup), 
                task_list.end()
            );
        }
        }
        Sleep(1000);

        MOVEAGENTS(timestep);  // Step 20
    }
}
