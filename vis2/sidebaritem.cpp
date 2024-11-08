
#include "sidebaritem.h"

#include <QLayout>
#include <QLabel>


SidebarItem::SidebarItem(QWidget *parent) : QWidget(parent)
{

    _iconLabel = new QLabel;
    _titleLabel = new QLabel;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_iconLabel);
    layout->addWidget(_titleLabel);
    setLayout(layout);
}
void SidebarItem::setIconLabel(const QPixmap& icon)
{
    _iconLabel->setPixmap(icon);
}
void SidebarItem::setTitleLabel(const QString& title)
{
    _titleLabel->setText(title);
}



