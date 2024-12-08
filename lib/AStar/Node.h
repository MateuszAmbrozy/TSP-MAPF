
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
#include <memory>
namespace A
{
struct Node : public map::Cell {
private:
    std::shared_ptr<Node> parent; 
public:
    float gCost, hCost, fCost;
    Node& operator=(const map::Cell& other);
    Node& operator=(const Node& other);
    Node() = default;
    Node(int x, int y, bool isObstacle = false, std::shared_ptr<Node> parent = nullptr, float gCost=0, float hCost=0, float fCost = 0);
    Node(const Node& other);
    std::shared_ptr<A::Node> getParent();
    void setParent(std::shared_ptr<A::Node> parent);

    virtual ~Node() {}
    bool operator<(const Node& other) const;
    bool operator == (const Node& other) const;
    bool operator == (Node& other) const;

    static float calculateH(int startX, int startY, int destX, int destY);
};
}
