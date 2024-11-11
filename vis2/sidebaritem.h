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
#ifndef SIDEBARITEM_H
#define SIDEBARITEM_H

#include <QWidget>

class QLabel;

class SidebarItem : public QWidget
{
    Q_OBJECT
public:
    explicit SidebarItem(QWidget *parent = 0);

    void setIconLabel(const QPixmap& icon);
    void setTitleLabel(const QString& title);

signals:

public slots:

private:
    QLabel * _iconLabel;
    QLabel * _titleLabel;
};

#endif
