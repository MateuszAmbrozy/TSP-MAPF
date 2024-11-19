/**
 * @file interactivegraphrectitem.h
 * @author Mateusz Ambroży
 * @brief Base class for interactiveagentgraph and interactivetaskgraph
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef INTERACTIVEGRAPHRECTITEM_H
#define INTERACTIVEGRAPHRECTITEM_H

#include <QObject>  // Dodajemy QObject dla sygnałów i slotów
#include "visualizationgraph.h"

class InteractiveGraphRectItem :public VisualizationGraph
{
    Q_OBJECT

public:
    explicit InteractiveGraphRectItem(map::Graph graph, QGraphicsItem* parent = nullptr);

    virtual void drawGraph(QGraphicsScene* scene) override;

    virtual void handleCellLeftClick(const QPoint& cellPos) = 0;
    virtual void handleCellRightClick(const QPoint& cellPos) = 0;
    virtual void toggleSelectionMode() = 0;

protected:
    bool toggle;
    QPoint getCellFromPosition(const QPointF& position);
};

#endif // INTERACTIVEGRAPHRECTITEM_H
