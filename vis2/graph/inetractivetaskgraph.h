#ifndef INTERACTIVETASKGRAPH_H
#define INTERACTIVETASKGRAPH_H

#include <QSet>
#include <QPoint>
#include <QGraphicsScene>
#include "interactivegraphrectitem.h"

enum class cellState {NORMAL = 0, PICKUP, DROPOFF};

class InteractiveTaskRectItem :public InteractiveGraphRectItem
{
    Q_OBJECT

public:
    InteractiveTaskRectItem(map::Graph graph, QGraphicsItem* parent = nullptr);

    QSet<QPoint> pickupPoints;
    QSet<QPoint> dropoffPoints;

    void handleCellLeftClick(const QPoint& cellPos) override;
    void handleCellRightClick(const QPoint& cellPos) override;
    void clear();

signals:
    void pickupPointSelected(const QPoint& point);
    void dropoffPointSelected(const QPoint& point);
    void pickupPointRemoved(const QPoint& point);
    void dropoffPointRemoved(const QPoint& point);

private:
    void updateCellColor(int x, int y, cellState state);
};

#endif // INTERACTIVETASKGRAPH_H
