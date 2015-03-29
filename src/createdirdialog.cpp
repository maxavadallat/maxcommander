#include "createdirdialog.h"
#include "ui_createdirdialog.h"

//==============================================================================
// Constructor
//==============================================================================
CreateDirDialog::CreateDirDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CreateDirDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Get Directory Name
//==============================================================================
QString CreateDirDialog::getDirectory()
{
    return ui->directoryPathEdit->text();
}

//==============================================================================
// Destructor
//==============================================================================
CreateDirDialog::~CreateDirDialog()
{
    delete ui;
}
