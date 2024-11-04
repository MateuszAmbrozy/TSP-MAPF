#include "../lib/WHCA_Environment.h"

#include "../lib/setup.h"
#include "QDebug"
WHCA_Environment::WHCA_Environment(std::vector<Agent> agents, map::Graph graph)
    : BaseEnvironment(graph), whca(graph)
{
    for(auto& agent: agents)
    {
        this->addAgent(agent);
    }
}



void WHCA_Environment::addAgent(Agent &newAgent)
{
    for (auto& agent : agents)
    {
        agent.agent.addIllicitCell(newAgent.getStartPosition());  // Add newAgent's startPos to other agents
        newAgent.addIllicitCell(agent.agent.getStartPosition());  // Add existing agents' start positions to newAgent
    }
    agents.push_back(WHCA_Agent(Agent(newAgent)));
}

void WHCA_Environment::assignVacantAgents()
{
    vacant_agents.clear();
    for(WHCA_Agent& agent : agents)
    {
        if(agent.agent.isIdle())
            vacant_agents.push_back(agent);
    }
}


std::vector<WHCA_Agent> WHCA_Environment::capacity(const TaskGroup& task) const
{
    std::vector<WHCA_Agent> buf;
    for(const auto& agent :vacant_agents)
    {
        if(agent.agent.getAccessibleCapacity() >= task.getCapacity())
        {
            buf.push_back(agent);
        }
    }
    return buf;

}


std::optional<WHCA_Agent> WHCA_Environment::random(std::vector<WHCA_Agent>& capableAgents) const {
    if (!capableAgents.empty()) {
        int randomNum = rand() % capableAgents.size();
        return capableAgents.at(randomNum); // Return a reference
    }
    return std::nullopt; // No agent available
}




void WHCA_Environment::MOVEAGENTS(int timestep)
{
    for(auto& agent: agents)
    {
        if (!agent.agent.isIdle()) {

            std::vector<SpaceTimeCell::Cell> path = agent.agent.getPath();

            if (path.size() > 0 && timestep < static_cast<int>(path.size()) + path.front().t)
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
                agent.agent.move(newPosition);

                qDebug() <<agent.agent.getId() << ": (" << agent.agent.getPosition().x << ", " << agent.agent.getPosition().y << "), t = " << timestep;
                if (timestep == path.back().t)
                {
                    if (!agent.reachedCurrentWaypoint())
                    {
                        std::vector<SpaceTimeCell::Cell> nextPath = whca.findNextWSteps(agent, timestep, table);
                        if (!nextPath.empty())
                        {
                            agent.agent.assignPath(nextPath);
                        }
                    }
                }
            }
            else
            {

                for (const auto& cell : agent.agent.getPath())
                {
                    table.removeReservation(cell.x, cell.y, cell.t);  // Remove cell reservations

                    const auto nextCell = agent.agent.getPath().back();
                    if (&cell != &nextCell)
                    {
                        table.removeEdgeReservation(cell.x, cell.y, cell.t);
                    }
                }
                agent.agent.clearPath();
                if(agent.finished())
                {
                    agent.agent.clearTask();
                    agent.agent.setIdle(true);
                    agent.clearWaypoints();
                    agent.clearCurrentWayPointIndex();
                    vacant_agents.push_back(agent);
                }
                else
                {
                    std::vector<SpaceTimeCell::Cell> path = whca.findNextWSteps(agent, timestep, table);
                    agent.agent.assignPath(path);
                }


            }

        }
    }

}



void WHCA_Environment::runTimestep(int timestep, TaskGroup *task)
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
    // else if(timestep % 10 == 0)
    // {
    //     task_list.push_back(TASKGROUPGENERATOR(avaliablePickupX, avaliablePickupY, avaliableDropofX, avaliableDropofY));
    // }


    // Iterate over tasks and assign to agents if possible
    for (size_t l = 0; l < task_list.size(); ++l)
    {
        const TaskGroup& taskGroup = task_list[l];
        std::vector<WHCA_Agent> capableAgents = capacity(taskGroup);

        if (!capableAgents.empty())
        {
            auto selectedAgentOpt = random(capableAgents);
            if (selectedAgentOpt.has_value())
            {
                WHCA_Agent& selectedAgent = *selectedAgentOpt;
                std::cout << "agent: (" << selectedAgent.agent.getPosition().x << ", " << selectedAgent.agent.getPosition().y << ")\n";
                std::cout << taskGroup << std::endl;
                std::vector<int> order = tsp.solveTSP(selectedAgent.agent, taskGroup);
                std::vector<std::pair<map::Cell, int>> waypoints;

                for(int idx : order)
                {
                    waypoints.push_back({taskGroup.getPickupLocation(idx), taskGroup.getPickupStopTime(idx)});
                }
                waypoints.push_back({taskGroup.getDropoffLocation(), taskGroup.getDropoffStopTime()});
                waypoints.push_back({selectedAgent.agent.getStartPosition(), 0});

                selectedAgent.setWaypoints(waypoints);

                std::vector<SpaceTimeCell::Cell> path = whca.findNextWSteps(selectedAgent, timestep, table);

                if(path.empty())
                    continue;
                //auto it = std::find(agents.begin(), agents.end(), selectedAgent);
                for(size_t i=0; i<agents.size(); i++)
                {
                    if(agents[i].agent.getId() == selectedAgent.agent.getId())
                    {
                        agents[i].setWaypoints(waypoints);
                        agents[i].agent.assignPath(path);
                        task == nullptr ? agents[i].agent.assignTask(taskGroup) : agents[i].agent.assignTask(*task);
                        agents[i].agent.setIdle(false);
                        break;
                    }
                }

                vacant_agents.erase(std::remove(vacant_agents.begin(), vacant_agents.end(), selectedAgent), vacant_agents.end());

                task_list.erase(task_list.begin() + l);
            }
        }
    }

    MOVEAGENTS(timestep);  // Move agents based on the timestep
}
