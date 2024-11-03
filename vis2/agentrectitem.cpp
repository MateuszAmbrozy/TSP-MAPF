    #include "agentrectitem.h"
    #include "qgraphicsscene.h"
    #include <QVariantAnimation>

    const int GRID_CELL_SIZE = 20;

    AgentRectItem::AgentRectItem()
        : idLabel(nullptr), dropoffGraphicsItem(nullptr), moveAnimation(nullptr)
    {
        agentColor = assignRandomColor();
    }

    QColor AgentRectItem::assignRandomColor()
    {
        int r = QRandomGenerator::global()->bounded(256);
        int g = QRandomGenerator::global()->bounded(256);
        int b = QRandomGenerator::global()->bounded(256);
        return QColor(r, g, b);
    }

    void AgentRectItem::animateMoveAgent(const QRectF& newPosition)
    {
        if (!agentRectItem) return;

        if (!moveAnimation) {
            moveAnimation = new QVariantAnimation();
            moveAnimation->setDuration(500);
            moveAnimation->setEasingCurve(QEasingCurve::Linear);

            QAbstractAnimation::connect(moveAnimation, &QVariantAnimation::valueChanged, [=](const QVariant &value) {
                agentRectItem->setRect(value.toRectF());

                if (idLabel)
                {
                    idLabel->setPos(value.toRectF().x(), value.toRectF().y() - 10);
                }
            });
        }

        moveAnimation->stop();

        moveAnimation->setStartValue(agentRectItem->rect());
        moveAnimation->setEndValue(newPosition);
        moveAnimation->start();
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
        if (dropoffGraphicsItem) return;
        QPen pen(Qt::gray);
        QBrush brush(Qt::gray);
        QRectF rect(agent.getTask().getDropoffLocation().x * 20 + 5, agent.getTask().getDropoffLocation().y * 20 + 5, 10, 10);
        QGraphicsRectItem* rectItem = scene->addRect(rect, pen, brush);
        dropoffGraphicsItem = rectItem;
    }

    void AgentRectItem::drawStartPoint(QGraphicsScene* scene, const Agent& agent)
    {
        QPen pen(agentColor);
        QBrush brush(agentColor);
        QRectF rect(
            agent.getStartPosition().x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2 - 5,
            agent.getStartPosition().y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2 - 5,
            10, 10
            );
        QGraphicsEllipseItem* circleItem = scene->addEllipse(rect, pen, brush);
        pickupGraphicsItems.push_back(circleItem);
    }

    void AgentRectItem::drawPath(QGraphicsScene* scene, const Agent& agent)
    {
        clearPath(scene);

        QPen pathPen(agentColor, 1, Qt::DashLine);
        std::vector<SpaceTimeCell::Cell> path = agent.getPath();

        for (size_t i = 1; i < path.size(); ++i) {
            const SpaceTimeCell::Cell& from = path[i - 1];
            const SpaceTimeCell::Cell& to = path[i];

            QGraphicsLineItem* line = scene->addLine(from.x * 20 + 10, from.y * 20 + 10,
                                                     to.x * 20 + 10, to.y * 20 + 10, pathPen);
            pathGraphicsItems.push_back(line);
        }
    }

    void AgentRectItem::clearPath(QGraphicsScene* scene)
    {
        for (auto& pathItem : pathGraphicsItems) {
            if (scene->items().contains(pathItem)) {
                scene->removeItem(pathItem);
                delete pathItem;
            }
        }
        pathGraphicsItems.clear();
    }

    void AgentRectItem::clearPickupDropoff(QGraphicsScene* scene)
    {
        for (auto& pickupItem : pickupGraphicsItems) {
            if (scene->items().contains(pickupItem)) {
                scene->removeItem(pickupItem);
                delete pickupItem;
            }
        }
        pickupGraphicsItems.clear();

        if (dropoffGraphicsItem) {
            if (scene->items().contains(dropoffGraphicsItem)) {
                scene->removeItem(dropoffGraphicsItem);
                delete dropoffGraphicsItem;
                dropoffGraphicsItem = nullptr;
            }
        }

        clearPath(scene);
    }

    void AgentRectItem::drawAgent(QGraphicsScene* scene, const Agent& agent)
    {
        int agentSize = 10;
        QPen pen(agentColor);
        QBrush agentBrush(agentColor);

        // Przeliczenie pozycji agenta na piksele zgodnie ze skalą siatki - tylko przy pierwszym rysowaniu
        QPointF initialPosition(
            agent.getPosition().x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2 - agentSize / 2,
            agent.getPosition().y * GRID_CELL_SIZE + GRID_CELL_SIZE / 2 - agentSize / 2
            );

        QRectF rect(initialPosition, QSize(agentSize, agentSize));

        if (!agentRectItem) {
            // Ustawienie `setRect` tylko raz, aby ustalić rozmiar i początkową pozycję
            agentRectItem = scene->addRect(rect, pen, agentBrush);
        } else {
            animateMoveAgent(rect);
        }

        // Ustawienie etykiety ID agenta
        if (!idLabel) {
            idLabel = new QGraphicsTextItem(QString::number(agent.getId()));
            scene->addItem(idLabel);
        }
        idLabel->setPos(rect.x(), rect.y() - 10);
    }


    void AgentRectItem::draw(QGraphicsScene* scene, int timestep, const Agent& agent)
    {
        std::vector<SpaceTimeCell::Cell> path = agent.getPath();

        if (path.size() > 0 && timestep < path.size() + path.front().t)
        {
            drawAgent(scene, agent);
            drawStartPoint(scene, agent);
            drawPickupPoints(scene, agent);
            drawDropoffPoint(scene, agent);
            drawPath(scene, agent);
        }
        else
        {
            clearPickupDropoff(scene);
            drawAgent(scene, agent);
        }
    }
