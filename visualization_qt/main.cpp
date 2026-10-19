/**
 * @file main.cpp
 * @author Mateusz Ambroży
 * @brief main
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "menu.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Menu menu;


    QFile styleFile(":/styles/styles/default.qss");
    styleFile.open( QFile::ReadOnly );


    QString style( styleFile.readAll() );
    app.setStyleSheet( style );

    menu.show();

    return app.exec();
}
