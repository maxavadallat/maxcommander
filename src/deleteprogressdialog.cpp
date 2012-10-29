
// INCLUDES

#include <QPushButton>
#include <QDebug>

#include "deleteprogressdialog.h"
#include "ui_deleteprogressdialog.h"
#include "constants.h"
#include "fileoperations.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
DeleteProgressDialog::DeleteProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteProgressDialog)
    , detailsShown(false)
    , pauseButton(NULL)
    , abortButton(NULL)
    , clearButton(NULL)
    , paused(false)
    , opQueue(NULL)
{
    qDebug() << "Creating DeleteProgressDialog...";

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

    qDebug() << "Creating DeleteProgressDialog...done";
}

//==============================================================================
// Add File Operation Entry
//==============================================================================
void DeleteProgressDialog::addOperationEntry(FileOperationEntry* aEntry)
{
    // Check New Entry
    if (aEntry) {
        qDebug() << "DeleteProgressDialog::addOperationEntry";

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
            QListWidgetItem* newWidgetItem = new QListWidgetItem(QString("%1:%2").arg(aEntry->getOperationName())
                                                                                 .arg(aEntry->getSource()));
            // Add New Item
            ui->detailsList->addItem(newWidgetItem);
        }
    }
}

//==============================================================================
// Remove Operation
//==============================================================================
void DeleteProgressDialog::removeOperationEntry(const int& aIndex)
{
    // Check Operation Queue
    if (opQueue) {
        qDebug() << "DeleteProgressDialog::removeOperationEntry - aIndex: " << aIndex;
        // Remove Entry
        opQueue->removeOperation(aIndex);
    }
}

//==============================================================================
// Get Operations Count
//==============================================================================
int DeleteProgressDialog::opEntryCount()
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
FileOperationEntry* DeleteProgressDialog::getOperationEntry(const int& aIndex)
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
void DeleteProgressDialog::processQueue()
{
    qDebug() << "DeleteProgressDialog::processQueue";

    // ...
}

//==============================================================================
// Show
//==============================================================================
void DeleteProgressDialog::show()
{
    qDebug() << "DeleteProgressDialog::show";

    QDialog::show();
}

//==============================================================================
// Hide
//==============================================================================
void DeleteProgressDialog::hide()
{
    qDebug() << "DeleteProgressDialog::hide";

    QDialog::hide();
}

