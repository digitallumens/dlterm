#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class LRModel;
class LRZone;

namespace Ui {
class preferencesDialog;
}

class preferencesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit preferencesDialog(LRModel *model, QList<LRZone*> *zoneList, QWidget *parent = 0);
  ~preferencesDialog();

private:
  Ui::preferencesDialog *ui;
  LRModel *m_model;
  QList<LRZone*> *m_zoneList;
};

#endif // PREFERENCESDIALOG_H
