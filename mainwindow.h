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
  void on_lineEdit_returnPressed();
  void on_actionUseFTDICable_triggered();
  void on_actionUseTelegesisAdapter_triggered();
  void on_actionConnect_triggered();
  void on_actionDisconnect_triggered();
  void on_actionConfigure_triggered();

private:
  Ui::MainWindow *ui;
  void setupApp();
};

#endif // MAINWINDOW_H
