/**
 * @file agentinputwidget.h
 * @author Mateusz Ambroży
 * @brief Window to editing graph (add Agents and Obstacles)
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef AGENTINPUTWIDGET_H
#define AGENTINPUTWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "graph/interactiveagentgraph.h"
#include "../lib/Agent.h"
#include <QMessageBox>
#include <QSpinBox>

#include <QListWidget>
class AgentInputWidget : public QWidget
{
    Q_OBJECT
    struct AgentData
    {
        QPoint point;
        int capacity;
    };


    friend class EditorView;
private:
    enum class options{Agent = 0, Obstacle, Pickup, Dropoff};
    QGraphicsScene *scene;
    InteractiveAgentRectItem *graphItem;

    QGraphicsView *graphicsView;

    QSpinBox *widthInput;
    QSpinBox *heightInput;
    QListWidget *agentListWidget;


    void setScene(QGraphicsScene* scene);
    std::vector<QPoint> obstacles;
    std::vector<AgentData> agentsData;
    int width, height;
    std::vector<QPoint> avaliablePickupPoints;
    std::vector<QPoint> avaliableDropoffPoints;

    void updateMap();

private slots:

    void addAgent(const QPoint &point);
    void addPickupPoint(const QPoint &point);
    void addDropoffPoint(const QPoint& point);
    void addObstacle(const QPoint &point);
    void removeAgent(const QPoint& point);
    void removeObstacle(const QPoint& point);
    void removePickupPoint(const QPoint& point);
    void removeDropoffPoint(const QPoint& point);


    void createMap();
    void updateAgentList();
public:
    AgentInputWidget(QGraphicsScene *scene, InteractiveAgentRectItem *graphItem, QWidget *parent = nullptr);
    ~AgentInputWidget();
    void clear();
    void loadMap(int width, int height, const std::vector<QPoint>& obstacles, const std::vector<AgentData>& agentsData, const std::vector<QPoint>& pickupsData, const std::vector<QPoint>& dropoffsData);
signals:
    void mapCreated(std::shared_ptr<map::Graph> map);
    void agentsCreated(std::shared_ptr<std::vector<Agent>> agents);
};

#endif // AGENTINPUTWIDGET_H
