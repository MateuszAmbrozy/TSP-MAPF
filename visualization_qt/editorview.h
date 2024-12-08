/**
 * @file editorview.h
 * @author Mateusz Ambroży
 * @brief Window to display edit graph
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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
#include "sidebar.h"
#include "qjsonarray.h"
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QIcon>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
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

    std::shared_ptr<AgentInputWidget> inputWidget;

    std::shared_ptr<map::Graph> createdMap;
    std::shared_ptr<std::vector<Agent>> created_agents;

    bool sidebarVisible;
    QPropertyAnimation *sidebarAnimation;
    void setCreatedGraph(std::shared_ptr<map::Graph> map);
    void setCreatedAgents(std::shared_ptr<std::vector<Agent>> agents);
};

#endif
