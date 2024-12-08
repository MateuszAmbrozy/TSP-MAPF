QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = settingsview
TEMPLATE = app

CONFIG += c++14

# Dynamiczne dodawanie wsystkich plików .cpp z katalogów
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
    menuItem.cpp \
    pointlistitemwidget.cpp \
    $$files(../src/WCA/*.cpp) \
    $$files(../src/*.cpp) \
    $$files(../src/AStar/*.cpp) \
    $$files(../src/CA/*.cpp) \
    sidebar.cpp \
    taskinputwidget.cpp

# Dynamiczne dodawanie wszystkich plików .h z katalogów
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
    menuItem.h \
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
INCLUDEpATH += ../lib/WCA

FORMS    +=
RESOURCES += default.qrc

target.files = settingsview
target.path = /usr/bin
INSTALLS += target
