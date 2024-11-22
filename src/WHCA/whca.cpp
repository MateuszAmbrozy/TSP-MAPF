#include "../../lib/WHCA/whca.h"
#include "../../lib/setup.h"
//#include <QDebug>
WHCA::WHCA(map::Graph graph)
    : A::Astar(graph)
{}

std::vector<SpaceTime::Cell> WHCA::getNeighbors(const WHCA_Agent& agent, map::Cell target,  SpaceTime::Node &current, const Reservation &table) {
    std::vector<SpaceTime::Cell> neighbors;
    std::vector<SpaceTime::Cell> reservedNeighbors;
    std::vector<map::Cell> illictis = agent.agent.getIllicits();

    for (auto dir : setup::moves)
    {
        SpaceTime::Cell neighbor(current.x + dir.first, current.y + dir.second, current.t + 1);
        bool containtsIllicits = std::find(illictis.begin(), illictis.end(), neighbor) != illictis.end();

        // Check if the neighbor is valid and doesn't contain any illicits
        if (isValid(neighbor.x, neighbor.y) && !containtsIllicits)
        {

            if (isDestination(neighbor.x, neighbor.y, target.x, target.y))
            {
                if (!table.wouldCauseEdgeCollision(current.x, current.y, current.t, neighbor.x, neighbor.y)) {
                    neighbors.push_back(neighbor);  // Prioritize target cell even if reserved
                }
            }
            else {
                if (!table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
                    !table.wouldCauseEdgeCollision(current.x, current.y, current.t, neighbor.x, neighbor.y))
                {
                    neighbors.push_back(neighbor);
                }
            }
        }
    }


    return neighbors;
}
std::vector<SpaceTime::Cell> WHCA::pathToTarget(WHCA_Agent& unit, map::Cell start, map::Cell target, int currentTime, Reservation& table)
{
    std::vector<SpaceTime::Cell> path;

    int waitTime = unit.getWaypoints()[unit.getCurrentWaypointIndex()].second;
    int windowSize = unit.W;
    int stepSize = unit.K;

    if (!isValid(target.x, target.y))
    {
        std::cerr << "Target is an obstacle or invalid." << std::endl;
        return path;
    }

    if (isDestination(start.x, start.y, target.x, target.y))
    {
        //std::cout << "Already at the destination." << std::endl;
        SpaceTime::Cell buf(target, currentTime + 1);
        return {buf};
    }

    std::vector<std::vector<bool>> closedList(aStarGraph.size(), std::vector<bool>(aStarGraph[0].size(), false));
    std::priority_queue<SpaceTime::Node, std::vector<SpaceTime::Node>, A::CompareCells> openList;

    SpaceTime::Node startNode(currentTime, start.x, start.y, start.isObstacle);
    startNode.gCost = 0;
    startNode.hCost = SpaceTime::Node::calculateH(start.x, start.y, target.x, target.y);
    startNode.fCost = startNode.gCost + startNode.hCost;
    startNode.parent = nullptr;

    openList.push(startNode);
    int maxWaitTime = 15;
    bool waiting = false;
    int startTime = currentTime;
    SpaceTime::Node currentCell = openList.top();
    std::shared_ptr<SpaceTime::Node> bestCell = std::make_shared<SpaceTime::Node>(startNode);

    while (!openList.empty() && bestCell->t < startTime + windowSize)
    {
        currentCell = openList.top();
        openList.pop();

        if (currentCell.hCost < bestCell->hCost)
        {
            bestCell = std::make_shared<SpaceTime::Node>(currentCell);
        }



        if (isDestination(currentCell.x, currentCell.y, target.x, target.y))
        {
            auto isDestinationStartTime = std::chrono::high_resolution_clock::now();
            int waitT = currentCell.t;

            std::shared_ptr<SpaceTime::Node> temp = std::make_shared<SpaceTime::Node>(currentCell);
            while (temp != nullptr)
            {
                SpaceTime::Cell cell(temp->x, temp->y, temp->t);
                path.push_back(cell);
                temp = temp->parent;
            }
            std::reverse(path.begin(), path.end());


            if(!isTargetFreeForEntireWaitTime(target, currentCell.t, waitTime, table))
            {
                path.pop_back();

                 std::shared_ptr<SpaceTime::Node> parentCell = currentCell.parent;
                if (!parentCell) {
                    std::cerr << "No parent cell to wait in. Exiting...\n";
                    return path;
                }

                while (!isTargetFreeForEntireWaitTime(target, waitT, waitTime, table))
                {
                    SpaceTime::Cell waitCell(parentCell->x, parentCell->y, waitT-1);
                    recursiveWaitAndReturn(unit, waitTime, target, &waitCell, nullptr, waitT, path, table);
                }


                SpaceTime::Cell buf = path.back();
                if(table.wouldCauseEdgeCollision(buf.x, buf.y, buf.t, target.x, target.y))
                {
                    SpaceTime::Cell waitCell(parentCell->x, parentCell->y, waitT);
                    auto alternativePath = pathToTarget(unit, waitCell, target, currentTime, table);

                    if (!alternativePath.empty()) {
                        path.insert(path.end(), alternativePath.begin(), alternativePath.end());
                    } else {
                        std::cerr << "No alternative path found after waiting. Exiting...\n";
                    }
                    currentTime = alternativePath.back().t + 1;
                }

                if(!isDestination(path.back().x, path.back().y, target.x, target.y))
                {
                    SpaceTime::Cell finalCell(target.x, target.y, waitT);
                    path.push_back(finalCell);
                }

            }
            auto isDestinationEndTime = std::chrono::high_resolution_clock::now();
            auto isDestinationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(isDestinationEndTime - isDestinationStartTime).count();

            // Print the elapsed time
            std::cout << "Execution time for agent " << unit.agent.getId() << " of isDestination: " << isDestinationDuration << " millisenconds" << std::endl;
            return path;
        }


        closedList[currentCell.x][currentCell.y] = true;
        std::vector<SpaceTime::Cell> neighbors = getNeighbors(unit, target, currentCell, table);


        if (neighbors.empty() && maxWaitTime > 0)
        {
            if (!table.isReserved(currentCell.x, currentCell.y, currentCell.t + 1)) 
            {
                auto parentNode = std::make_shared<SpaceTime::Node>(currentCell);
                SpaceTime::Node waitNode(currentCell.t + 1, currentCell.x, currentCell.y, false, parentNode);
                openList.push(waitNode);
                maxWaitTime--;
                waiting = true;
            }
            else
            {
                std::cerr << "Wait node at (" << currentCell.x << ", " << currentCell.y 
                        << ") t = " << currentCell.t + 1 << " is reserved.\n";
                return {};
            }
        }
        else if(!neighbors.empty())
        {
            waiting = false;
            maxWaitTime = 15;
            for (const auto& neighbor : neighbors) 
            {
                    double gNew = currentCell.gCost + 1;
                    double hNew = SpaceTime::Node::calculateH(neighbor.x, neighbor.y, target.x, target.y);
                    double fNew = gNew + hNew;

                    auto parentNode = std::make_shared<SpaceTime::Node>(currentCell);
                    SpaceTime::Node neighborCell(currentCell.t + 1, neighbor.x, neighbor.y, false, parentNode, gNew, hNew, fNew);
                    openList.push(neighborCell);
            }
        }


    }


    if (waiting) {
        std::cout<<"!!!!!!!Wating cell\n";
        // SpaceTime::Cell buf = findAlternativeWaitingCell(unit, SpaceTime::Cell(currentCell.x, currentCell.y, currentCell.t), currentTime, table);
        // if(buf.x != 1)
        //     path.push_back(buf);
    }

    std::shared_ptr<SpaceTime::Node> temp = bestCell;
    while (temp != nullptr) 
    {
        SpaceTime::Cell cell(temp->x, temp->y, temp->t);
        path.push_back(cell);
        temp = temp->parent;
    }
    std::reverse(path.begin(), path.end());

    std::vector<SpaceTime::Cell> limitedPath;
    int stepsToMove = std::min(stepSize, static_cast<int>(path.size()));
    limitedPath.insert(limitedPath.end(), path.begin(), path.begin() + stepsToMove);
    // if(limitedPath.empty())
    // {
    //     std::cout<<"!!!!!!!Path empty genereting new cell\n";
    //     SpaceTime::Cell buf = findAlternativeWaitingCell(unit, SpaceTime::Cell(currentCell.x, currentCell.y, currentCell.t), currentTime, table);
    //     if(buf.x != -1)
    //         return {buf};
    // }

    return limitedPath;

}


