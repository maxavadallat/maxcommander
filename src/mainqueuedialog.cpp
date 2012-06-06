
// INCLUDES

#include <QPushButton>
#include <QFont>
#include <QDebug>

#include "mainqueuedialog.h"
#include "ui_mainqueuedialog.h"
#include "constants.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
MainQueueDialog::MainQueueDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainQueueDialog)
    , pauseButton(NULL)
    , abortButton(NULL)
    , clearButton(NULL)
    , paused(false)
    , opQueue(NULL)
{
    qDebug() << "Creating MainQueueDialog...";

    // Set Up UI
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

    // Reset
    resetProgress();

    qDebug() << "Creating MainQueueDialog...done";
}

//==============================================================================
// Add File Operation Entry
//==============================================================================
void MainQueueDialog::addOperationEntry(FileOperationEntry* aEntry)
{
    // Check New Entry
    if (aEntry) {
        qDebug() << "MainQueueDialog::addOperationEntry - aEntry: " << aEntry;

        // Check Operation Queue
        if (!opQueue) {
            // Create Operation Queue
            opQueue = new FileOperationQueue(this);

            // Connect Signals

            connect(opQueue, SIGNAL(opStarted()), this, SLOT(opQueueStarted()));
            connect(opQueue, SIGNAL(opStopped()), this, SLOT(opQueueStopped()));
            connect(opQueue, SIGNAL(opFinished()), this, SLOT(opQueueFinished()));

            connect(opQueue, SIGNAL(operationAdded(int,int)), this, SLOT(operationAdded(int,int)));
            connect(opQueue, SIGNAL(operationStarted(int)), this, SLOT(operationStarted(int)));
            connect(opQueue, SIGNAL(operationCompleted(int)), this, SLOT(operationCompleted(int)));
        }

        // Add Entry
        opQueue->addOperation(aEntry);
    }
}

//==============================================================================
// Remove Operation
//==============================================================================
void MainQueueDialog::removeOperationEntry(const int& aIndex)
{
    // Check Operation Queue
    if (opQueue) {
        qDebug() << "MainQueueDialog::removeOperationEntry - aIndex: " << aIndex;
        // Remove Entry
        opQueue->removeOperation(aIndex);
    }
}

//==============================================================================
// Get Operations Count
//==============================================================================
int MainQueueDialog::opEntryCount()
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
FileOperationEntry* MainQueueDialog::getOperationEntry(const int& aIndex)
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
void MainQueueDialog::processQueue()
{
    // Check Operation Queue
    if (opQueue) {
        // Get Operations Count
        int qCount =  opQueue->count();

        qDebug() << "MainQueueDialog::processQueue - qCount: " << qCount;

        // Process Queue
        opQueue->processQueue();
    }
}

//==============================================================================
// Show
//==============================================================================
void MainQueueDialog::show()
{
    qDebug() << "MainQueueDialog::show";

    QDialog::show();
}

//==============================================================================
// Hide
//==============================================================================
void MainQueueDialog::hide()
{
    qDebug() << "MainQueueDialog::hide";

    QDialog::hide();
}

//==============================================================================
// Set Dialog Modal
//==============================================================================
void MainQueueDialog::setModal(const bool& aModal)
{
    //qDebug() << "MainQueueDialog::setModal - aModal: " << aModal;

    if (aModal) {
        // Set Window Modality
        setWindowModality(Qt::ApplicationModal);
    } else {
        // Reset Window Modality
        setWindowModality(Qt::NonModal);
    }
}

//==============================================================================
// Reset All Count & Progress
//==============================================================================
void MainQueueDialog::resetProgress()
{
    qDebug() << "MainQueueDialog::resetProgress";

    // Check UI
    if (ui && ui->queueProgress) {
        // Reset Min Value
        ui->queueProgress->setMinimum(0);
        // Reset Max Value
        ui->queueProgress->setMaximum(0);
        // Reset Value
        ui->queueProgress->setValue(0);
    }
}

//==============================================================================
// Set Progress
//==============================================================================
void MainQueueDialog::setProgress(const int& aProgress)
{
    qDebug() << "MainQueueDialog::setProgress - aProgress: " << aProgress;

    // Check UI
    if (ui && ui->queueProgress) {
        // Set Value
        ui->queueProgress->setValue(aProgress);
    }
}

//==============================================================================
// Add To Progress
//==============================================================================
void MainQueueDialog::addToProgress(const int& aValue)
{
    qDebug() << "MainQueueDialog::addToProgress - aValue: " << aValue;

    // Check UI
    if (ui && ui->queueProgress) {
        // Add To Value
        ui->queueProgress->setValue(ui->queueProgress->value() + aValue);
    }
}

//==============================================================================
// Get Progress
//==============================================================================
int MainQueueDialog::getProgress()
{
    // Check UI
    if (ui && ui->queueProgress) {
        return ui->queueProgress->value();
    }

    return 0;
}

