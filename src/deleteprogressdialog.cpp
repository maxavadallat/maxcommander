#include <QTimer>
#include <QDir>
#include <QQmlEngine>
#include <QQmlContext>
#include <QImageReader>
#include <QDebug>

#include <mcwinterface.h>

#include "ui_deleteprogressdialog.h"

#include "deleteprogressdialog.h"
#include "deleteprogressmodel.h"
#include "infodialog.h"
#include "confirmdialog.h"
#include "busyindicator.h"
#include "remotefileutilclient.h"
#include "filelistimageprovider.h"
#include "settingscontroller.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
DeleteProgressDialog::DeleteProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteProgressDialog)
    , settings(SettingsController::getInstance())
    , queueModel(NULL)
    , fileUtil(NULL)
    , closeWhenFinished(false)
    , queueIndex(-1)
    , panelHasFocus(true)
    , dirPath("")
    , needQueue(false)
    , progressRefreshTimerID(-1)
    , archiveMode(false)
{
    qDebug() << "DeleteProgressDialog::DeleteProgressDialog";

    // Setup UI
    ui->setupUi(this);

    // Set Minimum
    ui->currentProgress->setMinimum(0);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void DeleteProgressDialog::init()
{
    qDebug() << "DeleteProgressDialog::init";

    // Create New Queue Model
    queueModel = new DeleteProgressModel();

    // Get Root Contenxt
    QQmlContext* ctx = ui->deleteQueueListView->rootContext();

    // Set Context Property - Main Controller
    ctx->setContextProperty(DEFAULT_MAIN_CONTROLLER_NAME, this);
    // Set Context Property - Transfer List Model
    ctx->setContextProperty(DEFAULT_FILE_DELETE_LIST_MODEL, queueModel);
    // Set Context Property - Settings
    ctx->setContextProperty(DEFAULT_GLOBAL_SETTINGS_CONTROLLER, settings);

    // Get Engine
    QQmlEngine* engine = ui->deleteQueueListView->engine();
    // Create New Image Provider
    FileListImageProvider* newImageProvider = new FileListImageProvider();
    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_FILE_ICON_PROVIDER_ID), newImageProvider);

    // Register Busy Indicator
    qmlRegisterType<BusyIndicator>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, DEFAULT_CUSTOM_COMPONENTS_BUSY_INDICATOR);

    // Set Resize Mode
    ui->deleteQueueListView->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // Set Source
    ui->deleteQueueListView->setSource(QUrl(DEFAULT_FILE_DELETE_LIST_QMLFILE_URL));

    // ...

    // Create File Util
    fileUtil = new RemoteFileUtilClient();

    // Check File Util
    if (fileUtil) {
        // Connect Signals
        connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpProgress(uint,QString,QString,quint64,quint64)), this, SLOT(fileOpProgress(uint,QString,QString,quint64,quint64)));
        connect(fileUtil, SIGNAL(fileOpSuspended(uint,QString,QString,QString,QString)), this, SLOT(fileOpSuspended(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpResumed(uint,QString,QString,QString,QString)), this, SLOT(fileOpResumed(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(dirListItemFound(uint,QString,QString)), this, SLOT(dirListItemFound(uint,QString,QString)));
    }

    // Connect To File Server
    //fileUtil->connectToFileServer();

    // Connect Signals
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBoxAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(buttonBoxRejected()));

    // Connect Signals
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    // Get Supported Image Formats Bye Array
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    // Get Count
    int flCount = formats.count();
    // Go Thru Formats
    for (int i=0; i<flCount; ++i) {
        // Add Format String
        supportedImageFormats << QString(formats[i]).toLower();
    }

    // ...
}

//==============================================================================
// Build Queue
//==============================================================================
bool DeleteProgressDialog::buildQueue(const QString& aDirPath, const QStringList& aSelectedFiles)
{
    // Get Selected Files Count
    int sfCount = aSelectedFiles.count();

    // Check Selected Files Count
    if (sfCount <= 0) {
        qDebug() << "DeleteProgressDialog::buildQueue - aDirPath: " << aDirPath << " - NO SELECTED FILES TO DELETE!";

        return false;
    }

    // Check Queue Model
    if (!queueModel) {
        qDebug() << "DeleteProgressDialog::buildQueue - aDirPath: " << aDirPath << " - NO QUEUE MODEL!";

        return false;
    }

    qDebug() << "DeleteProgressDialog::buildQueue - aDirPath: " << aDirPath << " - sfCount: " << sfCount;

    // Go Thru Selected Files
    for (int i=0; i<sfCount; ++i) {
        // Add To Queue Model
        queueModel->addItem(aSelectedFiles[i]);
    }

    return true;
}

