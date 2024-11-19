#include "interactivecellitem.h"
#include "inetractivetaskgraph.h"
#include "qevent.h"
#include <iostream>

InteractiveCellItem::InteractiveCellItem(const QRectF &rect, QPoint cellPosition, InteractiveGraphRectItem* parent)
    : QGraphicsRectItem(rect), cellPosition(cellPosition), parentGraphItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsFocusable);

}
void InteractiveCellItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

    if (event->button() == Qt::LeftButton)
    {
        parentGraphItem->handleCellLeftClick(cellPosition);
    }
    if (event->button() == Qt::RightButton)
    {
        parentGraphItem->handleCellRightClick(cellPosition);
    }
    event->accept();
}

void InteractiveCellItem::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        parentGraphItem->toggleSelectionMode();
    }
    event->accept();
}
