/********************************************************************************
** Form generated from reading UI file 'generalview.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERALVIEW_H
#define UI_GENERALVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GeneralView
{
public:
    QGridLayout *gridLayout;
    QRadioButton *radioButton_2;
    QSpacerItem *verticalSpacer;
    QToolButton *toolButton;
    QCheckBox *checkBox;
    QVBoxLayout *verticalLayout;
    QLabel *titleLbl;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QRadioButton *radioButton;
    QCommandLinkButton *commandLinkButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *GeneralView)
    {
        if (GeneralView->objectName().isEmpty())
            GeneralView->setObjectName("GeneralView");
        GeneralView->resize(400, 300);
        gridLayout = new QGridLayout(GeneralView);
        gridLayout->setObjectName("gridLayout");
        radioButton_2 = new QRadioButton(GeneralView);
        radioButton_2->setObjectName("radioButton_2");

        gridLayout->addWidget(radioButton_2, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 8, 1, 1, 1);

        toolButton = new QToolButton(GeneralView);
        toolButton->setObjectName("toolButton");

        gridLayout->addWidget(toolButton, 2, 1, 1, 1);

        checkBox = new QCheckBox(GeneralView);
        checkBox->setObjectName("checkBox");

        gridLayout->addWidget(checkBox, 4, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        titleLbl = new QLabel(GeneralView);
        titleLbl->setObjectName("titleLbl");
        QFont font;
        font.setPointSize(26);
        font.setBold(false);
        titleLbl->setFont(font);

        verticalLayout->addWidget(titleLbl);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_2 = new QLabel(GeneralView);
        label_2->setObjectName("label_2");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEdit = new QLineEdit(GeneralView);
        lineEdit->setObjectName("lineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);


        verticalLayout->addLayout(formLayout);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        radioButton = new QRadioButton(GeneralView);
        radioButton->setObjectName("radioButton");

        gridLayout->addWidget(radioButton, 1, 1, 1, 1);

        commandLinkButton = new QCommandLinkButton(GeneralView);
        commandLinkButton->setObjectName("commandLinkButton");

        gridLayout->addWidget(commandLinkButton, 5, 1, 1, 1);

        buttonBox = new QDialogButtonBox(GeneralView);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 7, 1, 1, 1);


        retranslateUi(GeneralView);

        QMetaObject::connectSlotsByName(GeneralView);
    } // setupUi

    void retranslateUi(QWidget *GeneralView)
    {
        GeneralView->setWindowTitle(QCoreApplication::translate("GeneralView", "Form", nullptr));
        radioButton_2->setText(QCoreApplication::translate("GeneralView", "RadioButton", nullptr));
        toolButton->setText(QCoreApplication::translate("GeneralView", "...", nullptr));
        checkBox->setText(QCoreApplication::translate("GeneralView", "CheckBox", nullptr));
        titleLbl->setText(QCoreApplication::translate("GeneralView", "General", nullptr));
        label_2->setText(QCoreApplication::translate("GeneralView", "First:", nullptr));
        radioButton->setText(QCoreApplication::translate("GeneralView", "RadioButton", nullptr));
        commandLinkButton->setText(QCoreApplication::translate("GeneralView", "CommandLinkButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GeneralView: public Ui_GeneralView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERALVIEW_H
