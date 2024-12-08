#include "../../lib/AStar/Node.h"

//Node
// A::Node::Node()
//     : Cell(), parent{nullptr}, gCost{0}, hCost{0}, fCost{0}
// {}
A::Node::Node(int x, int y, bool isObstacle, std::shared_ptr<A::Node> parent, float gCost, float hCost, float fCost)
    : Cell(x, y, isObstacle), parent(parent),
    gCost(gCost), hCost(hCost), fCost(fCost)
{}

A::Node::Node(const A::Node& other)
    : Cell(other),
      parent(other.parent),
      gCost(other.gCost),
      hCost(other.hCost),
      fCost(other.fCost)
{

}

bool A::Node::operator<(const A::Node& other) const {
    return fCost < other.fCost;
}
bool A::Node::operator == (const Node& other) const
{
    return Cell::operator==(other) && parent == other.parent && fCost==other.fCost;
}

bool A::Node::operator == (Node& other) const
{
    return Cell::operator==(other) && parent == other.parent && fCost==other.fCost;
}

A::Node& A::Node::operator=(const A::Node& other) {
    if (this == &other) return *this;
    Cell::operator=(other);
    this->parent = other.parent; 
    this->fCost = other.fCost;
    this->gCost = other.gCost;
    this->hCost = other.hCost;
    return *this;
}
std::shared_ptr<A::Node> A::Node::getParent()
{
    return this->parent;
}
void A::Node::setParent(std::shared_ptr<A::Node> parent)
{
    this->parent = parent;
}

A::Node& A::Node::operator=(const map::Cell& other) {
    if (this == &other) return *this;
    Cell::operator=(other);  // Use base class (map::Cell) assignment
    // this->parent = nullptr;  // Copy the parent pointer
    // this->fCost = 0;
    // this->gCost = 0;
    // this->hCost = 0;
    
    return *this;
}
