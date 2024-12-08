/**
 * @file pointlistitemwidget.h
 * @author your name (you@domain.com)
 * @brief widgets on menu bar
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <QWidget>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>

class PointListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointListItemWidget(const QString &pointType, const QPoint &point, int stopTime, QWidget *parent = nullptr)
        : QWidget(parent), point(point)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);

        QLabel *label = new QLabel(QString("%1: (%2, %3)").arg(pointType).arg(point.x()).arg(point.y()), this);
        layout->addWidget(label);

        spinBox = new QSpinBox(this);
        spinBox->setRange(0, 1000);
        spinBox->setValue(stopTime);
        layout->addWidget(spinBox);

        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &PointListItemWidget::onStopTimeChanged);
    }

    int stopTime() const { return spinBox->value(); }
    QPoint getPoint() const { return point; }

signals:
    void stopTimeChanged(const QPoint &point, int stopTime);

private slots:
    void onStopTimeChanged(int newStopTime)
    {
        emit stopTimeChanged(point, newStopTime);
    }

private:
    QPoint point;
    QSpinBox *spinBox;
};
