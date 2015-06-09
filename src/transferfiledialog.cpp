
#include "transferfiledialog.h"
#include "ui_transferfiledialog.h"
#include "settingscontroller.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
TransferFileDialog::TransferFileDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferFileDialog)
    , settings(SettingsController::getInstance())
{
    // Setup UI
    ui->setupUi(this);
    // Set Copy Hidden File Checkbox
    ui->copyHiddenFilesCheckBox->setChecked(settings ? settings->getCopyHiddenFiles() : DEFAULT_SETTINGS_COPY_HIDDEN_FILES);
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
void TransferFileDialog::setSourceFileText(const QString& aSourceFile, const bool& aReadOnly)
{
    // Set Text
    ui->sourceFileEdit->setText(aSourceFile);
    // Set Read Only
    ui->sourceFileEdit->setReadOnly(aReadOnly);
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
void TransferFileDialog::setTargetFileText(const QString& aTargetFile, const bool& aReadOnly)
{
    // Set Text
    ui->targetFileEdit->setText(aTargetFile);
    // Set Read Only
    ui->targetFileEdit->setReadOnly(aReadOnly);
}

//==============================================================================
// Get Copy Hidden Files
//==============================================================================
bool TransferFileDialog::getCopyHidden()
{
    return ui->copyHiddenFilesCheckBox->isChecked();
}

//==============================================================================
// Set Copy Hidden Files
//==============================================================================
void TransferFileDialog::setCopyHidden(const bool& aCopyHidden)
{
    // Set Checked
    ui->copyHiddenFilesCheckBox->setChecked(aCopyHidden);

    // Check Settings
    if (settings) {
        // Set Copy Hidden Files
        settings->setCopyHiddenFiles(ui->copyHiddenFilesCheckBox->isChecked());
    }
}

//==============================================================================
// On Copy Hidden Files Checkbox Clicked Slot
//==============================================================================
void TransferFileDialog::on_copyHiddenFilesCheckBox_clicked()
{
    // Check Settings
    if (settings) {
        // Set Copy Hidden Files
        settings->setCopyHiddenFiles(ui->copyHiddenFilesCheckBox->isChecked());
    }
}

//==============================================================================
// Destructor
//==============================================================================
TransferFileDialog::~TransferFileDialog()
{
    // Delete UUI
    delete ui;

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        // Reset Settings
        settings = NULL;
    }
}

