#include "interface.h"
#include "dllib.h"
#include "globalgateway.h"
#include <QApplication>

interface::interface(QObject *parent) : QObject(parent),
  m_pmuRemote(NULL),
  m_pmuUSB(NULL),
  m_joined(false),
  m_connected(false),
  m_closed(false) {
}

void interface::configure(QString networkStr, quint32 serialNumber) {
  m_networkStr = networkStr;
  m_serialNumber = serialNumber;
}

void interface::connectFTDI(void) {
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

void interface::disconnect(void) {
  if (m_discoveryAgent) {
    m_discoveryAgent->clearLists();
    delete m_discoveryAgent;
  }
  GlobalGateway::Instance()->leaveAnyNetwork();
  m_connected = false;
  emit connectionStatusChanged("Disconnected");
}

bool interface::isConnected(void) {
  return m_connected;
}

void interface::slotPMUDiscovered(PMU* pmu) {
  m_pmuUSB = qobject_cast<PMU_USB*>(pmu);
  if (!m_pmuUSB) {
    m_connected = false;
    return;
  }
  m_connected = true;
  emit connectionStatusChanged(QString("FTDI connection established"));
  emit connectionEstablished();
}

void interface::connectTelegesis(void) {
  m_panid = LRNetwork::panidFromNwid(m_networkStr);
  m_chmask = LRNetwork::chmaskFromNwid(m_networkStr);
  joinAndConnectWirelessly();
}

void interface::joinAndConnectWirelessly(void) {
  GlobalGateway *ggw = GlobalGateway::Instance();
  Gateway *gw = ggw->getGateway(0);
  if (!gw) return;
  if (m_joined && m_joinedNetworkStr != m_networkStr) {
    emit connectionStatusChanged(QString("Leaving %1 to join %2").arg(m_joinedNetworkStr).arg(m_networkStr));
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
    emit connectionStatusChanged(QString("Joined network %1").arg(m_networkStr));
    m_joined = true;
    m_joinedNetworkStr = m_networkStr;
    return true;
  } else {
    m_joined = false;
    if (ret == DLLIB_USB_DISCONNECTED) {
      emit connectionStatusChanged("USB Wireless Adapter disconnected");
    } else {
      emit connectionStatusChanged(QString("Failed to join network %1").arg(m_networkStr));
    }
  }
  return false;
}

void interface::connectToFixture(void) {
  DLResult ret;
  unsigned short shortAddr = 0xBAAD;
  GlobalGateway *ggw = GlobalGateway::Instance();
  Gateway *gw = ggw->getGateway(0);
  if (!m_joined)
    return;
  // create a fake PMU bound to m_gateway
  if (m_pmuRemote != NULL) {
    delete m_pmuRemote;
  }
  m_pmuRemote = new PMU_Remote(m_serialNumber, shortAddr);
  m_pmuRemote->setGateway(gw);
  // attempt to connect to it
  QString ignoreStr;
  m_connected = false;
  ret = m_pmuRemote->getRegister(PMU_FIRMWARE_VERSION, ignoreStr);
  if (ret == DLLIB_USB_DISCONNECTED) {
    emit connectionStatusChanged("USB Wireless Adapter disconnected");
    ggw->deleteGateway();
    m_joined = false;
  } else if (ret != DLLIB_SUCCESS) {
    emit connectionStatusChanged(QString("Failed to connect to Fixture %1").arg(m_serialNumber));
  } else {
    emit connectionStatusChanged(QString("Telegesis connection established").arg(m_serialNumber));
    m_connected = true;
    emit connectionEstablished();
  }
}

QStringList interface::queryPmu(QStringList cmdList) {
  DLResult ret;
  QString response;
  QStringList responseList;
  QMap <QString, QString> errorResponses;
  errorResponses.insert("ERROR: FFFF", "ERROR: Invalid opcode");
  errorResponses.insert("ERROR: FFFE", "ERROR: Syntax error");
  errorResponses.insert("ERROR: FFFD", "ERROR: Invalid register");
  errorResponses.insert("ERROR: FFFC", "ERROR: Register is read only");
  errorResponses.insert("ERROR: FFFB", "ERROR: Invalid register length");
  errorResponses.insert("ERROR: FFFA", "ERROR: ARP not addressed");
  errorResponses.insert("ERROR: FFF9", "ERROR: Flash error");
  errorResponses.insert("ERROR: FFF8", "ERROR: Storage out of bounds");
  errorResponses.insert("ERROR: FFF7", "ERROR: Storage unaligned");
  errorResponses.insert("ERROR: FFF6", "ERROR: Message queue full");
  errorResponses.insert("ERROR: FFF5", "ERROR: I2C error");
  errorResponses.insert("ERROR: FFF4", "ERROR: Internal error");
  errorResponses.insert("ERROR: FFF3", "ERROR: Insufficient free buffers");
  errorResponses.insert("ERROR: FFF2", "ERROR: Bad image");
  errorResponses.insert("ERROR: FFF1", "ERROR: Remote install fail");
  errorResponses.insert("ERROR: FFF0", "ERROR: Bus error");
  errorResponses.insert("ERROR: FFEF", "ERROR: Bus busy");
  errorResponses.insert("ERROR: FFEE", "ERROR: Resource busy");
  foreach (const QString &cmd, cmdList) {
    // figure out the length NOT including the space
    int len = cmd.length();
    int i = cmd.indexOf(' ');
    if (i != -1) {
      len = cmd.left(i).length();
    }
    if (m_pmuUSB == NULL) {
      GlobalGateway *ggw = GlobalGateway::Instance();
      Gateway *gw = ggw->getGateway(0);
      ret = gw->issuePMUCommand(m_pmuRemote, cmd, response, len);
    } else {
      ret = m_pmuUSB->issueCommand(cmd, response, len);
      if (ret != DLLIB_SUCCESS ) {
        responseList << QString("ERROR: %1").arg(ret);
      }
    }
    // parse error
    if (response.startsWith("ERROR")) {
      responseList << errorResponses[response];
    } else {
      responseList << response;
    }
  }
  return responseList;
}
