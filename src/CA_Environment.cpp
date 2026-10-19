#include "../lib/CA_Environment.h"



CA_Environment::CA_Environment(std::vector<Agent> agents, map::Graph graph, std::vector<std::pair<int, int>> avaliablePickups, std::vector<std::pair<int, int>> avaliableDropoffs)
    : BaseEnvironment(graph, avaliablePickups, avaliableDropoffs), sta(graph)
    {
        for(auto& agent: agents)
       {
           this->addAgent(agent);
       }
    }



void CA_Environment::addAgent(Agent& newAgent)
{

    for (Agent& agent : agents) {
            agent.addIllicitCell(newAgent.getStartPosition());
            newAgent.addIllicitCell(agent.getStartPosition());
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

bool CA_Environment::allTasksCompleted()
{
        if (!task_list.empty()) 
        {
            return false;
        }

        for (const auto& agent : agents) {
        if (!agent.isIdle()) {
            return false;
        }
    }
    return true;
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
        return capableAgents.at(randomNum);
    }
    return std::nullopt;
}



void CA_Environment::MOVEAGENTS(int timestep)
{
    for(auto& agent: agents)
    {
        if (!agent.isIdle()) 
        {

            std::vector<CA::Cell> path = agent.getPath();

            if (path.size() > 0 && timestep < static_cast<int>(path.size()) + path.front().t)
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
                agent.move(newPosition);

            }
            else
            {
                for (const auto& cell : agent.getPath())
                {
                    table.removeReservation(cell.x, cell.y, cell.t);
                    table.removeReservation(cell.x, cell.y, cell.t + 1);

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


void CA_Environment::runTimestep(int timestep, TaskGroup* task)
{
    if(task)
    {
        task_list.push_back(*task);
    }
    else if(timestep % 10 == 0)
    {
        task_list.push_back(TASKGROUPGENERATOR());
    }
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
                std::vector<int> order = tsp.solveTSP(selectedAgent, taskGroup);
                std::vector<CA::Cell> path = sta.findPath(selectedAgent, timestep, taskGroup, order, table);
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

    MOVEAGENTS(timestep);
}
