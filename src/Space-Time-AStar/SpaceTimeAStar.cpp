#include "../../lib/Space-Time-AStar/SpaceTimeAStar.h"

SpaceTimeAStar::SpaceTimeAStar(map::Graph graph)
: A::Astar(graph)
{}

std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::getNeighbors(const Agent& agent, SpaceTimeCell::Node &current, const Reservation &table) {
    std::vector<SpaceTimeCell::Cell> neighbors;
    std::vector<map::Cell> illictis = agent.getIllicits();
    
    for (auto dir : setup::moves) {
        SpaceTimeCell::Cell neighbor(current.x + dir.first, current.y + dir.second, current.t + 1);
        bool containtsIllicits = std::find(illictis.begin(), illictis.end(), neighbor) != illictis.end();
        if (isValid(neighbor.x, neighbor.y) &&
             !table.isReserved(neighbor.x, neighbor.y, neighbor.t) &&
             !table.wouldCauseEdgeCollision(current.x, current.y, current.t, neighbor.x, neighbor.y, neighbor.t) &&
             !containtsIllicits)
        {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

//czasem agent wchodzi tutaj w nieskończoną petlę. Dlaczego?
std::vector<SpaceTimeCell::Cell> SpaceTimeAStar::pathToTarget(const Agent& unit, const map::Cell& start, const int waitTime, int currentTime, const map::Cell& target, Reservation& table)
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
        SpaceTimeCell::Cell buf(target, currentTime);
        return {buf};
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
        std::vector<SpaceTimeCell::Cell> neighbors = getNeighbors(unit, currentCell, table);

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
            maxWaitTime = 10;
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

        std::vector<SpaceTimeCell::Cell> path = pathToTarget(unit, currentAgentPosition, stopTime, currentTime, currentTaskPosition, table);
        if(path.size() == 0)
        {
            std::cout<<"ERROR::SpaceTimeAStar::findPath - not found path. Return {}.\n";
            return {};

        }
        // Ensure no duplication of cells when appending the path
        if (!fullPath.empty() && fullPath.back().x == path.front().x &&
            fullPath.back().y == path.front().y && fullPath.back().t == path.front().t) {
            path.erase(path.begin());
        }


        fullPath.insert(fullPath.end(), path.begin(), path.end());
        currentTime = fullPath.back().t;
        currentAgentPosition = currentTaskPosition;
    }
    // std::cout<<std::endl;
    // for(auto c : fullPath)
    // {
    //     std::cout<<c.x << ", " << c.y << " t: " << c.t << " -> ";
    // }

    ///////////////// Dropoff handling
    {
        std::vector<SpaceTimeCell::Cell> dropoffPath = pathToTarget(unit, currentAgentPosition, groupTask.getDropoffStopTime(), currentTime, groupTask.getDropoffLocation(), table);
        if(dropoffPath.size() == 0)
        {
            std::cout<<"ERROR::SpaceTimeAStar::findPath - not found path. Return {}.\n";
            return {};

        }
        if (!fullPath.empty() && fullPath.back().x == dropoffPath.front().x &&
            fullPath.back().y == dropoffPath.front().y && fullPath.back().t == dropoffPath.front().t) 
            {
                dropoffPath.erase(dropoffPath.begin());
            }

        fullPath.insert(fullPath.end(), dropoffPath.begin(), dropoffPath.end());
        currentTime = fullPath.back().t;
        
        currentAgentPosition = dropoffPath.back();
    }
    //     std::cout<<std::endl;
    // for(auto c : fullPath)
    // {
    //     std::cout<<c.x << ", " << c.y << " t: " << c.t << " -> ";
    // }

    //////////////// Return to start position
    {
        std::vector<SpaceTimeCell::Cell> returnPath = pathToTarget(unit, currentAgentPosition, 0, currentTime, startPos, table);
        if(returnPath.size() == 0)
        {
            std::cout<<"ERROR::SpaceTimeAStar::findPath - not found path. Return {}.\n";
            return {};

        }
        if (!fullPath.empty() && fullPath.back().x == returnPath.front().x &&
            fullPath.back().y == returnPath.front().y && fullPath.back().t == returnPath.front().t) 
        {
            returnPath.erase(returnPath.begin());
        }

        fullPath.insert(fullPath.end(), returnPath.begin(), returnPath.end());
        currentTime = fullPath.back().t;

    }
    // std::cout<<std::endl;
    // for(auto c : fullPath)
    // {
    //     std::cout<<c.x << ", " << c.y << " t: " << c.t << " -> ";
    // }
    
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
            table.reserve(fullPath[i].x, fullPath[i].y, fullPath[i].t + 1);
            if(i<fullPath.size() - 1)
            {
                table.reserveEdge(fullPath[i].x, fullPath[i].y, fullPath[i + 1].x, fullPath[i + 1].y, fullPath[i].t);
            }
            std::cout<<"(" << fullPath[i].x << "," << fullPath[i].y << ", t: " <<  fullPath[i].t << ") -> ";
        }
        std::cout<<std::endl;
    }
    return fullPath;
}
