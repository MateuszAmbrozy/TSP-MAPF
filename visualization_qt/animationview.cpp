#include "animationview.h"
#include "WCA_Environment.h"
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

    sidebarAnimation = new QPropertyAnimation(sidebar, "maximumWidth");
    sidebarAnimation->setDuration(300);
    sidebarAnimation->setStartValue(200);
    sidebarAnimation->setEndValue(0);

    connect(sidebarAnimation, &QPropertyAnimation::finished, this, [this]() {
        sidebar->setVisible(sidebarVisible);
    });

    QPushButton *toggleButton = new QPushButton();
    toggleButton->setFixedSize(40, 40);
    toggleButton->setText("☰");
    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");
    connect(toggleButton, &QPushButton::clicked, this, &AnimationView::toggleSidebar);

    QWidget *sidebarContainer = new QWidget(this);
    QVBoxLayout *sidebarContainerLayout = new QVBoxLayout(sidebarContainer);
    sidebarContainerLayout->addWidget(sidebar);
    sidebarContainerLayout->addWidget(toggleButton, 0, Qt::AlignLeft | Qt::AlignBottom);
    sidebarContainerLayout->setSpacing(5);
    sidebarContainerLayout->setContentsMargins(0, 0, 0, 0);

    aniamtionSpeedSlider = new QSlider();
    aniamtionSpeedSlider->setOrientation(Qt::Horizontal);
    aniamtionSpeedSlider->setTickInterval(5);
    aniamtionSpeedSlider->setRange(1, 100);
    aniamtionSpeedSlider->setFixedSize(150, 20);

    QLabel* animationSpeedLabel = new QLabel();
    animationSpeedLabel->setText("Animation Speed");

    QVBoxLayout *mapLayout = new QVBoxLayout();
    mapLayout->addWidget(animationSpeedLabel, 0, Qt::AlignCenter);
    mapLayout->addWidget(aniamtionSpeedSlider, 0, Qt::AlignCenter);
    mapfView = new QGraphicsView(mapfScene, this);
    mapLayout->addWidget(mapfView);

    mainLayout->addWidget(sidebarContainer, 0, Qt::AlignLeft);
    mainLayout->addLayout(mapLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Animation"));
    setGeometry(0, 0, 700, sidebar->minimumHeight());

    connect(aniamtionSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(onChange()));
    connect(timer, &QTimer::timeout, this, &AnimationView::updateTimestep);
}


AnimationView::~AnimationView() {
    closeTaskWindow();
    delete timer;
    if(mapfScene)
        delete mapfScene;
    if(mapfView)
        delete mapfView;

    delete environment;
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
            btn->setChecked(false);
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
    currentTaskGroup = std::move(taskGroup);
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


void AnimationView::closeTaskWindow()
{
    if (!taskGroupWindow) return;
    if (interactive_graph && interactive_graph->scene()) {
        interactive_graph->scene()->removeItem(interactive_graph.get());
    }
    delete taskGroupWindow;
    taskGroupWindow = nullptr;
}

void AnimationView::showTaskGroupScene()
{
    if(interactive_graph == nullptr)
    {
        qDebug("Interactive_graph == nullptr\n");
        return;
    }
    if (taskGroupWindow) {
        taskGroupWindow->show();
        taskGroupWindow->raise();
        return;
    }
    taskGroupWindow = new QWidget(this, Qt::Window);
    taskGroupScene = new QGraphicsScene(taskGroupWindow);


    inputWidget = new TaskInputWidget(taskGroupScene, interactive_graph.get());

    connect(inputWidget, &TaskInputWidget::taskGroupCreated, this,
            [this](std::shared_ptr<TaskGroup> taskGroup) {
                this->setTaskGroup(std::move(taskGroup));
            });

    taskGroupView = new QGraphicsView(taskGroupScene);

    taskGroupScene->addItem(interactive_graph.get());
    interactive_graph->setPos(100, 300);

    interactive_graph->setFocus();
    taskGroupView->setFocusPolicy(Qt::StrongFocus);

    taskGroupView->setMinimumSize(400, 400);

    QVBoxLayout *taskGroupLayout = new QVBoxLayout();
    taskGroupLayout->addWidget(inputWidget);
    taskGroupLayout->addWidget(taskGroupView);

    taskGroupWindow->setLayout(taskGroupLayout);
    taskGroupWindow->setWindowTitle("Task Group");

    connect(QApplication::instance(), &QApplication::aboutToQuit, taskGroupWindow, &QWidget::close);



    taskGroupWindow->show();
}

void AnimationView::loadMap()
{



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

        int width = mapData["width"].toInt();
        int height = mapData["height"].toInt();

        QJsonArray obstaclesArray = mapData["obstacles"].toArray();
        std::vector<map::Cell> obstacles;
        for (const QJsonValue &obstacleVal : obstaclesArray)
        {
            QJsonObject obstacleObj = obstacleVal.toObject();
            int x = obstacleObj["x"].toInt();
            int y = obstacleObj["y"].toInt();
            obstacles.push_back(map::Cell(x, y));
        }

        agents.clear();
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
        timer->stop();
        closeTaskWindow();
        timestep = 0;
        currentTaskGroup.reset();
        delete mapfScene;
        mapfScene = nullptr;
        delete environment;
        environment = nullptr;
        if (algorithm == AlgType::A_STAR)
        {
            environment = new CA_Environment(agents, graph, avaliablePickups, avaliableDropoffs);
        }
        else
        {
            environment = new WCA_Environment(agents, graph, avaliablePickups, avaliableDropoffs);
        }
        environment->assignVacantAgents();

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


void AnimationView::stop()
{
    timer->stop();
}

void AnimationView::updateTimestep()
{
    if (!environment || !mapfScene) return;
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
