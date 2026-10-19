QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tsp_mapf_gui
TEMPLATE = app

CONFIG += c++17

SOURCES += main.cpp\
    agentinputwidget.cpp \
    agentlistitemwidget.cpp \
    generalview.cpp \
    graph/inetractivetaskgraph.cpp \
    graph/interactiveagentgraph.cpp \
    agentrectitem.cpp \
    animationscene.cpp \
    animationview.cpp \
    editorview.cpp \
    graph/interactivegraphrectitem.cpp \
    graph/visualizationgraph.cpp \
    graph/graphrectitem.cpp \
    graph/interactivecellitem.cpp \
    menu.cpp \
    pointlistitemwidget.cpp \
    $$files(../src/WCA/*.cpp) \
    $$files(../src/*.cpp) \
    $$files(../src/AStar/*.cpp) \
    $$files(../src/CA/*.cpp) \
    sidebar.cpp \
    taskinputwidget.cpp

HEADERS += \
    agentinputwidget.h \
    agentlistitemwidget.h \
    generalview.h \
    graph/inetractivetaskgraph.h \
    graph/interactiveagentgraph.h \
    agentrectitem.h \
    animationscene.h \
    animationview.h \
    editorview.h \
    graph/interactivegraphrectitem.h \
    graph/visualizationgraph.h \
    graph/graphrectitem.h \
    graph/interactivecellitem.h \
    menu.h \
    pointlistitemwidget.h \
    $$files(../lib/WCA/*.h) \
    $$files(../lib/*.h) \
    $$files(../lib/AStar/*.h) \
    $$files(../lib/CA/*.h) \
    sidebar.h \
    taskinputwidget.h

INCLUDEPATH += ../lib
INCLUDEPATH += ../lib/AStar
INCLUDEPATH += ../lib/CA
INCLUDEPATH += ../lib/WCA

FORMS    +=
RESOURCES += default.qrc

target.files = tsp_mapf_gui
target.path = /usr/bin
INSTALLS += target
