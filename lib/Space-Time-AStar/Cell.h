/**
 * @file Cell.h
 * @author Mateusz Ambroży
 * @brief Cell and Node for Space Time A*
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "../AStar/Node.h"
#include "../Graph.h"
#include <memory>

namespace SpaceTime
{
    struct Cell : public map::Cell
    {
        int t;

        Cell(int x, int y, int t, bool isObstacle = false)
            : map::Cell(x, y, isObstacle), t{t}
        {}

        Cell(const Cell& other)
            : map::Cell(other), t(other.t)
        {}

        Cell(const map::Cell& other, int t)
            : map::Cell(other), t{t}
        {}
        Cell() = default;

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

    class Node : public A::Node
    {
    public:

        int t;
        std::shared_ptr<Node> parent; 
        Node() = default;

        Node(int t, int x, int y, bool isObstacle = false,
             std::shared_ptr<Node> parent = nullptr, float gCost = 0,
             float hCost = 0, float fCost = 0)
            : A::Node(x, y, isObstacle, nullptr, gCost, hCost, fCost),
              t(t), parent(parent)
        {}

        Node(const A::Node& other, int t)
            : A::Node(other),
              t(t)
        {}

        Node& operator=(const A::Node* other) 
        {
            if (this == other) return *this;
            A::Node::operator=(*other);
            t = 0;
            return *this;
        }
    };

}
