#include "../../lib/Space-Time-AStar/SpaceTimeAStar.h"

SpaceTimeAStar::SpaceTimeAStar(map::Graph graph)
: A::Astar(graph)
{}

std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::getNeighbors(SpaceTimeCell::Node &current, const Reservation &table) {
    std::vector<SpaceTimeCell::Cell> neighbors;
    for (auto dir : setup::moves) {
        SpaceTimeCell::Cell neighbor(current.x + dir.first, current.y + dir.second, current.t + 1);
        if (isValid(neighbor.x, neighbor.y) &&
             !table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
             !table.wouldCauseEdgeCollision(current.x, current.y, current.t, neighbor.x, neighbor.y, neighbor.t))
        {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::pathToTarget(const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table)
{
    std::vector<SpaceTimeCell::Cell> path;

    if (!isValid(target.x, target.y)) 
    {
        std::cerr << "Target is an obstacle or invalid." << std::endl;
        return path;
    }

    if (isDestination(start.x, start.y, target.x, target.y)) 
    {
        std::cout << "Already at the destination." << std::endl;
        return path;
    }

    std::vector<std::vector<bool>> closedList(aStarGraph.size(), std::vector<bool>(aStarGraph[0].size(), false));
    std::priority_queue<SpaceTimeCell::Node, std::vector<SpaceTimeCell::Node>, A::CompareCells> openList;

    SpaceTimeCell::Node startNode(currentTime, start.x, start.y, start.isObstacle);

    startNode.gCost = 0;
    startNode.hCost = SpaceTimeCell::Node::calculateH(start.x, start.y, target.x, target.y);
    startNode.fCost = startNode.gCost + startNode.hCost;
    startNode.parent = nullptr;

    openList.push(startNode);
    int maxWaitTime = 10;
    bool waiting = false;
    SpaceTimeCell::Node currentCell;
    while (!openList.empty()) 
    {
        currentCell = openList.top();
        openList.pop();

        if (isDestination(currentCell.x, currentCell.y, target.x, target.y)) 
        {
            int waitT = currentCell.t;
            SpaceTimeCell::Node* temp = &currentCell;
            while (temp != nullptr) {
                SpaceTimeCell::Cell cell(temp->x, temp->y, temp->t);
                path.push_back(cell);
                temp = dynamic_cast<SpaceTimeCell::Node*>(temp->parent);
            }
            std::reverse(path.begin(), path.end());

            
            if(!isTargetFreeForEntireWaitTime(target, currentCell.t, waitTime, table))
            {
                path.pop_back();

                SpaceTimeCell::Node* parentCell = dynamic_cast<SpaceTimeCell::Node*>(currentCell.parent);
                if (!parentCell) {
                    std::cerr << "No parent cell to wait in. Exiting...\n";
                    return path;
                }

                while (!isTargetFreeForEntireWaitTime(target, waitT, waitTime, table)) 
                {
                    SpaceTimeCell::Cell waitCell(parentCell->x, parentCell->y, waitT-1); 
                    recursiveWaitAndReturn(&waitCell, nullptr, waitT, path, table);
                }

                
                // TO może spowodować kolejną kolizję w target bo jest opóźnienie
                // ręczne usuwanie tego może powodować duże opóźnienia
                
                /*
                    Jakie pomysły
                    a) prioretyzacja agentów i ustępowanie w ten sposób aby makespan był min
                    b) przy takich sytuacjach replanowanie ścieżek (wszystkich????)
                    c)
                */
                SpaceTimeCell::Cell buf = path.back();
                if(table.wouldCauseEdgeCollision(buf.x, buf.y, buf.t, target.x, target.y, waitT))
                {
                    SpaceTimeCell::Cell waitCell(parentCell->x, parentCell->y, waitT); 
                    recursiveWaitAndReturn(&waitCell, nullptr, waitT, path, table);
                }
                

                SpaceTimeCell::Cell finalCell(target.x, target.y, waitT);
                path.push_back(finalCell);
            }

            for (int t = 1; t <= waitTime; ++t) 
            {
                SpaceTimeCell::Cell waitCell(target.x, target.y,  waitT + t);
                path.push_back(waitCell);
            }
            
            
            return path;
        }
    

        closedList[currentCell.x][currentCell.y] = true;
        std::vector<SpaceTimeCell::Cell> neighbors = getNeighbors(currentCell, table);

        if (neighbors.empty() && maxWaitTime > 0) 
        {
            std::cout<<"neighbours empty\n";
            SpaceTimeCell::Node waitNode(currentCell.t + 1, currentCell.x, currentCell.y, false, new SpaceTimeCell::Node(currentCell));
            openList.push(waitNode);
            maxWaitTime--;
            waiting = true;
        } 
        
        else if(!neighbors.empty())
        {
            waiting = false;
            for (const auto& neighbor : neighbors) {
                if (!closedList[neighbor.x][neighbor.y]) {
                    double gNew = currentCell.gCost + 1;
                    double hNew = SpaceTimeCell::Node::calculateH(neighbor.x, neighbor.y, target.x, target.y);
                    double fNew = gNew + hNew;

                    SpaceTimeCell::Node neighborCell(currentCell.t + 1, neighbor.x, neighbor.y, false, new SpaceTimeCell::Node(currentCell), gNew, hNew, fNew);
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

    return path;
}



bool SpaceTimeAStar::recursiveWaitAndReturn(SpaceTimeCell::Cell* currentCell, SpaceTimeCell::Cell* parentCell, int& waitT, std::vector<SpaceTimeCell::Cell>& path, Reservation& table) {
    if (!table.isReserved(currentCell->x, currentCell->y, waitT)) {
        path.push_back(SpaceTimeCell::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return true;
    }

    if (parentCell && !table.isReserved(parentCell->x, parentCell->y, waitT)) {
        path.push_back(SpaceTimeCell::Cell(parentCell->x, parentCell->y, waitT));
        waitT++;
        return true;
    }

    SpaceTimeCell::Cell waitCell(currentCell->x, currentCell->y, waitT);
    SpaceTimeCell::Cell alternative = findAlternativeWaitingCell(waitCell, waitT, table);
    
    if (alternative.x != -1) {
        path.push_back(alternative);
        waitT++;
        
        if (recursiveWaitAndReturn(&alternative, currentCell, waitT, path, table)) {

            return recursiveWaitAndReturn(currentCell, parentCell, waitT, path, table);
        }
        else {
            return false;
        }
    } else {
        std::cerr << "No free neighboring cell found. Exiting...\n";
        return false;  
    }
}

SpaceTimeCell::Cell SpaceTimeAStar::findAlternativeWaitingCell(const SpaceTimeCell::Cell& cell, int currentTime, Reservation& table) {
    currentTime--;
    for (auto dir : setup::moves) {
        SpaceTimeCell::Cell neighbor(cell.x + dir.first, cell.y + dir.second, currentTime + 1);
        if (isValid(neighbor.x, neighbor.y) &&
             !table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
             !table.wouldCauseEdgeCollision(cell.x, cell.y, currentTime, neighbor.x, neighbor.y, neighbor.t))
        {
            return neighbor;
        }
    }

    return SpaceTimeCell::Cell(-1, -1, currentTime);
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

// std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::solveCollision(std::vector<SpaceTimeCell::Cell>& path,
//                                                                 Reservation& table,
//                                                                 const std::vector<map::Cell>& waypoints)
// {
//     std::vector<SpaceTimeCell::Cell> adjustedPath;
//     int additionalTime = 0;
//     size_t waypointIndex = 0;
    
//     for(size_t i = 0; i<path.size(); ++i) {
//         SpaceTimeCell::Cell currentCell = path[i];

//         // Sprawdź, czy kolejna komórka jest zarezerwowana
//         if(i + 1 < path.size() && table.isReserved(path[i + 1].x, path[i + 1].y, path[i + 1].t)) {
//             std::cout<<"\nNORMAL COLISION x: " << path[i].x << ", y: " << path[i].y << " - t:" <<  path[i].t <<"\n";
            
//             adjustedPath.push_back(currentCell);

//             for (size_t j = i; j < path.size(); ++j) 
//             {
//                 path[j].t++;  // Zwiększ czas dla każdej kolejnej komórki
//             }
//             currentCell = path[i];
//         }

//         if (i + 1 < path.size() && table.wouldCauseEdgeCollision(currentCell.x, currentCell.y, currentCell.t, path[i + 1].x, path[i + 1].y, path[i + 1].t)) {
//             std::cout << "\nIMPAS " << path[i].x << ", " << path[i].y << " - t: " << path[i].t << "\n";
            
//             auto waitCell = path.front();
//             path.insert(path.begin(), waitCell);
//             for (size_t j = 1; j < path.size(); ++j) 
//             {
//                 path[j].t++;  // Zwiększ czas dla każdej kolejnej komórki
//             }


//             std::vector<SpaceTimeCell::Cell> remainingPath = pathFromWaypoints(waitCell, waitCell.t, waypoints, waypointIndex, table);
            
//             adjustedPath.clear();
//             adjustedPath = remainingPath;
//             solveCollision(adjustedPath, table, waypoints);

//             break;
//         }
        
//         adjustedPath.push_back(currentCell);



//         if (waypointIndex < waypoints.size() && path[i].x == waypoints[waypointIndex].x && path[i].y == waypoints[waypointIndex].y) {
//             waypointIndex++;
//         }
//     }

//     return adjustedPath;
// }

// std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::pathFromWaypoints(const SpaceTimeCell::Cell& start, int startTime, const std::vector<map::Cell>& waypoints, size_t waypointIndex, Reservation& table) {
//     std::vector<SpaceTimeCell::Cell> fullPath;
//     SpaceTimeCell::Cell currentPosition = start;
//     int currentTime = startTime;

//     // Przejdź przez wszystkie nieodwiedzone punkty pośrednie
//     for (size_t i = waypointIndex; i < waypoints.size(); ++i) {
//         std::vector<SpaceTimeCell::Cell> partialPath = pathToTarget(currentPosition,0, currentTime, waypoints[i], table);
//         fullPath.insert(fullPath.end(), partialPath.begin(), partialPath.end());

//         if (!partialPath.empty()) {
//             currentPosition = partialPath.back();
//             currentTime = currentPosition.t;
//         }
//     }
//     std::vector<SpaceTimeCell::Cell> partialPath = pathToTarget(currentPosition, 0, currentTime, start, table);
//     fullPath.insert(fullPath.end(), partialPath.begin(), partialPath.end());

    

//     return fullPath;
// }




std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table) {
    std::vector<SpaceTimeCell::Cell> fullPath;
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

        std::vector<SpaceTimeCell::Cell> path = pathToTarget(currentAgentPosition, stopTime, currentTime, currentTaskPosition, table);
        
        // Ensure no duplication of cells when appending the path
        if (!fullPath.empty() && fullPath.back().x == path.front().x &&
            fullPath.back().y == path.front().y && fullPath.back().t == path.front().t) {
            path.erase(path.begin());
        }


        fullPath.insert(fullPath.end(), path.begin(), path.end());
        try
        {
            currentTime = fullPath.back().t;
        }
        catch(std::exception& e)
        {
            std::cerr<<"Not found path, " << e.what() << "\n";
            return {};
        }
        currentAgentPosition = currentTaskPosition;
    }

    ///////////////// Dropoff handling
    {
        std::vector<SpaceTimeCell::Cell> dropoffPath = pathToTarget(currentAgentPosition, groupTask.getDropoffStopTime(), currentTime, groupTask.getDropoffLocation(), table);
        
        if (!fullPath.empty() && fullPath.back().x == dropoffPath.front().x &&
            fullPath.back().y == dropoffPath.front().y && fullPath.back().t == dropoffPath.front().t) 
            {
                dropoffPath.erase(dropoffPath.begin());
            }

        fullPath.insert(fullPath.end(), dropoffPath.begin(), dropoffPath.end());
        currentTime = fullPath.back().t;
        
        currentAgentPosition = dropoffPath.back();
    }

    //////////////// Return to start position
    {
        std::vector<SpaceTimeCell::Cell> returnPath = pathToTarget(currentAgentPosition, 0, currentTime, startPos, table);

        if (!fullPath.empty() && fullPath.back().x == returnPath.front().x &&
            fullPath.back().y == returnPath.front().y && fullPath.back().t == returnPath.front().t) 
        {
            returnPath.erase(returnPath.begin());
        }

        fullPath.insert(fullPath.end(), returnPath.begin(), returnPath.end());
        currentTime = fullPath.back().t;

    }
    
    //std::cout<<"CALA SCIEZKA PRZED SOLVE COLLISION\n";
    // for(int i=0; i<fullPath.size(); i++)
    // {
    //     std::cout<<"(" << fullPath[i].x << "," << fullPath[i].y << ", t: " <<  fullPath[i].t << ") -> ";
    // }

    
    //fullPath = solveCollision(fullPath, table, waypoints);
    
    //WYPISANIE SCIEZKI
    {
        std::cout<<"\n CALA SCIEZKA PO SOLVE COLLISION\n";
        for(int i=0; i<fullPath.size(); i++)
        {            
            table.reserve(fullPath[i].x, fullPath[i].y, fullPath[i].t);
            if(i<fullPath.size() - 1)
                table.reserveEdge(fullPath[i].x, fullPath[i].y, fullPath[i + 1].x, fullPath[i + 1].y, fullPath[i].t);
            std::cout<<"(" << fullPath[i].x << "," << fullPath[i].y << ", t: " <<  fullPath[i].t << ") -> ";
        }
        std::cout<<std::endl;
    }
    return fullPath;
}
