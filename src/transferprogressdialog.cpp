#include "src/transferprogressdialog.h"
#include "ui_transferprogressdialog.h"

//==============================================================================
// Constructor
//==============================================================================
TransferProgressDialog::TransferProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferProgressDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Title
//==============================================================================
void TransferProgressDialog::setTitle(const QString& aTitle)
{
    // Set Window Title
    setWindowTitle(aTitle);
}

//==============================================================================
// Set Current File Name
//==============================================================================
void TransferProgressDialog::setCurrentFileName(const QString& aCurrentFileName)
{
    ui->currentFileNameLabel->setText(aCurrentFileName);
}

//==============================================================================
// Set Current File Progress
//==============================================================================
void TransferProgressDialog::setCurrentProgress(const quint64& aProgress, const quint64& aTotal)
{
    // Set Minimum
    ui->currentProgress->setMinimum(0);

    // Check Total
    if (aTotal > INT_MAX) {
        // Set Maximum
        ui->currentProgress->setMaximum(aTotal >> 32);
        // Set Progress
        ui->currentProgress->setValue(aProgress >> 32);
    } else {
        // Set Maximum
        ui->currentProgress->setMaximum(aTotal);
        // Set Progress
        ui->currentProgress->setValue(aProgress);
    }
}

//==============================================================================
// Set Overall Progress
//==============================================================================
void TransferProgressDialog::setOverallProgress(const quint64& aProgress, const quint64& aTotal)
{
    // Set Minimum
    ui->overallProgress->setMinimum(0);

    // Check Total
    if (aTotal > INT_MAX) {
        // Set Maximum
        ui->overallProgress->setMaximum(aTotal >> 32);
        // Set Value
        ui->overallProgress->setValue(aProgress >> 32);
    } else {
        // Set Maximum
        ui->overallProgress->setMaximum(aTotal);
        // Set Value
        ui->overallProgress->setValue(aProgress);
    }
}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressDialog::~TransferProgressDialog()
{
    delete ui;
}
