// agentrectitem.cpp
#include "agentrectitem.h"
#include "qgraphicsscene.h"

AgentRectItem::AgentRectItem(int id, int capacity, map::Cell position, QWidget* parent)
    : agent(id, capacity, position)
{}

void AgentRectItem::draw(QGraphicsScene *scene)
{
    int agentSize = 10;
    QPen pen(Qt::red);
    QBrush agentBrush(Qt::red); // Red brush for agent

    QRectF rect(agent.getPosition().x * 20 + 5, agent.getPosition().y * 20 + 5, agentSize, agentSize);
    QGraphicsRectItem* rectItem = scene->addRect(rect, pen, agentBrush);
}

void AgentRectItem::moveAgent(QGraphicsScene *scene, int timestep)
{
    // Use agent's path for movement
    const auto& path = agent.getPath(); // Assuming agent has a getPath method

    if (timestep < path.size()) {
        // Update agent position at each timestep
        map::Cell newPos = path[timestep];
        agent.move(newPos); // Update agent's internal position
        draw(scene); // Redraw at new position
    }
}
