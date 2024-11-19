#include "sidebar.h"

Sidebar::Sidebar(QWidget *parent, int margin, int spacing)
    : QLayout(parent)
{
    setContentsMargins(margin, margin, margin, margin);
    setSpacing(spacing);
}

Sidebar::Sidebar(int spacing)
{
    setSpacing(spacing);
}

Sidebar::~Sidebar()
{
    while (!list.isEmpty()) {
        delete takeAt(0);
    }
}

void Sidebar::addItem(QLayoutItem *item)
{
    add(item, West);
}

void Sidebar::addWidget(QWidget *widget, Position position)
{
    add(new QWidgetItem(widget), position);
}

Qt::Orientations Sidebar::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

bool Sidebar::hasHeightForWidth() const
{
    return false;
}

int Sidebar::count() const
{
    return list.count();
}

QLayoutItem *Sidebar::itemAt(int index) const
{
    return (index >= 0 && index < list.size()) ? list.at(index)->item : nullptr;
}

QSize Sidebar::minimumSize() const
{
    return calculateSize(MinimumSize);
}

void Sidebar::setGeometry(const QRect &rect)
{
    int eastWidth = 0, westWidth = 0;
    int northHeight = 0, southHeight = 0;
    Wrapper *centerItem = nullptr;

    QLayout::setGeometry(rect);

    for (auto *wrapper : list) {
        QLayoutItem *item = wrapper->item;
        Position position = wrapper->position;

        switch (position) {
        case North:
            item->setGeometry(QRect(rect.x(), northHeight, rect.width(), item->sizeHint().height()));
            northHeight += item->geometry().height() + spacing();
            break;
        case South:
            southHeight += item->sizeHint().height() + spacing();
            item->setGeometry(QRect(rect.x(), rect.bottom() - southHeight + spacing(), rect.width(), item->sizeHint().height()));
            break;
        case Center:
            centerItem = wrapper;
            break;
        default:
            break;
        }
    }

    int centerHeight = rect.height() - northHeight - southHeight;

    for (auto *wrapper : list) {
        QLayoutItem *item = wrapper->item;
        Position position = wrapper->position;

        if (position == West) {
            item->setGeometry(QRect(rect.x() + westWidth, northHeight, item->sizeHint().width(), centerHeight));
            westWidth += item->geometry().width() + spacing();
        } else if (position == East) {
            eastWidth += item->sizeHint().width() + spacing();
            item->setGeometry(QRect(rect.right() - eastWidth + spacing(), northHeight, item->sizeHint().width(), centerHeight));
        }
    }

    if (centerItem) {
        centerItem->item->setGeometry(QRect(westWidth, northHeight, rect.width() - eastWidth - westWidth, centerHeight));
    }
}


QSize Sidebar::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *Sidebar::takeAt(int index)
{
    if (index >= 0 && index < list.size()) {
        Wrapper *wrapper = list.takeAt(index);
        QLayoutItem *item = wrapper->item;
        delete wrapper;
        return item;
    }
    return nullptr;
}

void Sidebar::add(QLayoutItem *item, Position position)
{
    list.append(new Wrapper(item, position));
}

QSize Sidebar::calculateSize(SizeType sizeType) const
{
    QSize totalSize;
    int maxWidth = 0;
    int totalHeight = 0;

    for (const auto *wrapper : list) {
        QSize itemSize = (sizeType == MinimumSize) ? wrapper->item->minimumSize() : wrapper->item->sizeHint();

        if (wrapper->position == North || wrapper->position == South || wrapper->position == Center) {
            totalHeight += itemSize.height();
        }

        if (wrapper->position == West || wrapper->position == East || wrapper->position == Center) {
            maxWidth = qMax(maxWidth, itemSize.width());
        }
    }

    totalSize.setWidth(maxWidth);
    totalSize.setHeight(totalHeight);
    return totalSize;
}

