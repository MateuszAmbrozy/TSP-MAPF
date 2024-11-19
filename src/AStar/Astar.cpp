#include "../../lib/AStar/AStar.h"


//Astar
bool A::Astar::isValid(int x, int y)
{
    return ((x >= 0) && (x < graph.width) &&
            (y >= 0) && (y<graph.height) &&
            (!graph.cells[x][y].isObstacle));
}

int A::Astar::lengthOfPath(std::shared_ptr<A::Node> node)
{
    std::shared_ptr<A::Node> cell = node;
    int len = 0;
    if(!node || !node->getParent())
        return 0;
    
    while (cell->getParent() != nullptr) {
        cell = cell->getParent();
        len++;
    }

    return len;
}


float A::Node::calculateH(int startX, int startY, int destX, int destY)
{
    return static_cast<float>(std::sqrt((startX - destX) * (startX - destX) + (startY - destY) * (startY - destY)));
}

 void A::Astar::copyGraph()
{
    aStarGraph.resize(graph.cells.size(), std::vector<Node>(graph.height, Node(0, 0)));
    for(size_t i = 0; i < graph.cells.size(); i++)
    {
        for(size_t j=0; j<graph.cells[i].size(); j++)
        {
            aStarGraph[i][j] = graph.cells[i][j];
        }
    }
}

bool A::Astar::isDestination(int startX, int startY, int destX, int destY)
{
    return (startX==destX && startY==destY);
}



A::Astar::Astar(map::Graph graph)
{
    this->graph = graph;
    copyGraph();
}



int A::Astar::calculate(map::Cell start, map::Cell dest)
{
    std::vector<std::vector<bool>> closedList(aStarGraph.size(), std::vector<bool>(aStarGraph[0].size(), false));
    std::priority_queue<Node, std::vector<Node>, CompareCells> openList;
    std::vector<Node> emptyPath;

    if (isValid(dest.x, dest.y) == false) {
       std::cout << "Destination is an obstacle" << std::endl;
       return std::numeric_limits<int>::max();
   }

   if (isDestination(start.x, start.y, dest.x, dest.y)) {
       //std::cout << "You are the destination" << std::endl;
       return 0;
   }

    Node startNode(start.x, start.y, start.isObstacle);
    
    startNode.gCost = 0;
    startNode.hCost = A::Node::calculateH(start.x, start.y, dest.x, dest.y);
    startNode.fCost = startNode.gCost + startNode.hCost;
    startNode.setParent(nullptr); 

    openList.push(startNode);
    while (!openList.empty()) {
        Node currentCell = openList.top();
        openList.pop();

        if (isDestination(currentCell.x, currentCell.y, dest.x, dest.y)) 
        {           
            return lengthOfPath(std::make_shared<A::Node>(currentCell));
        }

        closedList[currentCell.x][currentCell.y] = true;

        for (int addX = -1; addX <= 1; addX++) 
        {
            for (int addY = -1; addY <= 1; addY++) 
            {
                if (std::abs(addX) == std::abs(addY)) 
                    continue;

                std::pair<int, int> neighborPos = {currentCell.x + addX, currentCell.y + addY};

                if (isValid(neighborPos.first, neighborPos.second)) 
                {
                    if (!closedList[neighborPos.first][neighborPos.second]) {
                        double gNew = currentCell.gCost + 1;
                        double hNew = A::Node::calculateH(neighborPos.first, neighborPos.second, dest.x, dest.y);
                        double fNew = gNew + hNew;

                        std::shared_ptr<A::Node> parentNode = std::make_shared<A::Node>(currentCell);
                        Node neighborCell(neighborPos.first, neighborPos.second, false, parentNode, gNew, hNew, fNew);
                        openList.push(neighborCell);
                    }
                }
            }
        }
    }

    std::cout << "Nie znaleziono ścieżki do celu.\n";
    return std::numeric_limits<int>::max();
}
