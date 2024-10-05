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

std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::pathToTarget(const map::Cell& start, int currentTime, const map::Cell& target, Reservation& table) {
    std::vector<SpaceTimeCell::Cell> path;

    if (!isValid(target.x, target.y)) {
        std::cerr << "Target is an obstacle or invalid." << std::endl;
        return path;
    }

    if (isDestination(start.x, start.y, target.x, target.y)) {
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
    int maxWaitTime = 10; // Maximum time the agent can wait
    bool waiting = false;

    while (!openList.empty()) {
        SpaceTimeCell::Node currentCell = openList.top();
        openList.pop();
        maxWaitTime = 10;
        // If the agent is at the destination
        if (isDestination(currentCell.x, currentCell.y, target.x, target.y)) {
            SpaceTimeCell::Node* temp = &currentCell;
            while (temp != nullptr) {
                SpaceTimeCell::Cell cell(temp->x, temp->y, temp->t);
                path.push_back(cell);
                //table.reserve(temp->x, temp->y, temp->t);
                temp = dynamic_cast<SpaceTimeCell::Node*>(temp->parent);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedList[currentCell.x][currentCell.y] = true;

        std::vector<SpaceTimeCell::Cell> neighbors = getNeighbors(currentCell, table);

        // if (neighbors.empty() && maxWaitTime > 0) 
        // {
        //     SpaceTimeCell::Node waitNode(currentCell.t + 1, currentCell.x, currentCell.y, false, new SpaceTimeCell::Node(currentCell));
        //     openList.push(waitNode);
        //     maxWaitTime--;
        //     waiting = true;
        // } 
        // else 
        // {
        //     waiting = false;
            for (const auto& neighbor : neighbors) {
                if (!closedList[neighbor.x][neighbor.y]) {
                    double gNew = currentCell.gCost + 1;
                    double hNew = SpaceTimeCell::Node::calculateH(neighbor.x, neighbor.y, target.x, target.y);
                    double fNew = gNew + hNew;

                    SpaceTimeCell::Node neighborCell(currentCell.t + 1, neighbor.x, neighbor.y, false, new SpaceTimeCell::Node(currentCell), gNew, hNew, fNew);
                    openList.push(neighborCell);
                }
            }
        //}
    }

    if (waiting) {
        std::cerr << "Agent has reached max waiting time with no valid path found.\n";
    } else {
        std::cerr << "Path to target not found." << std::endl;
    }

    return path;
}

std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::solveCollision(std::vector<SpaceTimeCell::Cell>& path,
                                                                Reservation& table,
                                                                const std::vector<map::Cell>& waypoints)
{
    std::vector<SpaceTimeCell::Cell> adjustedPath;
    int additionalTime = 0;
    size_t waypointIndex = 0;
    
    for(size_t i = 0; i<path.size(); ++i) {
        SpaceTimeCell::Cell currentCell = path[i];

        // // Sprawdź, czy kolejna komórka jest zarezerwowana
        // if(i + 1 < path.size() && table.isReserved(path[i + 1].x, path[i + 1].y, path[i + 1].t)) {
        //     std::cout<<"\nNORMAL COLISION x: " << path[i].x << ", y: " << path[i].y << " - t:" <<  path[i].t <<"\n";
            
        //     adjustedPath.push_back(currentCell);

        //     for (size_t j = i; j < path.size(); ++j) 
        //     {
        //         path[j].t++;  // Zwiększ czas dla każdej kolejnej komórki
        //     }
        //     currentCell = path[i];
        // }

        if (i + 1 < path.size() && table.wouldCauseEdgeCollision(currentCell.x, currentCell.y, currentCell.t, path[i + 1].x, path[i + 1].y, path[i + 1].t)) {
            std::cout << "\nIMPAS " << path[i].x << ", " << path[i].y << " - t: " << path[i].t << "\n";
            
            auto waitCell = path.front();
            path.insert(path.begin(), waitCell);
            for (size_t j = 1; j < path.size(); ++j) 
            {
                path[j].t++;  // Zwiększ czas dla każdej kolejnej komórki
            }


            std::vector<SpaceTimeCell::Cell> remainingPath = pathFromWaypoints(waitCell, waitCell.t, waypoints, waypointIndex, table);
            
            adjustedPath.clear();
            adjustedPath = remainingPath;
            solveCollision(adjustedPath, table, waypoints);

            break;
        }
        
        adjustedPath.push_back(currentCell);



        if (waypointIndex < waypoints.size() && path[i].x == waypoints[waypointIndex].x && path[i].y == waypoints[waypointIndex].y) {
            waypointIndex++;
        }
    }

    return adjustedPath;
}

std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::pathFromWaypoints(const SpaceTimeCell::Cell& start, int startTime, const std::vector<map::Cell>& waypoints, size_t waypointIndex, Reservation& table) {
    std::vector<SpaceTimeCell::Cell> fullPath;
    SpaceTimeCell::Cell currentPosition = start;
    int currentTime = startTime;

    // Przejdź przez wszystkie nieodwiedzone punkty pośrednie
    for (size_t i = waypointIndex; i < waypoints.size(); ++i) {
        std::vector<SpaceTimeCell::Cell> partialPath = pathToTarget(currentPosition, currentTime, waypoints[i], table);
        fullPath.insert(fullPath.end(), partialPath.begin(), partialPath.end());

        if (!partialPath.empty()) {
            currentPosition = partialPath.back();
            currentTime = currentPosition.t;
        }
    }
    std::vector<SpaceTimeCell::Cell> partialPath = pathToTarget(currentPosition, currentTime, start, table);
    fullPath.insert(fullPath.end(), partialPath.begin(), partialPath.end());

    

    return fullPath;
}




std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table) {
    std::vector<SpaceTimeCell::Cell> fullPath;
    map::Cell startPos = unit.getPosition();
    map::Cell currentAgentPosition = unit.getPosition();
    std::vector<map::Cell> waypoints;



    for (int taskIdx : taskOrder) {
        if (taskIdx >= groupTask.getNumTasks() || taskIdx < 0) {
            std::cerr << "Invalid task _" << taskIdx << "_ in task order." << std::endl;
            return fullPath;
        }

        map::Cell currentTaskPosition = groupTask[taskIdx];
        waypoints.push_back(groupTask[taskIdx]);
        int stopTime = groupTask.getPickupStopTime(taskIdx);

        std::vector<SpaceTimeCell::Cell> path = pathToTarget(currentAgentPosition, currentTime, currentTaskPosition, table);
        // path = solveCollision(path, table);
        
        // Ensure no duplication of cells when appending the path
        if (!fullPath.empty() && fullPath.back().x == path.front().x &&
            fullPath.back().y == path.front().y && fullPath.back().t == path.front().t) {
            path.erase(path.begin());
        }


        fullPath.insert(fullPath.end(), path.begin(), path.end());
        currentTime = fullPath.back().t;


        SpaceTimeCell::Cell lastPickupCell = path.back();
        for (int t = 0; t < stopTime; ++t) 
        {
            SpaceTimeCell::Cell waitCell = lastPickupCell;
            waitCell.t += t + 1; 
            fullPath.push_back(waitCell); 
            currentTime++;
            
        }

        //currentTime = fullPath.back().t;
        currentAgentPosition = currentTaskPosition;
    }

    ///////////////// Dropoff handling
    {
        std::vector<SpaceTimeCell::Cell> dropoffPath = pathToTarget(currentAgentPosition, currentTime, groupTask.getDropoffLocation(), table);
        // dropoffPath = solveCollision(dropoffPath, table);
        
        if (!fullPath.empty() && fullPath.back().x == dropoffPath.front().x &&
            fullPath.back().y == dropoffPath.front().y && fullPath.back().t == dropoffPath.front().t) 
            {
                dropoffPath.erase(dropoffPath.begin());
            }

        fullPath.insert(fullPath.end(), dropoffPath.begin(), dropoffPath.end());
        currentTime = fullPath.back().t;
        //// Handle dropoff stop time
        SpaceTimeCell::Cell lastDropoffCell = dropoffPath.back();
        
        
        int dropoffStopTime = groupTask.getDropoffStopTime();
        for (int t = 0; t < dropoffStopTime; ++t) {
            SpaceTimeCell::Cell waitCell = lastDropoffCell;
            waitCell.t += t + 1;
            fullPath.push_back(waitCell);
        }
        currentTime += dropoffStopTime;
        currentAgentPosition = lastDropoffCell;
    }

    //////////////// Return to start position
    {
        std::vector<SpaceTimeCell::Cell> returnPath = pathToTarget(currentAgentPosition,currentTime, startPos, table);
        // returnPath = solveCollision(returnPath, table);

        if (!fullPath.empty() && fullPath.back().x == returnPath.front().x &&
            fullPath.back().y == returnPath.front().y && fullPath.back().t == returnPath.front().t) 
        {
            returnPath.erase(returnPath.begin());
        }

        fullPath.insert(fullPath.end(), returnPath.begin(), returnPath.end());
        currentTime = fullPath.back().t;

    }
    
    std::cout<<"CALA SCIEZKA PRZED SOLVE COLLISION\n";
    for(int i=0; i<fullPath.size(); i++)
    {
        std::cout<<"(" << fullPath[i].x << "," << fullPath[i].y << ", t: " <<  fullPath[i].t << ") -> ";
    }

    
    fullPath = solveCollision(fullPath, table, waypoints);
    
    //WYPISANIE SCIEZKI
    {
        std::cout<<"\n CALA SCIEZKA PO SOLVE COLLISION\n";
        for(int i=0; i<fullPath.size(); i++)
        {
            //table.reserveEdge(fullPath[i].x, fullPath[i].y, fullPath[i].t, fullPath[i+1].x, fullPath[i+1].y);
            
            table.reserve(fullPath[i].x, fullPath[i].y, fullPath[i].t);
            if(i<fullPath.size() - 1)
                table.reserveEdge(fullPath[i].x, fullPath[i].y, fullPath[i + 1].x, fullPath[i + 1].y, fullPath[i].t);
                //table.reserve(fullPath[i+1].x, fullPath[i+1].y, fullPath[i].t + 1);
            std::cout<<"(" << fullPath[i].x << "," << fullPath[i].y << ", t: " <<  fullPath[i].t << ") -> ";
        }
        std::cout<<std::endl;
    }
    return fullPath;
}
