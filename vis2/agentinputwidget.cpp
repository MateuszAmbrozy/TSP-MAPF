#include "agentinputwidget.h"
#include "qpushbutton.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QListWidgetItem>
#include "agentlistitemwidget.h"

AgentInputWidget::AgentInputWidget(QGraphicsScene *scene, InteractiveAgentRectItem *graphItem, QWidget *parent)
    : QWidget(parent), scene(scene), graphItem(graphItem)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout;

    widthInput = new QSpinBox(this);
    widthInput->setRange(1, 100);
    formLayout->addRow(new QLabel("Width:"), widthInput);

    heightInput = new QSpinBox(this);
    heightInput->setRange(1, 100);
    formLayout->addRow(new QLabel("Height:"), heightInput);

    layout->addLayout(formLayout);

    QPushButton *createMapButton = new QPushButton("Create Map", this);
    connect(createMapButton, &QPushButton::clicked, this, &AgentInputWidget::createMap);
    layout->addWidget(createMapButton);

    graphicsView = new QGraphicsView(this);
    graphicsView->setScene(scene);
    layout->addWidget(graphicsView);

    agentListWidget = new QListWidget(this);
    agentListWidget->setMaximumHeight(110);
    layout->addWidget(agentListWidget);

    updateAgentList();
}
AgentInputWidget::~AgentInputWidget()
{
    if(graphItem) delete graphItem;
}
void AgentInputWidget::clear()
{

    agentsData.clear();
    obstacles.clear();
    avaliableDropoffPoints.clear();
    avaliablePickupPoints.clear();

    if (scene) {
        scene->clear();
    }

    if (agentListWidget) {
        agentListWidget->clear();
    }

    createMap();
}

void AgentInputWidget::loadMap(int width, int height, const std::vector<QPoint>& obstacles,
                               const std::vector<AgentData>& agentsData, const std::vector<QPoint>& pickupsData,
                               const std::vector<QPoint>& dropoffsData)
{
    this->agentsData.clear();
    this->obstacles.clear();
    this->avaliableDropoffPoints.clear();
    this->avaliablePickupPoints.clear();

    if (scene) {
        scene->clear();
    }


    std::shared_ptr<map::Graph> newMap = std::make_shared<map::Graph>(width, height);
    graphItem = new InteractiveAgentRectItem(*newMap, nullptr);  // Create new graph item


    this->width = width;
    this->height = height;
    this->obstacles = obstacles;
    this->agentsData = agentsData;
    this->avaliablePickupPoints = pickupsData;
    this->avaliableDropoffPoints = dropoffsData;



    graphItem->drawGraph(scene);
    scene->addItem(graphItem);


    for (const QPoint& obstacle : obstacles)
    {
        graphItem->obstaclePoints.insert(obstacle);
        graphItem->updateCellColor(obstacle.x(), obstacle.y(), InteractiveAgentRectItem::Option::OBSTACLE);
    }
    for (const auto& agent : agentsData)
    {
        graphItem->agentPoints.insert(agent.point);
        graphItem->updateCellColor(agent.point.x(), agent.point.y(), InteractiveAgentRectItem::Option::AGENT);
    }
    for (const auto& pickup : pickupsData)
    {
        graphItem->avaliablePickupPoints.insert(pickup);
        graphItem->updateCellColor(pickup.x(), pickup.y(), InteractiveAgentRectItem::Option::PICKUP);
    }
    for (const auto& dropoff : dropoffsData)
    {
        graphItem->avaliableDropoffPoints.insert(dropoff);
        graphItem->updateCellColor(dropoff.x(), dropoff.y(), InteractiveAgentRectItem::Option::DROPOFF);
    }
    connect(graphItem, &InteractiveAgentRectItem::agentSelected, this, &AgentInputWidget::addAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleSelected, this, &AgentInputWidget::addObstacle);
    connect(graphItem, &InteractiveAgentRectItem::agentRemoved, this, &AgentInputWidget::removeAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleRemoved, this, &AgentInputWidget::removeObstacle);

    connect(graphItem, &InteractiveAgentRectItem::pickupSelected, this, &AgentInputWidget::addPickupPoint);
    connect(graphItem, &InteractiveAgentRectItem::dropoffSelected, this, &AgentInputWidget::addDropoffPoint);
    connect(graphItem, &InteractiveAgentRectItem::pickupRemoved, this, &AgentInputWidget::removePickupPoint);
    connect(graphItem, &InteractiveAgentRectItem::dropoffRemoved, this, &AgentInputWidget::removeDropoffPoint);

    graphicsView->setScene(scene);
    updateAgentList();
}

