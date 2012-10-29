
// INCLUDES

#include <QPushButton>
#include <QDebug>

#include "copyprogressdialog.h"
#include "ui_copyprogressdialog.h"
#include "constants.h"
#include "fileoperations.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
CopyProgressDialog::CopyProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CopyProgressDialog)
    , detailsShown(false)
    , pauseButton(NULL)
    , abortButton(NULL)
    , clearButton(NULL)
    , paused(false)
    , opQueue(NULL)
{
    qDebug() << "Creating CopyProgressDialog...";

    // Set up UI
    ui->setupUi(this);

    // Add Buttons
    pauseButton = ui->buttonBox->addButton(QString(DEFAULT_BUTTON_TEXT_PAUSE), QDialogButtonBox::ActionRole);
    abortButton = ui->buttonBox->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole);
    clearButton = ui->buttonBox->addButton(QString(DEFAULT_BUTTON_TEXT_CLEAR), QDialogButtonBox::ResetRole);

    // Set Close Button Focused
    ui->buttonBox->button(QDialogButtonBox::Close)->setFocus();

    // Connect Signals
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonPressed()));
    connect(abortButton, SIGNAL(clicked()), this, SLOT(abortButtonPressed()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearButtonPressed()));

    // Update UI
    updateUI();

    // Reset
    reset();

    qDebug() << "Creating CopyProgressDialog...done";
}

//==============================================================================
// Add File Operation Entry
//==============================================================================
void CopyProgressDialog::addOperationEntry(FileOperationEntry* aEntry)
{
    // Check New Entry
    if (aEntry) {
        qDebug() << "CopyProgressDialog::addOperationEntry";

        // Check Operation Queue
        if (!opQueue) {
            // Create Operation Queue
            opQueue = new FileOperationQueue(this);
        }

        // Add Entry
        opQueue->addOperation(aEntry);

        // Check UI
        if (ui && ui->detailsList) {
            // Create New Widget Item
            QListWidgetItem* newWidgetItem = new QListWidgetItem(QString("%1:%2 - > %3").arg(aEntry->getOperationName())
                                                                                        .arg(aEntry->getSource())
                                                                                        .arg(aEntry->getTarget()));
            // Add New Item
            ui->detailsList->addItem(newWidgetItem);
        }
    }
}

//==============================================================================
// Remove Operation
//==============================================================================
void CopyProgressDialog::removeOperationEntry(const int& aIndex)
{
    // Check Operation Queue
    if (opQueue) {
        qDebug() << "CopyProgressDialog::removeOperationEntry - aIndex: " << aIndex;
        // Remove Entry
        opQueue->removeOperation(aIndex);
    }
}

//==============================================================================
// Get Operations Count
//==============================================================================
int CopyProgressDialog::opEntryCount()
{
    // Check Operations Queue
    if (opQueue) {
        return opQueue->count();
    }

    return 0;
}

//==============================================================================
// Get Operation
//==============================================================================
FileOperationEntry* CopyProgressDialog::getOperationEntry(const int& aIndex)
{
    // Check Operations Queue
    if (opQueue) {
        return opQueue->getOperation(aIndex);
    }

    return NULL;
}

//==============================================================================
// Process Queue
//==============================================================================
void CopyProgressDialog::processQueue()
{
    qDebug() << "CopyProgressDialog::processQueue";

    // ...
}

//==============================================================================
// Show
//==============================================================================
void CopyProgressDialog::show()
{
    qDebug() << "CopyProgressDialog::show";

    QDialog::show();
}

//==============================================================================
// Hide
//==============================================================================
void CopyProgressDialog::hide()
{
    qDebug() << "CopyProgressDialog::hide";

    QDialog::hide();
}

//==============================================================================
// Set Dialog Modal
//==============================================================================
void CopyProgressDialog::setModal(const bool& aModal)
{
    qDebug() << "CopyProgressDialog::setModal - aModal: " << aModal;

    if (aModal) {
        // Set Window Modality
        setWindowModality(Qt::ApplicationModal);
    } else {
        // Reset Window Modality
        setWindowModality(Qt::NonModal);
    }
}

