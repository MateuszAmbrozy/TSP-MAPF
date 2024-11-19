#include "animationview.h"
#include "WHCA_Environment.h"
#include "qapplication.h"
#include "qtoolbutton.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QLabel>
AnimationView::AnimationView(AlgType algorithm, QWidget *parent)
    : QWidget(parent),
    inputWidget(nullptr),
    timestep(0),
    animationSpeed(500),
    algorithm(algorithm),
    sidebarVisible(true)
{
    mapfScene = nullptr;
    mapfView = nullptr;
    mainLayout = new QHBoxLayout(this);

    // Sidebar setup
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebar = new QWidget(this);

    timer = new QTimer(this);

    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/play.svg", "Start", [this]() { start(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/pause.svg", "Stop", [this]() { stop(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/add.svg", "Add Task", [this]() { showTaskGroupScene(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/download.svg", "Upload map", [this]() { loadMap(); }));
    sidebar->setLayout(sidebarLayout);
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumHeight(sidebarLayout->count() * 76);

    // Sidebar animation setup
    sidebarAnimation = new QPropertyAnimation(sidebar, "maximumWidth");
    sidebarAnimation->setDuration(300);
    sidebarAnimation->setStartValue(200);
    sidebarAnimation->setEndValue(0);

    connect(sidebarAnimation, &QPropertyAnimation::finished, this, [this]() {
        sidebar->setVisible(sidebarVisible);
    });

    // Toggle button for sidebar
    QPushButton *toggleButton = new QPushButton();
    toggleButton->setFixedSize(40, 40);
    toggleButton->setText("☰");
    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");
    connect(toggleButton, &QPushButton::clicked, this, &AnimationView::toggleSidebar);

    // Sidebar container
    QWidget *sidebarContainer = new QWidget(this);
    QVBoxLayout *sidebarContainerLayout = new QVBoxLayout(sidebarContainer);
    sidebarContainerLayout->addWidget(sidebar);
    sidebarContainerLayout->addWidget(toggleButton, 0, Qt::AlignLeft | Qt::AlignBottom);
    sidebarContainerLayout->setSpacing(5);
    sidebarContainerLayout->setContentsMargins(0, 0, 0, 0);

    // Slider setup
    aniamtionSpeedSlider = new QSlider();
    aniamtionSpeedSlider->setOrientation(Qt::Horizontal);
    aniamtionSpeedSlider->setTickInterval(5);
    aniamtionSpeedSlider->setRange(1, 100);
    aniamtionSpeedSlider->setFixedSize(150, 20);

    QLabel* animationSpeedLabel = new QLabel();
    animationSpeedLabel->setText("Animation Speed");

    // Pionowy layout dla slidera i mapfView
    QVBoxLayout *mapLayout = new QVBoxLayout();
    mapLayout->addWidget(animationSpeedLabel, 0, Qt::AlignCenter);
    mapLayout->addWidget(aniamtionSpeedSlider, 0, Qt::AlignCenter);
    mapfView = new QGraphicsView(mapfScene, this);
    mapLayout->addWidget(mapfView);

    // Dodaj sidebar i map layout do mainLayout
    mainLayout->addWidget(sidebarContainer, 0, Qt::AlignLeft);
    mainLayout->addLayout(mapLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Animation"));
    setGeometry(0, 0, 700, sidebar->minimumHeight());

    // Connect slider changes to animation speed adjustment
    connect(aniamtionSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(onChange()));
    connect(timer, &QTimer::timeout, this, &AnimationView::updateTimestep);
}


AnimationView::~AnimationView() {
    delete timer;
    if(mapfScene)
        delete mapfScene;
    if(mapfView)
        delete mapfView;

    // // Close and delete any additional windows opened during the session
    // for (QWidget* window : additionalWindows) {
    //     if (window) {
    //         window->close();  // Close the window
    //         delete window;     // Delete the window object
    //     }
    // }
}

void AnimationView::setAlgorithm(AlgType algorithm)
{
    this->algorithm = algorithm;
}

QToolButton *AnimationView::createSidebarButton(const QString &iconPath, const QString &title, std::function<void()> onClickFunction)
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
        QObject::connect(btn, &QToolButton::clicked, this, [onClickFunction, btn]() {
            onClickFunction();
            btn->setChecked(false);  // Uncheck after action is performed
        });
    }

    return btn;
}


