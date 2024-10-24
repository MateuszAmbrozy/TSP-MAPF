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

    // Tworzymy QGraphicsView do wyświetlania sceny
    graphicsView = new QGraphicsView(this);
    graphicsView->setScene(scene);  // Ustawiamy początkową scenę
    layout->addWidget(graphicsView);  // Dodajemy widok do layoutu

    agentListWidget = new QListWidget(this);
    agentListWidget->setMaximumHeight(50);
    layout->addWidget(agentListWidget);

    updateAgentList();
}
void AgentInputWidget::clear()
{
    // 1. Usuń wszystkich agentów i przeszkody
    agentsData.clear();   // Czyścimy wektor przechowujący agentów i ich pojemności
    obstacles.clear();    // Czyścimy wektor przechowujący przeszkody

    // 2. Wyczyść scenę graficzną
    if (scene) {
        scene->clear();  // Usunięcie wszystkich elementów ze sceny graficznej
    }

    // 3. Zaktualizuj listę agentów
    if (agentListWidget) {
        agentListWidget->clear();  // Wyczyść listę widżetów dla agentów
    }

    // 4. Przywróć pierwotny stan grafu (opcjonalne dodanie pustej mapy)
    // Możesz opcjonalnie przywrócić początkową szerokość/wysokość mapy i utworzyć pustą mapę
    createMap();  // Przywrócenie pustego grafu
}

void AgentInputWidget::loadMap(int width, int height, const std::vector<QPoint>& obstacles, const std::vector<AgentData>& agentsData)
{
    // Clear existing agents and obstacles
    this->agentsData.clear();
    this->obstacles.clear();

    if (scene) {
        scene->clear();  // Usunięcie wszystkich elementów ze sceny graficznej
    }
    //graphItem->clear();

    // Create a new map based on the given dimensions
    std::shared_ptr<map::Graph> newMap = std::make_shared<map::Graph>(width, height);
    graphItem = new InteractiveAgentRectItem(*newMap, nullptr);  // Create new graph item

    // Add obstacles to the new map
    this->width = width;
    this->height = height;
    this->obstacles = obstacles;
    this->agentsData = agentsData;



    graphItem->drawGraph(scene);
    scene->addItem(graphItem);
    for (const QPoint& obstacle : obstacles) {
        graphItem->obstaclePoints.insert(obstacle);  // Update internal set
        graphItem->updateCellColor(obstacle.x(), obstacle.y(), InteractiveAgentRectItem::cellState::OBSTACLE);
    }

    // Add agents to the graph item
    for (const auto& agent : agentsData) {
        graphItem->agentPoints.insert(agent.point);  // Update internal set
        graphItem->updateCellColor(agent.point.x(), agent.point.y(), InteractiveAgentRectItem::cellState::AGENT);
    }
    connect(graphItem, &InteractiveAgentRectItem::agentSelected, this, &AgentInputWidget::addAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleSelected, this, &AgentInputWidget::addObstacle);
    connect(graphItem, &InteractiveAgentRectItem::agentRemoved, this, &AgentInputWidget::removeAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleRemoved, this, &AgentInputWidget::removeObstacle);


    graphicsView->setScene(scene);  // Ensure the graphics view shows the updated scene
    updateAgentList();
}
// Metoda do aktualizacji sceny w QGraphicsView
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

        // Tworzymy widżet dla agenta z pozycją i pojemnością
        auto *itemWidget = new AgentListItemWidget("Agent", agentData.point, agentData.capacity, this);

        // Po zmianie pojemności, aktualizujemy dane w `agentsData`
        connect(itemWidget, &AgentListItemWidget::capacityChanged, this, [this, i](const QPoint &, int newCapacity) {
            agentsData[i].capacity = newCapacity;  // Aktualizacja pojemności agenta
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

void AgentInputWidget::updateMap()
{
    // Placeholder function to update the UI or other components when the map changes.
}

void AgentInputWidget::createMap()
{
    width = widthInput->value();
    height = heightInput->value();

    agentsData.clear();
    obstacles.clear();
    //graphItem->clear();  // Clear current graph item before creating a new one


    std::shared_ptr<map::Graph> newMap = std::make_shared<map::Graph>(width, height);
    emit mapCreated(newMap);  // Emit the signal with the new map


    scene->clear();
    graphItem = new InteractiveAgentRectItem(*newMap);
    graphItem->drawGraph(scene);
    scene->addItem(graphItem);


    connect(graphItem, &InteractiveAgentRectItem::agentSelected, this, &AgentInputWidget::addAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleSelected, this, &AgentInputWidget::addObstacle);
    connect(graphItem, &InteractiveAgentRectItem::agentRemoved, this, &AgentInputWidget::removeAgent);
    connect(graphItem, &InteractiveAgentRectItem::obstacleRemoved, this, &AgentInputWidget::removeObstacle);

    graphicsView->setScene(scene);
    updateAgentList();
 }

