/**
 * @file settingsview.cpp
 * @date 2016/06/07
 * @author fl0mll
 *
 * This document contains proprietary information belonging to mllapps.com
 * Passing on and copying of this document, use and communication of its
 * contents is not permitted without prior written authorization.
 */
#include <QTextBrowser>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QToolButton>
#include <QDebug>
#include <QStackedWidget>

#include "settingsview.h"
#include "borderlayout.h"

#include "ui_generalview.h"

/*------------------------------------------------------------------------------
 * CTOR / DTOR
 *----------------------------------------------------------------------------*/
/**
 * @brief Create a instance of the settings widget
 *
 * @param parent
 */
SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent),
    _stackedWidget(nullptr),
    _activeButton(nullptr),
    graph(10, 10),
    sidebarVisible(true)
{
    this->initGraph();
    // Główny layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    /* Create a layout for the sidebar */
    sidebar = new QWidget();
    QVBoxLayout * sidebarLayout = new QVBoxLayout();
    _activeButton = createSidebarButton(":/icons/assets/settings.svg", tr("General"));

    _activeButton->setChecked(true);
    sidebarLayout->addWidget(_activeButton);
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/squares-2.svg", tr("Animation") ));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/wifi.svg", tr("Editor") ));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/storage.svg", tr("Maps") ));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/cloud.svg", tr("Storage") ));
    sidebarLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    sidebarLayout->setSpacing(0);

    sidebar->setLayout(sidebarLayout);
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumHeight(sidebarLayout->count() * 76);

    /* Create the stacked widget */
    _stackedWidget = new QStackedWidget;

    /* Create the board layout */
    BorderLayout *layout = new BorderLayout();
    layout->addWidget(_stackedWidget, BorderLayout::Center);
    layout->addWidget(sidebar, BorderLayout::West);

    // Dodajemy BorderLayout do głównego layoutu
    QWidget *borderLayoutContainer = new QWidget;
    borderLayoutContainer->setLayout(layout);
    mainLayout->addWidget(borderLayoutContainer);

    setLayout(mainLayout);  // Ustawiamy główny layout
    layout->setSpacing(0);

    setWindowTitle(tr("Settings"));
    setGeometry(0,0, 700, sidebar->minimumHeight());

    animation = new AnimationView();
    editor = new EditorView();
    generalView = new GeneralView();

    /* Create the first view */

    _stackedWidget->addWidget(generalView);

    // Inicjalizacja animacji paska bocznego
    sidebarAnimation = new QPropertyAnimation(sidebar, "maximumWidth");
    sidebarAnimation->setDuration(300);  // Czas trwania animacji w ms
    sidebarAnimation->setStartValue(200);  // Początkowa szerokość paska
    sidebarAnimation->setEndValue(0);  // Końcowa szerokość paska (ukryty)

    // Po zakończeniu animacji, ustaw ostateczny stan szerokości
    connect(sidebarAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (!sidebarVisible) {
            this->sidebar->setVisible(false);  // Ukryj widget po schowaniu
        } else {
            this->sidebar->setVisible(true);   // Pokaż widget przed rozwinięciem
        }
    });

    // Przycisk do chowania/rozwijania paska
    QPushButton *toggleButton = new QPushButton(this);
    toggleButton->setFixedSize(40, 40);  // Ustawienie kwadratowego rozmiaru 40x40
    toggleButton->setText("☰");          // Ustawienie tekstu (lub ikony menu, np. "☰")

    // Stylizacja (opcjonalnie, aby nadać przyciskowi styl)
    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");

    // Dodajemy przycisk do głównego layoutu, na samej górze
    mainLayout->addWidget(toggleButton, 0, Qt::AlignLeft | Qt::AlignTop);  // Przycisk w lewym górnym rogu

    connect(toggleButton, &QPushButton::clicked, this, &SettingsView::toggleSidebar);
}
/**
 * @brief Free allocated memory
 */
SettingsView::~SettingsView()
{
    delete _stackedWidget; _stackedWidget = nullptr;
}

