#ifndef CMDIO_H
#define CMDIO_H

#include <QObject>
#include "interface.h"
#include "cmdhelper.h"

class cmdIO : public QObject
{
  Q_OBJECT
public:
  explicit cmdIO(QObject *parent = 0);
  QString processUserRequest(QString *request, bool showPmuCommand, interface *io, cmdHelper *helper);
  QString buildPrompt(bool showTimestamp);
signals:

public slots:
private:
  QString buildParsedResponse(struct cmdEntry * cmdEntry, QStringList cmdList, QStringList pmuResponseList, bool showPmuCommand);
  QString buildUnparsedResponse(QStringList cmdList, QStringList pmuResponseList, bool showPmuCommand, cmdHelper *helper);
  QString buildBasicCommandResponse(QStringList pmuResponseList, cmdHelper *helper);
  QString buildAppHelp(cmdHelper *helper);
  QString buildUserInputError(QStringList cmdList);
};

#endif // CMDIO_H
