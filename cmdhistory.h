#ifndef CMDHISTORY_H
#define CMDHISTORY_H

#include <QObject>
#include <QLineEdit>

class cmdHistory : public QObject
{
  Q_OBJECT
public:
  explicit cmdHistory(QObject *parent = 0);
  void append(QString cmd);
  QString scrollBack(void);
  QString scrollForward(void);

signals:

public slots:

private:
  QStringList m_history;
  int m_index;
};

#endif // CMDHISTORY_H
