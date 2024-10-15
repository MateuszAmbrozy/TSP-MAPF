#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QTimer>
#include "mapfscene.h"
#include "../lib/Environment.h"

class View : public QGraphicsView
{
    Q_OBJECT

public:
    explicit View(Environment* env);

private:
    MapfScene* mapfScene;
    QTimer* timer;
    int timestep;

public slots:
    void updateTimestep();  // Slot to trigger updates for each timestep
};

#endif // VIEW_H
