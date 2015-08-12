#include "cmdhistory.h"

cmdHistory::cmdHistory(QObject *parent) : QObject(parent),
  m_history(NULL),
  m_index(0) {
}

void cmdHistory::append(QString cmd) {
  // append to command history
  if ((m_history.count() == 0) || (cmd != m_history.at(m_history.count() - 1))) {
    m_history << cmd;
  }
  // reset history pointer
  m_index = m_history.count() - 1;
}

QString cmdHistory::scrollBack(void) {
  QString lastCmd = NULL;
  if (m_history.count() > 0) {
    lastCmd = m_history.at(m_index);
    if (m_index > 0) {
      m_index--;
    }
  }
  return lastCmd;
}

QString cmdHistory::scrollForward(void) {
  QString nextCmd = NULL;
  if (m_history.count() > 0) {
    nextCmd = m_history.at(m_index);
    if ((m_index + 1) < m_history.count()) {
      m_index++;
    }
  }
  return nextCmd;
}