//==============================================================================
// Set Max Value
//==============================================================================
void MainQueueDialog::setMaxValue(const int& aMaxValue)
{
    qDebug() << "MainQueueDialog::setMaxValue - aMaxValue: " << aMaxValue;

    // Check UI
    if (ui && ui->queueProgress) {
        // Set Max Value
        ui->queueProgress->setMaximum(aMaxValue);
    }
}

//==============================================================================
// Add To Max Value
//==============================================================================
void MainQueueDialog::addToMaxValue(const int& aValue)
{
    qDebug() << "MainQueueDialog::addToMaxValue - aValue: " << aValue;

    // Check UI
    if (ui && ui->queueProgress) {
        // Add To Max Value
        ui->queueProgress->setMaximum(ui->queueProgress->maximum() + aValue);
    }
}

//==============================================================================
// Get Max Value
//==============================================================================
int MainQueueDialog::getMaxValue()
{
    // Check UI
    if (ui && ui->queueProgress) {
        return ui->queueProgress->maximum();
    }

    return 0;
}

//==============================================================================
// Abort
//==============================================================================
void MainQueueDialog::abort()
{
    qDebug() << "MainQueueDialog::abort";

    // Check Operation Queue
    if (opQueue) {
        // Abort
        opQueue->stop();
    }

    // ...
}

//==============================================================================
// Pause
//==============================================================================
void MainQueueDialog::pause()
{
    qDebug() << "MainQueueDialog::pause";

    // Set Paused State
    paused = true;
    // Set Pause Button Text
    pauseButton->setText(QString(DEFAULT_BUTTON_TEXT_RESUME));

    // Check Operation Queue
    if (opQueue) {
        // Pause
        opQueue->pause();
    }

    // ...
}

