#include "inetractivetaskgraph.h"

InteractiveTaskRectItem::InteractiveTaskRectItem(map::Graph graph, QGraphicsItem* parent)
    : InteractiveGraphRectItem(graph, parent) {
}

void InteractiveTaskRectItem::handleCellLeftClick(const QPoint& cellPos) {
    qDebug("InteractiveTaskRectItem::handleCellLeftClick\n");

    if (graph.cells[cellPos.x()][cellPos.y()].isObstacle ||
        pickupPoints.contains(cellPos) ||
        dropoffPoints.contains(cellPos)) {
        return;
    }

    if (this->toggle) {
        pickupPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::PICKUP);
        emit pickupPointSelected(cellPos);
    } else {
        dropoffPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::DROPOFF);
        emit dropoffPointSelected(cellPos);
    }
}

void InteractiveTaskRectItem::handleCellRightClick(const QPoint& cellPos) {
    qDebug("InteractiveTaskRectItem::handleCellRightClick\n");

    if (pickupPoints.contains(cellPos)) {
        pickupPoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::NORMAL);
        emit pickupPointRemoved(cellPos);
    } else if (dropoffPoints.contains(cellPos)) {
        dropoffPoints.remove(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::NORMAL);
        emit dropoffPointRemoved(cellPos);
    }
}

void InteractiveTaskRectItem::clear() {
    pickupPoints.clear();
    dropoffPoints.clear();

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

void InteractiveTaskRectItem::updateCellColor(int x, int y, cellState state) {
    int cellSize = 20;
    QPen pen(Qt::black);
    QRectF rect(x * cellSize, y * cellSize, cellSize, cellSize);

    QGraphicsRectItem* rectItem = scene()->addRect(rect, pen);

    if (state == cellState::PICKUP) {
        rectItem->setBrush(QBrush(Qt::red));
    } else if (state == cellState::DROPOFF) {
        rectItem->setBrush(QBrush(Qt::black));
    } else {
        rectItem->setBrush(QBrush(Qt::white));
    }

    update();
}
