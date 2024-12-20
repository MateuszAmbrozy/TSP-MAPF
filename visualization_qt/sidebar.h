/**
 * @file borderlayout.h
 * @author Mateusz Ambroży
 * @brief Border for menu
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QLayout>
#include <QRect>

class Sidebar : public QLayout
{
public:
    enum Position { West, North, South, East, Center };

    explicit Sidebar(QWidget *parent, int margin = 0, int spacing = -1);
    Sidebar(int spacing = -1);
    ~Sidebar();

    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;
    void addWidget(QWidget *widget, Position position);
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;
    int count() const Q_DECL_OVERRIDE;
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;
    QSize minimumSize() const Q_DECL_OVERRIDE;
    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

    void add(QLayoutItem *item, Position position);

private:
    struct Wrapper
    {
        Wrapper(QLayoutItem *i, Position p) {
            item = i;
            position = p;
        }

        QLayoutItem *item;
        Position position;
    };

    enum SizeType { MinimumSize, SizeHint };
    QSize calculateSize(SizeType sizeType) const;

    QList<Wrapper *> list;
};

#endif
