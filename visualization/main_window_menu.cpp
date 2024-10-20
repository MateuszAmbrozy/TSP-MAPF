#include "main_window_menu.h"

MainWindowMenu::MainWindowMenu(QWidget *parent)
    : QMainWindow(parent)
{
    // Layout i przycisk
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    startButton = new QPushButton("Start wizualizacji", this);
    layout->addWidget(startButton);

    editButton = new QPushButton("Edit", this);
    layout->addWidget(editButton);

    setCentralWidget(centralWidget);

    // Połączenie sygnału kliknięcia z sygnałem do rozpoczęcia wizualizacji
    connect(startButton, &QPushButton::clicked, this, &MainWindowMenu::startVisualization);
    connect(startButton, &QPushButton::clicked, this, &MainWindowMenu::startVisualization);
}
