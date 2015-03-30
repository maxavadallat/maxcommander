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
// Set Directory Path
//==============================================================================
void CreateDirDialog::setDirPath(const QString& aDirPath)
{
    // Set Text
    ui->directoryPathEdit->setText(aDirPath);
}

//==============================================================================
// Destructor
//==============================================================================
CreateDirDialog::~CreateDirDialog()
{
    delete ui;
}
