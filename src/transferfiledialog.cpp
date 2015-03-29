#include "transferfiledialog.h"
#include "ui_transferfiledialog.h"

//==============================================================================
// Constructor
//==============================================================================
TransferFileDialog::TransferFileDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferFileDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Get Source File Text
//==============================================================================
QString TransferFileDialog::getSourceFileText()
{
    return ui->sourceFileEdit->text();
}

//==============================================================================
// Set Source File Text
//==============================================================================
void TransferFileDialog::setSourceFileText(const QString& aSourceFile)
{
    ui->sourceFileEdit->setText(aSourceFile);
}

//==============================================================================
// Get Target File Text
//==============================================================================
QString TransferFileDialog::getTargetFileText()
{
    return ui->targetFileEdit->text();
}

//==============================================================================
// Set Target Fiel Text
//==============================================================================
void TransferFileDialog::setTargetFileText(const QString& aTargetFile)
{
    ui->targetFileEdit->setText(aTargetFile);
}

//==============================================================================
// Destructor
//==============================================================================
TransferFileDialog::~TransferFileDialog()
{
    delete ui;
}
