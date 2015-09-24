#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

class DiscoveryAgent;
class PMU;
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

signals:
  void ftdiConnectionEstablished(void);

public slots:

private:
  PMU_USB *m_pmuUSB;
  DiscoveryAgent *m_discoveryAgent;

private slots:
  void slotPMUDiscovered(PMU* pmu);
};

#endif // INTERFACE_H
