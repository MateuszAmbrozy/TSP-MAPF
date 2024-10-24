#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsView>
#include <memory>
#include "../lib/Environment.h"
#include "animationscene.h"
#include "taskinputwidget.h"
#include "graph/inetractivetaskgraph.h"
class AnimationView : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationView(map::Graph graph, QWidget *parent = 0);
    ~AnimationView();

private:
    MapfScene* mapfScene;
    QGraphicsScene* taskGroupScene;
    TaskInputWidget *inputWidget;
    std::shared_ptr<TaskGroup> currentTaskGroup;

    QGraphicsView *mapfView;
    QGraphicsView *taskGroupView;
    QTimer* timer;
    int timestep;
    Environment* e;
    QPushButton* backButton;
    QHBoxLayout *topLayout;
    QVBoxLayout *mainLayout;
    QPushButton *startAnimation;
    QPushButton *stopAnimation;
    QPushButton *taskGroupBtn;

    map::Graph graph;
    std::unique_ptr<InteractiveTaskRectItem> interactive_graph;
    std::vector<Agent> agents;

    void initEnvironment();
     void setTaskGroup(std::shared_ptr<TaskGroup> taskGroup);

public slots:
    void stop();
    void start();
    void showTaskGroupScene();
    void updateTimestep();
};

#endif // ANIMATIONVIEW_H
