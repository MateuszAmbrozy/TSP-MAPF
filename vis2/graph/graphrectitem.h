#ifndef GRAPHRECTITEM_H
#define GRAPHRECTITEM_H

#include "visualizationgraph.h"

class GraphRectItem : public VisualizationGraph {
public:
    GraphRectItem(map::Graph graph, QWidget* widget = nullptr, QGraphicsItem* parent = nullptr);
    void drawGraph(QGraphicsScene* scene) override;
};

#endif // GRAPHRECTITEM_H
