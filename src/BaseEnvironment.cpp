#include "../lib/BaseEnvironment.h"
#include "../lib/setup.h"

BaseEnvironment::BaseEnvironment(map::Graph graph)
    : graph(graph),
    tsp(graph)
{
    srand(time(0));
}
TaskGroup BaseEnvironment::TASKGROUPGENERATOR(std::vector<int> avaliablePickupX, std::vector<int> avaliablePickupY, std::vector<int> avaliableDropofX, std::vector<int> avaliableDropofY ) {
    int numTasks = rand() % setup::maxTasks + 1;

    std::vector<map::Cell> pickupPoints;

    for (int i = 0; i < numTasks; ++i) {
        int x = *select_randomly(avaliablePickupX.begin(), avaliablePickupX.end());
        int y = *select_randomly(avaliablePickupY.begin(), avaliablePickupY.end());
        pickupPoints.push_back(map::Cell(x, y));
    }


    int dropoffX = *select_randomly(avaliableDropofX.begin(), avaliableDropofX.end());
    int dropoffY = *select_randomly(avaliableDropofY.begin(), avaliableDropofY.end());
    map::Cell dropoffPoint(dropoffX, dropoffY);


    int taskCapacity = rand() % setup::maxCapacity + 1;
    int dropoffTime;
    std::vector<int> stopTimes;
    if (setup::maxStopTime > 0) {
        for(int i=0; i<numTasks; i++)
        {
            stopTimes.push_back(rand() % setup::maxStopTime);
        }
        dropoffTime = rand() % setup::maxStopTime;
    }
    else
    {
        stopTimes = std::vector<int>(numTasks, 0);  // Default to 0 if maxStopTime is 0
        dropoffTime = 0;
    }

    TaskGroup taskGroup(taskCapacity, pickupPoints, dropoffPoint, stopTimes, dropoffTime);

    return taskGroup;
}

void BaseEnvironment::addTaskGroup(const TaskGroup& taskGroup)
{
    task_list.push_back(taskGroup);
}
