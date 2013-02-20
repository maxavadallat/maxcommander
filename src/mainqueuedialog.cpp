
// INCLUDES

#include <QPushButton>
#include <QFont>
#include <QDebug>

#include "mainwindow.h"
#include "confirmationdialog.h"
#include "infodialog.h"
#include "mainqueuedialog.h"
#include "ui_mainqueuedialog.h"
#include "constants.h"
#include "fileoperations.h"


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
    , qFinished(false)
    , opQueueHandler(NULL)
    , infoDialog(NULL)
    , deleteConfirmDialog(NULL)
    , overWriteConfirmDialog(NULL)
    , errorDialog(NULL)
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

    // Create Info Dialog - MUST BE DONE IN UI THREAD CONTEXT!
    infoDialog = new InfoDialog();



    // Create Delete Confirmation Dialog - MUST BE DONE IN UI THREAD CONTEXT!
    deleteConfirmDialog = new ConfirmationDialog();
    // Set Clear Custom Buttons On Exit
    deleteConfirmDialog->setClearCustomButtonsOnConfirm(false);
    // Set Title
    deleteConfirmDialog->setTitle(QString(DEFAULT_DIALOG_TITLE_DELETE_CONFIRM));
    // Clear Default Buttons
    deleteConfirmDialog->clearButtons();
    // Add Dialog Buttons - MUST BE DONE IN UI THREAD CONTEXT!
    deleteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_YES), QDialogButtonBox::ActionRole, (int)FOORTYes);
    deleteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_YESTOALL), QDialogButtonBox::ActionRole, (int)FOORTYesToAll);
    deleteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_NO), QDialogButtonBox::ActionRole, (int)FOORTNo, true);
    deleteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_NOTOALL), QDialogButtonBox::ActionRole, (int)FOORTNoToAll);
    deleteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);


    // Create Overwrite Confirm Dialog
    overWriteConfirmDialog = new ConfirmationDialog();
    // Set Clear Custom Buttons On Exit
    overWriteConfirmDialog->setClearCustomButtonsOnConfirm(false);
    // Set Title
    overWriteConfirmDialog->setTitle(QString(DEFAULT_DIALOG_TITLE_OVERWRITE_CONFIRM));
    // Clear Default Buttons
    overWriteConfirmDialog->clearButtons();
    // Add Dialog Buttons - MUST BE DONE IN UI THREAD CONTEXT!
    overWriteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_YES), QDialogButtonBox::ActionRole, (int)FOORTYes);
    overWriteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_YESTOALL), QDialogButtonBox::ActionRole, (int)FOORTYesToAll);
    overWriteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_NO), QDialogButtonBox::ActionRole, (int)FOORTNo, true);
    overWriteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_NOTOALL), QDialogButtonBox::ActionRole, (int)FOORTNoToAll);
    overWriteConfirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);


    // Create Error Dialog - MUST BE DONE IN UI THREAD CONTEXT!
    errorDialog = new ConfirmationDialog();
    // Set Clear Custom Buttons On Exit
    errorDialog->setClearCustomButtonsOnConfirm(false);
    // Set Title
    errorDialog->setTitle(QString(DEFUALT_DIALOG_TITLE_UNKNOWN_ERROR));
    // Clear Default Buttons
    errorDialog->clearButtons();
    // Add Dialog Buttons - MUST BE DONE IN UI THREAD CONTEXT!
    errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_RETRY), QDialogButtonBox::ActionRole, (int)FOORTRetry, true);
    errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ASADMIN), QDialogButtonBox::ActionRole, (int)FOORTAsRoot);
    errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNORE), QDialogButtonBox::ActionRole, (int)FOORTIgnore);
    errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNOREALL), QDialogButtonBox::ActionRole, (int)FOORTIgnoreAll);
    errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);


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
        if (!opQueueHandler) {
            // Create Operation Queue
            opQueueHandler = new FileOpQueueHandler(this, NULL, this, this, this, this);

            // Connect Signals
            connect(opQueueHandler, SIGNAL(opStarted()), this, SLOT(opQueueStarted()));
            connect(opQueueHandler, SIGNAL(opStopped()), this, SLOT(opQueueStopped()));
            connect(opQueueHandler, SIGNAL(opFinished()), this, SLOT(opQueueFinished()));

            connect(opQueueHandler, SIGNAL(operationAdded(int,int)), this, SLOT(operationAdded(int,int)));
            connect(opQueueHandler, SIGNAL(operationStarted(int)), this, SLOT(operationStarted(int)));
            connect(opQueueHandler, SIGNAL(operationCompleted(int)), this, SLOT(operationCompleted(int)));
        }

        // Add Entry
        opQueueHandler->addOperation(aEntry);
    }
}

