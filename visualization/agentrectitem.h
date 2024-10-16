// #ifndef AGENTRECTITEM_H
// #define AGENTRECTITEM_H

// #include "../lib/Agent.h"
// #include <QGraphicsRectItem>
// #include <QGraphicsTextItem>
// #include <QColor>
// #include <QGraphicsEllipseItem>
// #include <QRandomGenerator>
// class AgentRectItem : public QGraphicsRectItem
// {
// private:
//     Agent agent;
//     QColor agentColor;  // Persistent color for each agent
//     std::vector<map::Cell> pickupPoints;
//     map::Cell dropoffPoint;
//     bool taskCompleted = false;

//     // Store the QGraphicsItems so they can be removed later
//     QGraphicsRectItem* agentRectItem = nullptr;
//     std::vector<QGraphicsEllipseItem*> pickupGraphicsItems;
//     QGraphicsEllipseItem* dropoffGraphicsItem = nullptr;

// public:
//     AgentRectItem(int id, int capacity, map::Cell position, const std::vector<map::Cell>& pickups, const map::Cell& dropoff, QWidget* parent = nullptr);

//     void draw(QGraphicsScene* scene);
//     void moveAgent(QGraphicsScene* scene, int timestep);
//     void drawPickupPoints(QGraphicsScene* scene);
//     void drawDropoffPoint(QGraphicsScene* scene);
//     QColor assignRandomColor();
//     void completeTask(QGraphicsScene* scene);
// };

// #endif
#ifndef AGENTRECTITEM_H
#define AGENTRECTITEM_H

#include "../lib/Agent.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QColor>

class AgentRectItem : public QGraphicsRectItem
{
private:
    Agent agent;
    QGraphicsTextItem* idLabel;
    QColor agentColor;  // Color for each agent

public:
    AgentRectItem(int id, int capacity, map::Cell position, QWidget* parent = nullptr);

    void draw(QGraphicsScene* scene);
    void moveAgent(QGraphicsScene* scene, int timestep);
    QColor assignColorBasedOnId(int id);  // Assign color based on the agent's ID
};

#endif // AGENTRECTITEM_H