//==============================================================================
// Process Queue
//==============================================================================
void DeleteProgressDialog::processQueue()
{
    // Check File Util & Queue Model
    if (fileUtil && queueModel) {
        // Check Queue Index
        if (queueIndex >= 0 && queueIndex < queueModel->rowCount()) {
            qDebug() << "DeleteProgressDialog::processQueue";
            // Get File Name
            QString fileName = queueModel->getFileName(queueIndex);
            // Delete File
            fileUtil->deleteFile(fileName);
            // Configure Buttons
            configureButtons(QDialogButtonBox::Abort);
        } else {

            // Check Close When Finished
            if (closeWhenFinished) {
                // Close
                close();
            } else {
                // Configure Buttons
                configureButtons(QDialogButtonBox::Close);
                // Set Current File
                setCurrentFileName("");
                // Set Maximum
                ui->currentProgress->setMaximum(1);
                // Set Current Value
                ui->currentProgress->setValue(1);
            }
        }
    }
}

//==============================================================================
// Clear Queue
//==============================================================================
void DeleteProgressDialog::clearQueue()
{
    // Check Queue
    if (queueModel) {
        qDebug() << "DeleteProgressDialog::clearQueue";

        // Clear Model
        queueModel->clear();

        // Reset Queue Index
        setQueueIndex(-1);
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void DeleteProgressDialog::restoreUI()
{
    qDebug() << "DeleteProgressDialog::restoreUI";

    // Get Close When Finished
    closeWhenFinished = settings->value(SETTINGS_KEY_CLOSE_WHEN_FINISHED, false).toBool();
    // Set Checkbox
    ui->closeWhenFinishedCheckBox->setChecked(closeWhenFinished);

    // Update Queue Column Sizes
    updateQueueColumnSizes();
}

//==============================================================================
// Save Settings
//==============================================================================
void DeleteProgressDialog::saveSettings()
{
    // Check Settings
    if (settings) {
        return;
    }

    qDebug() << "DeleteProgressDialog::saveSettings";

    // Get Close When Finished
    closeWhenFinished = ui->closeWhenFinishedCheckBox->checkState() == Qt::Checked;
    // Save Setting
    settings->setValue(SETTINGS_KEY_CLOSE_WHEN_FINISHED, closeWhenFinished);
}

//==============================================================================
// Configure Buttons
//==============================================================================
void DeleteProgressDialog::configureButtons(const QDialogButtonBox::StandardButtons& aButtons)
{
    qDebug() << "DeleteProgressDialog::configureButtons";

    // Configure Buttons
    ui->buttonBox->setStandardButtons(aButtons);
}

//==============================================================================
// Update Queue Column Sizes
//==============================================================================
void DeleteProgressDialog::updateQueueColumnSizes()
{
    qDebug() << "DeleteProgressDialog::updateQueueColumnSizes";

    // ...
}

//==============================================================================
// Start Progress Refresh Timer
//==============================================================================
void DeleteProgressDialog::startProgressRefreshTimer()
{
    // Check Timer ID
    if (progressRefreshTimerID == -1) {
        // Start Timer
        progressRefreshTimerID = startTimer(DEFAULT_ONE_SEC / 10);
    }
}

//==============================================================================
// Stop Progress Refresh Timer
//==============================================================================
void DeleteProgressDialog::stopProgressRefreshTimer()
{
    // Chekc Timer ID
    if (progressRefreshTimerID != -1) {
        // Kill Timer
        killTimer(progressRefreshTimerID);
        // Reset Timer ID
        progressRefreshTimerID = -1;
    }
}

//==============================================================================
// Get Supported Image Formats For Delete Queue List
//==============================================================================
QStringList DeleteProgressDialog::getSupportedImageFormats()
{
    return supportedImageFormats;
}

//==============================================================================
// Set Queue Index
//==============================================================================
void DeleteProgressDialog::setQueueIndex(const int& aQueueIndex)
{
    // Check Queue Index
    if (queueIndex != aQueueIndex) {
        // Set Queue Index
        queueIndex = aQueueIndex;
        // Emit Current Index Changed Signal
        emit currentIndexChanged(queueIndex);
    }
}

//==============================================================================
// Set Archive Mode
//==============================================================================
void DeleteProgressDialog::setArchiveMode(const bool& aArchiveMode)
{
    // Check Archive Mode
    if (archiveMode != aArchiveMode) {
        // Set Archive Mode
        archiveMode = aArchiveMode;
        // Emit Archive Mode Changed Signal
        emit archiveModeChanged(archiveMode);

        // ...
    }
}

//==============================================================================
// Launch Progress Dialog
//==============================================================================
void DeleteProgressDialog::launch(const QString& aDirPath, const QStringList& aSelectedFiles)
{
    qDebug() << "DeleteProgressDialog::launch - aDirPath: " << aDirPath << " - count: " << aSelectedFiles.count();

    // Set Dir Path
    dirPath = aDirPath;
    // Reset Pattern
    pattern = "";

    // Restore UI
    restoreUI();
    // Show
    show();

    // Reset Need Queue
    needQueue = false;

    // Build Queue
    if (buildQueue(aDirPath, aSelectedFiles)) {
        // Set Queue Index
        setQueueIndex(0);

        // Chekc File Util
        if (fileUtil) {
            // Check If Connected
            if (!fileUtil->isConnected()) {
                // Connect
                fileUtil->connectToFileServer();
            } else {
                // Clear Options
                fileUtil->clearFileTransferOptions();
                // Process Queue
                QTimer::singleShot(1, this, SLOT(processQueue()));
            }
        }
    } else {
        // Reset Queue Index
        setQueueIndex(-1);
    }
}

//==============================================================================
// Launch Progress Dialog
//==============================================================================
void DeleteProgressDialog::launch(const QString& aDirPath, const QString& aPattern)
{
    qDebug() << "DeleteProgressDialog::launch - aDirPath: " << aDirPath << " - aPattern: " << aPattern;

    // Set Dir Path
    dirPath = aDirPath;
    // Set Pattern
    pattern = aPattern;

    // Restore UI
    restoreUI();
    // Show
    show();

    // Set Queue Index
    setQueueIndex(0);

    // Set Need Queue
    needQueue = true;

    // Chekc File Util
    if (fileUtil) {
        // Check If Connected
        if (!fileUtil->isConnected()) {
            // Connect
            fileUtil->connectToFileServer();
        } else {
            // Get Dir List
            fileUtil->getDirList(dirPath);
        }
    }
}

//==============================================================================
// Get Dir Path
//==============================================================================
QString DeleteProgressDialog::getDirPath()
{
    return dirPath;
}

//==============================================================================
// Suspend
//==============================================================================
void DeleteProgressDialog::suspend()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "DeleteProgressDialog::suspend";

        // Suspend
        fileUtil->suspend();
    }
}

