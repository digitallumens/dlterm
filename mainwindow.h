#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>

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
  void custom_on_lineEdit_returnPressed();

private:
  Ui::MainWindow *ui;
  void setupApp();
  bool eventFilter(QObject *target, QEvent *event);
};

#endif // MAINWINDOW_H
