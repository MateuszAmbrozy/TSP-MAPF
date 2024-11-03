#ifndef TASKINPUTWIDGET_H
#define TASKINPUTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QListWidget>
// #include "taskgroupscene.h"
#include "../lib/TaskGroup.h"
#include "graph/inetractivetaskgraph.h"
#include "pointlistitemwidget.h"
#include <QMessageBox>

class TaskInputWidget : public QWidget
{
    Q_OBJECT

public:
    TaskInputWidget(QGraphicsScene *scene, InteractiveTaskRectItem *graphItem, QWidget *parent = nullptr);

private slots:
    void addPickupPoint(const QPoint &point);
    void setDropoffPoint(const QPoint &point);
    void removePickupPoint(const QPoint& point);
    void removeDropoffPoint(const QPoint& point);
    void onPickupStopTimeChanged(const QPoint &point, int newStopTime);
    void onDropoffStopTimeChanged(const QPoint &point, int newStopTime);
    void createTaskGroup();

private:
    QGraphicsScene *scene;
    InteractiveTaskRectItem *graphItem;
    QLineEdit *capacityInput;
    QListWidget *pointListWidget;
    QSpinBox *stopTimeInput;


    std::vector<map::Cell> pickupPoints;
    map::Cell dropoffPoint;
    std::vector<int> stopTimes;
    int dropoffStopTime;


    struct PointData
    {
        QPoint point;
        int value;
    };

    std::vector<PointData> pickupPointData;
    PointData dropoffPointData;

    void updatePointList();
signals:
    void taskGroupCreated(std::shared_ptr<TaskGroup> taskGroup);

};

#endif // TASKINPUTWIDGET_H