//==============================================================================
// Resume
//==============================================================================
void DeleteProgressDialog::resume()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "DeleteProgressDialog::resume";

        // Resume
        fileUtil->resume();
    }
}

//==============================================================================
// Abort
//==============================================================================
void DeleteProgressDialog::abort()
{
    // Check File Util
    if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTSuspended || fileUtil->getStatus() == ECSTWaiting)) {
        qDebug() << "DeleteProgressDialog::abort";

        // Abort
        fileUtil->abort();
    }
}

//==============================================================================
// Get Current Index
//==============================================================================
int DeleteProgressDialog::getCurrentIndex()
{
    return queueIndex;
}

//==============================================================================
// Get Panel Focus
//==============================================================================
bool DeleteProgressDialog::getPanelFocus()
{
    return panelHasFocus;
}

//==============================================================================
// Set Panel Focus
//==============================================================================
void DeleteProgressDialog::setPanelFocus(const bool& aPanelFocus)
{
    // Chekc Panel Has Focus
    if (panelHasFocus != aPanelFocus) {
        // Set Panel Has Focus
        panelHasFocus = aPanelFocus;
        // Emit Panel Focus Changed Signal
        emit panelFocusChanged(panelHasFocus);
    }
}

//==============================================================================
// Get Archive Mode
//==============================================================================
bool DeleteProgressDialog::getArchiveMode()
{
    return archiveMode;
}

