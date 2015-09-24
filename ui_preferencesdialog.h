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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_preferencesDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *_zoneNetwork_label;
    QComboBox *zoneNetworkGroup_comboBox;
    QComboBox *zoneNetworkFreq_comboBox;

    void setupUi(QDialog *preferencesDialog)
    {
        if (preferencesDialog->objectName().isEmpty())
            preferencesDialog->setObjectName(QStringLiteral("preferencesDialog"));
        preferencesDialog->resize(251, 103);
        buttonBox = new QDialogButtonBox(preferencesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 70, 231, 21));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(preferencesDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(9, 9, 231, 41));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        _zoneNetwork_label = new QLabel(gridLayoutWidget);
        _zoneNetwork_label->setObjectName(QStringLiteral("_zoneNetwork_label"));
        _zoneNetwork_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(_zoneNetwork_label, 0, 0, 1, 1);

        zoneNetworkGroup_comboBox = new QComboBox(gridLayoutWidget);
        zoneNetworkGroup_comboBox->setObjectName(QStringLiteral("zoneNetworkGroup_comboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(zoneNetworkGroup_comboBox->sizePolicy().hasHeightForWidth());
        zoneNetworkGroup_comboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(zoneNetworkGroup_comboBox, 0, 1, 1, 1);

        zoneNetworkFreq_comboBox = new QComboBox(gridLayoutWidget);
        zoneNetworkFreq_comboBox->setObjectName(QStringLiteral("zoneNetworkFreq_comboBox"));
        sizePolicy.setHeightForWidth(zoneNetworkFreq_comboBox->sizePolicy().hasHeightForWidth());
        zoneNetworkFreq_comboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(zoneNetworkFreq_comboBox, 0, 2, 1, 1);


        retranslateUi(preferencesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), preferencesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), preferencesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(preferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *preferencesDialog)
    {
        preferencesDialog->setWindowTitle(QApplication::translate("preferencesDialog", "Dialog", 0));
        _zoneNetwork_label->setText(QApplication::translate("preferencesDialog", "Network:", 0));
    } // retranslateUi

};

namespace Ui {
    class preferencesDialog: public Ui_preferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
