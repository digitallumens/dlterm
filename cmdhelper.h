#ifndef CMDHELPER_H
#define CMDHELPER_H

#include <QObject>
#include <QCompleter>

class cmdHelper : public QObject
{
    Q_OBJECT
public:
    explicit cmdHelper(QObject *parent = 0);
    QCompleter *completer;

signals:

public slots:

private:
  bool eventFilter(QObject *target, QEvent *event);  
};

#endif // CMDHELPER_H
