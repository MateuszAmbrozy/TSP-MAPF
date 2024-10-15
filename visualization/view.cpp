#include "view.h"

View::View(Environment* env)
    : QGraphicsView{}, mapfScene(new MapfScene(env, this)), timestep(0)
{
    setScene(mapfScene);
    resize(mapfScene->sceneRect().width() + 2, mapfScene->sceneRect().height() + 2);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set up timer for real-time updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &View::updateTimestep);
    timer->start(1000);  // Update every second (1000 ms)
}

void View::updateTimestep()
{
    mapfScene->updateScene(timestep);  // Update the scene
    timestep++;  // Move to the next timestep
}
