#include "interactivegraphrectitem.h"
#include "interactivecellitem.h"

InteractiveGraphRectItem::InteractiveGraphRectItem(map::Graph graph, QGraphicsItem* parent)
    : VisualizationGraph(graph, parent), toggle(true) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptTouchEvents(true);
    setCursor(Qt::PointingHandCursor);
}

void InteractiveGraphRectItem::drawGraph(QGraphicsScene *scene) {
    int cellSize = 20;
    //QPen pen(Qt::black);
    QBrush obstacleBrush(Qt::gray);
    QBrush normalBrush(Qt::white);

    for (int i = 0; i < graph.width; ++i) {
        for (int j = 0; j < graph.height; ++j) {
            const map::Cell& cell = graph.cells[i][j];
            QRectF rect(i * cellSize, j * cellSize, cellSize, cellSize);

            InteractiveCellItem* cellItem = new InteractiveCellItem(rect, QPoint(i, j), this);
            cellItem->setBrush(cell.isObstacle ? obstacleBrush : normalBrush);
            scene->addItem(cellItem);
        }
    }
}

QPoint InteractiveGraphRectItem::getCellFromPosition(const QPointF& position) {
    int cellSize = 20;
    int x = position.x() / cellSize;
    int y = position.y() / cellSize;
    return QPoint(x, y);
}

void InteractiveGraphRectItem::toggleSelectionMode() {
    toggle = !toggle;
}
