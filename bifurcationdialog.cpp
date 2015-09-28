#include "bifurcationdialog.h"
#include "dllib.h"
#include "ui_bifurcationdialog.h"

BifurcationDialog::BifurcationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BifurcationDialog)
{
    ui->setupUi(this);
}

BifurcationDialog::~BifurcationDialog()
{
    delete ui;
}

void BifurcationDialog::setNetworkList(QStringList & strList)
{
    ui->listWidget->clear();

    QString s, itemStr;
    QStringList splitStrList;
    unsigned long long panid;
    unsigned int panid_group, panid_freq;
    unsigned short shortpanid;
    bool ok;
    foreach(s, strList) {
        splitStrList = s.split(QChar(','));
        panid = splitStrList[0].toULongLong(&ok, 16);
        if (ok) {
            shortpanid = splitStrList[1].toUInt(&ok, 16);
            if (ok) {
                if (panid == LR_FACTORY_DEFAULT_NETWORK_PANID) {
                    itemStr = tr("Factory Default Network [%1]")
                                    .arg(shortpanid, 4, 16, QLatin1Char('0'));
                } else {
                    panid_group = LR_PANID_CHANNEL_GROUP(panid);
                    if (!LR_CHANNEL_GROUP_ISVALID(panid_group)) 
                        DLDebug(1, DL_FUNC_INFO) << "Bug: Illegal panid_group from panid:" << panid;
                    panid_freq = LR_PANID_CHANNEL_FREQ(panid);
                    if (!LR_CHANNEL_FREQ_ISVALID(panid_freq))
                        DLDebug(1, DL_FUNC_INFO) << "Bug: Illegal panid_freq from panid:" << panid;
                    itemStr = QString("%1%2 [%3]").arg(QChar(panid_group+'A'-1))
                                                 .arg(panid_freq, 2, 10, QLatin1Char('0'))
                                                 .arg(shortpanid, 4, 16, QLatin1Char('0'));
                }
                QListWidgetItem *item = new QListWidgetItem(itemStr, ui->listWidget);
                item->setData(Qt::UserRole, s); // save the originating string
                ui->listWidget->addItem(item);
            } else {
                DLDebug(1, DL_FUNC_INFO) << "Bug: Failed to convert str to short panid:" << s[1];
            }
        } else {
            DLDebug(1, DL_FUNC_INFO) << "Bug: Failed to convert str to panid:" << s[0];
        }
    }
}

QString BifurcationDialog::getNetworkStr()
{
    QString s = ui->listWidget->currentItem()->data(Qt::UserRole).toString();
    return s;
}

void BifurcationDialog::on_join_pushButton_pressed()
{
    accept();
}

void BifurcationDialog::on_cancel_pushButton_pressed()
{
    reject();
}
