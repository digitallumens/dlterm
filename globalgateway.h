#ifndef GLOBALGATEWAY_H
#define GLOBALGATEWAY_H

#include "dllib.h"
#include <QObject>
#include <QMessageBox>
#include <QTimer>

class Gateway_USB_Ember;
class EmberDialog;
class QWidget;

class GlobalGateway : public QObject
{
    Q_OBJECT

public:
    static GlobalGateway* Instance();

    GlobalGateway();
    ~GlobalGateway();

    EmberGateway* getGateway(QWidget *parent, bool allowUI = true);
    bool exists() { return m_gw != NULL; }
    void leaveAnyNetwork() { if (m_gw != NULL) m_gw->leaveNetwork(); }
    void deleteGateway();

    DLResult joinNetwork(unsigned long long panid,
                         unsigned long chmask,
                         bool askToCoordinate = true,
                         bool allowCoordination = true,
                         bool handleBifurcation = true,
                         unsigned int hopCount = 0,
                         bool coordinateImmediately = false);

    QMessageBox::StandardButton allowWirelessCoordinationDialog(QString nwid);

    bool joinedAsCoordinator() const { return m_joinedAsCoordinator; }

    static bool askForBifurcatedJoin(EmberGateway *gw, QWidget *parentWidget, QString & selectedNetworkStr);

    static DLResult tryBifurcatedJoin(EmberGateway *gw,
                                        unsigned long long panid,
                                        unsigned long chmask,
                                        unsigned short shortpanid,
                                        unsigned int hopCount = 0);

public slots:
    void slot_cancelDialog();
    void slot_gatewayDeleted();

private:
    EmberGateway       *m_gw;
    QTimer              m_gwTimer;
    QWidget            *m_parentWidget;
    EmberDialog        *m_emberDialog;
    bool                m_canceled;
    bool                m_joinedAsCoordinator;

    void pollForEmberGateway(bool allowUI = true);
    void showEmberDialog();
    void dismissEmberDialog();
    void msSleepWithEvents(const int milliseconds);
};

#endif // GLOBALGATEWAY_H
