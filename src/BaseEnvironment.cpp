#include "../lib/BaseEnvironment.h"
#include "../lib/setup.h"

BaseEnvironment::BaseEnvironment(map::Graph graph, std::vector<std::pair<int, int>> avaliablePickups, std::vector<std::pair<int, int>> avaliableDropoff)
    : graph(graph),
    tsp(graph),
    avaliablePickups(avaliablePickups),
    avaliableDropoff(avaliableDropoff)
{
    //srand(time(0));
    table = Reservation();
}
TaskGroup BaseEnvironment::TASKGROUPGENERATOR()
{
    int numTasks = rand() % setup::maxTasks + 1;

    if(avaliablePickups.empty() || avaliableDropoff.empty()) return {};


    std::vector<map::Cell> pickupPoints;
    for (int i = 0; i < numTasks; ++i) {
        auto [x, y] = *select_randomly(avaliablePickups.begin(), avaliablePickups.end());
        pickupPoints.push_back(map::Cell(x, y));
    }


    auto [dropoffX, dropoffY] = *select_randomly(avaliableDropoff.begin(), avaliableDropoff.end());
    map::Cell dropoffPoint(dropoffX, dropoffY);


    int taskCapacity = rand() % setup::maxCapacity + 1;


    int dropoffTime;
    std::vector<int> stopTimes;
    if (setup::maxStopTime > 0)
    {
        for (int i = 0; i < numTasks; ++i)
        {
            stopTimes.push_back(rand() % setup::maxStopTime);
        }
        dropoffTime = rand() % setup::maxStopTime;
    } else
    {
        stopTimes = std::vector<int>(numTasks, 0); // Domyślnie 0, jeśli brak ograniczenia
        dropoffTime = 0;
    }

    // Utworzenie grupy zadań
    TaskGroup taskGroup(taskCapacity, pickupPoints, dropoffPoint, stopTimes, dropoffTime);

    return taskGroup;
}

void BaseEnvironment::addTaskGroup(const TaskGroup& taskGroup)
{
    task_list.push_back(taskGroup);
}

void BaseEnvironment::assignTasks(std::vector<TaskGroup> tasks)
{
    this->task_list = tasks;
}

