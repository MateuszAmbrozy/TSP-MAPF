#pragma once

#include "../AStar/Node.h"
#include "../Graph.h"


namespace SpaceTimeCell
{
    struct Cell : public map::Cell
    {
        int t;

        Cell(int x, int y, int t, bool isObstacle = false)
            : t{t}, map::Cell(x, y, isObstacle)
        {}

        Cell(map::Cell other, int t)
            : t{t}, map::Cell(other)
        {}

        bool operator == (const Cell& other) const
        {
            return (other.x == this->x &&
                    other.y == this->y &&
                    other.isObstacle == this->isObstacle &&
                    other.t == this->t);
        }

        Cell& operator=(const Cell& other) {
            if (this != &other) {
                this->x = other.x;
                this->y = other.y;
                this->isObstacle = other.isObstacle;
                this->t = other.t;
            }
            return *this;
        }
    };

    class Node : public A::Node  // Referencing Node from the A namespace
    {
    public:

        int t;
        Node(int t, int x, int y, bool isObstacle = false,
             A::Node* parent = nullptr, float gCost = 0,
             float hCost = 0, float fCost = 0)
            : A::Node(x, y, isObstacle, parent, gCost, hCost, fCost),
              t(t)
        {}

        Node(const A::Node& other, int t)
            : A::Node(other),
              t(t)
        {}

        Node& operator=(const A::Node* other) {
            if (this == other) return *this;  // Self-assignment check
            A::Node::operator=(*other);  // Assign the base class properties
            t = 0;  // Set t to default or based on other logic
            return *this;
        }
    };

}  // End of namespace SpaceTimeCell