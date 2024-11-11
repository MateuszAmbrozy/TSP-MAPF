/**
 * @file visualizationgraph.h
 * @author Mateusz Ambroży
 * @brief Displaying graph
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef VISUALIZATIONGRAPH_H
#define VISUALIZATIONGRAPH_H

#include <QGraphicsRectItem>
#include <QObject> // Dodajemy QObject dla mechanizmu MOC
#include "../../lib/Graph.h"

class VisualizationGraph : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    explicit VisualizationGraph(map::Graph graph, QGraphicsItem* parent = nullptr);

protected:
    map::Graph graph;
    virtual void drawGraph(QGraphicsScene* scene) = 0;
};

#endif // VISUALIZATIONGRAPH_H
