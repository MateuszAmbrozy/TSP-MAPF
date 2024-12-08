
#include <QTextBrowser>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QToolButton>
#include <QDebug>
#include <QStackedWidget>

#include "menu.h"
#include "sidebar.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    _stackedWidget(nullptr),
    _activeButton(nullptr),
    graph(10, 10),
    sidebarVisible(true)
{
    this->setupSidebar();
    this->setupMainLayout();

    this->setupToglleAnimation();
    /* Create the board layout */

    setWindowTitle(tr("Warehouse simulator"));
    setGeometry(0,0, 700, sidebar->minimumHeight());

}

Menu::~Menu()
{
    delete _stackedWidget; _stackedWidget = nullptr;
}

void Menu::changeCenterWidget(bool event) {
    Q_UNUSED(event);
    auto *button = qobject_cast<QToolButton*>(sender());
    if (!button) return;

    if (_activeButton != nullptr) {
        _activeButton->setChecked(false);
    }
    _activeButton = button;
    _activeButton->setChecked(true);

    QString viewName = _activeButton->objectName();
    QWidget *targetView = getViewByName(viewName);

    if (targetView) {
        setStackedWidgetPage(targetView);
    }
}

QWidget* Menu::getViewByName(const QString &viewName) {
    if (viewName == "General") {
        return generalView;
    } else if (viewName == "Animation")
    {
        //if (animation) delete animation;
        animation = new AnimationView(generalView->getSelectedAlgorithm());
        return animation;

    }
    else if (viewName == "Editor")
    {
        return editor;
    }
    return nullptr;
}

void Menu::setStackedWidgetPage(QWidget *view) {
    if (_stackedWidget->indexOf(view) == -1) {
        _stackedWidget->addWidget(view);
    }
    _stackedWidget->setCurrentWidget(view);
}


void Menu::toggleSidebar()
{
    if (sidebarVisible)
    {
        sidebarAnimation->setStartValue(sidebar->width());
        sidebarAnimation->setEndValue(0);
        sidebarAnimation->setDirection(QPropertyAnimation::Forward);
    }
    else
    {
        sidebar->setVisible(true);
        sidebarAnimation->setStartValue(0);
        sidebarAnimation->setEndValue(200);
        sidebarAnimation->setDirection(QPropertyAnimation::Forward);
    }

    sidebarVisible = !sidebarVisible;
    sidebarAnimation->start();
}

QToolButton * Menu::createSidebarButton(const QString& iconPath, const QString& title)
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

void Menu::setupSidebar()
{
    sidebar = new QWidget();
    QVBoxLayout * sidebarLayout = new QVBoxLayout();
    _activeButton = createSidebarButton(":/icons/assets/settings.svg", tr("General"));

    _activeButton->setChecked(true);
    sidebarLayout->addWidget(_activeButton);
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/animation.svg", tr("Animation") ));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/editor.svg", tr("Editor") ));
    sidebarLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    sidebarLayout->setSpacing(0);

    sidebar->setLayout(sidebarLayout);
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumHeight(sidebarLayout->count() * 76 + 150);

    /* Create the stacked widget */
    _stackedWidget = new QStackedWidget;


}

void Menu::setupMainLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    Sidebar *layout = new Sidebar();

    layout->addWidget(_stackedWidget, Sidebar::Center);
    layout->addWidget(sidebar, Sidebar::West);

    editor = new EditorView();
    generalView = new GeneralView();

    // Dodajemy BorderLayout do głównego layoutu
    QWidget *borderLayoutContainer = new QWidget;
    borderLayoutContainer->setLayout(layout);
    mainLayout->addWidget(borderLayoutContainer);

    QPushButton *toggleButton = new QPushButton(this);
    toggleButton->setFixedSize(40, 40);
    toggleButton->setText("☰");


    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");
    connect(toggleButton, &QPushButton::clicked, this, &Menu::toggleSidebar);

    mainLayout->addWidget(toggleButton, 0, Qt::AlignLeft | Qt::AlignTop);
    _stackedWidget->addWidget(generalView);
    setLayout(mainLayout);  // Ustawiamy główny layout
    layout->setSpacing(0);
}

void Menu::setupToglleAnimation()
{
    sidebarAnimation = new QPropertyAnimation(sidebar, "maximumWidth");
    sidebarAnimation->setDuration(300);
    sidebarAnimation->setStartValue(200);
    sidebarAnimation->setEndValue(0);


    connect(sidebarAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (!sidebarVisible) {
            this->sidebar->setVisible(false);
        } else {
            this->sidebar->setVisible(true);
        }
    });

}


void Menu::push(QWidget *page)
{
    _stackedWidget->addWidget(page);
    _stackedWidget->setCurrentWidget(page);
}

void Menu::pop()
{
    QWidget * currentWidget = _stackedWidget->currentWidget();
    _stackedWidget->removeWidget(currentWidget);

    delete currentWidget; currentWidget = nullptr;
}
