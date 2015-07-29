#ifndef CMD_H
#define CMD_H

#include <QObject>

class cmd : public QObject
{
  Q_OBJECT
public:
  typedef QString(*parsePmuResponse_t)(QString);
  explicit cmd(QObject *parent = 0, QString helperCmd = NULL, QString pmuCmd = NULL, parsePmuResponse_t parsePmuResponse = NULL);
  QString m_helperCmd;
  QString m_pmuCmd;
  parsePmuResponse_t m_parsePmuResponse;

signals:

public slots:
};

#endif // CMD_H
