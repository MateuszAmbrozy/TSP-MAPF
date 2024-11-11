/**
 * @file agentrectitem.h
 * @author Mateusz Ambroży
 * @brief Class to display agents
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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
    void stopAnimation();

    void setAnimationSpeed(int speed);
};

#endif