//==============================================================================
// Remove Operation
//==============================================================================
void MainQueueDialog::removeOperationEntry(const int& aIndex)
{
    // Check Operation Queue
    if (opQueueHandler) {
        qDebug() << "MainQueueDialog::removeOperationEntry - aIndex: " << aIndex;
        // Remove Entry
        opQueueHandler->removeOperation(aIndex);
    }
}

//==============================================================================
// Get Operations Count
//==============================================================================
int MainQueueDialog::opEntryCount()
{
    // Check Operations Queue
    if (opQueueHandler) {
        return opQueueHandler->count();
    }

    return 0;
}

//==============================================================================
// Get Operation
//==============================================================================
FileOperationEntry* MainQueueDialog::getOperationEntry(const int& aIndex)
{
    // Check Operations Queue
    if (opQueueHandler) {
        return opQueueHandler->getOperation(aIndex);
    }

    return NULL;
}

//==============================================================================
// Process Queue
//==============================================================================
void MainQueueDialog::processQueue()
{
    // Check Operation Queue
    if (opQueueHandler) {
        // Get Operations Count
        int qCount =  opQueueHandler->count();

        qDebug() << "MainQueueDialog::processQueue - qCount: " << qCount;

        // Process Queue
        opQueueHandler->processQueue();
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
// Operation Entry Added Callback - SIGNALS DON't WORK
//==============================================================================
void MainQueueDialog::operationEntryAdded(const int& aIndex, const int& aCount)
{
    qDebug() << "MainQueueDialog::operationEntryAdded - aIndex: " << aIndex << " - aCount: " << aCount;

    // ...
}

//==============================================================================
// Operation Entry Removed Callback - SIGNALS DON'T WORK
//==============================================================================
void MainQueueDialog::operationEntryRemoved(const int& aIndex, const int& aCount)
{
    qDebug() << "MainQueueDialog::operationEntryRemoved - aIndex: " << aIndex << " - aCount: " << aCount;

    // ...
}

//==============================================================================
// Operation Entry Updated Callback - SIGNALS DON'T WORK
//==============================================================================
void MainQueueDialog::operationEntryUpdated(const int& aIndex)
{
    qDebug() << "MainQueueDialog::operationEntryUpdated - aIndex: " << aIndex;

    // ...
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

    // ...
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
    if (opQueueHandler) {
        // Abort
        opQueueHandler->stop();
    }

    // Configure Buttons
    configureButtons();

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
    if (opQueueHandler) {
        // Pause
        opQueueHandler->pause();
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
    if (opQueueHandler) {
        // Resume
        opQueueHandler->resume();
    }

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void MainQueueDialog::clear()
{
    qDebug() << "MainQueueDialog::clear";

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
        // Set Mminimum Value
        ui->queueProgress->setMinimum(0);
        // Set Maximum Value
        ui->queueProgress->setMaximum(0);
    }

    // ...

    // Check Operation Queue
    if (opQueueHandler) {
        // Stop Queue
        opQueueHandler->stop();
        // Clear
        opQueueHandler->clear();
    }

    // Configure Buttons
    configureButtons();

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void MainQueueDialog::configureButtons()
{
    // Check UI
    if (ui && ui->mainQueueList) {
        qDebug() << "MainQueueDialog::configureButtons";

        // Init Pause Enabled
        bool pauseEnabled = (ui->mainQueueList->count() > 0);
        // Init Abort Enabled
        bool abortEnabled = (ui->mainQueueList->count() > 0);
        // Init Clear Enabled
        bool clearEnabled = (ui->mainQueueList->count() > 0);

        // Check Finished State
        if (qFinished || (opQueueHandler && opQueueHandler->stopping())) {
            // Set Pause Enabled
            pauseEnabled = false;
            // Set Abort Enabled
            abortEnabled = false;
        }

        // Check Pause Button
        if (pauseButton) {
            // Set Enabled State
            pauseButton->setEnabled(pauseEnabled);
        }

        // Check Abort Button
        if (abortButton) {
            // Set Enabled State
            abortButton->setEnabled(abortEnabled);
        }

        // Check Clear Button
        if (clearButton) {
            // Set Enabled State
            clearButton->setEnabled(clearEnabled);
        }
    }
}


//==============================================================================
// Confirm Deletion Callback
//==============================================================================
int MainQueueDialog::confirmDeletion(const QString& aFilePath, const bool& aReadOnly, const bool& aNonEmpty)
{
    // Check Delete Confirm Dialog
    if (!deleteConfirmDialog) {
        return (int)FOORTNo;
    }

    qDebug() << "MainQueueDialog::confirmDeletion - aFilePath: " << aFilePath << " - aReadOnly: " << aReadOnly << " - aNonEmpty: " << aNonEmpty;

    // Check Read Only Flag
    if (aReadOnly) {
        // Set Dialog Text
        deleteConfirmDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_DELETE_CONFIRM_READONLY).arg(aFilePath));
    // Check Non Empty Dir Flag
    } else if (aNonEmpty) {
        // Set Dialog Text
        deleteConfirmDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_DELETE_CONFIRM_NONEMPTY).arg(aFilePath));
    } else {
        // Set Dialog Text
        deleteConfirmDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_DELETE_CONFIRM).arg(aFilePath));
    }

    // Get Main Window Instance
    MainWindow* mainWindow = MainWindow::getInstance();

    // Check Main Window Instance
    if (mainWindow) {

        // Init Confirm Event Loop
        QEventLoop ceLoop;

        // Connect Signal
        connect(this, SIGNAL(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)), mainWindow, SLOT(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)));
        // Emit Show Confirm Signal
        emit showConfirmation(this, aNonEmpty ? ECTDeleteNonEmpty : aReadOnly ? ECTDeleteReadOnly : ECTDelete, &ceLoop);
        // Disconnect Signal
        disconnect(this, SIGNAL(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)), mainWindow, SLOT(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)));

        // Start Confirm Event Loop
        int result = ceLoop.exec();

        qDebug() << "MainQueueDialog::confirmDeletion - aFilePath: " << aFilePath << " - result: " << result;

        // Release Main Window Instance
        mainWindow->release();

        return result;
    }

    return (int)FOORTNo;
}

