/********************************************************************************
** Form generated from reading UI file 'bifurcationdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIFURCATIONDIALOG_H
#define UI_BIFURCATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BifurcationDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listWidget;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_pushButton;
    QPushButton *join_pushButton;

    void setupUi(QDialog *BifurcationDialog)
    {
        if (BifurcationDialog->objectName().isEmpty())
            BifurcationDialog->setObjectName(QStringLiteral("BifurcationDialog"));
        BifurcationDialog->resize(387, 255);
        verticalLayout = new QVBoxLayout(BifurcationDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(BifurcationDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        listWidget = new QListWidget(BifurcationDialog);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        label_2 = new QLabel(BifurcationDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancel_pushButton = new QPushButton(BifurcationDialog);
        cancel_pushButton->setObjectName(QStringLiteral("cancel_pushButton"));

        horizontalLayout->addWidget(cancel_pushButton);

        join_pushButton = new QPushButton(BifurcationDialog);
        join_pushButton->setObjectName(QStringLiteral("join_pushButton"));

        horizontalLayout->addWidget(join_pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(BifurcationDialog);

        QMetaObject::connectSlotsByName(BifurcationDialog);
    } // setupUi

    void retranslateUi(QDialog *BifurcationDialog)
    {
        BifurcationDialog->setWindowTitle(QApplication::translate("BifurcationDialog", "Bifurcated Network", 0));
        label->setText(QApplication::translate("BifurcationDialog", "The network you are joining appears to be bifurcated. Please select which segment you wish to join and click Join, or Cancel.", 0));
        label_2->setText(QApplication::translate("BifurcationDialog", "* Note: It is important NOT to have fixtures joined to two different networks with the same ID. You should move any fixtures on this network to a new network.", 0));
        cancel_pushButton->setText(QApplication::translate("BifurcationDialog", "Cancel", 0));
        join_pushButton->setText(QApplication::translate("BifurcationDialog", "Join", 0));
    } // retranslateUi

};

namespace Ui {
    class BifurcationDialog: public Ui_BifurcationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIFURCATIONDIALOG_H
