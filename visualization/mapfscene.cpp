#include "mapfscene.h"

MapfScene::MapfScene(Environment* env, QObject* parent)
    : QGraphicsScene(parent), environment(env), graph(env->getGraph())
{
    vis_graph = new GraphRectItem(graph);
    vis_graph->drawGraph(this);

    const std::vector<Agent>& agents = environment->getAgents();
    for (const auto& agent : agents)
    {
        std::vector<map::Cell> pickups = agent.getTask().getPickupPoints();
        map::Cell dropoff = agent.getTask().getDropoffLocation();

        AgentRectItem* vis_agent = new AgentRectItem();
        vis_agent->drawAgent(this, agent);

        vis_agents.push_back(vis_agent);
    }
}

void MapfScene::updateScene(int timestep)
{
    const std::vector<Agent>& agents = environment->getAgents();


    for (int i = 0; i < vis_agents.size(); ++i)
    {
        const Agent& backendAgent = agents[i];

        // vis_agents[i]->setPosition(backendAgent.getPosition());
        // vis_agents[i]->setPath();
        // vis_agents[i]->setPickupPoints(backendAgent.getTask().getPickupPoints());
        // vis_agents[i]->setDropoffPoint(backendAgent.getTask().getDropoffLocation());

        vis_agents[i]->draw(this, timestep, backendAgent);
    }

    update();
}
