#ifndef AGENTRECTITEM_H
#define AGENTRECTITEM_H

#include "../lib/Agent.h"
#include <QGraphicsRectItem>
#include <QGraphicsObject>
#include <QGraphicsTextItem>
#include <QColor>
#include <QTimer>
#include <QRandomGenerator>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>

class AgentRectItem
{
private:
    QGraphicsTextItem* idLabel;
    QColor agentColor;


    // To keep track of items for removal
    QGraphicsRectItem* agentRectItem = nullptr;

    std::vector<QGraphicsEllipseItem*> pickupGraphicsItems;
    QGraphicsRectItem* dropoffGraphicsItem = nullptr;
    QGraphicsEllipseItem* startGraphicsItem = nullptr;


    QColor assignRandomColor();
    void drawPickupPoints(QGraphicsScene* scene, const Agent& agent);
    void drawDropoffPoint(QGraphicsScene* scene, const Agent& agent);

    void clearPickupDropoff(QGraphicsScene* scene);
    void animateMoveAgent(QGraphicsScene* scene, const QPointF& newPosition);
public:
    AgentRectItem(QWidget* parent = nullptr);

    // void setPosition(const map::Cell& newPosition);
    // void setPickupPoints(std::vector<map::Cell> points);
    // void setDropoffPoint(map::Cell point);

    void drawAgent(QGraphicsScene* scene, const Agent& agent);
    void drawStartPoint(QGraphicsScene* scene, const Agent& agent);

    void draw(QGraphicsScene* scene, int timestep, const Agent& agent);


};

#endif // AGENTRECTITEM_H
