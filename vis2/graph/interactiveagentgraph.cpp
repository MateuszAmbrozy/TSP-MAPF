#include "interactiveagentgraph.h"

InteractiveAgentRectItem::InteractiveAgentRectItem(map::Graph graph, QGraphicsItem* parent)
    : InteractiveGraphRectItem(graph, parent) {
}

void InteractiveAgentRectItem::handleCellLeftClick(const QPoint& cellPos) {
    if (graph.cells[cellPos.x()][cellPos.y()].isObstacle ||
        agentPoints.contains(cellPos) ||
        obstaclePoints.contains(cellPos)) {
        return;
    }

    if (this->toggle)
    {
        agentPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::AGENT);
        emit agentSelected(cellPos);
    } else {
        obstaclePoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::OBSTACLE);
        emit obstacleSelected(cellPos);
    }
}

void InteractiveAgentRectItem::handleCellRightClick(const QPoint& cellPos) {
    if (agentPoints.contains(cellPos)) {
        agentPoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::NORMAL);
        emit agentRemoved(cellPos);
    } else if (obstaclePoints.contains(cellPos)) {
        obstaclePoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::NORMAL);
        emit obstacleRemoved(cellPos);
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

void InteractiveAgentRectItem::updateCellColor(int x, int y, cellState state) {
    int cellSize = 20;
    QPen pen(Qt::black);
    QRectF rect(x * cellSize, y * cellSize, cellSize, cellSize);

    QGraphicsRectItem* rectItem = scene()->addRect(rect, pen);

    if (state == cellState::AGENT) {
        rectItem->setBrush(QBrush(Qt::green));
    } else if (state == cellState::OBSTACLE) {
        rectItem->setBrush(QBrush(Qt::gray));
    } else {
        rectItem->setBrush(QBrush(Qt::white));
    }

    update();
}