//==============================================================================
// Deletion Error Callback
//==============================================================================
int MainQueueDialog::deleteError(const QString& aFilePath, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::deleteError - aFilePath: " << aFilePath << " - aErrorCode: " << aErrorCode;


    // ...

    return (int)FOORTIgnore;
}

//==============================================================================
// Delete File Started Notification Callback
//==============================================================================
void MainQueueDialog::deleteFileStarted(const QString& aFilePath)
{
    qDebug() << "MainQueueDialog::deleteFileStarted - aFilePath: " << aFilePath;

    // ...
}

//==============================================================================
// Delete File Finished Notification Callback
//==============================================================================
void MainQueueDialog::deleteFileFinished(const QString& aFilePath, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::deleteFileFinished - aFilePath: " << aFilePath << " - aErrorCode: " << aErrorCode;

    // ...
}

//==============================================================================
// Confirm OverWrite Callback
//==============================================================================
int MainQueueDialog::confirmCopyOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly)
{
    // Check Copy Overwrite Confirm Dialog
    if (!overWriteConfirmDialog) {
        return (int)FOORTNo;
    }

    qDebug() << "MainQueueDialog::confirmCopyOverWrite - aSource: " << aSource << " - aTarget: " << aTarget << " - aReadOnly: " << aReadOnly;

    // Check Read Only Flag
    if (aReadOnly) {
        // Set Dialog Text
        overWriteConfirmDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_OVERWRITE_CONFIRM_READONLY).arg(aTarget));
    } else {
        // Set Dialog Text
        overWriteConfirmDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_OVERWRITE_CONFIRM).arg(aTarget));
    }

    // Get Main Window Instance
    MainWindow* mainWindow = MainWindow::getInstance();

    // Check Main Window Instance
    if (mainWindow) {

        // Init Confirm Event Loop
        QEventLoop ceLoop;

        // Connect Signal
        connect(this, SIGNAL(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)), mainWindow, SLOT(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)));
        // Emit Show Confirm Signal
        emit showConfirmation(this, aReadOnly ? ECTOverWriteReadOnly : ECTOverWrite, &ceLoop);
        // Disconnect Signal
        disconnect(this, SIGNAL(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)), mainWindow, SLOT(showConfirmation(ConfirmDialogProvider*,int,QEventLoop*)));

        // Start Confirm Event Loop
        int result = ceLoop.exec();

        qDebug() << "MainQueueDialog::confirmCopyOverWrite - aTarget: " << aTarget << " - result: " << result;

        // Release Main Window Instance
        mainWindow->release();

        return result;
    }

    return (int)FOORTNo;
}

