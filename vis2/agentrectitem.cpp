#include "agentrectitem.h"
#include "qgraphicsscene.h"

AgentRectItem::AgentRectItem(QWidget* parent)
    : idLabel(nullptr)
{
    agentColor = assignRandomColor();  // Assign color based on the agent's ID
}

QColor AgentRectItem::assignRandomColor()
{
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);
    return QColor(r, g, b);
}
void AgentRectItem::drawPickupPoints(QGraphicsScene* scene, const Agent& agent)
{
    for (const auto& pickup : agent.getTask().getPickupPoints())
    {
        QPen pen(agentColor);
        QBrush brush(agentColor);
        QRectF circle(pickup.x * 20 + 5, pickup.y * 20 + 5, 5, 5);
        QGraphicsEllipseItem* circleItem = scene->addEllipse(circle, pen, brush);

        pickupGraphicsItems.push_back(circleItem);
    }
}
void AgentRectItem::drawDropoffPoint(QGraphicsScene* scene, const Agent& agent)
{
    if(dropoffGraphicsItem) return;
    QPen pen(Qt::gray);
    QBrush brush(Qt::gray);
    QRectF rect(agent.getTask().getDropoffLocation().x * 20 + 5, agent.getTask().getDropoffLocation().y * 20 + 5, 10, 10);
    QGraphicsRectItem* rectItem = scene->addRect(rect, pen, brush);
    dropoffGraphicsItem = rectItem;
}

void AgentRectItem::drawStartPoint(QGraphicsScene *scene, const Agent& agent)
{
    QPen pen(agentColor);
    QBrush brush(agentColor);
    QRectF rect(agent.getStartPosition().x * 20, agent.getStartPosition().y * 20, 20, 20);
    QGraphicsEllipseItem* circleItem = scene->addEllipse(rect, pen, brush);

    pickupGraphicsItems.push_back(circleItem);
}
void AgentRectItem::drawAgent(QGraphicsScene *scene, const Agent& agent)
{
    int agentSize = 10;
    QPen pen(agentColor);
    QBrush agentBrush(agentColor);  // Use assigned color

    // Draw agent rectangle
    QRectF rect(agent.getPosition().x * 20 + 5, agent.getPosition().y * 20 + 5, agentSize, agentSize);
    if (agentRectItem) {
        agentRectItem->setRect(rect);  // Update position
    } else {
        agentRectItem = scene->addRect(rect, pen, agentBrush);  // Draw new agent
    }

    // Draw agent ID as text
    if (!idLabel) {
        idLabel = new QGraphicsTextItem(QString::number(agent.getId()));
        scene->addItem(idLabel);
    }
    idLabel->setPos(agent.getPosition().x * 20 + 5, agent.getPosition().y * 20 - 10);  // Position the label above the agent
}

void AgentRectItem::clearPickupDropoff(QGraphicsScene* scene)
{
    for (auto& pickupItem : pickupGraphicsItems)
    {
        if (scene->items().contains(pickupItem))
        {
            scene->removeItem(pickupItem);
            delete pickupItem;
        }
    }
    pickupGraphicsItems.clear();

    if (dropoffGraphicsItem != nullptr) {
        if (scene->items().contains(dropoffGraphicsItem)) {
            scene->removeItem(dropoffGraphicsItem);
            delete dropoffGraphicsItem;   // Delete the dropoff item
            dropoffGraphicsItem = nullptr;  // Set to nullptr after deletion
        }
    }
}

void AgentRectItem::draw(QGraphicsScene* scene, int timestep, const Agent& agent)
{
    std::vector<SpaceTimeCell::Cell> path = agent.getPath();

    // Ensure the agent has a valid path and draw based on the timestep
    if (path.size() > 0 && timestep < path.size() + path.front().t)
    {

        drawAgent(scene, agent);
        drawStartPoint(scene, agent);   // Always draw the start point
        drawPickupPoints(scene, agent); // Draw pickup points
        drawDropoffPoint(scene, agent); // Draw the dropoff point
    }
    else
    {

        clearPickupDropoff(scene);

        drawAgent(scene, agent);
    }
}

