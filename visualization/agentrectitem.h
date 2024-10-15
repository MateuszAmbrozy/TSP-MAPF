// agentrectitem.h
#ifndef AGENTRECTITEM_H
#define AGENTRECTITEM_H
#include "../lib/Agent.h"
#include "../lib/Graph.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

class AgentRectItem: public QGraphicsRectItem
{
private:
    Agent agent;

public:
    AgentRectItem(int id, int capacity, map::Cell position, QWidget* parent = nullptr);
    void draw(QGraphicsScene* scene); // Draw agent in initial position
    void moveAgent(QGraphicsScene* scene, int timestep); // Move agent in the scene
};

#endif // AGENTRECTITEM_H
