#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

#include "cmd.h"

class cmdHelper : public QObject
{
    Q_OBJECT
public:
  explicit cmdHelper(QObject *parent = 0);
  QCompleter *m_cmdCompleter;
  cmd * findCmd(QString cmd);
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);

signals:

public slots:

private:
  QList<cmd *> m_cmds;
};

#endif // CMDHELPER_H
