/**
 * @file interactiveagentgraph.h
 * @author Mateusz Ambroży
 * @brief Graph to add agents and obstacles
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef INTERACTIVEAGENTGRAPH_H
#define INTERACTIVEAGENTGRAPH_H

#include <QSet>
#include <QPoint>
#include <QGraphicsScene>
#include "interactivegraphrectitem.h"



class InteractiveAgentRectItem : public InteractiveGraphRectItem
{
    Q_OBJECT

public:
    enum class Option {NORMAL = 0, AGENT, OBSTACLE, PICKUP, DROPOFF};
    InteractiveAgentRectItem(map::Graph graph, QGraphicsItem* parent = nullptr);

    QSet<QPoint> agentPoints;
    QSet<QPoint> obstaclePoints;
    QSet<QPoint> avaliablePickupPoints;
    QSet<QPoint> avaliableDropoffPoints;

    void handleCellLeftClick(const QPoint& cellPos) override;
    void handleCellRightClick(const QPoint& cellPos) override;
    void updateCellColor(int x, int y, Option state);
    void clear();



signals:
    void agentSelected(const QPoint& point);
    void obstacleSelected(const QPoint& point);
    void agentRemoved(const QPoint& point);
    void obstacleRemoved(const QPoint& point);

    void pickupSelected(const QPoint& point);
    void pickupRemoved(const QPoint& point);
    void dropoffSelected(const QPoint& point);
    void dropoffRemoved(const QPoint& point);

private:
    Option option;
    void toggleSelectionMode() override;
};

#endif // INTERACTIVEAGENTGRAPH_H
