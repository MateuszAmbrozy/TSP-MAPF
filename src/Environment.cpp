#include "../lib/Environment.h"

Environment::Environment(std::vector<Agent> agents, int width, int height)
    : width(width), height(height),
     graph(width, height), agents(agents),
     tsp(graph)
    {
       srand(time(0));
    }



void Environment::addAgent(const Agent& agent) {
    agents.push_back(agent);
}


std::vector<Agent> Environment::capacity(const Task& task) const
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

std::optional<Agent> Environment::random() const
{
    if(vacant_agents.size() > 0)
    {
        int randomNum = rand() % vacant_agents.size();
        return vacant_agents.at(randomNum);
    }
     return std::nullopt; // Brak agenta
        
}

void Environment::addTaskGroup(const std::vector<Task>& taskGroup) 
{
    task_list.push_back(taskGroup);
}