//==============================================================================
// Copy File Error Callback
//==============================================================================
int MainQueueDialog::copyError(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::copyError - aSource: " << aSource << " - aTarget: " << aTarget << " - aErrorCode: " << aErrorCode;

    // Check Delete Confirm Dialog
    if (!errorDialog) {
        return (int)FOORTIgnore;
    }

    // Switch Error Code
    switch (aErrorCode) {

#if defined (Q_OS_WIN)
        case ERROR_INVALID_PARAMETER:
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error Manually
        case EINVAL:
#endif // Q_OS_MAC || Q_OS_UNIX
            // Set Dialog Title
            errorDialog->setTitle(QString(DEFUALT_DIALOG_TITLE_COPY_ERROR));
            // Set Dialog Text
            errorDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_ERROR_TARGET_SAME).arg(aTarget));
        break;

        default:
        break;
    }

    // Set Error Dialog Text

    // Get Main Window Instance
    MainWindow* mainWindow = MainWindow::getInstance();

    // Check Main Window Instance
    if (mainWindow) {

        // Init Error Event Loop
        QEventLoop eeLoop;

        // Connect Signal
        connect(this, SIGNAL(showError(ErrorDialogProvider*,int,QEventLoop*)), mainWindow, SLOT(showError(ErrorDialogProvider*,int,QEventLoop*)));
        // Emit Show Error Signal
        emit showError(this, aErrorCode, &eeLoop);
        // Disconnect Signal
        disconnect(this, SIGNAL(showError(ErrorDialogProvider*,int,QEventLoop*)), mainWindow, SLOT(showError(ErrorDialogProvider*,int,QEventLoop*)));

        // Start Confirm Event Loop
        int result = eeLoop.exec();

        qDebug() << "MainQueueDialog::copyError - aTarget: " << aTarget << " - result: " << result;

        // Release Main Window Instance
        mainWindow->release();

        // Return Result
        return result;
    }

    return (int)FOORTIgnore;
}

//==============================================================================
// Copy File Started Callback
//==============================================================================
void MainQueueDialog::copyStarted(const QString& aSource, const QString& aTarget)
{
    qDebug() << "MainQueueDialog::copyStarted - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// Copy File Progress Callback
//==============================================================================
void MainQueueDialog::copyProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize)
{
    qDebug() << "MainQueueDialog::copyProgress - aSource: " << aSource << " - aTarget: " << aTarget << " - aCurrentSize: " << aCurrentSize << " - aFullSize: " << aFullSize;

    // ...
}

//==============================================================================
// Copy File Finished Callback
//==============================================================================
void MainQueueDialog::copyFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::copyFinished - aSource: " << aSource << " - aTarget: " << aTarget << " - aErrorCode: " << aErrorCode;

    // ...
}

