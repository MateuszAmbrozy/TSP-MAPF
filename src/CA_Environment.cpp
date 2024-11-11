#include "../lib/CA_Environment.h"



CA_Environment::CA_Environment(std::vector<Agent> agents, map::Graph graph)
    : BaseEnvironment(graph), sta(graph)
    {
        for(auto& agent: agents)
       {
           this->addAgent(agent);
       }
    }



void CA_Environment::addAgent(Agent& newAgent)
{

    for (Agent& agent : agents) {
            agent.addIllicitCell(newAgent.getStartPosition());  // Add newAgent's startPos to other agents
            newAgent.addIllicitCell(agent.getStartPosition());  // Add existing agents' start positions to newAgent
        }
    agents.push_back(newAgent);
}

void CA_Environment::assignVacantAgents()
{
    vacant_agents.clear();
    for(const Agent& agent : agents)
    {
        if(agent.isIdle())
            vacant_agents.push_back(agent);
    }
}


std::vector<Agent> CA_Environment::capacity(const TaskGroup& task) const
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



std::optional<Agent> CA_Environment::random(std::vector<Agent>& capableAgents) const {
    if (!capableAgents.empty()) {
        int randomNum = rand() % capableAgents.size();
        return capableAgents.at(randomNum); // Return a reference
    }
    return std::nullopt; // No agent available
}



void CA_Environment::MOVEAGENTS(int timestep)
{
    for(auto& agent: agents)
    {
        if (!agent.isIdle()) {

            std::vector<SpaceTime::Cell> path = agent.getPath();

            if (path.size() > 0 && timestep < static_cast<int>(path.size()) + path.front().t)
            {

                SpaceTime::Cell nextPosition;

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

                    const auto nextCell = agent.getPath().back();
                    if (&cell != &nextCell)
                    {
                        table.removeEdgeReservation(cell.x, cell.y, cell.t);
                    }
                }
                agent.clearPath();
                agent.clearTask();

                vacant_agents.push_back(agent);
                agent.setIdle(true);

            }

        }
    }

}



void CA_Environment::mainAlgorithm() {

    assignVacantAgents();  // Step 3
    std::vector<int> avaliablePickupX={1, 2};
    std::vector<int> avaliablePickupY={1, 2};
    std::vector<int> avaliableDropofX={1, 2, 3};
    std::vector<int> avaliableDropofY={3};

    for (size_t timestep = 0; timestep <= setup::max_time; ++timestep) {  // Step 5
        if(timestep%30==0)
            task_list.push_back(TASKGROUPGENERATOR(avaliablePickupX, avaliablePickupY, avaliableDropofX, avaliableDropofY));  // Step 6

        for (size_t l = 0; l < task_list.size(); ++l) {  // Step 7
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
            std::vector<SpaceTime::Cell> path = sta.findPath(selectedAgent, timestep, taskGroup, order, table);
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

void CA_Environment::runTimestep(int timestep, TaskGroup* task)
{
    //assignVacanAgents();  // Make sure agents are assigned

    std::vector<int> avaliablePickupX={0, 1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> avaliablePickupY={1, 3, 5, 7};
    std::vector<int> avaliableDropofX={3, 4, 5, 6, 7};
    std::vector<int> avaliableDropofY={7};
    // Simulate task assignment and agent movements per timestep
    if(task)
    {
        task_list.push_back(*task);
    }
    else if(timestep % 10 == 0)
    {
        task_list.push_back(TASKGROUPGENERATOR(avaliablePickupX, avaliablePickupY, avaliableDropofX, avaliableDropofY));
    }
    // Iterate over tasks and assign to agents if possible
    for (size_t l = 0; l < task_list.size(); ++l)
    {
        const TaskGroup& taskGroup = task_list[l];
        std::vector<Agent> capableAgents = capacity(taskGroup);

        if (!capableAgents.empty())
        {
            auto selectedAgentOpt = random(capableAgents);
            if (selectedAgentOpt.has_value())
            {
                Agent& selectedAgent = *selectedAgentOpt;
                std::cout << "agent: (" << selectedAgent.getPosition().x << ", " << selectedAgent.getPosition().y << ")\n";
                std::cout << taskGroup << std::endl;
                std::vector<int> order = tsp.solveTSP(selectedAgent, taskGroup);
                std::vector<SpaceTime::Cell> path = sta.findPath(selectedAgent, timestep, taskGroup, order, table);
                if(path.empty())
                    continue;
                auto it = std::find(agents.begin(), agents.end(), selectedAgent);

                it->assignPath(path);
                it->assignTask(taskGroup);
                it->setIdle(false);

                vacant_agents.erase(std::remove(vacant_agents.begin(), vacant_agents.end(), selectedAgent), vacant_agents.end());

                task_list.erase(std::remove(task_list.begin(), task_list.end(), taskGroup), task_list.end());
            }
        }
    }

    MOVEAGENTS(timestep);  // Move agents based on the timestep
}
