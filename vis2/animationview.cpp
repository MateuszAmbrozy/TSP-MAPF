#include "animationview.h"
#include "qtoolbutton.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
AnimationView::AnimationView(QWidget *parent)
    : QWidget(parent), inputWidget(nullptr), timestep(0), sidebarVisible(true)
{
    mainLayout = new QHBoxLayout(this);

    // Create sidebar layout and add buttons
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebar = new QWidget(this);

    timer = new QTimer(this);

    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/visa.svg", "Start", [this]() { start(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/amex.svg", "Stop", [this]() { stop(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/add.svg", "Add Task", [this]() { showTaskGroupScene(); }));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/cloud.svg", "Upload map", [this]() { loadMap(); }));
    //sidebarLayout->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
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

    // Create the toggle button
    QPushButton *toggleButton = new QPushButton();
    toggleButton->setFixedSize(40, 40);
    toggleButton->setText("☰");
    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");
    connect(toggleButton, &QPushButton::clicked, this, &AnimationView::toggleSidebar);

    // Container for sidebar and toggle button
    QWidget *sidebarContainer = new QWidget(this);
    QVBoxLayout *sidebarContainerLayout = new QVBoxLayout(sidebarContainer);

    sidebarContainerLayout->addWidget(sidebar);
    //sidebarContainerLayout->addSpacerItem(new QSpacerItem(0, 50, QSizePolicy::Minimum, QSizePolicy::Expanding));
    sidebarContainerLayout->addWidget(toggleButton,0, Qt::AlignLeft | Qt::AlignBottom);

    sidebarContainerLayout->setSpacing(5);
    sidebarContainerLayout->setContentsMargins(0, 0, 0, 0);

    // Add sidebarContainer to main layout on the left
    mainLayout->addWidget(sidebarContainer, 0, Qt::AlignLeft);
    //mainLayout->addWidget(mapfView);
    setLayout(mainLayout);
    setWindowTitle(tr("Animation"));
    setGeometry(0, 0, 700, sidebar->minimumHeight());
    connect(timer, &QTimer::timeout, this, &AnimationView::updateTimestep);
}


AnimationView::~AnimationView() {
    delete timer;
    if(mapfScene)
        delete mapfScene;
    if(mapfView)
        delete mapfView;
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
        QObject::connect(btn, &QToolButton::clicked, this, [onClickFunction]() {
            onClickFunction();
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
void AnimationView::initEnvironment()
{

}

void AnimationView::setTaskGroup(std::shared_ptr<TaskGroup> taskGroup)
{
    currentTaskGroup = std::move(taskGroup);  // Transfer ownership using std::move
}
void AnimationView::start()
{
    timer->start(700);
}


void AnimationView::showTaskGroupScene()
{
    taskGroupScene = new QGraphicsScene();
    if(interactive_graph != nullptr)
    {

        qDebug("Interactive_graph != nullptr\n");
    }

    inputWidget = new TaskInputWidget(taskGroupScene, interactive_graph.get());

    connect(inputWidget, &TaskInputWidget::taskGroupCreated, this,
            [this](std::shared_ptr<TaskGroup> taskGroup) {
                this->setTaskGroup(std::move(taskGroup));  // Transfer ownership using std::move
            });

    // Create the taskGroupView for visualizing the TaskGroupScene
    taskGroupView = new QGraphicsView(taskGroupScene);

    // Add interactive_graph to the scene
    taskGroupScene->addItem(interactive_graph.get());
    interactive_graph->setPos(100, 300);

    // Make sure the interactive_graph has focus
    interactive_graph->setFocus();
    taskGroupView->setFocusPolicy(Qt::StrongFocus);
    // Set view properties
    taskGroupView->setMinimumSize(400, 400);
    taskGroupView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    taskGroupView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *taskGroupLayout = new QVBoxLayout();
    taskGroupLayout->addWidget(inputWidget);  // Add input widget to layout
    taskGroupLayout->addWidget(taskGroupView); // Add task group view to layout

    // Create a new window for task group management
    QWidget *taskGroupWindow = new QWidget();
    taskGroupWindow->setLayout(taskGroupLayout);
    taskGroupWindow->setWindowTitle("Task Group");

    // Show the newly created window
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

        for (int i=0; i<agentsArray.size(); i++)
        {
            QJsonObject agentObj = agentsArray[i].toObject();
            int x = agentObj["x"].toInt();
            int y = agentObj["y"].toInt();
            int capacity = agentObj["capacity"].toInt();
            agents.push_back({i, capacity, {x, y}});
        }


        file.close();
        qDebug() << "Map data loaded from" << fileName;

        graph = map::Graph(width, height, obstacles);


        //delete e;
        //if(e) delete e;
        e = new Environment(agents, graph);

        e->assignVacanAgents();
        interactive_graph = std::make_unique<InteractiveTaskRectItem>(graph);


        //if(mapfScene) delete mapfScene;
        if(mapfScene) mapfScene->clear();
        mapfScene = new MapfScene(e);

        //if(mapfView) delete mapfView;

        mapfView = new QGraphicsView(mapfScene, this);

        mainLayout->addWidget(mapfView);

    }
}
void AnimationView::stop()
{
    timer->stop();
}

void AnimationView::updateTimestep()
{
    if (currentTaskGroup) {
        e->runTimestep(timestep, currentTaskGroup.get());
        currentTaskGroup.reset();
    } else {
        e->runTimestep(timestep);
    }
    mapfScene->updateScene(timestep);
    timestep++;
}
