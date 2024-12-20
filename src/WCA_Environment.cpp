#include "../lib/WCA_Environment.h"

#include "../lib/setup.h"
//#include "QDebug"
WCA_Environment::WCA_Environment(std::vector<Agent> agents, map::Graph graph, std::vector<std::pair<int, int>> avaliablePickups, std::vector<std::pair<int, int>> avaliableDropoffs)
    : BaseEnvironment(graph, avaliablePickups, avaliableDropoffs), whca(graph)
{
    for(auto& agent: agents)
    {
        this->addAgent(agent);
    }
}

void WCA_Environment::addAgent(Agent &newAgent)
{
    for (auto& agent : agents)
    {
        agent.agent.addIllicitCell(newAgent.getStartPosition());  // Add newAgent's startPos to other agents
        newAgent.addIllicitCell(agent.agent.getStartPosition());  // Add existing agents' start positions to newAgent
    }
    agents.push_back(WCA_Agent(Agent(newAgent)));
}

void WCA_Environment::assignVacantAgents()
{
    vacant_agents.clear();
    for(WCA_Agent& agent : agents)
    {
        if(agent.agent.isIdle())
            vacant_agents.push_back(agent);
    }
}


std::vector<WCA_Agent> WCA_Environment::capacity(const TaskGroup& task) const
{
    std::vector<WCA_Agent> buf;
    for(const auto& agent :vacant_agents)
    {
        if(agent.agent.getAccessibleCapacity() >= task.getCapacity())
        {
            buf.push_back(agent);
        }
    }
    return buf;

}

std::optional<std::reference_wrapper<WCA_Agent>> WCA_Environment::random(std::vector<WCA_Agent>& capableAgents) const
{
    if (!capableAgents.empty()) {
        int randomNum = rand() % capableAgents.size();
        return capableAgents.at(randomNum); // Return a reference
    }
    return std::nullopt; // No agent available
}




void WCA_Environment::MOVEAGENTS(int timestep)
{
    for(auto& agent: agents)
    {

        if (!agent.agent.isIdle())
        {
            std::vector<CA::Cell> path = agent.agent.getPath();
            if (path.size() > 0 && timestep < static_cast<int>(path.size()) + path.front().t )
            {

                CA::Cell nextPosition;

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

                //std::cout<<agent.agent.getId() << ": (" << agent.agent.getPosition().x << ", " << agent.agent.getPosition().y << "), t = " << timestep << std::endl;
                if (timestep == path.back().t)
                {
                    if (!agent.reachedCurrentWaypoint())
                    {
                        //agent.incrementWaypointIndex();

                        std::vector<CA::Cell> nextPath = whca.findNextWSteps(agent, timestep, table);
                        if (!nextPath.empty())
                        {
                            for (const auto& cell : agent.agent.getPath())
                            {
                                table.removeReservation(cell.x, cell.y, cell.t);
                                table.removeReservation(cell.x, cell.y, cell.t + 1);
                                //table.removeReservation(cell.x, cell.y, cell.t + 2);
                                const auto nextCell = agent.agent.getPath().back();
                                if (&cell != &nextCell)
                                {
                                    table.removeEdgeReservation(cell.x, cell.y, cell.t);
                                }
                            }
                            agent.agent.clearPath();
                            agent.agent.assignPath(nextPath);
                        }
                        else
                        {
                            agent.agent.assignPath({CA::Cell(agent.agent.getPosition(), timestep+1)});
                            //std::cout<<"PROBLEMPROBLEMPROBLEMPROBLEMPROBLEMPROBLEMPROBLEM FOR: " << agent.agent.getId() << "\n";
                        }
                    }
                }
            }
            else
            {

                for (const auto& cell : agent.agent.getPath())
                {
                    table.removeReservation(cell.x, cell.y, cell.t);
                    table.removeReservation(cell.x, cell.y, cell.t + 1);
                    //table.removeReservation(cell.x, cell.y, cell.t + 2);
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
                    std::vector<CA::Cell> path = whca.findNextWSteps(agent, timestep, table);
                    agent.agent.assignPath(path);
                }
            }

        }
    }

}

bool WCA_Environment::allTasksCompleted()
{
        if (!task_list.empty()) {
            return false;
        }
        for (const auto& agent : agents) {
        if (!agent.agent.isIdle()) {
            return false;
        }
    }
    return true;
}

void WCA_Environment::runTimestep(int timestep, TaskGroup *task)
{
    if(task)
    {
        task_list.push_back(*task);
    }
    else if(timestep % 10 == 0)
    {
        task_list.push_back(TASKGROUPGENERATOR());
    }


    // Iterate over tasks and assign to agents if possible
    for (size_t l = 0; l < task_list.size(); ++l)
    {
        const TaskGroup& taskGroup = task_list[l];
        std::vector<WCA_Agent> capableAgents = capacity(taskGroup);

        if (!capableAgents.empty())
        {
            auto selectedAgentOpt = random(capableAgents);
            if (selectedAgentOpt.has_value())
            {
                WCA_Agent& selectedAgent = selectedAgentOpt->get();
                int agentID;
                //std::cout << "agent: (" << selectedAgent.agent.getPosition().x << ", " << selectedAgent.agent.getPosition().y << ")\n";
                //std::cout << taskGroup << std::endl;
                std::vector<int> order = tsp.solveTSP(selectedAgent.agent, taskGroup);
                std::vector<std::pair<map::Cell, int>> waypoints;

                for(int idx : order)
                {
                    waypoints.push_back({taskGroup.getPickupLocation(idx), taskGroup.getPickupStopTime(idx)});
                }
                waypoints.push_back({taskGroup.getDropoffLocation(), taskGroup.getDropoffStopTime()});
                waypoints.push_back({selectedAgent.agent.getStartPosition(), 0});

                // selectedAgent.setWaypoints(waypoints);
                // task == nullptr ? selectedAgent.agent.assignTask(taskGroup) : selectedAgent.agent.assignTask(*task);
                // selectedAgent.agent.setIdle(false);



                for(size_t i=0; i<agents.size(); i++)
                {
                    if(agents[i].agent.getId() == selectedAgent.agent.getId())
                    {
                        agents[i].setWaypoints(waypoints);
                        //task == nullptr ? agents[i].agent.assignTask(taskGroup) : agents[i].agent.assignTask(*task);
                        agents[i].agent.assignTask(taskGroup);
                        agents[i].agent.setIdle(false);
                        agentID = i;
                        break;
                    }
                }

                std::vector<CA::Cell> path = whca.findNextWSteps(agents[agentID], timestep, table);

                if(path.empty())
                    continue;
                agents[agentID].agent.assignPath(path);

                vacant_agents.erase(std::remove(vacant_agents.begin(), vacant_agents.end(), selectedAgent), vacant_agents.end());

                task_list.erase(task_list.begin() + l);
            }
        }
    }

    MOVEAGENTS(timestep);  // Move agents based on the timestep
}
