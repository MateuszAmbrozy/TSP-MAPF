#include "animationscene.h"

MapfScene::MapfScene(BaseEnvironment *env, QObject *parent)
    : QGraphicsScene(parent), graph(env->getGraph()), environment(env)
{
    vis_graph = new GraphRectItem(graph, nullptr);
    vis_graph->drawGraph(this);

    const std::vector<Agent>& agents = environment->getAgents();
    for (const auto& agent : agents)
    {
        std::vector<map::Cell> pickups = agent.getTask().getPickupPoints();
        //map::Cell dropoff = agent.getTask().getDropoffLocation();

        AgentRectItem* vis_agent = new AgentRectItem();
        vis_agent->drawAgent(this, agent);

        vis_agents.push_back(vis_agent);
    }
}

MapfScene::~MapfScene()
{
    for (auto* vis_agent : vis_agents) {
        vis_agent->stopAnimation();
    }
}

void MapfScene::updateScene(int timestep)
{
    const std::vector<Agent>& agents = environment->getAgents();

    for (size_t i = 0; i < vis_agents.size(); ++i)
    {
        const Agent& backendAgent = agents[i];
        vis_agents[i]->draw(this, timestep, backendAgent);
    }

    update();
}

void MapfScene::updateAgentAnimationSpeed(int speed)
{
    for (auto* vis_agent : vis_agents) {
        vis_agent->setAnimationSpeed(speed);
    }
}
