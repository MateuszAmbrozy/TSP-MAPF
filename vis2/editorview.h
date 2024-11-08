#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QPushButton>
#include <QWidget>
#include <QMap>
#include <QStackedWidget>
#include <QToolButton>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include "../lib/Graph.h"
#include "../lib/Agent.h"
#include "agentinputwidget.h"

class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(QWidget* parent = nullptr);
    void updateGraphInScene();
public slots:
    void toggleSidebar();
    void loadMap();
    void saveMap();
    void clearAgents();

private:
    QToolButton *createSidebarButton(const QString& iconPath, const QString& title, std::function<void()> onClickFunction);
    QStackedWidget *_stackedWidget;
    QToolButton *_activeButton;
    QWidget *sidebar;

    AgentInputWidget* inputWidget;

    std::shared_ptr<map::Graph> createdMap;
    std::shared_ptr<std::vector<Agent>> created_agents;

    bool sidebarVisible;
    QPropertyAnimation *sidebarAnimation;
    void setCreatedGraph(std::shared_ptr<map::Graph> map);
    void setCreatedAgents(std::shared_ptr<std::vector<Agent>> agents);
};

#endif // EDITORVIEW_H
