#ifndef INTERACTIVEGRAPHRECTITEM_H
#define INTERACTIVEGRAPHRECTITEM_H

#include <QObject>  // Dodajemy QObject dla sygnałów i slotów
#include "visualizationgraph.h"

class InteractiveGraphRectItem :public VisualizationGraph
{
    Q_OBJECT

public:
    explicit InteractiveGraphRectItem(map::Graph graph, QGraphicsItem* parent = nullptr);

    void drawGraph(QGraphicsScene* scene) override;

    virtual void handleCellLeftClick(const QPoint& cellPos) = 0;
    virtual void handleCellRightClick(const QPoint& cellPos) = 0;
    void toggleSelectionMode();

protected:
    bool toggle;
    QPoint getCellFromPosition(const QPointF& position);
};

#endif // INTERACTIVEGRAPHRECTITEM_H
