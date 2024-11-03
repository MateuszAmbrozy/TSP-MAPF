#ifndef GENERALVIEW_H
#define GENERALVIEW_H
#include "qwidget.h"
#include <iostream>
// #include <string>
// #include "../lib/setup.h"
class GeneralView : public QWidget
{
    enum Algorithm {
        A_STAR,
        WHCA
    };
public:


    GeneralView();

    void show();

    Algorithm getSelectedAlgorithm() const;

    void printSelectedAlgorithm() const;

private:
    Algorithm selectedAlgorithm;

    void selectAlgorithm(int choice);
};

#endif // GENERALVIEW_H
