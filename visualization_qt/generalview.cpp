#include "generalview.h"
#include <QGroupBox>

GeneralView::GeneralView(QWidget* parent) : QWidget(parent), selectedAlgorithm(AlgType::A_STAR)
{
    // Label for algorithm selection
    label = new QLabel("Select an Algorithm:", this);
    label->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");

    // Radio buttons with word wrapping and improved styles
    CA = new QRadioButton("Algorithm Cooperative A*", this);
    WCA = new QRadioButton("Algorithm Window Hierarchical Cooperative A*", this);
    CA->setChecked(true);
    CA->setStyleSheet("QRadioButton { font-size: 14px; padding: 5px; }");
    WCA->setStyleSheet("QRadioButton { font-size: 14px; padding: 5px; }");

    // Connect signals to update selected algorithm
    connect(CA, &QRadioButton::toggled, this, &GeneralView::onCASelected);
    connect(WCA, &QRadioButton::toggled, this, &GeneralView::onWHCASelected);

    // Group box to organize algorithm selection
    QGroupBox* algorithmGroupBox = new QGroupBox("Algorithm Selection", this);
    algorithmGroupBox->setStyleSheet(
        "QGroupBox { font-size: 16px; font-weight: bold; "
        "border: 1px solid #AAA; margin-top: 10px; padding: 10px 10px 10px 20px; } "
        "QGroupBox::title { subcontrol-origin: margin; left: 15px; top: -5px; }"  // Adjust title position
        );

    // Layout for the group box
    QVBoxLayout* radioLayout = new QVBoxLayout();
    radioLayout->addWidget(CA);
    radioLayout->addWidget(WCA);
    algorithmGroupBox->setLayout(radioLayout);

    // Main layout for GeneralView
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(algorithmGroupBox);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    setLayout(layout);
    setStyleSheet("background-color: #F9F9F9; border-radius: 8px;");
    setFixedSize(320, 220);  // Increase width and height to fit content better
}

AlgType GeneralView::getSelectedAlgorithm() const
{
    return selectedAlgorithm;
}

void GeneralView::onCASelected()
{
    this->selectedAlgorithm = AlgType::A_STAR;
}

void GeneralView::onWHCASelected()
{
    this->selectedAlgorithm = AlgType::WHCA;
}
