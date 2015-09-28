#include "globalgateway.h"
#include "bifurcationdialog.h"
#include "emberdialog.h"

#include <QCoreApplication>
#include <QtWidgets>

static GlobalGateway *sGlobalGateway = 0;

GlobalGateway* GlobalGateway::Instance()
{
    if (sGlobalGateway == 0) {
        sGlobalGateway = new GlobalGateway();
    }

    return sGlobalGateway;
}

GlobalGateway::GlobalGateway() :
    m_gw(NULL),
    m_parentWidget(NULL),
    m_emberDialog(NULL),
    m_canceled(false),
    m_joinedAsCoordinator(false)
{
}

GlobalGateway::~GlobalGateway()
{
    deleteGateway();
    m_joinedAsCoordinator = false;
}

EmberGateway* GlobalGateway::getGateway(QWidget *parentWidget, bool allowUI)
{
    if (parentWidget != 0) {
        m_parentWidget = parentWidget;
    }

    if (!m_gw) {
        pollForEmberGateway(allowUI);
    }

    return m_gw;
}

void GlobalGateway::pollForEmberGateway(bool allowUI)
{
    DiscoveryAgent da;

    m_canceled = false;

    while (m_gw == NULL && m_canceled == false) {
        // Find an Ember USB Wireless Adapter and join it to the network.
        da.discoverGateway_usbs();
        DLDebug(100, DL_FUNC_INFO);

        // If none found, show a USB Wireless Adapter dialog and keep looping
        if (da.m_gatewayList.isEmpty()) {
            if (allowUI) {
                showEmberDialog();
                msSleepWithEvents(750);
            } else {
                return; // someone else will handle ui and re-calling
            }
        } else {
            Gateway *gw = da.m_gatewayList.takeFirst();
            m_gw = qobject_cast<Gateway_USB_Ember *>(gw);
            if (!m_gw) {
                delete gw; // If we found an old non-Ember stick, ignore it
            } else {
                connect(m_gw, SIGNAL(sigUsbDisconnected()), this, SLOT(slot_gatewayDeleted()));
                dismissEmberDialog();
            }
        }
    }
}

void GlobalGateway::slot_cancelDialog()
{
    dismissEmberDialog();
    m_canceled = true;
}

void GlobalGateway::showEmberDialog()
{
    if (!m_emberDialog) {
        m_emberDialog = new EmberDialog(m_parentWidget);
        connect(m_emberDialog, SIGNAL(rejected()), this, SLOT(slot_cancelDialog()));

        m_emberDialog->show();
        m_emberDialog->raise();
        m_emberDialog->activateWindow();
    }
}

void GlobalGateway::dismissEmberDialog()
{
    if (m_emberDialog) {
        delete m_emberDialog;
        m_emberDialog = NULL;
    }
}

void GlobalGateway::msSleepWithEvents(const int milliseconds)
{
    QTime t;
    t.restart();
    do {
        QCoreApplication::processEvents();
    } while (t.elapsed() < milliseconds);
}

void GlobalGateway::deleteGateway()
{
    // Called on object deletion, but also called if
    // some gw method returns DLLIB_USB_DISCONNECTED
    // so the consumer can re-call getGateway()
    if (m_gw) {
        delete m_gw;
        m_gw = NULL;
    }
}

void GlobalGateway::slot_gatewayDeleted()
{
    DLDebug(100, DL_FUNC_INFO) << "Gateway was pulled out? Deleting.";
    deleteGateway();
}

