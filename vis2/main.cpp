#include "settingsview.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SettingsView w;


    QFile styleFile(":/styles/styles/default.qss");
    styleFile.open( QFile::ReadOnly );


    QString style( styleFile.readAll() );
    app.setStyleSheet( style );

    w.show();

    return app.exec();
}
