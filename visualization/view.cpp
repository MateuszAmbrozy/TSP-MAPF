// view.cpp
#include "view.h"

View::View(Environment* env)
    : QGraphicsView{}, mapfScene(new MapfScene(env, this)), timestep(0), e(env)
{
    setScene(mapfScene);
    setSceneRect(0, 0, 400, 400);
    resize(mapfScene->sceneRect().width() + 2, mapfScene->sceneRect().height() + 2);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(this);
    e->assignVacanAgents();

    // Przycisk powrotu do menu
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);

    backButton = new QPushButton("Powrót do menu", container);
    layout->addWidget(backButton);
    container->setLayout(layout);
    connect(backButton, &QPushButton::clicked, this, &View::backToMenu);
}

void View::start()
{
    connect(timer, &QTimer::timeout, this, &View::updateTimestep);
    timer->start(700);  // Update every 1 second (1000 ms)
}
void View::updateTimestep()
{
    e->runTimestep(timestep);   // Call one timestep of the algorithm
    mapfScene->updateScene(timestep);     // Update the scene visually
    timestep++;
}
