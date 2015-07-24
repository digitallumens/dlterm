#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cmdhelper.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void on_actionUseFTDICable_triggered();
  void on_actionUseTelegesisAdapter_triggered();
  void on_actionConnect_triggered();
  void on_actionDisconnect_triggered();
  void on_actionConfigure_triggered();
  void on_actionClear_Output_triggered();

private:
  Ui::MainWindow *ui;
  bool eventFilter(QObject *target, QEvent *event);
  cmdHelper *m_cmdHelper;
  quint8 m_cmdHistoryIndex;
  QStringList m_cmdHistory;
};

#endif // MAINWINDOW_H
