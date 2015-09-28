#ifndef EMBERDIALOG_H
#define EMBERDIALOG_H

#include <QDialog>

namespace Ui {
    class EmberDialog;
}

class EmberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmberDialog(QWidget *parent = 0);
    ~EmberDialog();

private:
    Ui::EmberDialog *ui;
};

#endif // EMBERDIALOG_H