void SettingsView::changeCenterWidget(bool event)
{
    Q_UNUSED(event);
    QString sender = QObject::sender()->objectName();

    // Wyłącz aktualny przycisk, jeśli istnieje
    if(_activeButton != nullptr)
    {
        _activeButton->setChecked(false);
    }

    // Ustaw nowy aktywny przycisk
    _activeButton = static_cast<QToolButton*>(QObject::sender());
    _activeButton->setChecked(true);

    // Zmieniaj widok w zależności od przycisku
    if(sender.compare("General") == 0)
    {
        if (_stackedWidget->indexOf(generalView) == -1)
        {
            _stackedWidget->addWidget(generalView);
        }
        _stackedWidget->setCurrentWidget(generalView);
    }
    else if(sender.compare("Animation") == 0)
    {
        // Dodaj AnimationView tylko raz
        //animation->setAlgorithm(generalView->getSelectedAlgorithm());
        if (_stackedWidget->indexOf(animation) == -1)
        {
            _stackedWidget->addWidget(animation);
        }
        _stackedWidget->setCurrentWidget(animation);
    }
    else if(sender.compare("Editor") == 0)
    {
        // Dodajemy editorView do stosu i ustawiamy jako aktywny widok
        if (_stackedWidget->indexOf(editor) == -1)
        {
            _stackedWidget->addWidget(editor);
        }
        _stackedWidget->setCurrentWidget(editor);
    }
    else if(sender.compare("Maps") == 0)
    {

    }
    else if(sender.compare("Storage") == 0)
    {

    }
}

void SettingsView::toggleSidebar()
{
    if (sidebarVisible)
    {
        // Schowanie paska
        sidebarAnimation->setStartValue(sidebar->width());  // Początkowa szerokość
        sidebarAnimation->setEndValue(0);  // Ukryj pasek
        sidebarAnimation->setDirection(QPropertyAnimation::Forward);
    }
    else
    {
        // Rozwinięcie paska
        sidebar->setVisible(true);  // Upewnij się, że pasek jest widoczny przed rozwinięciem
        sidebarAnimation->setStartValue(0);  // Początkowa szerokość 0 (schowany)
        sidebarAnimation->setEndValue(200);  // Ustaw docelową szerokość
        sidebarAnimation->setDirection(QPropertyAnimation::Forward);
    }

    sidebarVisible = !sidebarVisible;  // Zmień stan paska
    sidebarAnimation->start();  // Rozpocznij animację
}
/*------------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------*/

/**
 * @brief Create a button for the sidebar
 *
 * @param iconPath Path to the icon
 * @param title Tile to display under the icon
 *
 * @return A new instance of a button for the sidebar
 */
QToolButton * SettingsView::createSidebarButton(const QString& iconPath, const QString& title)
{
    QIcon icon(iconPath);

    QToolButton * btn = new QToolButton;
    btn->setIcon(icon);
    btn->setIconSize(QSize(42, 42));
    btn->setText(title);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setFixedSize(76, 76);
    btn->setObjectName(title);
    btn->setCheckable(true);
    QObject::connect(btn, SIGNAL(clicked(bool)),
                     this, SLOT(changeCenterWidget(bool)));

    return btn;
}

void SettingsView::initGraph()
{
    graph.cells[1][3].isObstacle = true;
    graph.cells[2][3].isObstacle = true;
    graph.cells[3][3].isObstacle = true;
    graph.cells[4][3].isObstacle = true;
    graph.cells[5][3].isObstacle = true;
    graph.cells[6][3].isObstacle = true;
    graph.cells[7][3].isObstacle = true;
    graph.cells[8][3].isObstacle = true;


    graph.cells[1][5].isObstacle = true;
    graph.cells[2][5].isObstacle = true;
    graph.cells[3][5].isObstacle = true;
    graph.cells[4][5].isObstacle = true;
    graph.cells[5][5].isObstacle = true;
    graph.cells[6][5].isObstacle = true;
    graph.cells[7][5].isObstacle = true;
    graph.cells[8][5].isObstacle = true;


    graph.cells[1][7].isObstacle = true;
    graph.cells[2][7].isObstacle = true;
    graph.cells[3][7].isObstacle = true;
    graph.cells[4][7].isObstacle = true;
    graph.cells[5][7].isObstacle = true;
    graph.cells[6][7].isObstacle = true;
    graph.cells[7][7].isObstacle = true;
    graph.cells[8][7].isObstacle = true;
}

/**
 * @brief Push the widget on the stack and set it active
 *
 * @param page Widget to push
 */
void SettingsView::push(QWidget *page)
{
    _stackedWidget->addWidget(page);
    _stackedWidget->setCurrentWidget(page);
}

/**
 * @brief Remove the current widget from the stack and switch to the previous
 */
void SettingsView::pop()
{
    QWidget * currentWidget = _stackedWidget->currentWidget();
    _stackedWidget->removeWidget(currentWidget);

    delete currentWidget; currentWidget = nullptr;
}
