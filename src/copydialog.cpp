#include "copydialog.h"
#include "ui_copydialog.h"

CopyDialog::CopyDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CopyDialog)
{
    ui->setupUi(this);
}

CopyDialog::~CopyDialog()
{
    delete ui;
}
