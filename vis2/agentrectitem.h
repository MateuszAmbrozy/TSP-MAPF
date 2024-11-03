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
#include <QVariantAnimation>
#include <QGraphicsLineItem>

class AgentRectItem
{
private:
    QGraphicsTextItem* idLabel;
    QColor agentColor;

    QGraphicsRectItem* agentRectItem = nullptr;
    std::vector<QGraphicsEllipseItem*> pickupGraphicsItems;
    QGraphicsRectItem* dropoffGraphicsItem = nullptr;
    QGraphicsEllipseItem* startGraphicsItem = nullptr;
    std::vector<QGraphicsLineItem*> pathGraphicsItems;

    // Animacja płynnego ruchu
    QVariantAnimation* moveAnimation = nullptr;

    QColor assignRandomColor();
    void drawPickupPoints(QGraphicsScene* scene, const Agent& agent);
    void drawDropoffPoint(QGraphicsScene* scene, const Agent& agent);
    void drawPath(QGraphicsScene* scene, const Agent& agent);
    void clearPath(QGraphicsScene* scene);
    void clearPickupDropoff(QGraphicsScene* scene);

public:
    AgentRectItem();

    void drawAgent(QGraphicsScene* scene, const Agent& agent);
    void drawStartPoint(QGraphicsScene* scene, const Agent& agent);
    void draw(QGraphicsScene* scene, int timestep, const Agent& agent);
    void animateMoveAgent(const QRectF& newPosition);
};

#endif // AGENTRECTITEM_H
