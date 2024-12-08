#include "inetractivetaskgraph.h"

InteractiveTaskRectItem::InteractiveTaskRectItem(map::Graph graph, const std::vector<Agent>& agents, QGraphicsItem* parent)
    : InteractiveGraphRectItem(graph, parent), agents(agents), currentDropoffPoint(QPoint(-1, -1)) {}

void InteractiveTaskRectItem::handleCellLeftClick(const QPoint& cellPos) {
    qDebug("InteractiveTaskRectItem::handleCellLeftClick\n");

    // Check if cell is an obstacle, an agent position, or already contains a pickup/dropoff point
    if (graph.cells[cellPos.x()][cellPos.y()].isObstacle ||
        isAgentCell(cellPos) ||
        pickupPoints.contains(cellPos) ||
        (cellPos == currentDropoffPoint)) {
        return;
    }

    if (this->toggle) {
        pickupPoints.insert(cellPos);
        updateCellColor(cellPos.x(), cellPos.y(), cellState::PICKUP);
        emit pickupPointSelected(cellPos);
    } else {
        // If a dropoff point already exists, remove it first
        if (currentDropoffPoint != QPoint(-1, -1)) {
            updateCellColor(currentDropoffPoint.x(), currentDropoffPoint.y(), cellState::NORMAL);
            emit dropoffPointRemoved(currentDropoffPoint);
        }

        // Set the new dropoff point
        currentDropoffPoint = cellPos;
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
    } else if (currentDropoffPoint == cellPos) {
        updateCellColor(cellPos.x(), cellPos.y(), cellState::NORMAL);
        emit dropoffPointRemoved(cellPos);
        currentDropoffPoint = QPoint(-1, -1);  // Reset dropoff point
    }
}

void InteractiveTaskRectItem::drawGraph(QGraphicsScene *scene)
{
    int cellSize = setup::cellSize;
    QBrush obstacleBrush(Qt::gray);
    QBrush normalBrush(Qt::white);
    QBrush agentBrush(Qt::green);

    for (int i = 0; i < graph.width; ++i) {
        for (int j = 0; j < graph.height; ++j) {
            const map::Cell& cell = graph.cells[i][j];
            QRectF rect(i * cellSize, j * cellSize, cellSize, cellSize);

            InteractiveCellItem* cellItem = new InteractiveCellItem(rect, QPoint(i, j), this);

            if (isAgentCell(QPoint(i, j))) {
                cellItem->setBrush(agentBrush);
            } else if (cell.isObstacle) {
                cellItem->setBrush(obstacleBrush);
            } else {
                cellItem->setBrush(normalBrush);
            }
            scene->addItem(cellItem);
        }
    }
}

void InteractiveTaskRectItem::clear() {
    pickupPoints.clear();
    dropoffPoints.clear();
    currentDropoffPoint = QPoint(-1, -1);  // Clear the single dropoff point

    int cellSize = setup::cellSize;
    QPen pen(Qt::black);
    QBrush obstacleBrush(Qt::gray);
    QBrush normalBrush(Qt::white);
    QBrush agentBrush(Qt::green);

    for (int i = 0; i < graph.width; ++i) {
        for (int j = 0; j < graph.height; ++j) {
            map::Cell cell = graph.cells[i][j];
            QRectF rect(i * cellSize, j * cellSize, cellSize, cellSize);

            QGraphicsRectItem* rectItem = scene()->addRect(rect, pen);

            if (isAgentCell(QPoint(i, j))) {
                rectItem->setBrush(agentBrush);
            } else if (cell.isObstacle) {
                rectItem->setBrush(obstacleBrush);
            } else {
                rectItem->setBrush(normalBrush);
            }
        }
    }

    update();
}

void InteractiveTaskRectItem::updateCellColor(int x, int y, cellState state) {
    if (isAgentCell(QPoint(x, y))) return;

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

// Helper function to check if a given cell is occupied by an agent
bool InteractiveTaskRectItem::isAgentCell(const QPoint& cellPos) const {
    for (const Agent& agent : agents) {
        if (agent.getPosition().x == cellPos.x() && agent.getPosition().y == cellPos.y()) {
            return true;
        }
    }
    return false;
}
void InteractiveTaskRectItem::toggleSelectionMode()
{
    this->toggle = !this->toggle;
}
