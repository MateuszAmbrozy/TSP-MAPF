#include "mapfscene.h"

MapfScene::MapfScene(QObject* parent)
    : QGraphicsScene(parent), graph(10, 10)
{
    // Create and draw the graph
    vis_graph = new GraphRectItem(graph);
    vis_graph->drawGraph(this);

    // Create and draw the agent
    vis_agent = new AgentRectItem(0, 10, map::Cell(2, 2));
    vis_agent->draw(this);
}

void MapfScene::updateScene(int timestep)
{
    // Move agent based on the current timestep
    vis_agent->moveAgent(this, timestep);
}
