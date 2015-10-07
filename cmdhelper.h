#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

typedef QStringList(*buildCmd_t)(QStringList argList);
typedef QStringList(*parseResponse_t)(QStringList pmuResponse);
typedef QStringList(*getHelp_t)(void);

struct cmdEntry {
  cmdEntry(){}
  cmdEntry(buildCmd_t def_buildCmd, getHelp_t def_getHelp) : buildCmd(def_buildCmd), parseResponse(NULL), getHelp(def_getHelp) {}
  cmdEntry(buildCmd_t def_buildCmd, parseResponse_t def_parseResponse, getHelp_t def_getHelp) : buildCmd(def_buildCmd), parseResponse(def_parseResponse), getHelp(def_getHelp) {}
  buildCmd_t buildCmd;
  parseResponse_t parseResponse;
  getHelp_t getHelp;
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

signals:

public slots:

private:
  QHash <QString, struct cmdEntry*> m_cmdTable;
  QMap <QString, QString> m_errorResponses;
};

#endif // CMDHELPER_H
