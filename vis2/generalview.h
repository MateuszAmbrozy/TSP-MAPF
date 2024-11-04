#ifndef GENERALVIEW_H
#define GENERALVIEW_H

#include <QWidget>
#include <QRadioButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "setup.h"

class GeneralView : public QWidget
{
    Q_OBJECT

public:
    GeneralView(QWidget* parent = nullptr);

    AlgType getSelectedAlgorithm() const;

private slots:
    void onCASelected();
    void onWHCASelected();

private:
    QRadioButton* CA;
    QRadioButton* WHCA;
    QLabel* label;
    AlgType selectedAlgorithm;
};

#endif // GENERALVIEW_H
