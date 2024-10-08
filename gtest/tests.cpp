// #pragma once
// #include <gtest/gtest.h>
// #include "../lib/Space-Time-AStar/SpaceTimeAStar.h"
// #include "../lib/Space-Time-AStar/Reservation.h"
// #include "../lib/Environment.h"
// #include "../lib/Agent.h"
// #include "../lib/TaskGroup.h"
// #include "../lib/Astar.h"

// #include <algorithm>
// class AStarTest : public ::testing::Test{
// protected:
//     map::Graph graph;
//     Agent a1;
//     A::Astar* astar;

//     AStarTest()
//         : graph(5, 5), a1(0, 20, map::Cell(0, 0))
//     {
//         this->graph.cells[1][0].isObstacle = true;
//         this->graph.cells[1][1].isObstacle = true;
//         this->graph.cells[1][2].isObstacle = true;
//         this->graph.cells[1][3].isObstacle = true;

//         this->graph.cells[3][1].isObstacle = true;
//         this->graph.cells[3][2].isObstacle = true;
//         this->graph.cells[3][3].isObstacle = true;
//         this->graph.cells[3][4].isObstacle = true;

//         astar = new A::Astar(graph);

//     }

//     ~AStarTest() {
//         delete astar;
//     }
//     // void SetUp() override {
//     // }
// };
// TEST_F(AStarTest, DISABLED_isObstacleTest)
// {

//     ASSERT_TRUE(this->graph.cells[1][0].isObstacle);
// }

// TEST_F(AStarTest, DISABLED_calculateLen)
// {
//     ASSERT_EQ(astar->calculate(a1.getPosition(), map::Cell(4, 4)), 16);
// }



// class TSPTest : public ::testing::Test
// {
//     protected:
//         TSP* tsp; 
//         map::Graph graph;
//         Agent a1;
//         std::vector<map::Cell> pickup_points;
//         TaskGroup* tasks;
//         TSPTest()
//             :a1(0, 20, map::Cell(0, 0)), graph(5, 5)
//         {
//             tsp = new TSP(graph);

//             pickup_points = {map::Cell(1,0), map::Cell(2, 0), map::Cell(3, 0), map::Cell(4, 0)};
            
//             tasks = new TaskGroup(4, pickup_points, map::Cell(4, 1));
//         }
//         ~TSPTest()
//         {
//             delete tsp;
//             delete tasks;
//         }
// };

// TEST_F(TSPTest, DISABLED_solveTEST)
// {

//     std::vector<int> expected = {1, 2, 3, 4};
//     std::vector<int> result = tsp->solveTSP(a1, *tasks);
//     ASSERT_EQ(result, expected);

//     std::reverse(pickup_points.begin(), pickup_points.end());

//     tasks = new TaskGroup(4, pickup_points, map::Cell(4, 1));
//     result = tsp->solveTSP(a1, *tasks);
//     expected = {4, 3, 2, 1};
//     // std::reverse(expected.begin(), expected.end());
//     ASSERT_EQ(result, expected);
// }

// class STATest :public ::testing::Test
// {
// protected:
//     SpaceTimeAStar* sta;
//     map::Graph graph;
//     Agent a1;
//     TaskGroup* tasks;
//     std::vector<map::Cell> pickup_points;

//     Reservation resTable;

//     std::vector<int> taskOrder;

//     int currentTime;

//     STATest()
//         :graph(4, 4), a1(0, 20, map::Cell(0, 0))
//     {
//         pickup_points = {map::Cell(3,2), map::Cell(3, 3), map::Cell(1, 0), map::Cell(2, 0)};

//         taskOrder = {1, 2, 3, 4};

//         currentTime=0;

//         sta = new SpaceTimeAStar(graph, currentTime);
//         tasks = new TaskGroup(4, pickup_points, map::Cell(4, 4));
//     }
//     ~STATest()
//     {
//         delete sta;
//         delete tasks;
//     }
// };

// TEST_F(STATest, findPathTest)
// {
//     resTable.reserve(0, 1, 1);
//     resTable.reserve(1, 0, 1);

//     resTable.reserve(3, 0, 4);
//     resTable.reserve(3, 1, 5);

//     std::vector<SpaceTimeCell::Cell> result;

//     result = sta->findPath(a1, *tasks, taskOrder, resTable);
    
//     for(int i=0; i<result.size(); i++)
//     {
        
//        std::cout<<"(" << result[i].x << "," << result[i].y << ") -> ";
        
//     }
//     ASSERT_TRUE(1==1);
// }

// int main(int argc, char* argv[])
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }