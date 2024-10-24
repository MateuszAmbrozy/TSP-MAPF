// mapfscene.h
#ifndef MAPFSCENE_H
#define MAPFSCENE_H

#include <QGraphicsScene>
#include "graph/graphrectitem.h"
#include "agentrectitem.h"
#include "../lib/Environment.h"

class MapfScene : public QGraphicsScene
{
private:
    map::Graph graph;
    GraphRectItem* vis_graph;
    std::vector<AgentRectItem*> vis_agents;
    Environment* environment;

public:
    MapfScene(Environment* env, QObject* parent = nullptr);

    void drawEnvironment();
    void updateScene(int timestep);
};

#endif // MAPFSCENE_H
