#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QMap>
#include "animationview.h"
#include "editorview.h"
#include "generalview.h"
class QLabel;
class QToolButton;
class QStackedWidget;
class QString;

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsView(QWidget *parent = 0);
    ~SettingsView();

    void push(QWidget * page);
    void pop();

public slots:
    void changeCenterWidget(bool);
    void toggleSidebar();  // Nowa funkcja do chowania/rozwijania paska

private:
    QToolButton * createSidebarButton(const QString& iconPath, const QString& title);
    QStackedWidget * _stackedWidget;
    QToolButton * _activeButton;
    AnimationView* animation;
    EditorView* editor;
    GeneralView* generalView;
    QWidget * sidebar;
    map::Graph graph;


    bool sidebarVisible;  // Zmienna do przechowywania stanu paska
    QPropertyAnimation* sidebarAnimation;  // Animacja paska
    void initGraph();
};

#endif /* SETTINGSVIEW_H */
