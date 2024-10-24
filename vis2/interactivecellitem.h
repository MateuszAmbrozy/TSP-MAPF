#ifndef INTERACTIVECELLITEM_H
#define INTERACTIVECELLITEM_H
#include <QGraphicsRectItem>
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QSet>
#include <QPoint>
#include <QGraphicsScene>
#include "interactivegraphrectitem.h"
#include "qgraphicssceneevent.h"

// class InteractiveGraphRectItem;

class InteractiveCellItem : public QGraphicsRectItem {
public:
    InteractiveCellItem(const QRectF &rect, QPoint cellPosition, InteractiveGraphRectItem* parent);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
private:
    QPoint cellPosition;  // Store the cell's grid position
    InteractiveGraphRectItem* parentGraphItem;  // Pointer to the parent graph item
};

#endif // INTERACTIVECELLITEM_H
