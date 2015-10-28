#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cmdhelper.h"
#include "cmdhistory.h"
#include "preferencesdialog.h"

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
  void on_actionConnect_Using_FTDI_triggered();
  void on_actionConnect_Using_Telegesis_triggered();
  void on_actionDisconnect_triggered();
  void on_actionClear_Output_triggered();
  void on_actionSave_Output_to_File_triggered();
  void on_actionPreferences_triggered();
  void on_actionAbout_triggered();
  void on_connectionEstablished();
  void on_connectionStatusChanged(QString status);

private:
  Ui::MainWindow *ui;
  bool eventFilter(QObject *target, QEvent *event);
  void checkForInstalledKexts(void);
  QString processUserRequest(QString *request);
  QString buildPrompt(void);
  QString buildAppHelp(void);
  cmdHelper *m_cmdHelper;
  cmdHistory *m_cmdHistory;
  interface *m_interface;
  preferencesDialog *m_preferencesDialog;
};

#endif // MAINWINDOW_H
