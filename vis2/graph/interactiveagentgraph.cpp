#include "interactiveagentgraph.h"

InteractiveAgentRectItem::InteractiveAgentRectItem(map::Graph graph, QGraphicsItem* parent)
    : InteractiveGraphRectItem(graph, parent), option(Option::AGENT) {
}

void InteractiveAgentRectItem::handleCellLeftClick(const QPoint& cellPos) {
    if (graph.cells[cellPos.x()][cellPos.y()].isObstacle ||
        agentPoints.contains(cellPos) ||
        obstaclePoints.contains(cellPos) ||
        avaliablePickupPoints.contains(cellPos) ||
        avaliableDropoffPoints.contains(cellPos))
    {
        return;
    }

    if (option == Option::AGENT)
    {
        agentPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::AGENT);
        emit agentSelected(cellPos);
    }
    else if(option == Option::OBSTACLE)
    {
        obstaclePoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::OBSTACLE);
        emit obstacleSelected(cellPos);
    }
    else if (option == Option::PICKUP)
    {
        avaliablePickupPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::PICKUP);
        emit pickupSelected(cellPos);
    }
    else if (option == Option::DROPOFF)
    {
        avaliableDropoffPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::DROPOFF);
        emit dropoffSelected(cellPos);
    }
}

void InteractiveAgentRectItem::handleCellRightClick(const QPoint& cellPos)
{
    if (agentPoints.contains(cellPos))
    {
        agentPoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::NORMAL);
        emit agentRemoved(cellPos);
    }
    else if (obstaclePoints.contains(cellPos))
    {
        obstaclePoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::NORMAL);
        emit obstacleRemoved(cellPos);
    }
    else if (avaliablePickupPoints.contains(cellPos))
    {
        avaliablePickupPoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::NORMAL);
        emit pickupRemoved(cellPos);
    }
    else if (avaliableDropoffPoints.contains(cellPos))
    {
        avaliableDropoffPoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), Option::NORMAL);
        emit dropoffRemoved(cellPos);
    }

}

void InteractiveAgentRectItem::clear() {
    agentPoints.clear();
    obstaclePoints.clear();

    int cellSize = 20;
    QPen pen(Qt::black);
    QBrush obstacleBrush(Qt::gray);
    QBrush normalBrush(Qt::white);

    for (int i = 0; i < graph.width; ++i) {
        for (int j = 0; j < graph.height; ++j) {
            map::Cell cell = graph.cells[i][j];
            QRectF rect(i * cellSize, j * cellSize, cellSize, cellSize);

            QGraphicsRectItem* rectItem = scene()->addRect(rect, pen);

            rectItem->setBrush(cell.isObstacle ? obstacleBrush : normalBrush);
        }
    }

    update();
}
void InteractiveAgentRectItem::toggleSelectionMode()
{
    switch (option)
    {
    case Option::OBSTACLE: option = Option::AGENT; break;
        case Option::AGENT: option = Option::PICKUP; break;
        case Option::PICKUP: option = Option::DROPOFF; break;
        case Option::DROPOFF: option = Option::OBSTACLE; break;
        default: option = Option::OBSTACLE;
    }
}
void InteractiveAgentRectItem::updateCellColor(int x, int y, Option state) {
    int cellSize = 20;
    QPen pen(Qt::black);
    QRectF rect(x * cellSize, y * cellSize, cellSize, cellSize);

    QGraphicsRectItem* rectItem = scene()->addRect(rect, pen);

    if (state == Option::AGENT)
    {
        rectItem->setBrush(QBrush(Qt::green));
    }
    else if (state == Option::OBSTACLE)
    {
        rectItem->setBrush(QBrush(Qt::gray));
    }
    else if (state == Option::PICKUP)
    {
        rectItem->setBrush(QBrush(Qt::cyan));
    }
    else if (state == Option::DROPOFF)
    {
        rectItem->setBrush(QBrush(Qt::black));
    }
    else
    {
        rectItem->setBrush(QBrush(Qt::white));
    }

    update();
}
