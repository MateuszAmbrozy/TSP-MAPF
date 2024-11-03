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
#include "qpropertyanimation.h"
#include "taskinputwidget.h"
#include "graph/inetractivetaskgraph.h"
class AnimationView : public QWidget
{
    Q_OBJECT
    enum Algorithm {
        A_STAR,
        WHCA
    };
public:
    explicit AnimationView(QWidget *parent = 0);
    ~AnimationView();

    void setAlgorithm(Algorithm algorithm);

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
    //Environment* e;
    WHCA_Environment* whca_environment;
    QPushButton* backButton;
    QHBoxLayout *mainLayout;
    QPushButton *startAnimation;
    QPushButton *stopAnimation;
    QPushButton *taskGroupBtn;

    QWidget *sidebar;
    QPropertyAnimation *sidebarAnimation;
    bool sidebarVisible;
    Algorithm algorithm;

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
    //void loadTasks();
    void updateTimestep();
    void toggleSidebar();
};

#endif // ANIMATIONVIEW_H
