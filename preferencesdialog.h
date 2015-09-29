#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class preferencesDialog;
}

class preferencesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit preferencesDialog(QWidget *parent = 0);
  ~preferencesDialog();
  void createNetworkCombos(const bool encryptionOn);
  QString m_networkStr;
  quint32 m_serialNumber;

private slots:
  void accept();

private:
  Ui::preferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