//==============================================================================
// Set Dialog Modal
//==============================================================================
void DeleteProgressDialog::setModal(const bool& aModal)
{
    qDebug() << "DeleteProgressDialog::setModal - aModal: " << aModal;

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
void DeleteProgressDialog::operationEntryAdded(const int& aIndex, const int& aCount)
{
    qDebug() << "DeleteProgressDialog::operationEntryAdded - aIndex: " << aIndex << " - aCount: " << aCount;


}

//==============================================================================
// Reset All Count & Progress
//==============================================================================
void DeleteProgressDialog::reset()
{
    // Check UI
    if (ui && ui->deleteProgress) {
        // Reset Min Value
        ui->deleteProgress->setMinimum(0);
        // Reset Max Value
        ui->deleteProgress->setMaximum(0);
        // Reset Value
        ui->deleteProgress->setValue(0);
    }
}

//==============================================================================
// Set Progress
//==============================================================================
void DeleteProgressDialog::setProgress(const int& aProgress)
{
    // Check UI
    if (ui && ui->deleteProgress) {
        // Set Value
        ui->deleteProgress->setValue(aProgress);
    }
}

//==============================================================================
// Add To Progress
//==============================================================================
void DeleteProgressDialog::addToProgress(const int& aValue)
{
    // Check UI
    if (ui && ui->deleteProgress) {
        // Add To Value
        ui->deleteProgress->setValue(ui->deleteProgress->value() + aValue);
    }
}

//==============================================================================
// Get Progress
//==============================================================================
int DeleteProgressDialog::getProgress()
{
    // Check UI
    if (ui && ui->deleteProgress) {
        return ui->deleteProgress->value();
    }

    return 0;
}

//==============================================================================
// Set Max Value
//==============================================================================
void DeleteProgressDialog::setMaxValue(const int& aMaxValue)
{
    // Check UI
    if (ui && ui->deleteProgress) {
        // Set Max Value
        ui->deleteProgress->setMaximum(aMaxValue);
    }
}

//==============================================================================
// Add To Max Value
//==============================================================================
void DeleteProgressDialog::addToMaxValue(const int& aValue)
{
    // Check UI
    if (ui && ui->deleteProgress) {
        // Add To Max Value
        ui->deleteProgress->setMaximum(ui->deleteProgress->maximum() + aValue);
    }
}

//==============================================================================
// Get Max Value
//==============================================================================
int DeleteProgressDialog::getMaxValue()
{
    // Check UI
    if (ui && ui->deleteProgress) {
        return ui->deleteProgress->maximum();
    }

    return 0;
}

//==============================================================================
// On Show details Button Clicked Slot
//==============================================================================
void DeleteProgressDialog::on_showDetailsButton_clicked()
{
    qDebug() << "DeleteProgressDialog::on_showDetailsButton_clicked";
    // Toggle Details Shown
    detailsShown = !detailsShown;
    // Update UI
    updateUI();
}

//==============================================================================
// Pause Button Pressed
//==============================================================================
void DeleteProgressDialog::pauseButtonPressed()
{
    qDebug() << "DeleteProgressDialog::pauseButtonPressed - paused: " << paused;

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
void DeleteProgressDialog::abortButtonPressed()
{
    qDebug() << "DeleteProgressDialog::abortButtonPressed";

    // ...
}

//==============================================================================
// Clear Button Pressed
//==============================================================================
void DeleteProgressDialog::clearButtonPressed()
{
    qDebug() << "DeleteProgressDialog::clearButtonPressed";
    // Abort
    abort();
    // Clear
    clear();

    // ...
}

//==============================================================================
// Update UI
//==============================================================================
void DeleteProgressDialog::updateUI()
{
    qDebug() << "DeleteProgressDialog::updateUI";
    // Set Deatils List Visible
    ui->detailsList->setVisible(detailsShown);
    // Set Details Title Label Visible
    ui->detailsTitleLabel->setVisible(detailsShown);
    // Set Counter Label Visible
    ui->queueCountLabel->setVisible(detailsShown);
    // Set Minimum Height
    setMinimumHeight(detailsShown ? DEFAULT_DELETE_PROGRESS_DIALOG_DETAILSSHOWN_HEIGHT : 0);
    // Set Height
    resize(geometry().width(), detailsShown ? DEFAULT_DELETE_PROGRESS_DIALOG_DETAILSSHOWN_HEIGHT : DEFAULT_DELETE_PROGRESS_DIALOG_DETAILSHIDDEN_HEIGHT);
}

//==============================================================================
// Abort
//==============================================================================
void DeleteProgressDialog::abort()
{
    qDebug() << "DeleteProgressDialog::abort";

    // ...
}

//==============================================================================
// Pause
//==============================================================================
void DeleteProgressDialog::pause()
{
    qDebug() << "DeleteProgressDialog::pause";

    // Set Paused State
    paused = true;
    // Set Pause Button Text
    pauseButton->setText(QString(DEFAULT_BUTTON_TEXT_RESUME));

    // ...
}

//==============================================================================
// Resume
//==============================================================================
void DeleteProgressDialog::resume()
{
    qDebug() << "DeleteProgressDialog::resume";

    // Set Paused State
    paused = false;
    // Set Pause Button Text
    pauseButton->setText(QString(DEFAULT_BUTTON_TEXT_PAUSE));

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void DeleteProgressDialog::clear()
{
    qDebug() << "DeleteProgressDialog::clear";

    // Disable Clear Button
    clearButton->setEnabled(false);
    // Disable Pause Button
    pauseButton->setEnabled(false);
    // Disable Abort Button
    abortButton->setEnabled(false);

    // ...

    // Check UI
    if (ui && ui->detailsList) {
        // Clear Details List
        ui->detailsList->clear();
    }

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressDialog::~DeleteProgressDialog()
{
    qDebug() << "Deleteing DeleteProgressDialog...";
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

    qDebug() << "Deleteing DeleteProgressDialog...done";
}

