/**
 * @file sidebaritem.h
 * @author Mateusz Ambroży
 * @brief one item for menu bar
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>

class QLabel;

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(QWidget *parent = 0);

    void setIconLabel(const QPixmap& icon);
    void setTitleLabel(const QString& title);

signals:

public slots:

private:
    QLabel * _iconLabel;
    QLabel * _titleLabel;
};

#endif
