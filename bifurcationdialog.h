#ifndef BIFURCATIONDIALOG_H
#define BIFURCATIONDIALOG_H

#include <QDialog>

namespace Ui {
class BifurcationDialog;
}

class BifurcationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BifurcationDialog(QWidget *parent = 0);
    ~BifurcationDialog();
    
    void setNetworkList(QStringList & strList);
    QString getNetworkStr();

private slots:
    void on_join_pushButton_pressed();
    void on_cancel_pushButton_pressed();


private:
    Ui::BifurcationDialog *ui;
};

#endif // BIFURCATIONDIALOG_H
