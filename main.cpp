//#include "lib/Environment.h"
#include "lib/Agent.h"
#include "lib/TaskGroup.h"
#include "lib/AStar/Astar.h" 
#include "lib/Space-Time-AStar/Reservation.h"
#include "lib/Space-Time-AStar/SpaceTimeAStar.h"

#include <algorithm>


void testAstarClass(map::Graph graph)
{
    graph.cells[1][0].isObstacle = true;
    graph.cells[1][1].isObstacle = true;
    graph.cells[1][2].isObstacle = true;
    graph.cells[1][3].isObstacle = true;

    graph.cells[3][1].isObstacle = true;
    graph.cells[3][2].isObstacle = true;
    graph.cells[3][3].isObstacle = true;
    graph.cells[3][4].isObstacle = true;

    Agent a1(0, 20, map::Cell(0, 0));
    //Agent a1(0, 20, map::Cell(0, 0));
    A::Astar astar(graph);

    std::cout << "Dlugosc sciezki a (0,0) do (4,4) = " << astar.calculate(a1.getPosition(), map::Cell(4, 4)) << std::endl;

}
// void testTSP1(map::Graph graph)
// {
//     TSP tsp(graph);

//     Agent a1(0, 20, map::Cell(0, 0));
//     std::vector<map::Cell> pickup_points = {map::Cell(1,1), map::Cell(3, 3), map::Cell(0, 4), map::Cell(4, 0)};
//     TaskGroup tasks(4, pickup_points, map::Cell(4, 4));

//     std::vector<int> result = tsp.solveTSP(a1, tasks);
//     std::cout<<"Kolejnosc wykonanych zadan gdzie: \n";

//     for(int i=0; i<pickup_points.size(); i++) std::cout<<"\t" << i+1 <<": ("<<pickup_points[i].x << "," << pickup_points[i].y << ")\n";

//     for(int i=0; i<result.size(); i++)
//     {
//         std::cout<< i+1 << ". " <<result[i] << std::endl;
//     }
    
// }
// void testTSP2(map::Graph graph)
// {
//     TSP tsp(graph);

//     Agent a1(0, 20, map::Cell(0, 0));
//     std::vector<map::Cell> pickup_points = {map::Cell(1,0), map::Cell(2, 0), map::Cell(3, 0), map::Cell(4, 0)};
//     std::reverse(pickup_points.begin(), pickup_points.end());
//     TaskGroup tasks(4, pickup_points, map::Cell(4, 1));

//     std::vector<int> result = tsp.solveTSP(a1, tasks);
//     std::cout<<"Kolejnosc wykonanych zadan gdzie: \n";

//     for(int i=0; i<pickup_points.size(); i++) std::cout<<"\t" << i <<": ("<<pickup_points[i].x << "," << pickup_points[i].y << ")\n";

//     for(int i=0; i<result.size(); i++)  std::cout<< i+1 << ". " <<result[i] << std::endl; 
    
// }

void testSTA()
{
    SpaceTimeAStar* sta;
    map::Graph graph(4, 4);

    Agent a1(0, 20, map::Cell(0, 0));
    Agent a2(0, 20, map::Cell(3, 0));
    Agent a3(0, 20, map::Cell(3, 3));

    TaskGroup* tasks, *tasks2, *tasks3;
    std::vector<map::Cell> pickup_points= {map::Cell(3,0)};
    std::vector<map::Cell> pickup_points2= {map::Cell(0,0)};
    std::vector<map::Cell> pickup_points3= {map::Cell(1,1), map::Cell(3,3)};

    Reservation resTable;

    std::vector<int> taskOrder = {0};
    std::vector<int> taskOrder2 = {0, 1};

    int currentTime = 0;

    sta = new SpaceTimeAStar(graph);
    tasks = new TaskGroup(4, pickup_points, map::Cell(1,1), {3}, 2);
    tasks2 = new TaskGroup(4, pickup_points2, map::Cell(3, 3), {3}, 2);
    tasks3 = new TaskGroup(4, pickup_points3, map::Cell(2, 3), {3, 3}, 2);

    resTable.reserve(0, 1, 1);
    resTable.reserve(1, 0, 1);
     resTable.reserve(2, 3, 12);
     resTable.reserve(2, 3, 13);
     resTable.reserve(1, 3, 13);
     resTable.reserve(1, 3, 14);
     resTable.reserve(0, 3, 14);
    // resTable.reserve(2, 3, 14);
    // resTable.reserve(2, 3, 15);


    std::vector<SpaceTimeCell::Cell> result, result2, result3;

    result = sta->findPath(a1,currentTime, *tasks, taskOrder, resTable);
    result2 = sta->findPath(a2,currentTime, *tasks2, taskOrder, resTable);
    result3 = sta->findPath(a3,currentTime, *tasks3, taskOrder2, resTable);

    std::cout<<std::endl<<"RESULT 1\n";
    for(int i=0; i<result.size(); i++)
    {
       std::cout<<"(" << result[i].x << "," << result[i].y << ") -> ";
    }
    
    std::cout<<std::endl<<"RESULT 2\n";
    for(int i=0; i<result2.size(); i++)
    {
       std::cout<<"(" << result2[i].x << "," << result2[i].y << ") -> ";
    }

    std::cout<<std::endl<<"RESULT 3\n";
    for(int i=0; i<result3.size(); i++)
    {
       std::cout<<"(" << result3[i].x << "," << result3[i].y << ") -> ";
    }
    // std::cout<<std::endl<<"RESERVATION TABLE 2\n";
    // for (const auto& outer_pair : resTable.reservations) {
    //     int first_key = outer_pair.first;
    //     const auto& inner_map1 = outer_pair.second;

    //     for (const auto& middle_pair : inner_map1) {
    //         int second_key = middle_pair.first;
    //         const auto& inner_map2 = middle_pair.second;

    //         for (const auto& inner_pair : inner_map2) {
    //             int third_key = inner_pair.first;
    //             bool value = inner_pair.second;

    //             // Tutaj możesz uzyskać dostęp do wartości
    //             std::cout << "Key1: " << first_key 
    //                     << ", Key2: " << second_key 
    //                     << ", Key3: " << third_key 
    //                     << ", Value: " << value 
    //                     << std::endl;
    //         }
    //     }
    // }

    delete sta;
    delete tasks;
}

// void testEnvi()
// {
//     Environment e();
// }
int main() {
    map::Graph g(5, 5);



    //testAstarClass(g);

    //testTSP2(g);

    testSTA();
    return 0;
}
