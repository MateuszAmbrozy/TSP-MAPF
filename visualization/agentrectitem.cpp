// #include "agentrectitem.h"
// #include "qgraphicsscene.h"

// AgentRectItem::AgentRectItem(int id, int capacity, map::Cell position, const std::vector<map::Cell>& pickups, const map::Cell& dropoff, QWidget* parent)
//     : agent(id, capacity, position), pickupPoints(pickups), dropoffPoint(dropoff)
// {
//     agentColor = assignRandomColor();  // Random color assigned once
// }

// QColor AgentRectItem::assignRandomColor()
// {
//     int r = QRandomGenerator::global()->bounded(256);
//     int g = QRandomGenerator::global()->bounded(256);
//     int b = QRandomGenerator::global()->bounded(256);
//     return QColor(r, g, b);
// }

// void AgentRectItem::draw(QGraphicsScene *scene)
// {
//     if (taskCompleted) return;

//     int agentSize = 10;
//     QPen pen(agentColor);
//     QBrush agentBrush(agentColor);

//     // Draw agent rectangle
//     QRectF rect(agent.getPosition().x * 20 + 5, agent.getPosition().y * 20 + 5, agentSize, agentSize);
//     agentRectItem = scene->addRect(rect, pen, agentBrush);

//     // Draw pickup and dropoff points
//     drawPickupPoints(scene);
//     drawDropoffPoint(scene);
// }

// void AgentRectItem::drawPickupPoints(QGraphicsScene* scene)
// {
//     for (const auto& pickup : pickupPoints)
//     {
//         QPen pen(agentColor);
//         QBrush brush(agentColor);
//         QRectF circle(pickup.x * 20 + 5, pickup.y * 20 + 5, 15, 15);
//         QGraphicsEllipseItem* circleItem = scene->addEllipse(circle, pen, brush);

//         QGraphicsTextItem* idLabel = scene->addText(QString::number(agent.getId()));
//         idLabel->setPos(pickup.x * 20 + 7, pickup.y * 20 + 7);

//         pickupGraphicsItems.push_back(circleItem);  // Store to remove later
//     }
// }

// void AgentRectItem::drawDropoffPoint(QGraphicsScene* scene)
// {
//     QPen pen(agentColor);
//     QBrush brush(agentColor);
//     QRectF circle(dropoffPoint.x * 20 + 5, dropoffPoint.y * 20 + 5, 20, 20);
//     dropoffGraphicsItem = scene->addEllipse(circle, pen, brush);

//     QGraphicsTextItem* idLabel = scene->addText(QString::number(agent.getId()));
//     idLabel->setPos(dropoffPoint.x * 20 + 7, dropoffPoint.y * 20 + 7);
// }

// void AgentRectItem::moveAgent(QGraphicsScene* scene, int timestep)
// {
//     if (taskCompleted) return;

//     std::vector<SpaceTimeCell::Cell> path = agent.getPath();
//     if (timestep < path.size())
//     {
//         agent.move(path[timestep]);

//         // Remove the old agent rectangle before drawing new position
//         if (agentRectItem != nullptr) {
//             scene->removeItem(agentRectItem);
//             delete agentRectItem;
//             agentRectItem = nullptr;
//         }

//         // Draw at new position
//         draw(scene);
//     }
//     else
//     {
//         completeTask(scene);  // Remove pickup/dropoff when task is completed
//     }
// }

// void AgentRectItem::completeTask(QGraphicsScene* scene)
// {
//     // taskCompleted = true;

//     // // Remove pickup points
//     // for (auto& pickupItem : pickupGraphicsItems) {
//     //     scene->removeItem(pickupItem);
//     //     delete pickupItem;
//     // }
//     // pickupGraphicsItems.clear();

//     // // Remove dropoff point
//     // if (dropoffGraphicsItem != nullptr) {
//     //     scene->removeItem(dropoffGraphicsItem);
//     //     delete dropoffGraphicsItem;
//     //     dropoffGraphicsItem = nullptr;
//     // }

//     // // Remove the agent rectangle
//     // if (agentRectItem != nullptr) {
//     //     scene->removeItem(agentRectItem);
//     //     delete agentRectItem;
//     //     agentRectItem = nullptr;
//     // }
// }
#include "agentrectitem.h"
#include "qgraphicsscene.h"

AgentRectItem::AgentRectItem(int id, int capacity, map::Cell position, QWidget* parent)
    : agent(id, capacity, position), idLabel(nullptr)
{
    agentColor = assignColorBasedOnId(id);  // Assign color based on the agent's ID
}

QColor AgentRectItem::assignColorBasedOnId(int id)
{
    // Example color assignment based on agent ID
    switch (id) {
    case 0: return QColor(Qt::red);      // Agent 0: Red
    case 1: return QColor(Qt::blue);     // Agent 1: Blue
    case 2: return QColor(Qt::green);    // Agent 2: Green
    case 3: return QColor(Qt::yellow);   // Agent 3: Yellow
    default: return QColor(Qt::cyan);    // Other agents: Cyan
    }
}

void AgentRectItem::draw(QGraphicsScene *scene)
{
    int agentSize = 10;
    QPen pen(agentColor);
    QBrush agentBrush(agentColor);  // Use assigned color

    // Draw agent rectangle
    QRectF rect(agent.getPosition().x * 20 + 5, agent.getPosition().y * 20 + 5, agentSize, agentSize);
    QGraphicsRectItem* rectItem = scene->addRect(rect, pen, agentBrush);  // Add brush to fill color

    // Draw agent ID as text
    if (!idLabel)
    {
        idLabel = new QGraphicsTextItem(QString::number(agent.getId()));
        scene->addItem(idLabel);
    }
    idLabel->setPos(agent.getPosition().x * 20 + 5, agent.getPosition().y * 20 - 10);  // Position the label above the agent
}

void AgentRectItem::moveAgent(QGraphicsScene* scene, int timestep)
{
    std::vector<SpaceTimeCell::Cell> path = agent.getPath();
    if (timestep < path.size())
    {
        agent.move(path[timestep]);
        scene->clear();  // Clear previous drawing
        draw(scene);  // Redraw agent at new position
    }
}
