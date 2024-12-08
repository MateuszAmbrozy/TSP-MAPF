#include "editorview.h"

EditorView::EditorView(QWidget *parent) :
    QWidget(parent),
    sidebarVisible(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebar = new QWidget();

    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/save.svg", tr("Save"), [this]() { saveMap(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/download.svg", tr("Load"), [this]() { loadMap(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/clear.svg", tr("Clear"), [this]() { clearAgents(); }));
    sidebarLayout->setSpacing(0);

    sidebar->setLayout(sidebarLayout);
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumHeight(sidebarLayout->count() * 76);

    Sidebar *layout = new Sidebar();
    layout->addWidget(sidebar, Sidebar::West);


    // Dodanie AgentInputWidget do layoutu
    QGraphicsScene *scene = new QGraphicsScene(this);
    InteractiveAgentRectItem *graphItem = nullptr;

    inputWidget = std::make_shared<AgentInputWidget>(scene, graphItem, this);

    connect(inputWidget.get(), &AgentInputWidget::mapCreated, this,
            [this](std::shared_ptr<map::Graph> map)
            {
                setCreatedGraph(map);
                updateGraphInScene();
            });

    connect(inputWidget.get(), &AgentInputWidget::agentsCreated, this,
            [this](std::shared_ptr<std::vector<Agent>> agents)
            {
                setCreatedAgents(agents);  // Zapisz agentów
            });

    layout->addWidget(inputWidget.get(), Sidebar::Center);  // AgentInputWidget w centralnej części

    QWidget *borderLayoutContainer = new QWidget;
    borderLayoutContainer->setLayout(layout);
    mainLayout->addWidget(borderLayoutContainer);

    setLayout(mainLayout);
    layout->setSpacing(0);

    setWindowTitle(tr("Editor"));
    setGeometry(0, 0, 700, sidebar->minimumHeight());

    sidebarAnimation = new QPropertyAnimation(sidebar, "maximumWidth");
    sidebarAnimation->setDuration(300);
    sidebarAnimation->setStartValue(200);
    sidebarAnimation->setEndValue(0);

    connect(sidebarAnimation, &QPropertyAnimation::finished, this, [this]() {
        sidebar->setVisible(sidebarVisible);
    });

    QPushButton *toggleButton = new QPushButton(this);
    toggleButton->setFixedSize(40, 40);
    toggleButton->setText("☰");
    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");

    mainLayout->addWidget(toggleButton, 0, Qt::AlignLeft | Qt::AlignTop);  // Przycisk w lewym górnym rogu

    connect(toggleButton, &QPushButton::clicked, this, &EditorView::toggleSidebar);

    // Wyświetl domyślny graf w scenie po otwarciu okna
    updateGraphInScene();
}
void EditorView::updateGraphInScene()
{
    if (inputWidget && createdMap)
    {
        QGraphicsScene *scene = new QGraphicsScene(this);

        InteractiveAgentRectItem *graphItem = new InteractiveAgentRectItem(*createdMap, nullptr);
        scene->addItem(graphItem);

        inputWidget->setScene(scene);
    }
}
void EditorView::toggleSidebar()
{
    if (sidebarVisible)
    {
        sidebarAnimation->setStartValue(sidebar->width());
        sidebarAnimation->setEndValue(0);
        sidebarAnimation->setDirection(QPropertyAnimation::Forward);
    }
    else
    {
        sidebar->setVisible(true);
        sidebarAnimation->setStartValue(0);
        sidebarAnimation->setEndValue(200);
        sidebarAnimation->setDirection(QPropertyAnimation::Forward);
    }

    sidebarVisible = !sidebarVisible;  // Zmień stan paska
    sidebarAnimation->start();  // Rozpocznij animację
}

QToolButton *EditorView::createSidebarButton(const QString &iconPath, const QString &title, std::function<void()> onClickFunction)
{
    QIcon icon(iconPath);
    QToolButton *btn = new QToolButton;
    btn->setIcon(icon);
    btn->setIconSize(QSize(42, 42));
    btn->setText(title);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setFixedSize(76, 76);
    btn->setObjectName(title);
    btn->setCheckable(true);

    if (onClickFunction) {
        QObject::connect(btn, &QToolButton::clicked, this, [btn, onClickFunction]() {
            onClickFunction();
            btn->setChecked(false);
        });
    }

    return btn;
}

void EditorView::setCreatedGraph(std::shared_ptr<map::Graph> map)
{
    createdMap = std::move(map);
}

void EditorView::setCreatedAgents(std::shared_ptr<std::vector<Agent>> agents)
{
    this->created_agents = std::move(agents);
}

void EditorView::loadMap()
{
    this->clearAgents();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("Map Files (*.json)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file:" << fileName;
            return;
        }

        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            qWarning() << "Invalid JSON format in file:" << fileName;
            return;
        }

        QJsonObject mapData = jsonDoc.object();

        int width = mapData["width"].toInt();
        int height = mapData["height"].toInt();

        QJsonArray obstaclesArray = mapData["obstacles"].toArray();
        std::vector<QPoint> obstacles;
        for (const QJsonValue &obstacleVal : obstaclesArray) {
            QJsonObject obstacleObj = obstacleVal.toObject();
            int x = obstacleObj["x"].toInt();
            int y = obstacleObj["y"].toInt();
            obstacles.push_back(QPoint(x, y));
        }

        QJsonArray agentsArray = mapData["agents"].toArray();
        std::vector<AgentInputWidget::AgentData> agentsData;
        for (const QJsonValue &agentVal : agentsArray) {
            QJsonObject agentObj = agentVal.toObject();
            int x = agentObj["x"].toInt();
            int y = agentObj["y"].toInt();
            int capacity = agentObj["capacity"].toInt();
            agentsData.push_back({QPoint(x, y), capacity});
        }

        QJsonArray pickupsArray = mapData["avaliablePickups"].toArray();
        std::vector<QPoint> pickupsData;
        for (const QJsonValue &pickupVal : pickupsArray)
        {
            QJsonObject pickupObj = pickupVal.toObject();
            int x = pickupObj["x"].toInt();
            int y = pickupObj["y"].toInt();
            pickupsData.push_back({QPoint(x, y)});
        }

        QJsonArray dropoffArray = mapData["avaliableDropoffs"].toArray();
        std::vector<QPoint> dropoffsData;
        for (const QJsonValue &dropoffVal : dropoffArray) {
            QJsonObject dropoffObj = dropoffVal.toObject();
            int x = dropoffObj["x"].toInt();
            int y = dropoffObj["y"].toInt();
            dropoffsData.push_back({QPoint(x, y)});
        }

        inputWidget->loadMap(width, height, obstacles, agentsData, pickupsData, dropoffsData);

        file.close();
        qDebug() << "Map data loaded from" << fileName;
    }
}

