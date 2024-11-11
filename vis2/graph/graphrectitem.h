/**
 * @file graphrectitem.h
 * @author Mateusz Ambroży
 * @brief Visualization of Graph
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef GRAPHRECTITEM_H
#define GRAPHRECTITEM_H

#include "visualizationgraph.h"

class GraphRectItem : public VisualizationGraph {
public:
    GraphRectItem(map::Graph graph, QWidget* widget = nullptr, QGraphicsItem* parent = nullptr);
    void drawGraph(QGraphicsScene* scene) override;
};

#endif // GRAPHRECTITEM_H
