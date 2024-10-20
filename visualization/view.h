// view.h
#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QTimer>
#include "mapfscene.h"
#include "../lib/Environment.h"  // Environment for backend logic
#include <QPushButton>
#include <QVBoxLayout>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    explicit View(Environment* env);
signals:
    void backToMenu();  // Sygnał do powrotu do menu
private:
    MapfScene* mapfScene;
    QTimer* timer;
    int timestep;
    Environment* e;
    QPushButton* backButton;

public slots:
    void start();
    void updateTimestep();  // Slot to update timestep periodically
};

#endif // VIEW_H
