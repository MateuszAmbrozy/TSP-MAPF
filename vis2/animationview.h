/**
 * @file animationview.h
 * @author Mateusz Ambroży
 * @brief Animation Window where everytihing is diplay
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsView>
#include <memory>
#include "../lib/CA_Environment.h"
#include <QSlider>
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
    int animationSpeed;

    AlgType algorithm;
    BaseEnvironment* environment;

    QPushButton* backButton;
    QHBoxLayout *mainLayout;
    QPushButton *startAnimation;
    QPushButton *stopAnimation;
    QPushButton *taskGroupBtn;
    QSlider* aniamtionSpeedSlider;

    QWidget *sidebar;
    QPropertyAnimation *sidebarAnimation;
    bool sidebarVisible;


    map::Graph graph;
    std::unique_ptr<InteractiveTaskRectItem> interactive_graph;
    std::vector<Agent> agents;

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

    void onChange();
};

#endif
