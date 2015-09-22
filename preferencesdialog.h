#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class connectionDialog;
}

class connectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit connectionDialog(QWidget *parent = 0);
  ~connectionDialog();

private:
  Ui::connectionDialog *ui;
};

#endif // CONNECTIONDIALOG_H
