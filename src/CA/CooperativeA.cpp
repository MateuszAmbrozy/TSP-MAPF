#include "../../lib/CA/CooperativeA.h"
#include <climits>
//#include <qDebug>
CooperativeA::CooperativeA(map::Graph graph)
: A::Astar(graph)
{}

std::vector<CA::Cell> CooperativeA::getNeighbors(const Agent& agent, map::Cell target,  CA::Node &current, const Reservation &table, std::vector<std::vector<bool>> closedList) {
    std::vector<CA::Cell> neighbors;
    std::vector<map::Cell> illictis = agent.getIllicits();

    for (auto dir : setup::moves)
    {
        CA::Cell neighbor(current.x + dir.first, current.y + dir.second, current.t + 1);
        bool containtsIllicits = std::find(illictis.begin(), illictis.end(), neighbor) != illictis.end();

        // Check if the neighbor is valid and doesn't contain any illicits
        if (isValid(neighbor.x, neighbor.y) && !containtsIllicits)
        {

            if (isDestination(neighbor.x, neighbor.y, target.x, target.y))
            {
                if (!table.wouldCauseEdgeCollision(current.x, current.y, current.t, neighbor.x, neighbor.y))
                {
                    neighbors.push_back(neighbor);  // Prioritize target cell even if reserved
                }
            }
            else
            {
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

std::vector<CA::Cell> CooperativeA::pathToTarget(const Agent& unit, const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table)
{
    auto isDestinationStartTime = std::chrono::high_resolution_clock::now();
    std::vector<CA::Cell> path;

    if (!isValid(target.x, target.y)) 
    {
        std::cerr << "Target is an obstacle or invalid." << std::endl;
        return path;
    }

    if (isDestination(start.x, start.y, target.x, target.y)) 
    {
        std::cout << "Already at the destination." << std::endl;
        CA::Cell buf(target, currentTime + 1);
        return {buf};
    }
    std::vector<map::Cell> illictis = unit.getIllicits();
    if(std::find(illictis.begin(), illictis.end(), target) != illictis.end())
    {
        std::cerr << "Target is an obstacle or invalid." << std::endl;
        return path;
    }
    std::vector<std::vector<bool>> closedList(graph.cells.size(), std::vector<bool>(graph.cells[0].size(), false));
    std::unordered_map<CA::Node, unsigned int> closedSet;
    std::priority_queue<CA::Node, std::vector<CA::Node>, A::CompareCells> openList;

    CA::Node startNode(currentTime, start.x, start.y, start.isObstacle);

    startNode.gCost = 0;
    //double hNew = CA::Node::calculateH(neighbor.x, neighbor.y, target.x, target.y);
    startNode.hCost = CA::Node::calculateH(start.x, start.y, target.x, target.y);
    startNode.fCost = startNode.gCost + startNode.hCost;
    startNode.parent = nullptr;

    openList.push(startNode);
    int maxWaitTime = 5;
    bool waiting = false;
    CA::Node currentCell;
    while (!openList.empty())
    {
        currentCell = openList.top();
        openList.pop();
        closedSet.insert({currentCell, currentCell.fCost});
        if (isDestination(currentCell.x, currentCell.y, target.x, target.y)) 
        {
            int waitT = currentCell.t;
           std::shared_ptr<CA::Node> temp = std::make_shared<CA::Node>(currentCell);
            while (temp != nullptr) 
            {
                CA::Cell cell(temp->x, temp->y, temp->t);
                path.push_back(cell);
                temp = temp->parent;
                //temp = dynamic_cast<CA::Node*>(temp->parent);
            }
            std::reverse(path.begin(), path.end());

            
            if(!isTargetFreeForEntireWaitTime(target, currentCell.t, waitTime, table))
            {
                path.pop_back();

                auto parentCell = currentCell.parent;
                //CA::Node* parentCell = dynamic_cast<CA::Node*>(currentCell.parent);
                if (!parentCell) {
                    std::cerr << "No parent cell to wait in. Exiting...\n";
                    return path;
                }

                while (!isTargetFreeForEntireWaitTime(target, waitT, waitTime, table)) 
                {
                    CA::Cell waitCell(parentCell->x, parentCell->y, waitT - 1);
                    recursiveWaitAndReturn(unit, waitTime, target, &waitCell, nullptr, waitT, path, table);
                }
                CA::Cell buf = path.back();
                if(table.wouldCauseEdgeCollision(buf.x, buf.y, buf.t, target.x, target.y))
                {
                    //to do poprawy, bo jak agent wychodzi z komorki na ktora agent chce wejśc to raczej nie znajdzie tej ścieżki i zwróci else
                    CA::Cell waitCell(parentCell->x, parentCell->y, waitT);
                    auto alternativePath = pathToTarget(unit, waitCell, waitTime, currentTime, target, table);

                    if (!alternativePath.empty()) 
                    {
                        path.insert(path.end(), alternativePath.begin(), alternativePath.end());
                    } 
                    else
                    {
                        std::cerr << "No alternative path found after waiting. Exiting...\n";
                        return {};
                    }
                    currentTime = alternativePath.back().t + 1;
                }
                CA::Cell finalCell(target.x, target.y, waitT);
                path.push_back(finalCell);
            }
            //auto isDestinationEndTime = std::chrono::high_resolution_clock::now();
            //auto isDestinationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(isDestinationEndTime - isDestinationStartTime).count();

            // Print the elapsed time
            //std::cout << "Execution time for agent " << unit.getId() << " of partPath: " << isDestinationDuration << " millisenconds" << std::endl;
            //std::cout<<"Iterator: " << iterator << ", openList.size(): " << openList.size() << ", closedList.size(): " << counter << " for agent: " << unit.getId() <<std::endl;
            return path;
        }
    

        //closedList[currentCell.x][currentCell.y] = true;
        std::vector<CA::Cell> neighbors = getNeighbors(unit, target, currentCell, table, closedList);

        if(maxWaitTime == 0)
        {
            return {};
        }
        else if (neighbors.empty() && maxWaitTime > 0) 
        {
            auto parentNode = std::make_shared<CA::Node>(currentCell);
            if (!table.isReserved(currentCell.x, currentCell.y, currentCell.t + 1))
            {
                CA::Node waitNode(currentCell.t + 1, currentCell.x, currentCell.y, false, parentNode, currentCell.gCost + 1, currentCell.hCost, currentCell.fCost + 1);
                openList.push(waitNode);
                maxWaitTime--;
                waiting = true;
                auto it = closedSet.find(waitNode);
                if (it != closedSet.end()) 
                {
                    it->second = currentCell.fCost + 1;
                }
            } else {
                
                CA::Node neighborCell(currentCell.t + 1, currentCell.x, currentCell.y, false, parentNode, currentCell.gCost, currentCell.hCost, currentCell.fCost);
                auto it = closedSet.find(neighborCell);
                std::cerr << "Wait node at (" << currentCell.x << ", " << currentCell.y
                          << ") t = " << currentCell.t + 1 << " is reserved.\n";
                closedSet[neighborCell] = INT_MAX;
            }
        } 
        
        else if(!neighbors.empty())
        {
            waiting = false;
            maxWaitTime = 5;
            for (CA::Cell& neighbor : neighbors)
            {
                    double gNew = currentCell.gCost + 1;
                    double hNew = CA::Node::calculateH(neighbor.x, neighbor.y, target.x, target.y);
                    //double hNew = calculate(neighbor, target);
                    double fNew = gNew + hNew;

                    auto parentNode = std::make_shared<CA::Node>(currentCell);
                    CA::Node neighborCell(currentCell.t + 1, neighbor.x, neighbor.y, false, parentNode, gNew, hNew, fNew);
                    auto it = closedSet.find(neighborCell);
                    if (it != closedSet.end()) {
                        if (fNew >= it->second) {
                            continue;
                        }
                    }
                    else
                    {
                        closedSet[neighborCell] = fNew;
                    }

                    openList.push(neighborCell);

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


bool CooperativeA::recursiveWaitAndReturn(const Agent& unit, const int waitTime, const map::Cell& target, CA::Cell* currentCell, CA::Cell* parentCell, int& waitT, std::vector<CA::Cell>& path, Reservation& table) {

    //jesli ma rodzica i rodzic jest pusty i ruch na rodzica nie spowoduje kolizji to znajduje ścieżkę z punktu początkowego do target i konczy dzialanie
    if (parentCell && !table.isReserved(parentCell->x, parentCell->y, waitT) &&
        !table.wouldCauseEdgeCollision(currentCell->x, currentCell->y, waitT, parentCell->x, parentCell->y))
    {

        CA::Cell waitCell(currentCell->x, currentCell->y, waitT);
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
        path.push_back(CA::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return recursiveWaitAndReturn(unit, waitTime, target, currentCell, parentCell, waitT, path, table);
    }

    //gdy nie ma rodzica
    if (!table.isReserved(currentCell->x, currentCell->y, waitT))
    {
        path.push_back(CA::Cell(currentCell->x, currentCell->y, waitT));
        waitT++;
        return true;
    }



    CA::Cell waitCell(currentCell->x, currentCell->y, waitT);
    CA::Cell alternative = findAlternativeWaitingCell(unit, waitCell, waitT, table);

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

CA::Cell CooperativeA::findAlternativeWaitingCell(const Agent& unit, const CA::Cell& cell, int currentTime, Reservation& table) {
    //currentTime--;
    std::vector<map::Cell> illictis = unit.getIllicits();

    for (auto dir : setup::moves)
    {
        CA::Cell neighbor(cell.x + dir.first, cell.y + dir.second, currentTime);
        bool containtsIllicits = std::find(illictis.begin(), illictis.end(), neighbor) != illictis.end();
        if (isValid(neighbor.x, neighbor.y) &&
             !table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
             !table.wouldCauseEdgeCollision(cell.x, cell.y, currentTime, neighbor.x, neighbor.y) &&
             !containtsIllicits)
        {
            return neighbor;
        }
    }

    return CA::Cell(-1, -1, currentTime);
}
bool CooperativeA::isTargetFreeForEntireWaitTime(const map::Cell& target, int startTime, int waitTime, Reservation& table) {
    for (int t = 0; t <= waitTime; ++t) 
    {
        if (table.isReserved(target.x, target.y,startTime + t)) 
        {
            return false;
        }
    }
    return true;
}


std::vector<CA::Cell> CooperativeA::findPath(Agent& unit, int currentTime, TaskGroup groupTask, std::vector<int> taskOrder, Reservation& table) {
    std::vector<CA::Cell> fullPath;
    map::Cell startPos = unit.getPosition();
    map::Cell currentAgentPosition = unit.getPosition();
    std::vector<map::Cell> waypoints;
    int stopTime;

    auto startTime = std::chrono::high_resolution_clock::now();
    for (int taskIdx : taskOrder) {
        if (taskIdx >= groupTask.getNumTasks() || taskIdx < 0)
        {
            std::cerr << "Invalid task _" << taskIdx << "_ in task order." << std::endl;
            return fullPath;
        }

        map::Cell currentTaskPosition = groupTask[taskIdx];
        waypoints.push_back(groupTask[taskIdx]);
        stopTime = groupTask.getPickupStopTime(taskIdx);

        std::vector<CA::Cell> path = pathToTarget(unit, currentAgentPosition, stopTime, currentTime, currentTaskPosition, table);

        if(path.empty())
        {
            //std::cout<<"ERROR::CooperativeA::findPath - not found path. Return {}.\n";
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
            CA::Cell waitCell(currentTaskPosition.x, currentTaskPosition.y,  s + t);
            path.push_back(waitCell);
        }
        fullPath.insert(fullPath.end(), path.begin(), path.end());
        currentTime = fullPath.back().t;
        currentAgentPosition = currentTaskPosition;
    }

    ///////////////// Dropoff handling
    {
        std::vector<CA::Cell> dropoffPath = pathToTarget(unit, currentAgentPosition, groupTask.getDropoffStopTime(), currentTime, groupTask.getDropoffLocation(), table);

        if(dropoffPath.size() == 0)
        {
            //std::cout<<"ERROR::CooperativeA::findPath - not found path. Return {}.\n";
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
            CA::Cell waitCell(groupTask.getDropoffLocation().x, groupTask.getDropoffLocation().y, s  + t);
            dropoffPath.push_back(waitCell);
        }

        fullPath.insert(fullPath.end(), dropoffPath.begin(), dropoffPath.end());
        currentTime = fullPath.back().t;
        
        currentAgentPosition = dropoffPath.back();
    }

    //////////////// Return to start position
    {
        std::vector<CA::Cell> returnPath = pathToTarget(unit, currentAgentPosition, 0, currentTime, startPos, table);
        if(returnPath.size() == 0)
        {
            //std::cout<<"ERROR::CooperativeA::findPath - not found path. Return {}.\n";
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
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the elapsed time
    //std::cout << "Execution time for agent " << unit.getId() << " of findTotalPath: " << duration << " millisenconds" << std::endl;

    return fullPath;
}
