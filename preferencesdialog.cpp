#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

preferencesDialog::preferencesDialog(LRModel *model, QList<LRZone*> *zoneList, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::preferencesDialog),
  m_model(model),
  m_zoneList(zoneList)
{
  ui->setupUi(this);
}

preferencesDialog::~preferencesDialog()
{
  delete ui;
}