//==============================================================================
// Set Current File Name
//==============================================================================
void DeleteProgressDialog::setCurrentFileName(const QString& aCurrentFileName)
{
    // Set Text
    ui->currentFileNameLabel->setText(aCurrentFileName);

    // Check Current File Name
    if (aCurrentFileName.isEmpty()) {
        // Set Title Label
        ui->currentFileTitleLabel->setText("");
    } else {
        // Set Title Label
        ui->currentFileTitleLabel->setText(tr(DEFAULT_LABEL_CURRENT_FILE_TITLE));
    }
}

//==============================================================================
// Set Current File Progress
//==============================================================================
void DeleteProgressDialog::setCurrentProgress(const int& aProgress)
{
    // Set Value
    ui->currentProgress->setValue(aProgress);
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void DeleteProgressDialog::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "DeleteProgressDialog::clientStatusChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected && fileUtil) {
        // Check Need Queue
        if (needQueue) {
            // Reset Need Queue
            needQueue = false;
            // Get Dir List
            fileUtil->getDirList(dirPath);
        } else {
            // Clear Options
            fileUtil->clearFileTransferOptions();
            // Process Queue
            QTimer::singleShot(1, this, SLOT(processQueue()));
        }
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void DeleteProgressDialog::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    qDebug() << "DeleteProgressDialog::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...
}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void DeleteProgressDialog::fileOpStarted(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // Set Current File Name
    setCurrentFileName(aPath);

    // Set Maximum
    ui->currentProgress->setMaximum(queueModel->rowCount());

    // Set Current Progress
    setCurrentProgress(queueIndex);

    // Start Progress Refresh Timer
    startProgressRefreshTimer();

    // ...
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void DeleteProgressDialog::fileOpProgress(const unsigned int& aID,
                                          const QString& aOp,
                                          const QString& aCurrFilePath,
                                          const quint64& aCurrProgress,
                                          const quint64& aCurrTotal)
{
    Q_UNUSED(aCurrProgress);
    Q_UNUSED(aCurrTotal);

    qDebug() << "DeleteProgressDialog::fileOpProgress - aID: " << aID << " - aOp: " << aOp;

    // Set Current File Name
    setCurrentFileName(aCurrFilePath);
}

//==============================================================================
// File Operation Suspended Slot
//==============================================================================
void DeleteProgressDialog::fileOpSuspended(const unsigned int& aID,
                                           const QString& aOp,
                                           const QString& aPath,
                                           const QString& aSource,
                                           const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpSuspended - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...
}

//==============================================================================
// File Operation Resumed Slot
//==============================================================================
void DeleteProgressDialog::fileOpResumed(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpResumed - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void DeleteProgressDialog::fileOpSkipped(const unsigned int& aID,
                                           const QString& aOp,
                                           const QString& aPath,
                                           const QString& aSource,
                                           const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // Check Operation - Delete File
    if (aOp == DEFAULT_OPERATION_DELETE_FILE) {
        // Check Queue Model
        if (queueModel) {
            // Set Done
            queueModel->setProgressState(queueIndex, EDPSkipped);
        }

        // Increase Current Queue Index
        setQueueIndex(queueIndex + 1);

        // Set Current Progress
        //setCurrentProgress(queueIndex);
    }

    // ...

    // Process Queue
    processQueue();
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void DeleteProgressDialog::fileOpFinished(const unsigned int& aID,
                                          const QString& aOp,
                                          const QString& aPath,
                                          const QString& aSource,
                                          const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

    // Check Operation - Delete File
    if (aOp == DEFAULT_OPERATION_DELETE_FILE) {

        // Check Queue Model
        if (queueModel) {
            // Set Done
            queueModel->setProgressState(queueIndex, EDPFinished);
        }

        // Increase Current Queue Index
        setQueueIndex(queueIndex + 1);

        // Set Current Progress
        //setCurrentProgress(queueIndex);

    // Check Operation - Queue
    } else if (aOp == DEFAULT_OPERATION_QUEUE) {

        // Just Move On To Inserted Files

        // ...

    } else if (aOp == DEFAULT_OPERATION_LIST_DIR) {

        // Check Queue Model
        if (!queueModel) {
            return;
        }

        // Check Row Count
        if (queueModel->rowCount() <= 0) {
            // Init Info Dialog
            InfoDialog infoDialog(tr(DEFAULT_INFO_TEXT_NO_MATCH_FOUND));
            // Show Info Dialog
            infoDialog.exec();

            // Check Close When Finished
            if (closeWhenFinished) {
                // Check Close When Finished
                reject();
            }

            return;
        }

        // Just Move On

        // ...
    }

    // Process Queue
    //processQueue();

    // Process Queue
    QTimer::singleShot(1, this, SLOT(processQueue()));

}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void DeleteProgressDialog::fileOpAborted(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

    // Check Close When Finished
    if (closeWhenFinished) {
        // Close
        close();
    } else {
        // Configure Buttons
        configureButtons(QDialogButtonBox::Close);
        // Set Current File
        setCurrentFileName("");
    }
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void DeleteProgressDialog::fileOpError(const unsigned int& aID,
                                       const QString& aOp,
                                       const QString& aPath,
                                       const QString& aSource,
                                       const QString& aTarget,
                                       const int& aError)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

    // Init Confirmation Dialog
    ConfirmDialog confirmDialog;

    // Set Dialog Title
    confirmDialog.setConfirmTitle(tr(DEFAULT_ERROR_TITLE_DELETE_FILE));

    // Switch Error
    switch (aError) {
        default:
        case DEFAULT_ERROR_GENERAL: {
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_DELETE_FILE));

            // Set Path
            confirmDialog.setPath(aPath);

            // Configure Standard Buttons
            confirmDialog.configureButtons(QDialogButtonBox::Abort);

            // Add Button
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_RETRY), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_RETRY);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIP), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIP);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIPALL);

            // Exec Confirm Dialog
            confirmDialog.exec();

            // Get Action Index
            int actionIndex = confirmDialog.getActionIndex();

            // Check File Util
            if (fileUtil) {
                // Send User Response
                fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
            }
        } break;

        case DEFAULT_ERROR_NOTEXISTS: {
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_FILE_DOESNT_EXIST));

            // Set Path
            confirmDialog.setPath(aPath);

            // Configure Standard Buttons
            confirmDialog.configureButtons(QDialogButtonBox::Abort);

            // Add Button
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_RETRY), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_RETRY);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIP), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIP);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIPALL);

            // Exec Confirm Dialog
            confirmDialog.exec();

            // Get Action Index
            int actionIndex = confirmDialog.getActionIndex();

            // Check File Util
            if (fileUtil) {
                // Send User Response
                fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
            }
        } break;
    }

}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void DeleteProgressDialog::fileOpNeedConfirm(const unsigned int& aID,
                                             const QString& aOp,
                                             const int& aCode,
                                             const QString& aPath,
                                             const QString& aSource,
                                             const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aCode: " << aCode;

    // Init Confirmation Dialog
    ConfirmDialog confirmDialog;

    // Set Dialog Title
    confirmDialog.setConfirmTitle(tr(DEFAULT_TITLE_CONFIRMATION));

    // Switch Error
    switch (aCode) {
        case DEFAULT_ERROR_NON_EMPTY: {
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_DIRECTORY_NOT_EMPTY).arg(aPath));

            // Set Path
            //confirmDialog.setPath(aPath);

            // Configure Standard Buttons
            confirmDialog.configureButtons(QDialogButtonBox::Abort);

            // Add Button
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_YES), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_YES);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_YESTOALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_YESALL);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_NO), QDialogButtonBox::RejectRole, DEFAULT_CONFIRM_NO);
            confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_NOTOALL), QDialogButtonBox::RejectRole, DEFAULT_CONFIRM_NOALL);

            // Exec Confirm Dialog
            confirmDialog.exec();

            // Get Action Index
            int actionIndex = confirmDialog.getActionIndex();

            // Check File Util
            if (fileUtil) {
                // Send User Response
                fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
            }
        } break;

        default:
        break;
    }

    // ...
}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void DeleteProgressDialog::fileOpQueueItemFound(const unsigned int& aID,
                                                const QString& aOp,
                                                const QString& aPath,
                                                const QString& aSource,
                                                const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    // Check Queue
    if (queueModel) {
        qDebug() << "DeleteProgressDialog::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

        // Insert Item To Queue
        queueModel->insertItem(queueIndex, aPath);

        // Set Maximum
        ui->currentProgress->setMaximum(queueModel->rowCount());
    }
}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void DeleteProgressDialog::dirListItemFound(const unsigned int& aID,
                                            const QString& aPath,
                                            const QString& aFileName)
{
    // Check Queue & Path
    if (queueModel && QDir(aPath) == QDir(dirPath)) {
        qDebug() << "DeleteProgressDialog::fileOpQueueItemFound - aID: " << aID << " - aPath: " << aPath << " - aFileName: " << aFileName;

        // Init Dir
        QDir dir(QDir::homePath());

        // Check If File Name Matches
        if (dir.match(pattern, aFileName)) {
            // Init Delete File Path
            QString deleteFilePath = aPath;

            // Check Delete File Name
            if (!deleteFilePath.endsWith("/")) {
                // Adjust Delete File Name
                deleteFilePath += "/";
            }

            // Append Item
            queueModel->addItem(deleteFilePath + aFileName);
        }
    }
}

