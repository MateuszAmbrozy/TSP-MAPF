#include "graphrectitem.h"
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>

GraphRectItem::GraphRectItem(map::Graph graph, QWidget* parent)
    : QGraphicsRectItem(nullptr), graph(graph) {}

void GraphRectItem::drawGraph(QGraphicsScene* scene)
{
    int cellSize = 20;
    QPen pen(Qt::black);
    QBrush obstacleBrush(Qt::gray); // Gray brush for obstacles
    QBrush normalBrush(Qt::white); // White brush for normal cells

    for (int i = 0; i < graph.width; ++i)
    {
        for (int j = 0; j < graph.height; ++j)
        {
            map::Cell& cell = graph.cells[i][j];

            QRectF rect(i * cellSize, j * cellSize, cellSize, cellSize);
            QGraphicsRectItem* rectItem = scene->addRect(rect, pen);

            if (cell.isObstacle) {
                rectItem->setBrush(obstacleBrush);
            } else {
                rectItem->setBrush(normalBrush);
            }
        }
    }
}
