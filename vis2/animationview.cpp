#include "animationview.h"


AnimationView::AnimationView(map::Graph graph, QWidget *parent)
    : QWidget(parent), inputWidget(nullptr), timestep(0), graph(graph)
{
    this->initEnvironment();
    this->interactive_graph = std::make_unique<InteractiveTaskRectItem>(graph);
    timer = new QTimer(this);

    // Tworzymy layout dla przycisków
    topLayout = new QHBoxLayout();
    mainLayout = new QVBoxLayout(this);  // Używamy mainLayout jako główny layout

    startAnimation = new QPushButton("Start Animation", this);
    stopAnimation = new QPushButton("Stop Animation", this);
    taskGroupBtn = new QPushButton("Add task", this);


    topLayout->addWidget(startAnimation);
    topLayout->addWidget(stopAnimation);
    topLayout->addWidget(taskGroupBtn);

    mainLayout->addLayout(topLayout);
    mainLayout->setAlignment(topLayout, Qt::AlignTop);

    mainLayout->addWidget(mapfView);

    mainLayout->addStretch();

    setLayout(mainLayout);


    connect(timer, &QTimer::timeout, this, &AnimationView::updateTimestep);
    connect(startAnimation, &QPushButton::clicked, this, &AnimationView::start);
    connect(stopAnimation, &QPushButton::clicked, this, &AnimationView::stop);
    connect(taskGroupBtn,  &QPushButton::clicked, this, &AnimationView::showTaskGroupScene);

}

AnimationView::~AnimationView() {
    delete timer;
    delete mapfScene;
    delete mapfView;
}

void AnimationView::initEnvironment()
{
    e = new Environment(agents, graph);
    e->addAgent(Agent(0, 1, {0, 0}));
    e->addAgent(Agent(1, 10, {9, 9}));
    e->addAgent(Agent(3, 1, {0, 9}));
    e->addAgent(Agent(4, 1, {1, 9}));
    e->addAgent(Agent(5, 1, {9, 3}));

    e->assignVacanAgents();

    mapfScene = new MapfScene(e);
    mapfView = new QGraphicsView(mapfScene, this);

    mapfView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mapfView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mapfView->setMinimumSize(400, 400);
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
void AnimationView::stop()
{
    timer->stop();
}

void AnimationView::updateTimestep()
{
    if (currentTaskGroup) {
        e->runTimestep(timestep, currentTaskGroup.get());  // Use .get() to pass raw pointer
        currentTaskGroup.reset();  // Release ownership after use
    } else {
        e->runTimestep(timestep);
    }
    mapfScene->updateScene(timestep);     // Update the scene visually
    timestep++;
}
