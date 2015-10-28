#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

class cmdHelper : public QObject
{
    Q_OBJECT

public:
  explicit cmdHelper(QObject *parent = 0);
  QCompleter *m_cmdCompleter;
  exe_t *getExe(QString cmd);
  QString parseError(QString pmuResponse);
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);
  QStringList help(void);

signals:

public slots:

private:
  typedef QStringList(*exe_t)(QStringList argList, interface *io);
  QHash <QString, cmd_t> m_cmdTable;
  QMap <QString, QString> m_errorResponses;
};

#endif // CMDHELPER_H
