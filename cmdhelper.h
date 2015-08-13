#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

typedef QString(*parser_t)(QString pmuResponse);

struct pmu {
  pmu(){}
  pmu(QString default_cmd) : cmd(default_cmd), parser(NULL) {}
  pmu(QString default_cmd, parser_t default_parser) : cmd(default_cmd), parser(default_parser) {}
  QString cmd;
  parser_t parser;
};

class cmdHelper : public QObject
{
    Q_OBJECT

public:
  explicit cmdHelper(QObject *parent = 0);
  QMap <QString, struct pmu*> cmdMap;
  QCompleter *m_cmdCompleter;
  QString getNextCompletion(void);
  int getCurrentCompletionLength(void);

signals:

public slots:

private:
};

#endif // CMDHELPER_H
