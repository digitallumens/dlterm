/********************************************************************************
** Form generated from reading UI file 'emberdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMBERDIALOG_H
#define UI_EMBERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmberDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EmberDialog)
    {
        if (EmberDialog->objectName().isEmpty())
            EmberDialog->setObjectName(QStringLiteral("EmberDialog"));
        verticalLayout = new QVBoxLayout(EmberDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(EmberDialog);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(EmberDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EmberDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), EmberDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EmberDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EmberDialog);
    } // setupUi

    void retranslateUi(QDialog *EmberDialog)
    {
        EmberDialog->setWindowTitle(QApplication::translate("EmberDialog", "USB Wireless Adapter", 0));
        label->setText(QApplication::translate("EmberDialog", "Insert a Digital Lumens USB Wireless Adapter into a USB port on this computer.", 0));
    } // retranslateUi

};

namespace Ui {
    class EmberDialog: public Ui_EmberDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMBERDIALOG_H
