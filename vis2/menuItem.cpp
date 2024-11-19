
#include "menuItem.h"

#include <QLayout>
#include <QLabel>


MenuItem::MenuItem(QWidget *parent) : QWidget(parent)
{

    _iconLabel = new QLabel;
    _titleLabel = new QLabel;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_iconLabel);
    layout->addWidget(_titleLabel);
    setLayout(layout);
}
void MenuItem::setIconLabel(const QPixmap& icon)
{
    _iconLabel->setPixmap(icon);
}
void MenuItem::setTitleLabel(const QString& title)
{
    _titleLabel->setText(title);
}



