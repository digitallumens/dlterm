#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

class interface;

typedef QStringList(*cmdHandler_t)(QStringList argList, interface *io);

class cmdHelper : public QObject
{
    Q_OBJECT

public:
  explicit cmdHelper(QObject *parent = 0);
  QCompleter *m_cmdCompleter;
  cmdHandler_t getCmdHandler(QString request);
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);
  QStringList help(void);

signals:

public slots:

private:
  QHash <QString, cmdHandler_t> m_cmdTable;
};

#endif // CMDHELPER_H