void AnimationView::toggleSidebar()
{
    if (sidebarVisible) {
        sidebarAnimation->setStartValue(sidebar->width());
        sidebarAnimation->setEndValue(0);
    } else {
        sidebar->setVisible(true);
        sidebarAnimation->setStartValue(0);
        sidebarAnimation->setEndValue(200);
    }
    sidebarAnimation->start();
    sidebarVisible = !sidebarVisible;
}

void AnimationView::onChange()
{
    int sliderValue = aniamtionSpeedSlider->value();
    animationSpeed = 1000 - sliderValue * 9;

    timer->setInterval(animationSpeed);

    if (mapfScene) {
        mapfScene->updateAgentAnimationSpeed(animationSpeed);
    }
}

void AnimationView::setTaskGroup(std::shared_ptr<TaskGroup> taskGroup)
{
    currentTaskGroup = std::move(taskGroup);  // Transfer ownership using std::move
}
void AnimationView::start()
{
    if(interactive_graph == nullptr)
    {
        qDebug("Interactive_graph == nullptr\n");
        return;
    }
    timer->start(animationSpeed);
}


void AnimationView::showTaskGroupScene()
{
    if(interactive_graph == nullptr)
    {
        qDebug("Interactive_graph == nullptr\n");
        return;
    }
    taskGroupScene = new QGraphicsScene();


    inputWidget = new TaskInputWidget(taskGroupScene, interactive_graph.get());

    connect(inputWidget, &TaskInputWidget::taskGroupCreated, this,
            [this](std::shared_ptr<TaskGroup> taskGroup) {
                this->setTaskGroup(std::move(taskGroup));  // Transfer ownership using std::move
            });

    taskGroupView = new QGraphicsView(taskGroupScene);

    taskGroupScene->addItem(interactive_graph.get());
    interactive_graph->setPos(100, 300);

    interactive_graph->setFocus();
    taskGroupView->setFocusPolicy(Qt::StrongFocus);

    taskGroupView->setMinimumSize(400, 400);
    // taskGroupView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // taskGroupView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *taskGroupLayout = new QVBoxLayout();
    taskGroupLayout->addWidget(inputWidget);  // Add input widget to layout
    taskGroupLayout->addWidget(taskGroupView); // Add task group view to layout

    // Create a new window for task group management
    QWidget *taskGroupWindow = new QWidget();
    taskGroupWindow->setLayout(taskGroupLayout);
    taskGroupWindow->setWindowTitle("Task Group");

    taskGroupWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(QApplication::instance(), &QApplication::aboutToQuit, taskGroupWindow, &QWidget::close);


    //additionalWindows.push_back(taskGroupWindow);

    taskGroupWindow->show();
}

