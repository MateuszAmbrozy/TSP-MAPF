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

        AgentRectItem* vis_agent = new AgentRectItem(agent.getId(), agent.getCapacity(), agent.getPosition());
        vis_agent->draw(this);
        vis_agents.push_back(vis_agent);
    }
}

void MapfScene::updateScene(int timestep)
{
    clear();  // Clear the scene before redrawing

    // Redraw the grid (graph)
    vis_graph->drawGraph(this);

    // Redraw the agents with their colors and IDs
    for (auto& agent : environment->getAgents())
    {
        AgentRectItem* agentItem = new AgentRectItem(agent.getId(), agent.getCapacity(), agent.getPosition());
        agentItem->draw(this);  // Draw agent and its ID in the assigned color
    }

    update();  // Refresh the scene
}