bool WHCA::recursiveWaitAndReturn(WHCA_Agent& unit, const int waitTime, const map::Cell& target, SpaceTime::Cell* currentCell, SpaceTime::Cell* parentCell, int& waitT, std::vector<SpaceTime::Cell>& path, Reservation& table) {

    //if (parentCell && !table.isReserved(target.x, target.y, waitT))// jak jest tak to
   // if (parentCell)
    if (parentCell && !table.isReserved(parentCell->x, parentCell->y, waitT) &&
        !table.wouldCauseEdgeCollision(currentCell->x, currentCell->y, waitT, parentCell->x, parentCell->y))
    {   SpaceTime::Cell waitCell(currentCell->x, currentCell->y, currentCell->t);
        auto alternativePath = pathToTarget(unit, waitCell, target, waitT, table);
        if (!alternativePath.empty())
        {
                 std::cout<< "recursiveWaitAndReturn 1 for unitID: " << unit.agent.getId()<< "\n";
                // for(const auto& cell: alternativePath)
                // {
                //     std::cout<< "(" << cell.x << ", " <<  cell.y << ") t: " << cell.t  << "-> ";
                // }
                // std::cout<<std::endl;
            //alternativePath.pop_back();

            auto last = alternativePath.back();
            if(isDestination(last.x, last.y, target.x, target.y))
            {
                path.insert(path.end(), alternativePath.begin(), alternativePath.end());
                waitT = alternativePath.back().t + 1;
                return true;
            }
        }
        // SpaceTime::Cell start(currentCell->x, currentCell->y, waitT);
        // auto alternativePath = pathToTarget(unit, start, target, waitT, table);

        // if (!alternativePath.empty())
        // {
        //     //path.pop_back();
        //     std::cout<< "recursiveWaitAndReturn 1 for unitID: " << unit.agent.getId()<< "\n";
        //     for(const auto& cell: alternativePath)
        //     {
        //         std::cout<< "(" << cell.x << ", " <<  cell.y << ") t: " << cell.t  << "-> ";
        //     }


        //     path.insert(path.end(), alternativePath.begin(), alternativePath.end());
        //     waitT = alternativePath.back().t + 1;
        //     return true;
        //     auto last = path.back();
            //path.pop_back();
            // if(isDestination(last.x, last.y, target.x, target.y))
            // {
            //     std::cout << "... found destination \n";
            //     return true;
            // }
            // else
            // {
            //         std::cout << "... ! NOT ! found destination \n";
            //         //std::cout<< "recursiveWaitAndReturn 2 " << "(" << currentCell->x << ", " <<  currentCell->y << ") t: " << waitT <<"\n";
            //         path.push_back(SpaceTime::Cell(last.x, last.y, waitT));
            //         waitT++;
            //         //return recursiveWaitAndReturn(unit, waitTime, target, currentCell, parentCell, waitT, path, table);
            //         return recursiveWaitAndReturn(unit, waitTime, target, &last, currentCell, waitT, path, table);
            // }
       // }
    }

    if(parentCell && !table.isReserved(currentCell->x, currentCell->y, waitT))
    {
        std::cout<< "recursiveWaitAndReturn 2 " << "(" << currentCell->x << ", " <<  currentCell->y << ") t: " << waitT <<"\n";
        path.push_back(SpaceTime::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return recursiveWaitAndReturn(unit, waitTime, target, currentCell, parentCell, waitT, path, table);
    }
    if (!table.isReserved(currentCell->x, currentCell->y, waitT))
    {
        std::cout<< "recursiveWaitAndReturn 3 " << "(" << currentCell->x << ", " <<  currentCell->y << ") t: " << waitT <<"\n";
        path.push_back(SpaceTime::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return true;
    }

    SpaceTime::Cell waitCell(currentCell->x, currentCell->y, waitT);
    SpaceTime::Cell alternative = findAlternativeWaitingCell(unit, waitCell, waitT, table);

    if (alternative.x != -1)
    {
       std::cout<< "recursiveWaitAndReturn 4 " << "(" << alternative.x << ", " <<  alternative.y << ") t: " << alternative.t <<"\n";
        path.push_back(alternative);
        waitT++;
        return recursiveWaitAndReturn(unit, waitTime, target, &alternative, currentCell, waitT, path, table);
    }
    else
    {
        // path.push_back(SpaceTime::Cell(currentCell->x, currentCell->y, waitT));
        // waitT++;
        return false;
        return recursiveWaitAndReturn(unit, waitTime, target, currentCell, parentCell, waitT, path, table);
    }
}
SpaceTime::Cell WHCA::findAlternativeWaitingCell(const WHCA_Agent& agent, const SpaceTime::Cell& cell, int currentTime, Reservation& table) {
    //currentTime--;
    std::vector<map::Cell> illictis = agent.agent.getIllicits();
    for (auto dir : setup::moves) {
        SpaceTime::Cell neighbor(cell.x + dir.first, cell.y + dir.second, currentTime);
        bool containtsIllicits = std::find(illictis.begin(), illictis.end(), neighbor) != illictis.end();
        if (isValid(neighbor.x, neighbor.y) &&
            !table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
            !table.wouldCauseEdgeCollision(cell.x, cell.y, currentTime, neighbor.x, neighbor.y) &&
            !containtsIllicits)
        {
            return neighbor;
        }
    }

    return SpaceTime::Cell(-1, -1, currentTime);
}
bool WHCA::isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table) {
    for (int t = 0; t <= waitTime + 1; ++t)
    {
        if (table.isReserved(target.x, target.y,startTime + t))
        {
            return false;
        }
    }
    return true;
}





std::vector<SpaceTime::Cell> WHCA::findNextWSteps(WHCA_Agent& unit, int currentTime, Reservation& table)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<SpaceTime::Cell> path;


    map::Cell target = unit.getWaypoints()[unit.getCurrentWaypointIndex()].first;
    int waitTime = unit.getWaypoints()[unit.getCurrentWaypointIndex()].second;

    map::Cell start = unit.agent.getPosition();
    auto findPathStartTime = std::chrono::high_resolution_clock::now();


    path = pathToTarget(unit, start, target, currentTime, table);

    auto findPathEndTime = std::chrono::high_resolution_clock::now();
    auto findPathDuration = std::chrono::duration_cast<std::chrono::milliseconds>(findPathEndTime - findPathStartTime).count();

    // Print the elapsed time
    std::cout << "Execution time for agent " << unit.agent.getId() << " of findPath: " << findPathDuration << " millisenconds" << std::endl;
    if(path.size() == 1)
    {
        currentTime++;
    }
    else if (path.empty())
    {
        std::cerr << "ERROR::SpaceTimeAStar::findPath - not found path to waypoint.\n";
        if(table.isReserved(unit.agent.getPosition().x, unit.agent.getPosition().y, currentTime))
            return {};
        else
        {
            path.push_back(SpaceTime::Cell(unit.agent.getPosition().x, unit.agent.getPosition().y, false));
            currentTime++;
        }

    }


    int waitT = path.back().t;
    if(isDestination(path.back().x, path.back().y, target.x, target.y))
    {
        for (int t = 1; t <= waitTime; ++t)
        {
            SpaceTime::Cell waitCell(target.x, target.y,  waitT + t);
            path.push_back(waitCell);
        }
        unit.incrementWaypointIndex();
    }
    int steps = static_cast<int>(path.size());
    //std::cout<<"Total path for agent: " << unit.agent.getId() << "\n";
    for (int i = 0; i < steps; ++i) {

        //std::cout << "(" << path[i].x<<", " << path[i].y << "), t: " << path[i].t <<  "->";
        table.reserve(path[i].x, path[i].y, path[i].t);
        table.reserve(path[i].x, path[i].y, path[i].t + 1);

        if (i < steps - 1)
        {
            //table.reserve(path[i+1].x, path[i+1].y, path[i+1].t);
            table.reserveEdge(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y, path[i].t);
        }
    }
    //std::cout << std::endl;
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the elapsed time
    std::cout << "Execution time for agent " << unit.agent.getId() << " of findNextWSteps: " << duration << " millisenconds" << std::endl;

    return path;
}
