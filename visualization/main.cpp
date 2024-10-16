#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    map::Graph graph(10, 10);

    std::vector<Agent> agents;

    Environment e(agents, graph);
    e.addAgent(Agent(0, 10, {1, 0}));
    e.addAgent(Agent(1, 10, {4, 4}));
    e.addAgent(Agent(3, 10, {6, 7}));
    e.addAgent(Agent(4, 10, {0, 8}));
    e.addAgent(Agent(5, 10, {8, 0}));

    //e.mainAlgorithm();
    View* view = new View(&e);
    view->show();


    return app.exec();
}
