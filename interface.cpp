#include "interface.h"
#include "dllib.h"
#include "globalgateway.h"
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
  if (m_discoveryAgent == NULL) {
    return true;
  } else if (m_discoveryAgent->m_pmuList.isEmpty()) {
    return true;
  } else {
    return false;
  }
}

void interface::slotPMUDiscovered(PMU* pmu) {
  m_pmuUSB = qobject_cast<PMU_USB*>(pmu);
  if (!m_pmuUSB) {
    return;
  }
  emit ftdiConnectionEstablished();
}

void interface::telegesisConnect(QString networkStr) {
  if (networkStr == "") {
    m_networkStr = LRNetwork::s_FactoryDefaultNwidStr;
  } else {
    m_networkStr = networkStr;
  }
  m_panid = LRNetwork::panidFromNwid(m_networkStr);
  m_chmask = LRNetwork::chmaskFromNwid(m_networkStr);
  joinAndConnectWirelessly();
}

void interface::joinAndConnectWirelessly(void) {
  GlobalGateway *ggw = GlobalGateway::Instance();
  Gateway *gw = ggw->getGateway(0);
  if (!gw) return;
  if (m_joined && m_joinedNetworkStr != m_networkStr) {
    //DLDebug(100, DL_FUNC_INFO) << "Leaving" << m_joinedNetworkStr << "to join" << m_networkStr;
    gw->leaveNetwork();
    m_joined = false;
    m_joinedNetworkStr = "";
  }
  if (m_joined || join()) {
    connectToFixture();
    QApplication::processEvents();
  }
}

bool interface::join(void) {
  GlobalGateway *ggw = GlobalGateway::Instance();
  DLResult ret = ggw->joinNetwork(m_panid, m_chmask);
  if (ret == DLLIB_SUCCESS) {
    // successfully joined network
    //report(tr("Joined network %1").arg(m_networkStr));
    m_joined = true;
    m_joinedNetworkStr = m_networkStr;
    return true;
  } else {
    m_joined = false;
    if (ret == DLLIB_USB_DISCONNECTED) {
      //report(tr("USB Wireless Adapter disconnected"));
    } else {
      //report(tr("Failed to join network %1").arg(m_networkStr));
    }
  }
  return false;
}

void interface::connectToFixture(void) {
  DLResult ret;
  GlobalGateway *ggw = GlobalGateway::Instance();
  Gateway *gw = ggw->getGateway(0);
  if (!m_joined)
    return;
  // create a fake PMU bound to m_gateway
  if (m_pmuRemote != NULL) {
    delete m_pmuRemote;
  }
  m_pmuRemote = new PMU_Remote(m_pmu->getEESN(), m_pmu->getEEShortAddress());
  m_pmuRemote->setGateway(gw);
  // attempt to connect to it
  QString ignoreStr;
  m_connected = false;
  ret = m_pmuRemote->getRegister(PMU_FIRMWARE_VERSION, ignoreStr);
  if (ret == DLLIB_USB_DISCONNECTED) {
    //report(tr("USB Wireless Adapter disconnected"));
    ggw->deleteGateway();
    m_joined = false;
  } else if (ret != DLLIB_SUCCESS) {
    //report(tr("Failed to connect to Fixture %1.").arg(m_pmu->getEESNStr()));
  } else {
    //report(tr("Fixture %1 connected.").arg(m_pmu->getEESNStr()));
    m_connected = true;
    emit telegesisConnectionEstablished();
  }
}
