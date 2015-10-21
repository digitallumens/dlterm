#include "cmdio.h"
#include "solarized.h"
#include <QDate>
#include <QTime>

cmdIO::cmdIO(QObject *parent) : QObject(parent) {
}

QString cmdIO::processUserRequest(QString *request, bool showPmuCommand, interface *io, cmdHelper *helper) {
  QString cmd;
  QStringList argList;
  QStringList cmdList;
  QStringList pmuResponseList;
  QStringList parsedResponseList;
  QString response;
  if (request->startsWith("help")) {
    solarized::setTextColor(request, solarized::SOLAR_MAGENTA);
    return buildAppHelp(helper);
  }
  if (request->contains(" ")) {
    // helper command format "verb object argList"
    // ex: set serialNumber 0400DEAD
    argList = request->split(" ");
    cmd = argList.at(0) + " " + argList.at(1);
    argList.removeFirst();
    argList.removeFirst();
  } else {
    // doesn't look like a helper command...
    cmd = *request;
  }
  // find the associated helper entry
  struct cmdEntry *cmdEntry = helper->getCmdEntry(cmd);
  if (cmdEntry != NULL) {
    // translate helper command
    cmdList = cmdEntry->buildCmd(argList);
    // command translator returned an error
    if (cmdList.at(0).startsWith("ERROR")) {
      return buildUserInputError(cmdList);
    }
    solarized::setTextColor(request, solarized::SOLAR_YELLOW);
    // send commands & get responses
    io->queryPmu(cmdList, &pmuResponseList);
    // parse responses
    if (cmdEntry->parseResponse != NULL) {
      response = buildParsedResponse(cmdEntry, cmdList, pmuResponseList, showPmuCommand);
    } else {
      response = buildUnparsedResponse(cmdList, pmuResponseList, showPmuCommand, helper);
    }
  } else {
    // not a helper command
    cmdList << *request;
    solarized::setTextColor(request, solarized::SOLAR_BASE_01);
    // send commands & get responses
    io->queryPmu(cmdList, &pmuResponseList);
    // no parser
    response = buildBasicCommandResponse(pmuResponseList, helper);
  }
  return response;
}

QString cmdIO::buildPrompt(bool showTimestamp) {
  QString prompt;
  QString timestamp;
  if (showTimestamp) {
    timestamp = QDate::currentDate().toString(Qt::ISODate) + " " + QTime::currentTime().toString(Qt::ISODate);
    prompt = timestamp + " > ";
  } else {
    prompt = " > ";
  }
  solarized::setTextColor(&prompt, solarized::SOLAR_BASE_01);
  return prompt;
}

QString cmdIO::buildParsedResponse(struct cmdEntry * cmdEntry, QStringList cmdList, QStringList pmuResponseList, bool showPmuCommand) {
  QString preamble;
  QString response;
  QStringList parsedResponseList;
  if (showPmuCommand) {
    preamble = QString("PMU command: %1").arg(cmdList.at(0));
    if (cmdList.length() > 1) {
      preamble += QString(" [plus %1 others...]").arg(cmdList.length() - 1);
    }
    preamble += "<br>";
    preamble += QString("PMU response: %1").arg(pmuResponseList.at(0));
    if (pmuResponseList.length() > 1) {
      preamble += QString(" [plus %1 others...]").arg(pmuResponseList.length() - 1);
    }
    preamble += "<br>";
    solarized::setTextColor(&preamble, solarized::SOLAR_BASE_01);
  }
  // use parser
  parsedResponseList = cmdEntry->parseResponse(pmuResponseList);
  // flatten
  foreach(QString r, parsedResponseList) {
    if (r.contains("ERROR")) {
      solarized::setTextColor(&r, solarized::SOLAR_RED);
    } else {
      solarized::setTextColor(&r, solarized::SOLAR_BLUE);
    }
    response.append(r + "<br>");
  }
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return preamble + response;
}

QString cmdIO::buildUnparsedResponse(QStringList cmdList, QStringList pmuResponseList, bool showPmuCommand, cmdHelper *helper) {
  QString preamble;
  QString response;
  if (showPmuCommand) {
    preamble = QString("PMU command: %1").arg(cmdList.at(0));
    if (cmdList.length() > 1) {
      preamble += QString(" [plus %1 others...]").arg(cmdList.length() - 1);
    }
    preamble += "<br>";
    solarized::setTextColor(&preamble, solarized::SOLAR_BASE_01);
  }
  // no parser available, flatten responses
  foreach (QString s, pmuResponseList) {
    response.append(s);
  }
  if (response.startsWith("OK")) {
    solarized::setTextColor(&response, solarized::SOLAR_GREEN);
  } else if (response.startsWith("ERROR")) {
    response = helper->parseError(response);
    solarized::setTextColor(&response, solarized::SOLAR_RED);
  } else {
    solarized::setTextColor(&response, solarized::SOLAR_VIOLET);
  }
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return preamble + response;
}

QString cmdIO::buildBasicCommandResponse(QStringList pmuResponseList, cmdHelper *helper) {
  QString response;
  // not a helper command, flatten responses
  foreach (QString s, pmuResponseList) {
    response.append(s);
  }
  if (response.startsWith("OK")) {
    solarized::setTextColor(&response, solarized::SOLAR_GREEN);
  } else if (response.startsWith("ERROR")) {
    response = helper->parseError(response);
    solarized::setTextColor(&response, solarized::SOLAR_RED);
  } else {
    solarized::setTextColor(&response, solarized::SOLAR_VIOLET);
  }
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}

QString cmdIO::buildAppHelp(cmdHelper *helper) {
  QString response;
  QStringList helpList = helper->help();
  foreach(QString h, helpList) {
    response.append(h + "<br>");
  }
  solarized::setTextColor(&response, solarized::SOLAR_BASE_01);
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}

QString cmdIO::buildUserInputError(QStringList cmdList) {
  QString response = cmdList.at(0);
  solarized::setTextColor(&response, solarized::SOLAR_RED);
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}
