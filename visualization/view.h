// view.h
#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QTimer>
#include "mapfscene.h"
#include "../lib/Environment.h"  // Environment for backend logic

class View : public QGraphicsView
{
    Q_OBJECT

public:
    explicit View(Environment* env);

private:
    MapfScene* mapfScene;
    QTimer* timer;
    int timestep;
    Environment* e;

public slots:
    void updateTimestep();  // Slot to update timestep periodically
};

#endif // VIEW_H
