#include "taskgroupscene.h"
#include <QPen>
#include <QBrush>

TaskGroupScene::TaskGroupScene(QObject *parent)
    : QGraphicsScene(parent), dropoffStopTime(0)
{
    setBackgroundBrush(Qt::lightGray);
}

void TaskGroupScene::addPickupPoint(int x, int y, int stopTime)
{
    if (x < 0 || y < 0) {
        throw std::invalid_argument("Coordinates must be positive!");
    }
    map::Cell pickup(x, y);
    pickupPoints.push_back(pickup);
    stopTimes.push_back(stopTime);
}
void TaskGroupScene::setDropoffPoint(int x, int y, int stopTime)
{
    dropoffPoint = map::Cell(x, y);
    dropoffStopTime = stopTime;
}

TaskGroup TaskGroupScene::createTaskGroup(int capacity)
{
    if (pickupPoints.empty())
    {
        throw std::runtime_error("No pickup points!");
    }
    return TaskGroup(capacity, pickupPoints, dropoffPoint, stopTimes, dropoffStopTime);
}

