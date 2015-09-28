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
  void ftdiConnect(void);
  void ftdiDisconnect(void);
  QString ftdiQueryPmu(QStringList cmdList, QStringList *responseList);
  bool ftdiIsDisconnected(void);
  void telegesisConnect(QString networkStr);

signals:
  void ftdiConnectionEstablished(void);
  void telegesisConnectionEstablished(void);

public slots:

private:
  PMU* m_pmu;
  PMU_Remote *m_pmuRemote;
  PMU_USB *m_pmuUSB;
  DiscoveryAgent *m_discoveryAgent;
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
