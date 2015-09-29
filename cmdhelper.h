#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

typedef QStringList(*buildCmd_t)(QStringList argList);
typedef QString(*parseResponse_t)(QStringList pmuResponse);

struct cmdEntry {
  cmdEntry(){}
  cmdEntry(buildCmd_t default_buildCmd) : buildCmd(default_buildCmd), parseResponse(NULL) {}
  cmdEntry(buildCmd_t default_buildCmd, parseResponse_t default_parseResponse) : buildCmd(default_buildCmd), parseResponse(default_parseResponse) {}
  buildCmd_t buildCmd;
  parseResponse_t parseResponse;
};

class cmdHelper : public QObject
{
    Q_OBJECT

public:
  explicit cmdHelper(QObject *parent = 0);
  QHash <QString, struct cmdEntry*> m_cmdTable;
  QCompleter *m_cmdCompleter;
  QMap <QString, QString> m_errorResponses;
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);

signals:

public slots:

private:
};

#endif // CMDHELPER_H
