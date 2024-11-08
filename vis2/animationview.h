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
#include "../lib/BaseEnvironment.h"
#include "animationscene.h"
#include "qpropertyanimation.h"
#include "taskinputwidget.h"
#include "graph/inetractivetaskgraph.h"
class AnimationView : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationView(AlgType algorithm, QWidget *parent = 0);
    ~AnimationView();

    void setAlgorithm(AlgType algorithm);

private:
    MapfScene* mapfScene;
    QGraphicsScene* taskGroupScene;
    TaskInputWidget *inputWidget;
    std::shared_ptr<TaskGroup> currentTaskGroup;
    std::vector<TaskGroup> loadedTasks;

    QGraphicsView *mapfView;
    QGraphicsView *taskGroupView;
    QTimer* timer;
    int timestep;

    AlgType algorithm;
    BaseEnvironment* environment;

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

    std::vector<QWidget*> additionalWindows;
public slots:
    void stop();
    void start();
    void showTaskGroupScene();
    void loadMap();
    //void loadTasks();
    void updateTimestep();
    void toggleSidebar();
};

#endif // ANIMATIONVIEW_H