//==============================================================================
// Resume
//==============================================================================
void MainQueueDialog::resume()
{
    qDebug() << "MainQueueDialog::resume";

    // Set Paused State
    paused = false;
    // Set Pause Button Text
    pauseButton->setText(QString(DEFAULT_BUTTON_TEXT_PAUSE));

    // Check Operation Queue
    if (opQueue) {
        // Resume
        opQueue->resume();
    }

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void MainQueueDialog::clear()
{
    qDebug() << "MainQueueDialog::clear";

    // Disable Clear Button
    clearButton->setEnabled(false);
    // Disable Pause Button
    pauseButton->setEnabled(false);
    // Disable Abort Button
    abortButton->setEnabled(false);

    // ...

    // Check UI
    if (ui && ui->mainQueueList) {
        // Clear Main Queue List
        ui->mainQueueList->clear();
    }

    // Check UI
    if (ui && ui->queueProgress) {
        // Set Current Progress
        ui->queueProgress->setValue(0);
        // Set Maximum Value
        ui->queueProgress->setMaximum(0);
    }

    // ...
}

//==============================================================================
// Pause Button Pressed
//==============================================================================
void MainQueueDialog::pauseButtonPressed()
{
    qDebug() << "MainQueueDialog::pauseButtonPressed - paused: " << paused;

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
void MainQueueDialog::abortButtonPressed()
{
    qDebug() << "MainQueueDialog::abortButtonPressed";

    // ...
}

//==============================================================================
// Clear Button Pressed
//==============================================================================
void MainQueueDialog::clearButtonPressed()
{
    qDebug() << "MainQueueDialog::clearButtonPressed";
    // Abort
    abort();
    // Clear
    clear();

    // ...
}

//==============================================================================
// Operation Queue Processing Started Slot
//==============================================================================
void MainQueueDialog::opQueueStarted()
{
    qDebug() << "MainQueueDialog::opQueueStarted";

    // ...
}

//==============================================================================
// Operation Queue Processing Stopped Slot
//==============================================================================
void MainQueueDialog::opQueueStopped()
{
    qDebug() << "MainQueueDialog::opQueueStopped";

    // ...
}

//==============================================================================
// Operation Queue Processing Finished Slot
//==============================================================================
void MainQueueDialog::opQueueFinished()
{
    qDebug() << "MainQueueDialog::opQueueFinished";

    // Get Settings Instance

    // Get Settings - Close Dialog On Completed


}

//==============================================================================
// Operation Added Slot
//==============================================================================
void MainQueueDialog::operationAdded(const int& aIndex, const int& aCount)
{
    // Check UI
    if (ui && ui->mainQueueList && opQueue) {
        // Get Entry
        FileOperationEntry* newEntry = opQueue->getOperation(aIndex);
        // Check Entry
        if (newEntry) {
            qDebug() << "MainQueueDialog::operationAdded - aIndex: " << aIndex << " - aCount: " << aCount << " - newEntry: " << newEntry->getSource();

            // Init Template Text
            QString itemTextTemplate = QString("");
            // Init New Widget Item
            QListWidgetItem* newWidgetItem = NULL;

            // Check Operation Index
            if (newEntry->getOperationIndex() == OPERATION_ID_DELETE) {
                // Set Template Text
                itemTextTemplate = QString(FILE_OPERATION_QUEUE_TEXT_TEMPLATE_DELETE);
                // Create New Widget Item
                newWidgetItem = new QListWidgetItem(itemTextTemplate.arg(newEntry->getOperationName())
                                                                    .arg(newEntry->getSource()));
            } else {
                // Set Template Text
                itemTextTemplate = QString(FILE_OPERATION_QUEUE_TEXT_TEMPLATE_COPY);
                // Create New Widget Item
                newWidgetItem = new QListWidgetItem(itemTextTemplate.arg(newEntry->getOperationName())
                                                                    .arg(newEntry->getSource())
                                                                    .arg(newEntry->getTarget()));
            }

            // Get Main Queue List Count
            int mqlCount = ui->mainQueueList->count();

            // Check Index
            if (aIndex > mqlCount-1) {
                // Add New Item
                ui->mainQueueList->addItem(newWidgetItem);
            } else {
                // Add New Item
                ui->mainQueueList->insertItem(aIndex, newWidgetItem);
            }

            // Check Pause Button
            if (pauseButton && !pauseButton->isEnabled() && ui->mainQueueList->count() > 0) {
                // set Enabled State
                pauseButton->setEnabled(true);
            }

            // Check Abort Button
            if (abortButton && !abortButton->isEnabled() && ui->mainQueueList->count() > 0) {
                // Set Enabled State
                abortButton->setEnabled(true);
            }

            // Check Clear Button
            if (clearButton && !clearButton->isEnabled() && ui->mainQueueList->count() > 0) {
                // Set Enabled State
                clearButton->setEnabled(true);
            }
        }

        // Check UI
        if (ui->queueProgress) {
            // Set Max Value
            ui->queueProgress->setMaximum(ui->mainQueueList->count());
        }
    }
}

//==============================================================================
// Operation Removed Slot
//==============================================================================
void MainQueueDialog::operationRemoved(const int& aIndex, const int& aCount)
{
    qDebug() << "MainQueueDialog::operationRemoved - aIndex: " << aIndex << " - aCount: " << aCount;

    // Check UI
    if (ui && ui->mainQueueList) {
        // get Main Queue List Count
        int mqCount = ui->mainQueueList->count();
        // Check Index
        if (aIndex >= 0 && aIndex < mqCount) {
            // Get Item
            QListWidgetItem* item = ui->mainQueueList->item(aIndex);
            // Remove Item Widget
            ui->mainQueueList->removeItemWidget(item);
        }

        // Check UI
        if (ui->queueProgress) {
            // Set Max Value
            ui->queueProgress->setMaximum(ui->mainQueueList->count());
        }
    }
}

//==============================================================================
// Operation Started Slot
//==============================================================================
void MainQueueDialog::operationStarted(const int& aIndex)
{
    qDebug() << "MainQueueDialog::operationStarted - aIndex: " << aIndex;

    // Check UI
    if (ui && ui->mainQueueList) {
        // get Main Queue List Count
        int mqCount = ui->mainQueueList->count();
        // Check Index
        if (aIndex >= 0 && aIndex < mqCount) {
            // Get Item
            QListWidgetItem* item = ui->mainQueueList->item(aIndex);
            // Check Item
            if (item) {
                // Get Font
                QFont itemFont = item->font();
                // set Bold state
                itemFont.setBold(true);
                // Set Item Font
                item->setFont(itemFont);
            }
        }
    }
}

//==============================================================================
// Operation Stopped Slot
//==============================================================================
void MainQueueDialog::operationStopped(const int& aIndex)
{
    qDebug() << "MainQueueDialog::operationStarted - aIndex: " << aIndex;

    // Check UI
    if (ui && ui->mainQueueList) {
        // get Main Queue List Count
        int mqCount = ui->mainQueueList->count();
        // Check Index
        if (aIndex >= 0 && aIndex < mqCount) {
            // Get Item
            QListWidgetItem* item = ui->mainQueueList->item(aIndex);
            // Check Item
            if (item) {
                // Get Font
                QFont itemFont = item->font();
                // set Bold state
                itemFont.setBold(false);
                // Set Item Font
                item->setFont(itemFont);
            }
        }
    }
}

//==============================================================================
// Operation Completed Slot
//==============================================================================
void MainQueueDialog::operationCompleted(const int& aIndex)
{
    qDebug() << "MainQueueDialog::operationCompleted - aIndex: " << aIndex;

    // Add To Progress
    addToProgress(1);

    // Check UI
    if (ui && ui->mainQueueList) {
        // get Main Queue List Count
        int mqCount = ui->mainQueueList->count();
        // Check Index
        if (aIndex >= 0 && aIndex < mqCount) {
            // Get Item
            QListWidgetItem* item = ui->mainQueueList->item(aIndex);
            // Check Item
            if (item) {
                // Get Font
                QFont itemFont = item->font();
                // set Bold state
                itemFont.setBold(false);
                // Set Item Font
                item->setFont(itemFont);

                // Update Item Text
                item->setText(item->text() + QString(FILE_OPERATION_DONE));
            }
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
MainQueueDialog::~MainQueueDialog()
{
    qDebug() << "Deleteing MainQueueDialog...";
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

    qDebug() << "Deleteing MainQueueDialog...done";
}
