#ifndef MAIN_WINDOW_MENU_H
#define MAIN_WINDOW_MENU_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindowMenu : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindowMenu(QWidget *parent = nullptr);

signals:
    void startVisualization();  // Sygnał do rozpoczęcia wizualizacji
    void startEditor();

private:
    QPushButton* startButton;   // Przycisk do uruchomienia wizualizacji
    QPushButton* editButton;
};

#endif // MAIN_WINDOW_MENU_H