//==============================================================================
// Operation Entry Added Callback - SIGNALS DON't WORK
//==============================================================================
void CopyProgressDialog::operationEntryAdded(const int& aIndex, const int& aCount)
{
    qDebug() << "CopyProgressDialog::operationEntryAdded - aIndex: " << aIndex << " - aCount: " << aCount;


}

//==============================================================================
// Reset All Count & Progress
//==============================================================================
void CopyProgressDialog::reset()
{
    // Reset Values
    currProgress    = 0;
    currMax         = 0;
    totalProgress   = 0;
    totalMax        = 0;

    // Check UI & Current Progress
    if (ui && ui->currentProgress) {
        // Reset Values
        ui->currentProgress->setValue(0);
        ui->currentProgress->setMinimum(0);
        ui->currentProgress->setMaximum(0);
    }

    // Check UI & Total Progress
    if (ui && ui->overallProgress) {
        // Reset Values
        ui->overallProgress->setValue(0);
        ui->overallProgress->setMinimum(0);
        ui->overallProgress->setMaximum(0);
    }
}

//==============================================================================
// Set Progress
//==============================================================================
void CopyProgressDialog::setCurrentProgress(const quint64& aProgress)
{
    qDebug() << "CopyProgressDialog::setCurrentProgress - aProgress: " << aProgress;
    // Set Current Progress
    currProgress = aProgress;
}

//==============================================================================
// Add To Progress
//==============================================================================
void CopyProgressDialog::addToCurrentProgress(const quint64& aValue)
{
    qDebug() << "CopyProgressDialog::addToCurrentProgress - aValue: " << aValue;
    // Add Value To Current Progress
    currProgress += aValue;
}

//==============================================================================
// Get Progress
//==============================================================================
quint64 CopyProgressDialog::getCurrentProgress()
{
    return currProgress;
}

//==============================================================================
// Set Max Value
//==============================================================================
void CopyProgressDialog::setCurrentMaxValue(const quint64& aMaxValue)
{
    qDebug() << "CopyProgressDialog::setCurrentMaxValue - aMaxValue: " << aMaxValue;
    // Set Current Max Value
    currMax = aMaxValue;
}

//==============================================================================
// Add To Max Value
//==============================================================================
void CopyProgressDialog::addToCurrentMaxValue(const quint64& aValue)
{
    qDebug() << "CopyProgressDialog::addToCurrentMaxValue - aValue: " << aValue;
    // Add Value To Current Max Value
    currMax += aValue;
}

//==============================================================================
// Get Max Value
//==============================================================================
quint64 CopyProgressDialog::getCurrentMaxValue()
{
    return currMax;
}

//==============================================================================
// Set Total Progress
//==============================================================================
void CopyProgressDialog::setTotalProgress(const quint64& aProgress)
{
    qDebug() << "CopyProgressDialog::setTotalProgress - aProgress: " << aProgress;
    // Set Total Progress Value
    totalProgress = aProgress;
}

//==============================================================================
// Add To Total Progress
//==============================================================================
void CopyProgressDialog::addToTotalProgress(const quint64& aValue)
{
    qDebug() << "CopyProgressDialog::addToTotalProgress - aValue: " << aValue;
    // Add Value To Total Progress Value
    totalProgress += aValue;
}

//==============================================================================
// Get Total Progress
//==============================================================================
quint64 CopyProgressDialog::getTotalProgress()
{
    return totalProgress;
}

//==============================================================================
// Set Total Max Value
//==============================================================================
void CopyProgressDialog::setTotalMaxValue(const quint64& aMaxValue)
{
    qDebug() << "CopyProgressDialog::setTotalMaxValue - aMaxValue: " << aMaxValue;
    // Set Total Max Value
    totalMax = aMaxValue;
}

//==============================================================================
// Add To Total Max Value
//==============================================================================
void CopyProgressDialog::addToTotalMaxValue(const quint64& aValue)
{
    qDebug() << "CopyProgressDialog::addToTotalMaxValue - aValue: " << aValue;
    // Add Value To Total Max Value
    totalMax += aValue;

    // Check UI & Overall Progress
    if (ui && ui->overallProgress) {
        // Set Maximum - MAXINT!!
        ui->overallProgress->setMaximum(totalMax);
    }
}

