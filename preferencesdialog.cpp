#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "dllib.h"

preferencesDialog::preferencesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::preferencesDialog)
{
  ui->setupUi(this);
  createNetworkCombos(false);
  ui->serialNumber_lineEdit->setPlaceholderText("0400BEEF");
}

preferencesDialog::~preferencesDialog()
{
  delete ui;
}

void preferencesDialog::createNetworkCombos(const bool encryptionOn) {
    QString str;
    ui->netGroup_comboBox->clear();
    ui->netGroup_comboBox->addItem(tr("FDN"));
    for(int i = 0; i < LR_NETWORK_GROUPS; i++) {
        str = QChar(i + 'A');
        ui->netGroup_comboBox->addItem(str);
    }
    ui->netFreq_comboBox->clear();
    for(int i = 0; i < LR_NETWORK_FREQS; i++) {
        str = QString("%1").arg(i+1, 2, 10, QChar('0'));
        if (encryptionOn) {
            str += "E";
        }
        ui->netFreq_comboBox->addItem(str);
    }
}

void preferencesDialog::accept() {
  bool ok;
  m_serialNumber = ui->serialNumber_lineEdit->text().toULong(&ok, 16);
  if (ui->netGroup_comboBox->currentIndex() == 0) {
    m_networkStr = LRNetwork::s_FactoryDefaultNwidStr;
  } else {
    m_networkStr = ui->netGroup_comboBox->currentText() + ui->netFreq_comboBox->currentText();
  }
  QDialog::accept();
}