void AnimationView::loadMap()
{
    agents.clear();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("Map Files (*.json)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file:" << fileName;
            return;
        }

        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull() || !jsonDoc.isObject())
        {
            qWarning() << "Invalid JSON format in file:" << fileName;
            return;
        }

        QJsonObject mapData = jsonDoc.object();

        // Extract map dimensions
        int width = mapData["width"].toInt();
        int height = mapData["height"].toInt();

        // Extract obstacles as a vector of QPoint
        QJsonArray obstaclesArray = mapData["obstacles"].toArray();
        std::vector<map::Cell> obstacles;
        for (const QJsonValue &obstacleVal : obstaclesArray)
        {
            QJsonObject obstacleObj = obstacleVal.toObject();
            int x = obstacleObj["x"].toInt();
            int y = obstacleObj["y"].toInt();
            obstacles.push_back(map::Cell(x, y));
        }

        // Extract agents as a vector of AgentData
        QJsonArray agentsArray = mapData["agents"].toArray();

        for (int i = 0; i < agentsArray.size(); i++)
        {
            QJsonObject agentObj = agentsArray[i].toObject();
            int x = agentObj["x"].toInt();
            int y = agentObj["y"].toInt();
            int capacity = agentObj["capacity"].toInt();
            agents.push_back({i, capacity, {x, y}});
        }

        QJsonArray pickupsArray = mapData["avaliablePickups"].toArray();
        std::vector<std::pair<int, int>> avaliablePickups;
        for (const QJsonValue &pickupVal : pickupsArray)
        {
            QJsonObject pickupObj = pickupVal.toObject();
            int x = pickupObj["x"].toInt();
            int y = pickupObj["y"].toInt();
            avaliablePickups.emplace_back(x, y);
        }

        // Extract available dropoff points as std::vector<std::pair<int, int>>
        QJsonArray dropoffsArray = mapData["avaliableDropoffs"].toArray();
        std::vector<std::pair<int, int>> avaliableDropoffs;
        for (const QJsonValue &dropoffVal : dropoffsArray)
        {
            QJsonObject dropoffObj = dropoffVal.toObject();
            int x = dropoffObj["x"].toInt();
            int y = dropoffObj["y"].toInt();
            avaliableDropoffs.emplace_back(x, y);
        }

        file.close();
        qDebug() << "Map data loaded from" << fileName;

        graph = map::Graph(width, height, obstacles);

        // Initialize the environment
        if (algorithm == AlgType::A_STAR)
        {
            environment = new CA_Environment(agents, graph, avaliablePickups, avaliableDropoffs);
        }
        else
        {
            environment = new WHCA_Environment(agents, graph, avaliablePickups, avaliableDropoffs);
        }

        // const int seed = 42;  // Using a fixed seed value
        // srand(seed);

        // Define available positions for pickup and dropoff points
        // std::vector<std::pair<int, int>> availablePickupX
        // std::vector<std::pair<int, int>> availablePickupY
        // std::vector<std::pair<int, int>> availableDropofX
        // std::vector<std::pair<int, int>> availableDropofY
        // std::vector<TaskGroup> tasks;
        // for (int i = 0; i < 10; ++i)
        // {
        //     TaskGroup task = environment->TASKGROUPGENERATOR(availablePickupX, availablePickupY, availableDropofX, availableDropofY);
        //     tasks.push_back(task);
        // }

        // // CA_Environment
        // environment->assignTasks(tasks);
        environment->assignVacantAgents();

        // Reset mapfScene and update mapfView's scene
        if (mapfScene)
        {
            delete mapfScene;
            mapfScene = nullptr;
        }
        mapfScene = new MapfScene(environment);
        interactive_graph = std::make_unique<InteractiveTaskRectItem>(graph, agents);

        mapfView->setScene(mapfScene);
    }
}


// void AnimationView::loadTasks()
// {
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly))
//     {
//         qWarning() << "Could not open file:" << fileName;
//         return;
//     }

//     QByteArray jsonData = file.readAll();
//     QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
//     if (jsonDoc.isNull() || !jsonDoc.isObject())
//     {
//         qWarning() << "Invalid JSON format in file:" << fileName;
//         return;
//     }

//     QJsonObject mapData = jsonDoc.object();

//     // Extract map dimensions
//     int width = mapData["width"].toInt();
//     int height = mapData["height"].toInt();

//     // Extract obstacles as a vector of QPoint
//     QJsonArray obstaclesArray = mapData["obstacles"].toArray();
//     std::vector<map::Cell> obstacles;
//     for (const QJsonValue &obstacleVal : obstaclesArray)
//     {
//         QJsonObject obstacleObj = obstacleVal.toObject();
//         int x = obstacleObj["x"].toInt();
//         int y = obstacleObj["y"].toInt();
//         obstacles.push_back(map::Cell(x, y));
//     }

//     // Extract agents as a vector of AgentData
//     QJsonArray agentsArray = mapData["agents"].toArray();

//     for (int i=0; i<agentsArray.size(); i++)
//     {
//         QJsonObject agentObj = agentsArray[i].toObject();
//         int x = agentObj["x"].toInt();
//         int y = agentObj["y"].toInt();
//         int capacity = agentObj["capacity"].toInt();
//         agents.push_back({i, capacity, {x, y}});
//     }


//     file.close();
//     qDebug() << "Map data loaded from" << fileName;

//     graph = map::Graph(width, height, obstacles);


//     //delete e;
//     //if(e) delete e;
//     e = new Environment(agents, graph);

//     e->assignVacanAgents();
//     interactive_graph = std::make_unique<InteractiveTaskRectItem>(graph);


//     //if(mapfScene) delete mapfScene;
//     if(mapfScene) delete mapfScene;
//     mapfScene = new MapfScene(e);

//     if(mapfView) delete mapfView;
//     mapfView = new QGraphicsView(mapfScene, this);

//     mainLayout->addWidget(mapfView);

// }

void AnimationView::stop()
{
    timer->stop();
}

void AnimationView::updateTimestep()
{
    if (currentTaskGroup)
    {
        environment->runTimestep(timestep, currentTaskGroup.get());
        currentTaskGroup.reset();
    } else
    {
        environment->runTimestep(timestep);
    }
    mapfScene->updateScene(timestep);
    timestep++;
}
