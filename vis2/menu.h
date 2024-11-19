/**
 * @file settingsview.h
 * @author your name (you@domain.com)
 * @brief base function for menu bar
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QMap>
#include "animationview.h"
#include "editorview.h"
#include "generalview.h"
class QLabel;
class QToolButton;
class QStackedWidget;
class QString;

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

    void push(QWidget * page);
    void pop();

public slots:
    void changeCenterWidget(bool event);
    void toggleSidebar();

private:
    QToolButton * createSidebarButton(const QString& iconPath, const QString& title);
    QStackedWidget * _stackedWidget;
    QToolButton * _activeButton;
    AnimationView* animation;
    EditorView* editor;
    GeneralView* generalView;
    QWidget * sidebar;
    map::Graph graph;

    void setupSidebar();
    void setupMainLayout();
    void setupToglleAnimation();

    QWidget* getViewByName(const QString& viewName);
    void setStackedWidgetPage(QWidget* view);
    bool sidebarVisible;
    QPropertyAnimation* sidebarAnimation;
    void initGraph();
};

#endif /* MENU_H */
