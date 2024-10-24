#include "settingsview.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SettingsView w;

    // Load an application style
    QFile styleFile(":/styles/styles/default.qss");
    styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    app.setStyleSheet( style );

    w.show();

    return app.exec();
}
