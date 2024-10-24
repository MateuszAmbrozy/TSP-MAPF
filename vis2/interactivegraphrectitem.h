#ifndef INTERACTIVEGRAPHRECTITEM_H
#define INTERACTIVEGRAPHRECTITEM_H

#include <QGraphicsRectItem>
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QSet>
#include <QPoint>
#include <QGraphicsScene>
#include "../lib/Graph.h"
#include "pointlistitemwidget.h"

enum class cellState {NORMAL = 0, PICKUP, DROPOFF};


class InteractiveCellItem;

class InteractiveGraphRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

private:
    bool selectingPickup;
    map::Graph graph;

public:
    InteractiveGraphRectItem(map::Graph graph, QWidget* parent = nullptr);
    void drawGraph(QGraphicsScene* scene);

    QSet<QPoint> pickupPoints;
    QSet<QPoint> dropoffPoints;
    void handleCellLeftClick(const QPoint& cellPos);
    void handleCellRightClick(const QPoint& cellPos);
    void clear();
    void toggleSelectionMode();
signals:
    void pickupPointSelected(const QPoint& point);
    void dropoffPointSelected(const QPoint& point);
    void pickupPointRemoved(const QPoint& point);
    void dropoffPointRemoved(const QPoint& point);



private:
    void updateCellColor(int x, int y, cellState state);
    QPoint getCellFromPosition(const QPointF& position);
};

#endif