//==============================================================================
// Confirm OverWrite Callback
//==============================================================================
int MainQueueDialog::confirmMoveOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly)
{
    qDebug() << "MainQueueDialog::confirmMoveOverWrite - aSource: " << aSource << " - aTarget: " << aTarget << " - aReadOnly: " << aReadOnly;

    // ...

    return (int)FOORTNo;
}

//==============================================================================
// Confirm Source Deletion Callback
//==============================================================================
int MainQueueDialog::confirmMoveDeletion(const QString& aSource, const bool& aReadOnly, const bool& aNonEmpty)
{
    qDebug() << "MainQueueDialog::confirmMoveDeletion - aSource: " << aSource << " - aReadOnly: " << aReadOnly << " - aNonEmpty: " << aNonEmpty;

    // ...

    return (int)FOORTNo;
}

//==============================================================================
// Rename/Move File Error Callback
//==============================================================================
int MainQueueDialog::moveError(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::moveError - aSource: " << aSource << " - aTarget: " << aTarget << " - aErrorCode: " << aErrorCode;

    // ...

    return (int)FOORTIgnore;
}

//==============================================================================
// Rename/Move File Started Callback
//==============================================================================
void MainQueueDialog::moveStarted(const QString& aSource, const QString& aTarget)
{
    qDebug() << "MainQueueDialog::moveStarted - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// Renamve/Move File Progress Callback
//==============================================================================
void MainQueueDialog::moveProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize)
{
    qDebug() << "MainQueueDialog::moveProgress - aSource: " << aSource << " - aTarget: " << aTarget << " - aCurrentSize: " << aCurrentSize << " - aFullSize: " << aFullSize;

    // ...
}

//==============================================================================
// Rename/Move File Finished Callback
//==============================================================================
void MainQueueDialog::moveFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::moveFinished - aSource: " << aSource << " - aTarget: " << aTarget << " - aErrorCode: " << aErrorCode;

    // ...
}

//==============================================================================
// Confirm OverWrite Callback
//==============================================================================
int MainQueueDialog::confirmRenameOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly)
{
    qDebug() << "MainQueueDialog::confirmRenameOverWrite - aSource: " << aSource << " - aTarget: " << aTarget << " - aReadOnly: " << aReadOnly;

    // ...

    return (int)FOORTNo;
}

//==============================================================================
// Confirm Source Deletion Callback
//==============================================================================
int MainQueueDialog::confirmRenameDeletion(const QString& aSource, const bool& aReadOnly, const bool& aNonEmpty)
{
    qDebug() << "MainQueueDialog::confirmRenameDeletion - aSource: " << aSource << " - aReadOnly: " << aReadOnly << " - aNonEmpty: " << aNonEmpty;

    // ...

    return (int)FOORTNo;
}

//==============================================================================
// Rename File Error Callback
//==============================================================================
int MainQueueDialog::renameError(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::renameError - aSource: " << aSource << " - aTarget: " << aTarget << " - aErrorCode: " << aErrorCode;

    // ...

    return (int)FOORTIgnore;
}

//==============================================================================
// Rename File Started Callback
//==============================================================================
void MainQueueDialog::renameStarted(const QString& aSource, const QString& aTarget)
{
    qDebug() << "MainQueueDialog::renameStarted - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// Rename File Finished Callback
//==============================================================================
void MainQueueDialog::renameFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::renameFinished - aSource: " << aSource << " - aTarget: " << aTarget << " - aErrorCode: " << aErrorCode;

    // ...
}

//==============================================================================
// Launch Info Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
//==============================================================================
int MainQueueDialog::launchInfo(const int& aType)
{
    qDebug() << "MainQueueDialog::launchInfo - aType: " << aType;

    int result = (int)FOORTNo;

    // ...

    return result;
}

//==============================================================================
// Exit Info Dialog
//==============================================================================
void MainQueueDialog::exitInfo(QEventLoop* aEventLoop, const int& aResult)
{
    qDebug() << "MainQueueDialog::exitInfo - aEventLoop: " << aEventLoop << " - aResult: " << aResult;

    // Check Event Loop
    if (aEventLoop) {
        // Exit Event Loop
        aEventLoop->exit(aResult);
    }
}