DLResult GlobalGateway::joinNetwork(unsigned long long panid,
                                    unsigned long chmask,
                                    bool askToCoordinate,
                                    bool allowCoordination,
                                    bool handleBifurcation,
                                    unsigned int hopCount,
                                    bool coordinateImmediately)
{
    DLResult result;
    Q_ASSERT(m_gw);

    m_joinedAsCoordinator = false;

    if (!coordinateImmediately) {
        for (int i = 1; i < 7; i++) {

            DLDebug(100, DL_FUNC_INFO) << QString("Joining network %1: %2 as router: %3")
                                            .arg(panid, 16, 16, QChar('0'))
                                            .arg(chmask, 4, 16, QChar('0'))
                                            .arg(i);
            if (hopCount > 0) {
                result = m_gw->joinNetworkWithHopCount(hopCount, Gateway::Role_Router, panid, chmask);
            } else {
                result = m_gw->joinNetwork(Gateway::Role_Router, panid, chmask);
            }

            // One can fake a bifurcated result by enabling this line:
            // result = DLLIB_BIFURCATED_NETWORK;

            if (result == DLLIB_BIFURCATED_NETWORK) {
                if (handleBifurcation) {
                    QString selectedNetworkStr;
                    EmberGateway *gw = getGateway(0);
                    bool tryIt = askForBifurcatedJoin(gw, m_parentWidget, selectedNetworkStr);
                    if (tryIt) {
                        bool ok;
                        QStringList splitStrList = selectedNetworkStr.split(QChar(','));
                        unsigned short shortpanid = splitStrList[1].toUInt(&ok, 16);
                        if (ok) {
                            return tryBifurcatedJoin(m_gw, panid, chmask, shortpanid, hopCount);
                        } else {
                            DLDebug(1, DL_FUNC_INFO) << "Bug: Failed to convert BifurcationDialog str to short panid:" << selectedNetworkStr;
                            return DLLIB_FAILURE;
                        }
                    } else {
                        return result;
                    }
                } else {
                    return result;
                }
            }

            // Check for Chris' short address 'Bnnn' bug:
            if (result == DLLIB_SUCCESS) {
                quint16 nodeid;
                result = m_gw->ezspGetNodeId(nodeid);
                if (result == DLLIB_SUCCESS) {
                    if ((nodeid & 0xf000) == 0xb000) {
                        DLDebug(100, DL_FUNC_INFO) << "Joined with Bnnn short address:" << QString("%1").arg(nodeid,4,16,QChar('0'));
                        result = DLLIB_FAILURE;
                    } else {
                        DLDebug(900, DL_FUNC_INFO) << "Joined with short addr" << QString("%1").arg(nodeid,4,16,QChar('0'));
                    }
                } else {
                    result = DLLIB_FAILURE;
                }
            }

            if (result == DLLIB_SUCCESS) {
                return result;
            }
            else if (result == DLLIB_USB_DISCONNECTED) {
                deleteGateway();
                return result;
            }
        }
    }

    // Ask permission to try once as a coordinator.
    if (allowCoordination) {
        if (askToCoordinate) {
            unsigned group, freq;
            bool encrypted;
            if (!LRNetwork::groupAndFreqFromPanidAndChmask(panid, chmask,
                                                           &group, &freq, &encrypted)) {
                return DLLIB_FAILURE;
            }
            QMessageBox::StandardButton response =
                    allowWirelessCoordinationDialog(LRNetwork::nwidFromGroupAndFreq(group, freq, encrypted));
            if (response == QMessageBox::No) {
                DLDebug(100, DL_FUNC_INFO) << "User opted NOT to join as a coordinator";
                return DLLIB_FAILURE;
            }
        }

        MSLEEP(250);
        DLDebug(100, DL_FUNC_INFO) << QString("Joining network %1: %2 as coordinator")
                                        .arg(panid, 16, 16, QChar('0'))
                                        .arg(chmask, 4, 16, QChar('0'));
        if (hopCount > 0) {
            result = m_gw->joinNetworkWithHopCount(hopCount, Gateway::Role_Coordinator, panid, chmask);
        } else {
            result = m_gw->joinNetwork(Gateway::Role_Coordinator, panid, chmask);
        }

        if (result == DLLIB_USB_DISCONNECTED) {
            deleteGateway();
            return result;
        }
        m_joinedAsCoordinator = true;

        MSLEEP(3000); // Sleep a bit to allow fixtures to join our new network
    }

    return result;
}

QMessageBox::StandardButton GlobalGateway::allowWirelessCoordinationDialog(QString nwid)
{
    return QMessageBox::warning(
            NULL,
            tr("Allow Wireless Coordination"),
            tr("Unable to talk to the Fixtures in Network %1.  Would "
               "you like to enable wireless coordination of this Network %1?"
               "\n\n"
               "You should only enable wireless coordination immediately "
               "after all of the fixtures on a Network have been power "
               "cycled, or when switching a Fixture to a new Network that "
               "has not previously been used."
               "\n\n"
               "If you enable wireless coordination in other "
               "circumstances you may prevent some Fixtures from "
               "communicating properly on the Network."
               "\n\n"
               "Do you really want to enable wireless coordination?").arg(nwid),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
}

// STATIC
bool GlobalGateway::askForBifurcatedJoin(EmberGateway *gw, QWidget *parentWidget, QString & selectedNetworkStr)
{
    selectedNetworkStr.clear();

    QStringList networkList;
    gw->getLastFoundNetworkList(networkList);

    // Add a fake entry: for TESTING ONLY
    // if (!networkList.isEmpty()) networkList << networkList.first().section(',',0,0) + ",dead";

    if (networkList.count() < 2) {
        DLDebug(100, DL_FUNC_INFO) << QString("Bug: Gateway reports bifurcation, but the networkList has a count of") << networkList.count();
        return false;
    }

    BifurcationDialog *dlg = new BifurcationDialog(parentWidget);
    dlg->setNetworkList(networkList);
    dlg->show();
    dlg->activateWindow();

    int result = dlg->exec();

    if (result == QDialog::Accepted) {
        selectedNetworkStr = dlg->getNetworkStr();
        return true;
    }

    return false;
}

// STATIC
DLResult GlobalGateway::tryBifurcatedJoin(EmberGateway *gw,
                                            unsigned long long panid, 
                                            unsigned long chmask, 
                                            unsigned short shortpanid, 
                                            unsigned int hopCount)
{
    for (int i = 1; i < 7; i++) {
        DLDebug(100, DL_FUNC_INFO) << QString("Joining network %1: %2 as router with shortpanid %3: %4")
                                        .arg(panid, 16, 16, QChar('0'))
                                        .arg(chmask, 4, 16, QChar('0'))
                                        .arg(shortpanid, 4, 16, QChar('0'))
                                        .arg(i);
        DLResult result = gw->joinNetworkWithShortPanId(Gateway::Role_Router, panid, chmask, shortpanid, hopCount);
        if (result == DLLIB_SUCCESS) {
            return result;
        }
        else if (result == DLLIB_USB_DISCONNECTED) {
            if (gw) {
                delete gw;
                gw = NULL;
            }
            return result;
        }
    }

    return DLLIB_FAILURE;
}
