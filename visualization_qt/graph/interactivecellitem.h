/**
 * @file interactivecellitem.h
 * @author Mateusz Ambroży
 * @brief Class of cell which can be interactive with user (LMB/RMB)
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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

class InteractiveCellItem : public QGraphicsRectItem
{
public:
    explicit InteractiveCellItem(const QRectF &rect, QPoint cellPosition, InteractiveGraphRectItem* parent);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
private:
    QPoint cellPosition;  // Store the cell's grid position
    InteractiveGraphRectItem* parentGraphItem;  // Pointer to the parent graph item
};

#endif // INTERACTIVECELLITEM_H
