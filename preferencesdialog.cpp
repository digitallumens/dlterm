#include "preferencesdialog.h"
#include "ui_connectionDialog.h"

connectionDialog::connectionDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::connectionDialog)
{
  ui->setupUi(this);
}

connectionDialog::~connectionDialog()
{
  delete ui;
}
