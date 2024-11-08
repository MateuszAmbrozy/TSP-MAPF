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

    void updateMap();

private slots:
    void addAgent(const QPoint &point);
    void addObstacle(const QPoint &point);
    void removeAgent(const QPoint& point);
    void removeObstacle(const QPoint& point);

    void createMap();
    void updateAgentList();
public:
    AgentInputWidget(QGraphicsScene *scene, InteractiveAgentRectItem *graphItem, QWidget *parent = nullptr);

    void clear();
    void loadMap(int width, int height, const std::vector<QPoint>& obstacles, const std::vector<AgentData>& agentsData);
signals:
    void mapCreated(std::shared_ptr<map::Graph> map);
    void agentsCreated(std::shared_ptr<std::vector<Agent>> agents);
};

#endif // AGENTINPUTWIDGET_H
