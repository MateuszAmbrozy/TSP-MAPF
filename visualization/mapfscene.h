#ifndef MAPFSCENE_H
#define MAPFSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "graphrectitem.h"
#include "agentrectitem.h"

    class MapfScene : public QGraphicsScene
{
private:
    map::Graph graph;
    GraphRectItem* vis_graph;
    AgentRectItem* vis_agent;

public:
    MapfScene(QObject* parent = nullptr);
    void updateScene(int timestep); // Update scene for the next timestep
};

#endif // MAPFSCENE_H
