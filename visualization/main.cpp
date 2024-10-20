#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "view.h"
#include "main_window_menu.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    map::Graph graph(10, 10);

    std::vector<Agent> agents;

    Environment e(agents, graph);
    e.addAgent(Agent(0, 10, {0, 0}));
    e.addAgent(Agent(1, 10, {9, 9}));
    e.addAgent(Agent(3, 10, {0, 9}));
    e.addAgent(Agent(4, 10, {1, 9}));
    e.addAgent(Agent(5, 10, {9, 3}));
    
    
    MainWindowMenu menuWindow;
    menuWindow.show();

    // Stwórz obiekt widoku (wizualizacja agentów), ale nie pokazuj go od razu
    View* viewWindow = new View(&e);

    // Połącz sygnał z przyciskiem, aby uruchomić wizualizację po kliknięciu
    QObject::connect(&menuWindow, &MainWindowMenu::startVisualization, [&]() {
        menuWindow.close();  // Zamknij menu główne
        viewWindow->start();
        viewWindow->show();  // Pokaż okno z wizualizacją
    });

    // Obsługa powrotu do menu z okna wizualizacji
    QObject::connect(viewWindow, &View::backToMenu, [&]() {
        viewWindow->close();  // Zamknij okno wizualizacji
        menuWindow.show();   // Pokaż ponownie menu główne
    });
    return app.exec();
}
