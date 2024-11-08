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

#endif // SIDEBARITEM_H
