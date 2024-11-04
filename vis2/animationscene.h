// mapfscene.h
#ifndef MAPFSCENE_H
#define MAPFSCENE_H

#include <QGraphicsScene>
#include "graph/graphrectitem.h"
#include "agentrectitem.h"
// #include "../lib/Environment.h"
// #include "../lib/WHCA_Environment.h"
#include "../lib/BaseEnvironment.h"

class MapfScene : public QGraphicsScene
{
private:
    map::Graph graph;
    GraphRectItem* vis_graph;
    std::vector<AgentRectItem*> vis_agents;
    //Environment* environment;
    BaseEnvironment* environment;
public:
    //MapfScene(Environment* env, QObject* parent = nullptr);
    // MapfScene(WHCA_Environment* env, QObject* parent = nullptr);
    // MapfScene(Environment* env, QObject* parent = nullptr);

    MapfScene(BaseEnvironment* env, QObject* parent = nullptr);
    ~MapfScene();
    void drawEnvironment();
    void updateScene(int timestep);
};

#endif // MAPFSCENE_H