//==============================================================================
// Launch Confirmation Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
//==============================================================================
int MainQueueDialog::launchConfirm(const int& aType)
{
    qDebug() << "MainQueueDialog::launchConfirm - aType: " << aType;

    int result = (int)FOORTNo;

    // Switch Type
    switch (aType) {
        case ECTDelete:
        case ECTDeleteReadOnly:
        case ECTDeleteNonEmpty:
            // Check Delete Confirmation Dialog
            if (deleteConfirmDialog) {
                // Set Size
                //deleteConfirmDialog->resize(deleteConfirmDialog->minimumSize());
                // Exec Dialog
                result = deleteConfirmDialog->exec();
            }
        break;

        case ECTOverWrite:
        case ECTOverWriteReadOnly:
            // Check Overwrite Confirmation Dialog
            if (overWriteConfirmDialog) {
                // Set Size
                //overWriteConfirmDialog->resize(overWriteConfirmDialog->minimumSize());
                // Exec Dialog
                result = overWriteConfirmDialog->exec();
            }
        break;
    }

    return result;
}

//==============================================================================
// Exit Confirmation Dialog
//==============================================================================
void MainQueueDialog::exitConfirm(QEventLoop* aEventLoop, const int& aResult)
{
    qDebug() << "MainQueueDialog::exitConfirm - aEventLoop: " << aEventLoop << " - aResult: " << aResult;

    // Check Event Loop
    if (aEventLoop) {
        // Exit Event Loop
        aEventLoop->exit(aResult);
    }
}

//==============================================================================
// Launch Confirmation Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
//==============================================================================
int MainQueueDialog::launchError(const int& aErrorCode)
{
    qDebug() << "MainQueueDialog::launchError - aErrorCode: " << aErrorCode;

    // Init result
    int result = (int)FOORTIgnore;

    // Check Error Dialog
    if (errorDialog) {

        // Set Up Buttons

        // Switch Error Code
        switch (aErrorCode) {
#if defined (Q_OS_WIN)
            case ERROR_INVALID_PARAMETER:
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            case EINVAL:
#endif // Q_OS_MAC || Q_OS_UNIX
                // Clear Default Buttons
                errorDialog->clearButtons();
                // Add Dialog Buttons - MUST BE DONE IN UI THREAD CONTEXT!
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_RENAME), QDialogButtonBox::ActionRole, (int)FOORTRename);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNORE), QDialogButtonBox::ActionRole, (int)FOORTIgnore);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNOREALL), QDialogButtonBox::ActionRole, (int)FOORTIgnoreAll);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);
            break;

            default:

                // Clear Default Buttons
                errorDialog->clearButtons();
                // Add Dialog Buttons - MUST BE DONE IN UI THREAD CONTEXT!
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_RETRY), QDialogButtonBox::ActionRole, (int)FOORTRetry, true);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ASADMIN), QDialogButtonBox::ActionRole, (int)FOORTAsRoot);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNORE), QDialogButtonBox::ActionRole, (int)FOORTIgnore);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNOREALL), QDialogButtonBox::ActionRole, (int)FOORTIgnoreAll);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);

            break;
        }

        // ...

        // Exec Error Dialog
        result = errorDialog->exec();
    }

    return result;
}

//==============================================================================
// Exit Error Dialog
//==============================================================================
void MainQueueDialog::exitError(QEventLoop* aEventLoop, const int& aResult)
{
    qDebug() << "MainQueueDialog::exitError - aEventLoop: " << aEventLoop << " - aResult: " << aResult;

    // Check Event Loop
    if (aEventLoop) {
        // Exit Event Loop
        aEventLoop->exit(aResult);
    }
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

    // Abort
    abort();

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

    // Set Queue Finished Flag
    qFinished = false;

    // ...
}

//==============================================================================
// Operation Queue Processing Stopped Slot
//==============================================================================
void MainQueueDialog::opQueueStopped()
{
    qDebug() << "MainQueueDialog::opQueueStopped";

    // Configure Buttons
    configureButtons();

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

    // Setting Queue Finished Flag
    qFinished = true;

    // Configure Buttons
    configureButtons();

/*
    // Hide
    hide();

    // Clear
    clear();
*/
}

