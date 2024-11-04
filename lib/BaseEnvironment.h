#ifndef BASEENVIRONMENT_H
#define BASEENVIRONMENT_H
#include "Reservation.h"
#include "TaskGroup.h"
#include "TSP.h"
#include <windows.h>
#include  <random>
#include  <iterator>
class BaseEnvironment
{
protected:
    std::vector<TaskGroup> task_list;
    map::Graph graph;
    TSP tsp;
    Reservation table;

    template<typename Iter, typename RandomGenerator>
    Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    }

    template<typename Iter>
    Iter select_randomly(Iter start, Iter end) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return select_randomly(start, end, gen);
    }

public:
    BaseEnvironment(map::Graph graph);
    TaskGroup TASKGROUPGENERATOR(std::vector<int> avaliablePickupX, std::vector<int> avaliablePickupY, std::vector<int> avaliableDropofX, std::vector<int> avaliableDropofY );
    virtual void addTaskGroup(const TaskGroup& taskGroup);


    virtual void assignVacantAgents() = 0;
    virtual void MOVEAGENTS(int timestep) = 0;
    virtual std::vector<Agent> getAgents() = 0;
    virtual std::vector<Agent> getVacantAgents() = 0;
    virtual void runTimestep(int timestep, TaskGroup* task = nullptr) = 0;
    virtual void addAgent(Agent& newAgent) = 0;


    inline map::Graph getGraph() {return this->graph;}
};

#endif // BASEENVIRONMENT_H
