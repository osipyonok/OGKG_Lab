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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
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
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OGKG_Lab_GuiClass)
    {
        if (OGKG_Lab_GuiClass->objectName().isEmpty())
            OGKG_Lab_GuiClass->setObjectName(QStringLiteral("OGKG_Lab_GuiClass"));
        OGKG_Lab_GuiClass->resize(875, 625);
        centralWidget = new QWidget(OGKG_Lab_GuiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PlotWidget = new Plot(centralWidget);
        PlotWidget->setObjectName(QStringLiteral("PlotWidget"));
        PlotWidget->setGeometry(QRect(20, 20, 501, 501));
        OGKG_Lab_GuiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(OGKG_Lab_GuiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 875, 26));
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
    } // retranslateUi

};

namespace Ui {
    class OGKG_Lab_GuiClass: public Ui_OGKG_Lab_GuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OGKG_LAB_GUI_H
