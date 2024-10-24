#ifndef TASKGROUPSCENE_H
#define TASKGROUPSCENE_H

#include <QGraphicsScene>
#include <vector>
#include "../lib/TaskGroup.h"

class TaskGroupScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit TaskGroupScene(QObject *parent = nullptr);

    // Metody do dodawania punktów odbioru i dostawy
    void addPickupPoint(int x, int y, int stopTime);
    void setDropoffPoint(int x, int y, int stopTime);

    // Tworzenie TaskGroup na podstawie danych
    TaskGroup createTaskGroup(int capacity);

private:
    std::vector<map::Cell> pickupPoints;   // Lista punktów odbioru
    map::Cell dropoffPoint;                // Punkt dostawy
    std::vector<int> stopTimes;            // Czas postoju na punktach
    int dropoffStopTime;                   // Czas postoju przy dostawie
};

#endif // TASKGROUPSCENE_H
