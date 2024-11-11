
/**
 * @file Node.h
 * @author Mateusz Ambroży
 * @brief Node to A*
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "../Graph.h"

namespace A
{
struct Node : public map::Cell {
    
    Node* parent; 
    float gCost, hCost, fCost;
    Node& operator=(const map::Cell& other);
    Node& operator=(const Node& other);
    Node() = default;
    Node(int x, int y, bool isObstacle = false, Node* parent= nullptr, float gCost=0, float hCost=0, float fCost = 0);
    Node(const Node& other);

    virtual ~Node() {}
    bool operator<(const Node& other) const;

    static float calculateH(int startX, int startY, int destX, int destY);
};
}
