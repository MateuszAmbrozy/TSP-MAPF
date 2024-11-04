#include "generalview.h"
#include <QGroupBox>
GeneralView::GeneralView(QWidget* parent) : QWidget(parent), selectedAlgorithm(AlgType::A_STAR)
{
    // Tworzymy etykietę dla wyboru algorytmu
    label = new QLabel("Select an Algorithm:", this);
    label->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");

    // Tworzymy przyciski radiowe z ładnym CSS
    CA = new QRadioButton("Algorithm Cooperative A*", this);
    WHCA = new QRadioButton("Algorithm Window Hierarchical Cooperative A*", this);
    CA->setChecked(true);
    CA->setStyleSheet("QRadioButton { font-size: 14px; padding: 5px; }");
    WHCA->setStyleSheet("QRadioButton { font-size: 14px; padding: 5px; }");

    // Łączymy sygnały, aby zaktualizować wybrany algorytm
    connect(CA, &QRadioButton::toggled, this, &GeneralView::onCASelected);
    connect(WHCA, &QRadioButton::toggled, this, &GeneralView::onWHCASelected);

    // Ustawiamy QGroupBox, aby zorganizować wybór algorytmu
    QGroupBox* algorithmGroupBox = new QGroupBox("Algorithm Selection", this);
    algorithmGroupBox->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; border: 1px solid #AAA; margin-top: 20px; padding: 15px; }");

    // Ustawiamy układ dla QGroupBox
    QVBoxLayout* radioLayout = new QVBoxLayout();
    radioLayout->addWidget(CA);
    radioLayout->addWidget(WHCA);
    algorithmGroupBox->setLayout(radioLayout);

    // Ustawiamy główny układ dla GeneralView
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(algorithmGroupBox);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    setLayout(layout);
    setStyleSheet("background-color: #F9F9F9; border-radius: 8px;");
    setFixedSize(300, 200);
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
