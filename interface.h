#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

class DiscoveryAgent;
class Gateway;
class PMU;
class PMU_Remote;
class PMU_USB;

class interface : public QObject
{
  Q_OBJECT
public:
  explicit interface(QObject *parent = 0);
  void configure(QString network, quint32 serialNumber);
  void connectFTDI(void);
  void connectTelegesis(void);
  void disconnect(void);
  bool isConnected(void);
  void queryPmu(QStringList cmdList, QStringList *responseList);

signals:
  void connectionEstablished(void);
  void connectionStatusChanged(QString status);

public slots:

private:
  PMU_Remote *m_pmuRemote;
  PMU_USB *m_pmuUSB;
  DiscoveryAgent *m_discoveryAgent;
  quint32 m_serialNumber;
  unsigned long long m_panid;
  unsigned long m_chmask;
  QString m_networkStr;
  QString m_joinedNetworkStr;
  bool m_joined;
  bool m_connected;
  bool m_closed;
  void joinAndConnectWirelessly(void);
  bool join(void);
  void connectToFixture(void);

private slots:
  void slotPMUDiscovered(PMU* pmu);
};

#endif // INTERFACE_H
