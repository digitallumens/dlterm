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
    QCompleter *cmdCompleter;
    QList<cmd *> m_cmds;

signals:

public slots:

private:
  bool eventFilter(QObject *target, QEvent *event);
};

#endif // CMDHELPER_H
