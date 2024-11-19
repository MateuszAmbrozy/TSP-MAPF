/**
 * @file inetractivetaskgraph.cpp
 * @author Mateusz Ambroży
 * @brief Graph to add tasks
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef INTERACTIVETASKGRAPH_H
#define INTERACTIVETASKGRAPH_H

#include <QSet>
#include <QPoint>
#include <QGraphicsScene>
#include "interactivegraphrectitem.h"
#include "interactivecellitem.h"
#include "../lib/Agent.h"
#include "../lib/setup.h"
enum class cellState {NORMAL = 0, PICKUP, DROPOFF};

class InteractiveTaskRectItem :public InteractiveGraphRectItem
{
    Q_OBJECT

public:
    InteractiveTaskRectItem(map::Graph graph, const std::vector<Agent>& agents, QGraphicsItem* parent = nullptr);

    QSet<QPoint> pickupPoints;
    QSet<QPoint> dropoffPoints;

    void handleCellLeftClick(const QPoint& cellPos) override;
    void handleCellRightClick(const QPoint& cellPos) override;
    void drawGraph(QGraphicsScene *scene) override;
    void clear();

signals:
    void pickupPointSelected(const QPoint& point);
    void dropoffPointSelected(const QPoint& point);
    void pickupPointRemoved(const QPoint& point);
    void dropoffPointRemoved(const QPoint& point);

private:
    void updateCellColor(int x, int y, cellState state);
    std::vector<Agent> agents;
    bool isAgentCell(const QPoint& cellPos) const;

    QPoint currentDropoffPoint;
    void toggleSelectionMode() override;
};

#endif // INTERACTIVETASKGRAPH_H