//==============================================================================
// Get Total Max Value
//==============================================================================
quint64 CopyProgressDialog::getTotalMaxValue()
{
    return totalMax;
}

//==============================================================================
// On Show Details Button Clicked
//==============================================================================
void CopyProgressDialog::on_showDetailsButton_clicked()
{
    qDebug() << "CopyProgressDialog::on_showDetailsButton_clicked";
    // Toggle Details Shown
    detailsShown = !detailsShown;
    // Update UI
    updateUI();
}

//==============================================================================
// Pause Button Pressed
//==============================================================================
void CopyProgressDialog::pauseButtonPressed()
{
    qDebug() << "CopyProgressDialog::pauseButtonPressed - paused: " << paused;

    // Check Paused State
    if (paused) {
        // Resume Queue
        resume();
    } else {
        // Pause Queue
        pause();
    }
}

//==============================================================================
// Abort Button Pressed
//==============================================================================
void CopyProgressDialog::abortButtonPressed()
{
    qDebug() << "CopyProgressDialog::abortButtonPressed";

    // ...
}

//==============================================================================
// Clear Button Pressed
//==============================================================================
void CopyProgressDialog::clearButtonPressed()
{
    qDebug() << "CopyProgressDialog::clearButtonPressed";
    // Abort
    abort();
    // Clear
    clear();

    // ...
}

//==============================================================================
// Update UI
//==============================================================================
void CopyProgressDialog::updateUI()
{
    // Set Deatils List Visible
    ui->detailsList->setVisible(detailsShown);
    // Set Details Title Label Visible
    ui->detailsTitleLabel->setVisible(detailsShown);
    // Set Counter Label Visible
    ui->queueCountLabel->setVisible(detailsShown);
    // Set Minimum Height
    setMinimumHeight(detailsShown ? DEFAULT_COPY_PROGRESS_DIALOG_DETAILSSHOWN_HEIGHT : 0);
    // Set Height
    resize(geometry().width(), detailsShown ? DEFAULT_COPY_PROGRESS_DIALOG_DETAILSSHOWN_HEIGHT : DEFAULT_COPY_PROGRESS_DIALOG_DETAILSHIDDEN_HEIGHT);
}

//==============================================================================
// Abort
//==============================================================================
void CopyProgressDialog::abort()
{
    qDebug() << "CopyProgressDialog::abort";

    // ...
}

//==============================================================================
// Pause
//==============================================================================
void CopyProgressDialog::pause()
{
    qDebug() << "CopyProgressDialog::pause";

    // Set Paused State
    paused = true;
    // Set Pause Button Text
    pauseButton->setText(QString(DEFAULT_BUTTON_TEXT_RESUME));

    // ...
}

//==============================================================================
// Resume
//==============================================================================
void CopyProgressDialog::resume()
{
    qDebug() << "CopyProgressDialog::resume";

    // Set Paused State
    paused = false;
    // Set Pause Button Text
    pauseButton->setText(QString(DEFAULT_BUTTON_TEXT_PAUSE));

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void CopyProgressDialog::clear()
{
    qDebug() << "CopyProgressDialog::clear";

    // Disable Clear Button
    clearButton->setEnabled(false);
    // Disable Pause Button
    pauseButton->setEnabled(false);
    // Disable Abort Button
    abortButton->setEnabled(false);

    // ...

    // Check UI
    if (ui && ui->detailsList) {
        // Clear List
        ui->detailsList->clear();
    }

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
CopyProgressDialog::~CopyProgressDialog()
{
    qDebug() << "Deleteing CopyProgressDialog...";
    // Abort
    abort();
    // Clear
    clear();
    // Check Operation Queue
    if (opQueue) {
        // Delete Operations Queue
        delete opQueue;
        opQueue = NULL;
    }
    // Delete UI
    delete ui;

    qDebug() << "Deleteing CopyProgressDialog...done";
}