//==============================================================================
// Button Box Accepted Slot
//==============================================================================
void DeleteProgressDialog::buttonBoxAccepted()
{
    //qDebug() << "DeleteProgressDialog::buttonBoxAccepted";

    // ...

    // Emit Dialog Closed Signal
    //emit dialogClosed(this);
}

//==============================================================================
// Button Box Rejected Slot
//==============================================================================
void DeleteProgressDialog::buttonBoxRejected()
{
    //qDebug() << "DeleteProgressDialog::buttonBoxRejected";

    // ...

    // Emit Dialog Closed Signal
    //emit dialogClosed(this);
}

//==============================================================================
// Tab Changed Slot
//==============================================================================
void DeleteProgressDialog::tabChanged(const int& aIndex)
{
    //qDebug() << "DeleteProgressDialog::tabChanged - aIndex: " << aIndex;

    // Check Tab Index
    if (aIndex == 1) {
        // Update Queue Column Sizes
        updateQueueColumnSizes();
    }
}

//==============================================================================
// Resize Event
//==============================================================================
void DeleteProgressDialog::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent && aEvent->size().width() > 0 && aEvent->size().height() > 0) {
        //qDebug() << "DeleteProgressDialog::resizeEvent - aEvent: " << aEvent->size();

        // Update Queue Column Sizes
        updateQueueColumnSizes();
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void DeleteProgressDialog::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Timer ID
        if (aEvent->timerId() == progressRefreshTimerID) {
            // Check File Util Client
            if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTWaiting)) {
                // Set Current Progress
                setCurrentProgress(queueIndex);
            } else {
                // Stop Progress Refresh Timer
                stopProgressRefreshTimer();
            }
        }
    }
}

