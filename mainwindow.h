#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cmdhelper.h"
#include "cmdhistory.h"
#include "solarized.h"

class DiscoveryAgent;
class PMU;
class PMU_USB;

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
  void on_actionClear_Output_triggered();
  void on_actionSave_Output_to_File_triggered();
  void on_actionPreferences_triggered();
  void on_actionAbout_triggered();

private:
  Ui::MainWindow *ui;
  bool eventFilter(QObject *target, QEvent *event);
  cmdHelper *m_cmdHelper;
  cmdHistory *m_cmdHistory;
  solarized *m_solarized;
  PMU_USB *m_pmuUSB;
  DiscoveryAgent *m_discoveryAgent;
  QString queryPmu(QStringList cmdList, QStringList *responseList);
  QString processUserRequest(QString *request);
  QString buildPrompt(void);

private slots:
  void slotPMUDiscovered(PMU* pmu);
};

#endif // MAINWINDOW_H
