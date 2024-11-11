/**
 * @file generalview.h
 * @author Mateusz Ambroży
 * @brief main window to chose algorithm
 * @version 0.1
 * @date 2024-11-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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

#endif
