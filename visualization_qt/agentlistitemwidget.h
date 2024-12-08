/**
 * @file agentlistitemwidget.h
 * @author Mateusz Ambroży
 * @brief QSpinbox to display capacity of agents and edit them
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef AGENTLISTITEMWIDGET_H
#define AGENTLISTITEMWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>

class AgentListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AgentListItemWidget(const QString &agentType, const QPoint &position, int capacity, QWidget *parent = nullptr)
        : QWidget(parent), position(position)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);

        QLabel *label = new QLabel(QString("%1: (%2, %3)").arg(agentType).arg(position.x()).arg(position.y()), this);
        layout->addWidget(label);

        spinBox = new QSpinBox(this);
        spinBox->setRange(0, 1000);
        spinBox->setValue(capacity);
        layout->addWidget(spinBox);

        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &AgentListItemWidget::onCapacityChanged);
    }

    int capacity() const { return spinBox->value(); }
    QPoint getPosition() const { return position; }

signals:
    void capacityChanged(const QPoint &position, int capacity);

private slots:
    void onCapacityChanged(int newCapacity)
    {
        emit capacityChanged(position, newCapacity);
    }

private:
    QPoint position;
    QSpinBox *spinBox;
};

#endif // AGENTLISTITEMWIDGET_H