void EditorView::saveMap()
{
    if(!inputWidget)
    {
        qDebug("Could not save empty map\n");
        return;
    }

    // Tworzymy obiekt JSON, który będzie przechowywał dane
    QJsonObject mapData;

    // Dodajemy szerokość i wysokość mapy
    mapData["width"] = inputWidget->width;
    mapData["height"] = inputWidget->height;

    // Tworzymy tablicę JSON dla agentów
    QJsonArray agentsArray;
    for (const auto& agent : inputWidget->agentsData) {
        QJsonObject agentObject;
        agentObject["x"] = agent.point.x();
        agentObject["y"] = agent.point.y();
        agentObject["capacity"] = 15;
        agentsArray.append(agentObject);  // Dodajemy agent do tablicy
    }
    mapData["agents"] = agentsArray;  // Dodajemy tablicę agentów do mapy

    // Tworzymy tablicę JSON dla przeszkód
    QJsonArray obstaclesArray;
    for (const auto& obstacle : inputWidget->obstacles)
    {
        QJsonObject obstacleObject;
        obstacleObject["x"] = obstacle.x();
        obstacleObject["y"] = obstacle.y();
        obstaclesArray.append(obstacleObject);
    }
    mapData["obstacles"] = obstaclesArray;


    // Tworzymy tablicę JSON dla pickupPoints
    QJsonArray avaliablePickupPointsArray;
    for (const auto& pickup : inputWidget->avaliablePickupPoints)
    {
        QJsonObject pickupObject;
        pickupObject["x"] = pickup.x();
        pickupObject["y"] = pickup.y();
        avaliablePickupPointsArray.append(pickupObject);
    }
    mapData["avaliablePickups"] = avaliablePickupPointsArray;

    // Dropoff points
    QJsonArray avaliableDropoffPointsArray;
    for (const auto& dropoff : inputWidget->avaliableDropoffPoints)
    {
        QJsonObject dropoffObject;
        dropoffObject["x"] = dropoff.x();
        dropoffObject["y"] = dropoff.y();
        avaliableDropoffPointsArray.append(dropoffObject);
    }
    mapData["avaliableDropoffs"] = avaliableDropoffPointsArray;


    // Konwersja QJsonObject do QJsonDocument
    QJsonDocument jsonDoc(mapData);

    // Wybieramy miejsce do zapisu pliku
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("Map Files (*.json)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            // Zapisujemy dane do pliku
            file.write(jsonDoc.toJson());
            file.close();
            qDebug() << "Map data saved to" << fileName;
        } else {
            qWarning() << "Failed to open file for writing:" << fileName;
        }
    }
}

void EditorView::clearAgents()
{
    if (inputWidget)
    {
        inputWidget->clear();  // Wywołaj funkcję czyszczenia mapy
        qDebug("Cleared agents and obstacles from the map.");
    }
}
