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

SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent),
    _stackedWidget(nullptr),
    _activeButton(nullptr),
    graph(10, 10),
    sidebarVisible(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    /* Create a layout for the sidebar */
    sidebar = new QWidget();
    QVBoxLayout * sidebarLayout = new QVBoxLayout();
    _activeButton = createSidebarButton(":/icons/assets/settings.svg", tr("General"));

    _activeButton->setChecked(true);
    sidebarLayout->addWidget(_activeButton);
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/squares-2.svg", tr("Animation") ));
    sidebarLayout->addWidget(createSidebarButton(":/icons/assets/wifi.svg", tr("Editor") ));
    sidebarLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    sidebarLayout->setSpacing(0);

    sidebar->setLayout(sidebarLayout);
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumHeight(sidebarLayout->count() * 76 + 150);

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

    setWindowTitle(tr("Warehouse simulator"));
    setGeometry(0,0, 700, sidebar->minimumHeight());


    editor = new EditorView();
    generalView = new GeneralView();



    _stackedWidget->addWidget(generalView);


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


    QPushButton *toggleButton = new QPushButton(this);
    toggleButton->setFixedSize(40, 40);
    toggleButton->setText("☰");


    toggleButton->setStyleSheet("QPushButton { border: 1px solid black; }");


    mainLayout->addWidget(toggleButton, 0, Qt::AlignLeft | Qt::AlignTop);  // Przycisk w lewym górnym rogu

    connect(toggleButton, &QPushButton::clicked, this, &SettingsView::toggleSidebar);
}

SettingsView::~SettingsView()
{
    delete _stackedWidget; _stackedWidget = nullptr;
}

void SettingsView::changeCenterWidget(bool event)
{
    Q_UNUSED(event);
    QString sender = QObject::sender()->objectName();


    if(_activeButton != nullptr)
    {
        _activeButton->setChecked(false);
    }


    _activeButton = static_cast<QToolButton*>(QObject::sender());
    _activeButton->setChecked(true);

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
        if(animation)
        {
            delete animation;
            animation = nullptr;
        }
        animation = new AnimationView(generalView->getSelectedAlgorithm());

        if (_stackedWidget->indexOf(animation) == -1)
        {
            _stackedWidget->addWidget(animation);
        }
        _stackedWidget->setCurrentWidget(animation);
    }
    else if(sender.compare("Editor") == 0)
    {

        if (_stackedWidget->indexOf(editor) == -1)
        {
            _stackedWidget->addWidget(editor);
        }
        _stackedWidget->setCurrentWidget(editor);
    }

}

void SettingsView::toggleSidebar()
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


void SettingsView::push(QWidget *page)
{
    _stackedWidget->addWidget(page);
    _stackedWidget->setCurrentWidget(page);
}

void SettingsView::pop()
{
    QWidget * currentWidget = _stackedWidget->currentWidget();
    _stackedWidget->removeWidget(currentWidget);

    delete currentWidget; currentWidget = nullptr;
}
