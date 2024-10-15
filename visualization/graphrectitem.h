#ifndef GRAPHRECTITEM_H
#define GRAPHRECTITEM_H

#include <QGraphicsRectItem>
#include <QWidget>
#include <QPen>
#include <QBrush>
#include "../lib/Graph.h"

class GraphRectItem: public QGraphicsRectItem
{

private:
    map::Graph graph;

public:
    GraphRectItem(map::Graph graph, QWidget* parent = nullptr);

    void drawGraph(QGraphicsScene* scene);
};

#endif // GRAPHRECTITEM_H
