#include "animationscene.h"

MapfScene::MapfScene(WHCA_Environment* env, QObject* parent)
    : QGraphicsScene(parent), graph(env->getGraph()), whca_environment(env)
{
    vis_graph = new GraphRectItem(graph, nullptr);
    vis_graph->drawGraph(this);

    const std::vector<Agent>& agents = whca_environment->getAgents();
    for (const auto& agent : agents)
    {
        std::vector<map::Cell> pickups = agent.getTask().getPickupPoints();
        //map::Cell dropoff = agent.getTask().getDropoffLocation();

        AgentRectItem* vis_agent = new AgentRectItem();
        vis_agent->drawAgent(this, agent);

        vis_agents.push_back(vis_agent);
    }
}

MapfScene::MapfScene(Environment *env, QObject *parent)
{

}



void MapfScene::updateScene(int timestep)
{
    const std::vector<Agent>& agents = whca_environment->getAgents();

    for (size_t i = 0; i < vis_agents.size(); ++i)
    {
        const Agent& backendAgent = agents[i];
        vis_agents[i]->draw(this, timestep, backendAgent);
    }

    update();
}
