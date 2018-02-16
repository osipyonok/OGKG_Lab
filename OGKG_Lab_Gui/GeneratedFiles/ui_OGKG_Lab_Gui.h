/********************************************************************************
** Form generated from reading UI file 'OGKG_Lab_Gui.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OGKG_LAB_GUI_H
#define UI_OGKG_LAB_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "Plot.h"

QT_BEGIN_NAMESPACE

class Ui_OGKG_Lab_GuiClass
{
public:
    QWidget *centralWidget;
    Plot *PlotWidget;
    QPushButton *runAlgorithm;
    QLabel *label;
    QSpinBox *newPointsAmount;
    QPushButton *generateButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OGKG_Lab_GuiClass)
    {
        if (OGKG_Lab_GuiClass->objectName().isEmpty())
            OGKG_Lab_GuiClass->setObjectName(QStringLiteral("OGKG_Lab_GuiClass"));
        OGKG_Lab_GuiClass->resize(1199, 910);
        centralWidget = new QWidget(OGKG_Lab_GuiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PlotWidget = new Plot(centralWidget);
        PlotWidget->setObjectName(QStringLiteral("PlotWidget"));
        PlotWidget->setGeometry(QRect(20, 20, 831, 821));
        runAlgorithm = new QPushButton(centralWidget);
        runAlgorithm->setObjectName(QStringLiteral("runAlgorithm"));
        runAlgorithm->setGeometry(QRect(1030, 240, 93, 28));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(860, 45, 151, 21));
        newPointsAmount = new QSpinBox(centralWidget);
        newPointsAmount->setObjectName(QStringLiteral("newPointsAmount"));
        newPointsAmount->setGeometry(QRect(1010, 40, 61, 31));
        generateButton = new QPushButton(centralWidget);
        generateButton->setObjectName(QStringLiteral("generateButton"));
        generateButton->setGeometry(QRect(1080, 40, 93, 31));
        OGKG_Lab_GuiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(OGKG_Lab_GuiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1199, 26));
        OGKG_Lab_GuiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(OGKG_Lab_GuiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        OGKG_Lab_GuiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(OGKG_Lab_GuiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        OGKG_Lab_GuiClass->setStatusBar(statusBar);

        retranslateUi(OGKG_Lab_GuiClass);

        QMetaObject::connectSlotsByName(OGKG_Lab_GuiClass);
    } // setupUi

    void retranslateUi(QMainWindow *OGKG_Lab_GuiClass)
    {
        OGKG_Lab_GuiClass->setWindowTitle(QApplication::translate("OGKG_Lab_GuiClass", "OGKG_Lab_Gui", nullptr));
        runAlgorithm->setText(QApplication::translate("OGKG_Lab_GuiClass", "Run", nullptr));
        label->setText(QApplication::translate("OGKG_Lab_GuiClass", "\320\222\320\262\320\265\320\264i\321\202\321\214 \320\272i\320\273\321\214\320\272i\321\201\321\202\321\214 \321\202\320\276\321\207\320\276\320\272:", nullptr));
        generateButton->setText(QApplication::translate("OGKG_Lab_GuiClass", "\320\227\320\263\320\265\320\275\320\265\321\200\321\203\320\262\320\260\321\202\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OGKG_Lab_GuiClass: public Ui_OGKG_Lab_GuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OGKG_LAB_GUI_H
