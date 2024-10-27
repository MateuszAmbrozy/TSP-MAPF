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
    explicit AnimationView(QWidget *parent = 0);
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
    QHBoxLayout *mainLayout;
    QPushButton *startAnimation;
    QPushButton *stopAnimation;
    QPushButton *taskGroupBtn;

    QWidget *sidebar;
    QPropertyAnimation *sidebarAnimation;
    bool sidebarVisible;


    map::Graph graph;
    std::unique_ptr<InteractiveTaskRectItem> interactive_graph;
    std::vector<Agent> agents;

    void initEnvironment();
    void setTaskGroup(std::shared_ptr<TaskGroup> taskGroup);
    QToolButton *createSidebarButton(const QString& iconPath, const QString& title, std::function<void()> onClickFunction);
    void updateView();
public slots:
    void stop();
    void start();
    void showTaskGroupScene();
    void loadMap();
    void updateTimestep();
    void toggleSidebar();
};

#endif // ANIMATIONVIEW_H