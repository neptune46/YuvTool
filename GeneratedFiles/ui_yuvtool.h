/********************************************************************************
** Form generated from reading UI file 'yuvtool.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YUVTOOL_H
#define UI_YUVTOOL_H

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

QT_BEGIN_NAMESPACE

class Ui_YuvToolClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *YuvToolClass)
    {
        if (YuvToolClass->objectName().isEmpty())
            YuvToolClass->setObjectName(QStringLiteral("YuvToolClass"));
        YuvToolClass->resize(600, 400);
        menuBar = new QMenuBar(YuvToolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        YuvToolClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(YuvToolClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        YuvToolClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(YuvToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        YuvToolClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(YuvToolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        YuvToolClass->setStatusBar(statusBar);

        retranslateUi(YuvToolClass);

        QMetaObject::connectSlotsByName(YuvToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *YuvToolClass)
    {
        YuvToolClass->setWindowTitle(QApplication::translate("YuvToolClass", "YuvTool", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class YuvToolClass: public Ui_YuvToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YUVTOOL_H
