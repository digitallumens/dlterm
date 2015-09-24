#include "interface.h"
#include "dllib.h"
#include <QApplication>

interface::interface(QObject *parent) : QObject(parent)
{

}

void interface::ftdiConnect(void) {
  m_discoveryAgent = new DiscoveryAgent();
  connect(m_discoveryAgent, SIGNAL(signalPMUDiscovered(PMU*)), this, SLOT(slotPMUDiscovered(PMU*)));
  Q_CHECK_PTR(m_discoveryAgent);
  m_discoveryAgent->clearLists();
  do {
    m_discoveryAgent->discoverPMU_usbs();
    MSLEEP(500);
    QApplication::processEvents();
  } while (m_discoveryAgent->m_pmuList.isEmpty());
}

void interface::ftdiDisconnect(void) {
  m_discoveryAgent->clearLists();
  if (m_discoveryAgent) {
    delete m_discoveryAgent;
  }
}

QString interface::ftdiQueryPmu(QStringList cmdList, QStringList *responseList) {
  QString response;
  foreach (const QString &cmd, cmdList) {
    // figure out the length NOT including the space
    int len = cmd.length();
    int i = cmd.indexOf(' ');
    if (i != -1) {
      len = cmd.left(i).length();
    }
    if (m_pmuUSB->issueCommand(cmd, response, len) != DLLIB_SUCCESS) {
      // return error
      return response;
    } else {
      responseList->append(response);
    }
  }
  return NULL;
}

bool interface::ftdiIsDisconnected(void) {
  return m_discoveryAgent->m_pmuList.isEmpty();
}

void interface::slotPMUDiscovered(PMU* pmu) {
  m_pmuUSB = qobject_cast<PMU_USB*>(pmu);
  if (!m_pmuUSB) {
    return;
  }
  emit ftdiConnectionEstablished();
}