//==============================================================================
// Operation Added Slot
//==============================================================================
void MainQueueDialog::operationAdded(const int& aIndex, const int& aCount)
{
    // Check UI
    if (ui && ui->mainQueueList && opQueueHandler) {
        // Get Entry
        FileOperationEntry* newEntry = opQueueHandler->getOperation(aIndex);
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
        }
/*
        // Check UI
        if (ui->queueProgress) {
            // Set Max Value
            ui->queueProgress->setMaximum(ui->mainQueueList->count());
        }
*/
        // Set Main Queue Progress Bar Max Value
        setMaxValue(ui->mainQueueList->count());

        // Configure Buttons
        configureButtons();
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
/*
        // Check UI
        if (ui->queueProgress) {
            // Set Max Value
            ui->queueProgress->setMaximum(ui->mainQueueList->count());
        }
*/
        // Set Main Queue Progress Bar Max Value
        setMaxValue(ui->mainQueueList->count());
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
// Operation Updated Slot
//==============================================================================
void MainQueueDialog::operationUpdated(const int& aIndex)
{
    qDebug() << "MainQueueDialog::operationUpdated - aIndex: " << aIndex;

    // Check UI
    if (ui && ui->mainQueueList && opQueueHandler) {
        // get Main Queue List Count
        int mqCount = ui->mainQueueList->count();
        // Check Index
        if (aIndex >= 0 && aIndex < mqCount) {
            // Get Item
            QListWidgetItem* item = ui->mainQueueList->item(aIndex);
            // Get Operation Entry
            FileOperationEntry* opEntry = opQueueHandler->getOperation(aIndex);
            // Check Item
            if (item && opEntry) {
                // Get Font
                QFont itemFont = item->font();
                // set Bold state
                itemFont.setBold(false);
                // Set Item Font
                item->setFont(itemFont);

                // Init Status String
                QString statusString = QString("");

                // Compose Status String

                // Update Item Text
                item->setText(item->text() + statusString);
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
    if (ui && ui->mainQueueList && opQueueHandler) {
        // get Main Queue List Count
        int mqCount = ui->mainQueueList->count();
        // Check Index
        if (aIndex >= 0 && aIndex < mqCount) {
            // Get Item
            QListWidgetItem* item = ui->mainQueueList->item(aIndex);
            // Get Operation Entry
            FileOperationEntry* opEntry = opQueueHandler->getOperation(aIndex);
            // Check Item
            if (item && opEntry) {
                // Get Font
                QFont itemFont = item->font();
                // set Bold state
                itemFont.setBold(false);
                // Set Item Font
                item->setFont(itemFont);

                // Init Status String
                QString statusString = QString("");

                // Switch Entry State
                switch (opEntry->getState()) {
                    default:
                    case FOSDone:       statusString = QString(FILE_OPERATION_DONE);    break;
                    case FOSSkipped:    statusString = QString(FILE_OPERATION_SKIPPED); break;
                    case FOSFailed:     statusString = QString(FILE_OPERATION_FAILED);  break;
                }

                // Update Item Text
                item->setText(item->text() + statusString);
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

    // Check Info Dialog
    if (infoDialog) {
        // Delete Info Dialog
        delete infoDialog;
        infoDialog = NULL;
    }

    // Check Delete Confirmation Dialog
    if (deleteConfirmDialog) {
        // Delete Dialog
        delete deleteConfirmDialog;
        deleteConfirmDialog = NULL;
    }

    // Check Over Write Confirmation Dialog
    if (overWriteConfirmDialog) {
        // Delete OverWirte Confirmation Dialog
        delete overWriteConfirmDialog;
        overWriteConfirmDialog = NULL;
    }

    // Check Error Dialog
    if (errorDialog) {
        // Delete Error Dialog
        delete errorDialog;
        errorDialog = NULL;
    }

    // Check Operation Queue
    if (opQueueHandler) {
        // Delete Operations Queue
        delete opQueueHandler;
        opQueueHandler = NULL;
    }

    // Delete UI
    delete ui;

    qDebug() << "Deleteing MainQueueDialog...done";
}
