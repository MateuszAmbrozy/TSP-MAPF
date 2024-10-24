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
    void addPickupPoint(const QPoint &point);  // Slot to handle pickup point selection
    void setDropoffPoint(const QPoint &point); // Slot to handle dropoff point selection
    void removePickupPoint(const QPoint& point);
    void removeDropoffPoint(const QPoint& point);
    void onPickupStopTimeChanged(const QPoint &point, int newStopTime);
    void onDropoffStopTimeChanged(const QPoint &point, int newStopTime);
    void createTaskGroup(); // Slot to create TaskGroup

private:
    QGraphicsScene *scene;  // Pointer to the scene
    InteractiveTaskRectItem *graphItem; // Pointer to the interactive map item
    QLineEdit *capacityInput;
    QListWidget *pointListWidget;
    QSpinBox *stopTimeInput;


    std::vector<map::Cell> pickupPoints;   // Lista punktów odbioru
    map::Cell dropoffPoint;                // Punkt dostawy
    std::vector<int> stopTimes;            // Czas postoju na punktach
    int dropoffStopTime;


    struct PointData
    {
        QPoint point;
        int value;
    };

    std::vector<PointData> pickupPointData; // Store pickup points and stop times
    PointData dropoffPointData;             // Store dropoff point and stop time

    void updatePointList();  // Update the list of points with stop times
signals:
    void taskGroupCreated(std::shared_ptr<TaskGroup> taskGroup);

};

#endif // TASKINPUTWIDGET_H
