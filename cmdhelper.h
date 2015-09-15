#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

typedef QStringList(*cmd_t)(QStringList argList);
typedef QString(*parser_t)(QStringList pmuResponse);

struct pmu {
  pmu(){}
  pmu(cmd_t default_cmd) : cmd(default_cmd), parser(NULL) {}
  pmu(cmd_t default_cmd, parser_t default_parser) : cmd(default_cmd), parser(default_parser) {}
  cmd_t cmd;
  parser_t parser;
};

class cmdHelper : public QObject
{
    Q_OBJECT

public:
  explicit cmdHelper(QObject *parent = 0);
  QHash <QString, struct pmu*> m_cmdTable;
  QCompleter *m_cmdCompleter;
  QMap <QString, QString> m_errorResponses;
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);

signals:

public slots:

private:
};

#endif // CMDHELPER_H
