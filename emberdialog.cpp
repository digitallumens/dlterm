#include "emberdialog.h"
#include "ui_emberdialog.h"

EmberDialog::EmberDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmberDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags	wf = windowFlags(); // remove Windows' ? button
    wf = wf & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(wf);
}

EmberDialog::~EmberDialog()
{
    delete ui;
}