void AgentInputWidget::setScene(QGraphicsScene *newScene)
{
    scene = newScene;
    graphicsView->setScene(scene);

}
void AgentInputWidget::updateAgentList()
{
    agentListWidget->clear();

    for (size_t i = 0; i < agentsData.size(); ++i)
    {
        const auto &agentData = agentsData[i];


        auto *itemWidget = new AgentListItemWidget("Agent", agentData.point, agentData.capacity, this);


        connect(itemWidget, &AgentListItemWidget::capacityChanged, this, [this, i](const QPoint &, int newCapacity) {
            agentsData[i].capacity = newCapacity;
        });

        QListWidgetItem *item = new QListWidgetItem(agentListWidget);
        item->setSizeHint(itemWidget->sizeHint());
        agentListWidget->setItemWidget(item, itemWidget);
    }
}
void AgentInputWidget::addAgent(const QPoint &point)
{
    agentsData.push_back({point, 0});

    updateMap();
    updateAgentList();
}

void AgentInputWidget::addObstacle(const QPoint &point)
{
    obstacles.push_back(point);
    updateMap();
}
void AgentInputWidget::addPickupPoint(const QPoint &point)
{
    avaliablePickupPoints.push_back(point);
    updateMap();
}
void AgentInputWidget::addDropoffPoint(const QPoint &point)
{
    avaliableDropoffPoints.push_back(point);
    updateMap();
}
void AgentInputWidget::removeAgent(const QPoint &point)
{
    auto it = std::remove_if(agentsData.begin(), agentsData.end(), [&point](const AgentData &data) {
        return data.point == point;
    });
    if (it != agentsData.end()) {
        agentsData.erase(it);
        updateMap();
        updateAgentList();
    }
}

void AgentInputWidget::removeObstacle(const QPoint &point)
{
    auto it = std::remove_if(obstacles.begin(), obstacles.end(), [&point](const QPoint &cell) {
        return cell == point;
    });
    if (it != obstacles.end()) {
        obstacles.erase(it);
        updateMap();
    }
}
void AgentInputWidget::removePickupPoint(const QPoint &point)
{
    auto it = std::remove_if(avaliablePickupPoints.begin(), avaliablePickupPoints.end(), [&point](const QPoint &cell) {
        return cell == point;
    });
    if (it != avaliablePickupPoints.end()) {
        avaliablePickupPoints.erase(it);
        updateMap();
    }
}
void AgentInputWidget::removeDropoffPoint(const QPoint &point)
{
    auto it = std::remove_if(avaliableDropoffPoints.begin(), avaliableDropoffPoints.end(), [&point](const QPoint &cell) {
        return cell == point;
    });
    if (it != avaliableDropoffPoints.end()) {
        avaliableDropoffPoints.erase(it);
        updateMap();
    }
}
void AgentInputWidget::updateMap()
{
}

void AgentInputWidget::createMap()
{
    width = widthInput->value();
    height = heightInput->value();

    agentsData.clear();
    obstacles.clear();
    //graphItem->clear();


    std::shared_ptr<map::Graph> newMap = std::make_shared<map::Graph>(width, height);
    emit mapCreated(newMap);


    scene->clear();
    graphItem = new InteractiveAgentRectItem(*newMap);
    graphItem->drawGraph(scene);
    scene->addItem(graphItem);


    connect(graphItem, &InteractiveAgentRectItem::agentSelected, this, &AgentInputWidget::addAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleSelected, this, &AgentInputWidget::addObstacle);
    connect(graphItem, &InteractiveAgentRectItem::agentRemoved, this, &AgentInputWidget::removeAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleRemoved, this, &AgentInputWidget::removeObstacle);

    connect(graphItem, &InteractiveAgentRectItem::pickupSelected, this, &AgentInputWidget::addPickupPoint);
    connect(graphItem, &InteractiveAgentRectItem::dropoffSelected, this, &AgentInputWidget::addDropoffPoint);
    connect(graphItem, &InteractiveAgentRectItem::pickupRemoved, this, &AgentInputWidget::removePickupPoint);
    connect(graphItem, &InteractiveAgentRectItem::dropoffRemoved, this, &AgentInputWidget::removeDropoffPoint);

    graphicsView->setScene(scene);
    updateAgentList();
 }

