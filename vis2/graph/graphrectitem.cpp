#include "graphrectitem.h"
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>

GraphRectItem::GraphRectItem(map::Graph graph,QWidget* widget, QGraphicsItem* parent)
    : VisualizationGraph(graph, parent) {}

void GraphRectItem::drawGraph(QGraphicsScene* scene) {
    int cellSize = 20;
    QPen pen(Qt::black);
    QBrush obstacleBrush(Qt::gray);
    QBrush normalBrush(Qt::white);

    for (int i = 0; i < graph.width; ++i) {
        for (int j = 0; j < graph.height; ++j) {
            map::Cell cell = graph.cells[i][j];

            QRectF rect(i * cellSize, j * cellSize, cellSize, cellSize);
            QGraphicsRectItem* rectItem = scene->addRect(rect, pen);

            rectItem->setBrush(cell.isObstacle ? obstacleBrush : normalBrush);
        }
    }
}
