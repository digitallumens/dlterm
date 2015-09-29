/********************************************************************************
** Form generated from reading UI file 'preferencesdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_preferencesDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *network_label;
    QComboBox *netGroup_comboBox;
    QComboBox *netFreq_comboBox;
    QLabel *serialNumber_label;
    QSpacerItem *horizontalSpacer;
    QLineEdit *serialNumber_lineEdit;

    void setupUi(QDialog *preferencesDialog)
    {
        if (preferencesDialog->objectName().isEmpty())
            preferencesDialog->setObjectName(QStringLiteral("preferencesDialog"));
        preferencesDialog->resize(370, 120);
        buttonBox = new QDialogButtonBox(preferencesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(190, 90, 171, 20));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(preferencesDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 351, 71));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        network_label = new QLabel(gridLayoutWidget);
        network_label->setObjectName(QStringLiteral("network_label"));
        network_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(network_label, 1, 0, 1, 1);

        netGroup_comboBox = new QComboBox(gridLayoutWidget);
        netGroup_comboBox->setObjectName(QStringLiteral("netGroup_comboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(netGroup_comboBox->sizePolicy().hasHeightForWidth());
        netGroup_comboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(netGroup_comboBox, 1, 1, 1, 1);

        netFreq_comboBox = new QComboBox(gridLayoutWidget);
        netFreq_comboBox->setObjectName(QStringLiteral("netFreq_comboBox"));
        sizePolicy.setHeightForWidth(netFreq_comboBox->sizePolicy().hasHeightForWidth());
        netFreq_comboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(netFreq_comboBox, 1, 2, 1, 1);

        serialNumber_label = new QLabel(gridLayoutWidget);
        serialNumber_label->setObjectName(QStringLiteral("serialNumber_label"));

        gridLayout->addWidget(serialNumber_label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        serialNumber_lineEdit = new QLineEdit(gridLayoutWidget);
        serialNumber_lineEdit->setObjectName(QStringLiteral("serialNumber_lineEdit"));

        gridLayout->addWidget(serialNumber_lineEdit, 0, 1, 1, 1);


        retranslateUi(preferencesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), preferencesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), preferencesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(preferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *preferencesDialog)
    {
        preferencesDialog->setWindowTitle(QApplication::translate("preferencesDialog", "Dialog", 0));
        network_label->setText(QApplication::translate("preferencesDialog", "Network:", 0));
        serialNumber_label->setText(QApplication::translate("preferencesDialog", "Serial Number", 0));
    } // retranslateUi

};

namespace Ui {
    class preferencesDialog: public Ui_preferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
