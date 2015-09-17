#include <QDebug>

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
    , sourceChanged(false)
    , targetChanged(false)
{
    // Setup UI
    ui->setupUi(this);
    // Set Copy Hidden File Checkbox
    ui->copyHiddenFilesCheckBox->setChecked(settings ? settings->getCopyHiddenFiles() : DEFAULT_SETTINGS_COPY_HIDDEN_FILES);

    // Connect Signals
    connect(ui->sourceFileEdit, SIGNAL(textChanged(QString)), this, SLOT(sourceFileTextChanged(QString)));
    connect(ui->targetFileEdit, SIGNAL(textChanged(QString)), this, SLOT(targetFileTextChanged(QString)));
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
// Get Source Change
//==============================================================================
bool TransferFileDialog::getSourceChanged()
{
    return sourceChanged;
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
// Get Target Change
//==============================================================================
bool TransferFileDialog::getTargetChanged()
{
    return targetChanged;
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
// Set Copy Hidden Visible
//==============================================================================
void TransferFileDialog::setCopyHiddenVisible(const bool& aVisible)
{
    // Set Visibility
    ui->copyHiddenFilesCheckBox->setVisible(aVisible);
}

//==============================================================================
// Exec
//==============================================================================
int TransferFileDialog::exec()
{
    // Reset Source Changed
    sourceChanged = false;
    // reset Target changed
    targetChanged = false;

    // Exec
    int result = QDialog::exec();

    return result;
}

//==============================================================================
// Source File Text Changed
//==============================================================================
void TransferFileDialog::sourceFileTextChanged(const QString& aText)
{
    // Check If Source Changed
    if (!sourceChanged) {
        qDebug() << "TransferFileDialog::sourceFileTextChanged - aText: " << aText;

        // Set Source Changed
        sourceChanged = true;
    }
}

//==============================================================================
// Target File Text Changed
//==============================================================================
void TransferFileDialog::targetFileTextChanged(const QString& aText)
{
    // Check If Target Changed
    if (!targetChanged) {
        qDebug() << "TransferFileDialog::targetFileTextChanged - aText: " << aText;

        // Set Target Changed
        targetChanged = true;
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