//==============================================================================
// Hide Event
//==============================================================================
void DeleteProgressDialog::hideEvent(QHideEvent* aEvent)
{
    //qDebug() << "DeleteProgressDialog::hideEvent";

    QDialog::hideEvent(aEvent);

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// On Close When Finished Check Box Clicked
//==============================================================================
void DeleteProgressDialog::on_closeWhenFinishedCheckBox_clicked()
{
    // Get Close When Finished
    closeWhenFinished = ui->closeWhenFinishedCheckBox->isChecked();
    // Set Settings Value
    settings->setValue(SETTINGS_KEY_CLOSE_WHEN_FINISHED, closeWhenFinished);
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressDialog::~DeleteProgressDialog()
{
    // Save Settings
    saveSettings();

    // Check Settings
    if (settings) {
        // Release
        settings->release();
        // Reset Settings
        settings = NULL;
    }

    // Abort
    abort();

    // Clear Queue
    clearQueue();

    // Delete Delete Queue List View
    delete ui->deleteQueueListView;

    // Delete UI
    delete ui;

    // Check Qeuue Model
    if (queueModel) {
        // Delete Queue
        delete queueModel;
        queueModel = NULL;
    }

    // Check File Util
    if (fileUtil) {
        // Close
        fileUtil->close();

        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    //qDebug() << "DeleteProgressDialog::~DeleteProgressDialog";
}

