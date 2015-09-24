/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionUseFTDICable;
    QAction *actionUseTelegesisAdapter;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionClear_Output;
    QAction *actionShow_Timestamp;
    QAction *actionShow_PMU_Command;
    QAction *actionSave_Output_to_File;
    QAction *actionPreferences;
    QAction *actionAbout;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTextEdit *outputFeed;
    QLineEdit *commandLine;
    QFrame *line;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(688, 557);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QStringLiteral(""));
        actionUseFTDICable = new QAction(MainWindow);
        actionUseFTDICable->setObjectName(QStringLiteral("actionUseFTDICable"));
        actionUseFTDICable->setCheckable(true);
        actionUseFTDICable->setChecked(true);
        actionUseTelegesisAdapter = new QAction(MainWindow);
        actionUseTelegesisAdapter->setObjectName(QStringLiteral("actionUseTelegesisAdapter"));
        actionUseTelegesisAdapter->setCheckable(true);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        actionClear_Output = new QAction(MainWindow);
        actionClear_Output->setObjectName(QStringLiteral("actionClear_Output"));
        actionShow_Timestamp = new QAction(MainWindow);
        actionShow_Timestamp->setObjectName(QStringLiteral("actionShow_Timestamp"));
        actionShow_Timestamp->setCheckable(true);
        actionShow_Timestamp->setChecked(true);
        actionShow_PMU_Command = new QAction(MainWindow);
        actionShow_PMU_Command->setObjectName(QStringLiteral("actionShow_PMU_Command"));
        actionShow_PMU_Command->setCheckable(true);
        actionSave_Output_to_File = new QAction(MainWindow);
        actionSave_Output_to_File->setObjectName(QStringLiteral("actionSave_Output_to_File"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        actionPreferences->setMenuRole(QAction::PreferencesRole);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setMenuRole(QAction::NoRole);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        outputFeed = new QTextEdit(centralWidget);
        outputFeed->setObjectName(QStringLiteral("outputFeed"));

        gridLayout->addWidget(outputFeed, 2, 0, 1, 1);

        commandLine = new QLineEdit(centralWidget);
        commandLine->setObjectName(QStringLiteral("commandLine"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(commandLine->sizePolicy().hasHeightForWidth());
        commandLine->setSizePolicy(sizePolicy1);
        commandLine->setMinimumSize(QSize(0, 25));
        commandLine->setFrame(true);
        commandLine->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gridLayout->addWidget(commandLine, 0, 0, 1, 1);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        gridLayout->addWidget(line, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 688, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionUseFTDICable);
        menuFile->addAction(actionUseTelegesisAdapter);
        menuFile->addSeparator();
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionDisconnect);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Output_to_File);
        menuFile->addSeparator();
        menuFile->addAction(actionPreferences);
        menuFile->addAction(actionAbout);
        menuEdit->addAction(actionClear_Output);
        menuEdit->addAction(actionShow_Timestamp);
        menuEdit->addAction(actionShow_PMU_Command);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        actionUseFTDICable->setText(QApplication::translate("MainWindow", "Use FTDI Cable", 0));
        actionUseTelegesisAdapter->setText(QApplication::translate("MainWindow", "Use Telegesis Adapter", 0));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
#ifndef QT_NO_TOOLTIP
        actionConnect->setToolTip(QApplication::translate("MainWindow", "Connect to Device", 0));
#endif // QT_NO_TOOLTIP
        actionConnect->setShortcut(QApplication::translate("MainWindow", "Ctrl+K", 0));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
#ifndef QT_NO_TOOLTIP
        actionDisconnect->setToolTip(QApplication::translate("MainWindow", "Disconnect from Device", 0));
#endif // QT_NO_TOOLTIP
        actionDisconnect->setShortcut(QApplication::translate("MainWindow", "Ctrl+K", 0));
        actionClear_Output->setText(QApplication::translate("MainWindow", "Clear Output", 0));
        actionShow_Timestamp->setText(QApplication::translate("MainWindow", "Show Timestamp", 0));
        actionShow_PMU_Command->setText(QApplication::translate("MainWindow", "Show PMU Command", 0));
        actionSave_Output_to_File->setText(QApplication::translate("MainWindow", "Save Output to File", 0));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        commandLine->setPlaceholderText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
