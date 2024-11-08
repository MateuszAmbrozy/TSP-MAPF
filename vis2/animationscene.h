// mapfscene.h
#ifndef MAPFSCENE_H
#define MAPFSCENE_H

#include <QGraphicsScene>
#include "graph/graphrectitem.h"
#include "agentrectitem.h"
#include "../lib/BaseEnvironment.h"

class MapfScene : public QGraphicsScene
{
private:
    map::Graph graph;
    GraphRectItem* vis_graph;
    std::vector<AgentRectItem*> vis_agents;

    BaseEnvironment* environment;
public:

    MapfScene(BaseEnvironment* env, QObject* parent = nullptr);
    ~MapfScene();
    void drawEnvironment();
    void updateScene(int timestep);
};

#endif // MAPFSCENE_H
