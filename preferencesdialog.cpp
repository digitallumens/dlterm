#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

preferencesDialog::preferencesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::preferencesDialog)
{
  ui->setupUi(this);
}

preferencesDialog::~preferencesDialog()
{
  delete ui;
}
