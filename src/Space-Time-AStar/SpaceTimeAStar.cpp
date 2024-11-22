#include "../../lib/Space-Time-AStar/SpaceTimeAStar.h"
//#include <qDebug>
SpaceTimeAStar::SpaceTimeAStar(map::Graph graph)
: A::Astar(graph)
{}

// std::vector<SpaceTime::Cell> SpaceTimeAStar::getNeighbors(const Agent& agent, SpaceTime::Node &current, const Reservation &table) {
//     std::vector<SpaceTime::Cell> neighbors;
//     std::vector<map::Cell> illictis = agent.getIllicits();
    
//     for (auto dir : setup::moves) {
//         SpaceTime::Cell neighbor(current.x + dir.first, current.y + dir.second, current.t + 1);
//         bool containtsIllicits = std::find(illictis.begin(), illictis.end(), neighbor) != illictis.end();
//         if (isValid(neighbor.x, neighbor.y) &&
//              !table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
//              !table.wouldCauseEdgeCollision(current.x, current.y, current.t, neighbor.x, neighbor.y) &&
//              !containtsIllicits)
//         {
//             neighbors.push_back(neighbor);
//         }
//     }
//     return neighbors;
// }

std::vector<SpaceTime::Cell> SpaceTimeAStar::getNeighbors(const Agent& agent, map::Cell target,  SpaceTime::Node &current, const Reservation &table) {
    std::vector<SpaceTime::Cell> neighbors;
    std::vector<SpaceTime::Cell> reservedNeighbors;
    std::vector<map::Cell> illictis = agent.getIllicits();

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

std::vector<SpaceTime::Cell> SpaceTimeAStar::pathToTarget(const Agent& unit, const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table)
{
    std::vector<SpaceTime::Cell> path;

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
    int maxWaitTime = 5;
    bool waiting = false;
    SpaceTime::Node currentCell;
    while (!openList.empty()) 
    {
        currentCell = openList.top();
        openList.pop();

        if (isDestination(currentCell.x, currentCell.y, target.x, target.y)) 
        {
            int waitT = currentCell.t;
           std::shared_ptr<SpaceTime::Node> temp = std::make_shared<SpaceTime::Node>(currentCell);
            while (temp != nullptr) 
            {
                SpaceTime::Cell cell(temp->x, temp->y, temp->t);
                path.push_back(cell);
                temp = temp->parent;
                //temp = dynamic_cast<SpaceTime::Node*>(temp->parent);
            }
            std::reverse(path.begin(), path.end());

            
            if(!isTargetFreeForEntireWaitTime(target, currentCell.t, waitTime, table))
            {
                path.pop_back();

                auto parentCell = currentCell.parent;
                //SpaceTime::Node* parentCell = dynamic_cast<SpaceTime::Node*>(currentCell.parent);
                if (!parentCell) {
                    std::cerr << "No parent cell to wait in. Exiting...\n";
                    return path;
                }

                while (!isTargetFreeForEntireWaitTime(target, waitT, waitTime, table)) 
                {
                    SpaceTime::Cell waitCell(parentCell->x, parentCell->y, waitT - 1);
                    recursiveWaitAndReturn(unit, waitTime, target, &waitCell, nullptr, waitT, path, table);
                }
                SpaceTime::Cell buf = path.back();
                if(table.wouldCauseEdgeCollision(buf.x, buf.y, buf.t, target.x, target.y))
                {
                    //to do poprawy, bo jak agent wychodzi z komorki na ktora agent chce wejśc to raczej nie znajdzie tej ścieżki i zwróci else
                    SpaceTime::Cell waitCell(parentCell->x, parentCell->y, waitT);
                    auto alternativePath = pathToTarget(unit, waitCell, waitTime, currentTime, target, table);

                    if (!alternativePath.empty()) 
                    {
                        path.insert(path.end(), alternativePath.begin(), alternativePath.end());
                    } 
                    else
                    {
                        std::cerr << "No alternative path found after waiting. Exiting...\n";
                    }
                    currentTime = alternativePath.back().t + 1;
                }
                SpaceTime::Cell finalCell(target.x, target.y, waitT);
                path.push_back(finalCell);
            }



            
            return path;
        }
    

        closedList[currentCell.x][currentCell.y] = true;
        std::vector<SpaceTime::Cell> neighbors = getNeighbors(unit, target, currentCell, table);

        if(maxWaitTime == 0)
        {
            return {};
        }
        else if (neighbors.empty() && maxWaitTime > 0) 
        {
            if (!table.isReserved(currentCell.x, currentCell.y, currentCell.t + 1))
            {
                auto parentNode = std::make_shared<SpaceTime::Node>(currentCell);
                SpaceTime::Node waitNode(currentCell.t + 1, currentCell.x, currentCell.y, false, parentNode);
                openList.push(waitNode);
                maxWaitTime--;
                waiting = true;
            } else {
                std::cerr << "Wait node at (" << currentCell.x << ", " << currentCell.y
                          << ") t = " << currentCell.t + 1 << " is reserved.\n";
                return {};
            }
        } 
        
        else if(!neighbors.empty())
        {
            waiting = false;
            maxWaitTime = 5;
            for (const auto& neighbor : neighbors) {
                if (!closedList[neighbor.x][neighbor.y]) {
                    double gNew = currentCell.gCost + 1;
                    double hNew = SpaceTime::Node::calculateH(neighbor.x, neighbor.y, target.x, target.y);
                    double fNew = gNew + hNew;

                    auto parentNode = std::make_shared<SpaceTime::Node>(currentCell); // Tworzenie inteligentnego wskaźnika
                    SpaceTime::Node neighborCell(currentCell.t + 1, neighbor.x, neighbor.y, false, parentNode, gNew, hNew, fNew);
                    openList.push(neighborCell);
                }
            }
        }
    }
    

    if (waiting) {
        std::cerr << "Agent has reached max waiting time with no valid path found.\n";
    } else {
        std::cerr << "Path to target not found." << std::endl;
    }

    return {};
}


bool SpaceTimeAStar::recursiveWaitAndReturn(const Agent& unit, const int waitTime, const map::Cell& target, SpaceTime::Cell* currentCell, SpaceTime::Cell* parentCell, int& waitT, std::vector<SpaceTime::Cell>& path, Reservation& table) {

    //jesli ma rodzica i rodzic jest pusty i ruch na rodzica nie spowoduje kolizji to znajduje ścieżkę z punktu początkowego do target i konczy dzialanie
    if (parentCell && !table.isReserved(parentCell->x, parentCell->y, waitT) &&
        !table.wouldCauseEdgeCollision(currentCell->x, currentCell->y, waitT, parentCell->x, parentCell->y))
    {

        SpaceTime::Cell waitCell(currentCell->x, currentCell->y, waitT);
        auto alternativePath = pathToTarget(unit, waitCell, waitTime, waitT, target, table);
        if (!alternativePath.empty())
        {
            alternativePath.pop_back();
            path.insert(path.end(), alternativePath.begin(), alternativePath.end());
            waitT = alternativePath.back().t + 1;
            return true;
        }
    }

    if(parentCell && !table.isReserved(currentCell->x, currentCell->y, waitT))
     {
        path.push_back(SpaceTime::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return recursiveWaitAndReturn(unit, waitTime, target, currentCell, parentCell, waitT, path, table);
    }

    //gdy nie ma rodzica
    if (!table.isReserved(currentCell->x, currentCell->y, waitT))
    {
        path.push_back(SpaceTime::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return true;
    }



    SpaceTime::Cell waitCell(currentCell->x, currentCell->y, waitT);
    SpaceTime::Cell alternative = findAlternativeWaitingCell(unit, waitCell, waitT, table);

    if (alternative.x != -1)
    {
        path.push_back(alternative);
        waitT++;
        return recursiveWaitAndReturn(unit, waitTime, target, &alternative, currentCell, waitT, path, table);
    }
    else
    {
        // return recursiveWaitAndReturn(unit, waitTime, target, currentCell, parentCell, waitT, path, table);
        // std::cout << "No free neighboring cell found. Exiting...\n";
        // waitT++;
        return false;
        
    }
}

SpaceTime::Cell SpaceTimeAStar::findAlternativeWaitingCell(const Agent& unit, const SpaceTime::Cell& cell, int currentTime, Reservation& table) {
    //currentTime--;
    std::vector<map::Cell> illictis = unit.getIllicits();

    for (auto dir : setup::moves)
    {
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
bool SpaceTimeAStar::isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table) {
    for (int t = 0; t <= waitTime; ++t) 
    {
        if (table.isReserved(target.x, target.y,startTime + t)) 
        {
            return false;
        }
    }
    return true;
}





std::vector<SpaceTime::Cell> SpaceTimeAStar::findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table) {
    std::vector<SpaceTime::Cell> fullPath;
    map::Cell startPos = unit.getPosition();
    map::Cell currentAgentPosition = unit.getPosition();
    std::vector<map::Cell> waypoints;
    int stopTime;


    for (int taskIdx : taskOrder) {
        if (taskIdx >= groupTask.getNumTasks() || taskIdx < 0) {
            std::cerr << "Invalid task _" << taskIdx << "_ in task order." << std::endl;
            return fullPath;
        }

        map::Cell currentTaskPosition = groupTask[taskIdx];
        waypoints.push_back(groupTask[taskIdx]);
        stopTime = groupTask.getPickupStopTime(taskIdx);

        std::vector<SpaceTime::Cell> path = pathToTarget(unit, currentAgentPosition, stopTime, currentTime, currentTaskPosition, table);

        if(path.size() == 0)
        {
            //std::cout<<"ERROR::SpaceTimeAStar::findPath - not found path. Return {}.\n";
            return {};

        }
        else if(path.size() == 1)
        {
            currentTime++;
        }
        else if (!fullPath.empty() && fullPath.back().x == path.front().x &&
            fullPath.back().y == path.front().y && fullPath.back().t == path.front().t)
        {
            path.erase(path.begin());
        }
        int s = path.back().t;
        for (int t = 1; t < stopTime; ++t)
        {
            SpaceTime::Cell waitCell(currentTaskPosition.x, currentTaskPosition.y,  s + t);
            path.push_back(waitCell);
        }
        fullPath.insert(fullPath.end(), path.begin(), path.end());
        currentTime = fullPath.back().t;
        currentAgentPosition = currentTaskPosition;
    }

    ///////////////// Dropoff handling
    {
        std::vector<SpaceTime::Cell> dropoffPath = pathToTarget(unit, currentAgentPosition, groupTask.getDropoffStopTime(), currentTime, groupTask.getDropoffLocation(), table);

        if(dropoffPath.size() == 0)
        {
            //std::cout<<"ERROR::SpaceTimeAStar::findPath - not found path. Return {}.\n";
            return {};

        }
        else if(dropoffPath.size() == 1)
        {
            currentTime++;
        }
        else if (!fullPath.empty() && fullPath.back().x == dropoffPath.front().x &&
        fullPath.back().y == dropoffPath.front().y && fullPath.back().t == dropoffPath.front().t)
        {
            dropoffPath.erase(dropoffPath.begin());
        }
        int s = dropoffPath.back().t;
        for (int t = 1; t < groupTask.getDropoffStopTime(); ++t)
        {
            SpaceTime::Cell waitCell(groupTask.getDropoffLocation().x, groupTask.getDropoffLocation().y, s  + t);
            dropoffPath.push_back(waitCell);
        }

        fullPath.insert(fullPath.end(), dropoffPath.begin(), dropoffPath.end());
        currentTime = fullPath.back().t;
        
        currentAgentPosition = dropoffPath.back();
    }

    //////////////// Return to start position
    {
        std::vector<SpaceTime::Cell> returnPath = pathToTarget(unit, currentAgentPosition, 0, currentTime, startPos, table);
        if(returnPath.size() == 0)
        {
            //std::cout<<"ERROR::SpaceTimeAStar::findPath - not found path. Return {}.\n";
            return {};

        }
        else if(returnPath.size() == 1)
        {
            currentTime++;
        }
        else if (!fullPath.empty() && fullPath.back().x == returnPath.front().x &&
            fullPath.back().y == returnPath.front().y && fullPath.back().t == returnPath.front().t)
        {
            returnPath.erase(returnPath.begin());
        }

        fullPath.insert(fullPath.end(), returnPath.begin(), returnPath.end());
        currentTime = fullPath.back().t;

    }

    {
        //std::cout<<"\n CALA SCIEZKA \n";
        for(size_t i=0; i<fullPath.size(); i++)
        {            
            table.reserve(fullPath[i].x, fullPath[i].y, fullPath[i].t);
            table.reserve(fullPath[i].x, fullPath[i].y, fullPath[i].t + 1);
            if(i<fullPath.size() - 1)
            {
                table.reserveEdge(fullPath[i].x, fullPath[i].y, fullPath[i + 1].x, fullPath[i + 1].y, fullPath[i].t);
            }
            //std::cout<<"(" << fullPath[i].x << "," << fullPath[i].y << ", t: " <<  fullPath[i].t << ") -> ";
        }
        //std::cout<<std::endl;
    }
    return fullPath;
}
