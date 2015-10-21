#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

typedef QStringList(*buildCmd_t)(QStringList argList);
typedef QStringList(*parseResponse_t)(QStringList pmuResponse);

struct cmdEntry {
  cmdEntry(){}
  cmdEntry(buildCmd_t def_buildCmd) : buildCmd(def_buildCmd), parseResponse(NULL) {}
  cmdEntry(buildCmd_t def_buildCmd, parseResponse_t def_parseResponse) : buildCmd(def_buildCmd), parseResponse(def_parseResponse) {}
  buildCmd_t buildCmd;
  parseResponse_t parseResponse;
};

class cmdHelper : public QObject
{
    Q_OBJECT

public:
  explicit cmdHelper(QObject *parent = 0);
  QCompleter *m_cmdCompleter;
  struct cmdEntry *getCmdEntry(QString cmd);
  QString parseError(QString pmuResponse);
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);
  QStringList help(void);

signals:

public slots:

private:
  QHash <QString, struct cmdEntry*> m_cmdTable;
  QMap <QString, QString> m_errorResponses;
};

#endif // CMDHELPER_H
