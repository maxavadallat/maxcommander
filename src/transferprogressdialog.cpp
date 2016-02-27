#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QDialogButtonBox>
#include <QQmlEngine>
#include <QQmlContext>
#include <QImageReader>
#include <QDebug>

#include <mcwinterface.h>

#include "ui_transferprogressdialog.h"

#include "transferprogressdialog.h"
#include "transferprogressmodel.h"
#include "confirmdialog.h"
#include "infodialog.h"
#include "busyindicator.h"
#include "remotefileutilclient.h"
#include "filelistimageprovider.h"
#include "settingscontroller.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
TransferProgressDialog::TransferProgressDialog(const QString& aOperation, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferProgressDialog)
    , settings(SettingsController::getInstance())
    , queueModel(NULL)
    , fileUtil(NULL)
    , operation(aOperation)
    , closeWhenFinished(false)
    , queueIndex(-1)
    , panelHasFocus(true)
    , options(0)
    , sourcePattern("")
    , targetPattern("")
    , needQueue(false)
    , transferSpeedTimerID(-1)
    , lastTransferedSize(0)
    , currTransferedSize(0)
    , speedMeasureLastSize(0)
    , transferSpeed(0)
    , currentProgress(0)
    , currentSize(0)
    , currentProgressScale(0)
    , overallProgress(0)
    , overallSize(0)
    , overallProgressScale(0)
    , progressRefreshTimerID(-1)
    , archiveMode(false)
{
    qDebug() << "TransferProgressDialog::TransferProgressDialog";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void TransferProgressDialog::init()
{
    qDebug() << "TransferProgressDialog::init";

    // Create Queue Model
    queueModel = new TransferProgressModel();

    // Get Root Contenxt
    QQmlContext* ctx = ui->transferQueueListView->rootContext();

    // Set Context Property - Main Controller
    ctx->setContextProperty(DEFAULT_MAIN_CONTROLLER_NAME, this);
    // Set Context Property - Transfer List Model
    ctx->setContextProperty(DEFAULT_FILE_TRANSFER_LIST_MODEL, queueModel);
    // Set Context Property - Settings
    ctx->setContextProperty(DEFAULT_GLOBAL_SETTINGS_CONTROLLER, settings);

    // Get Engine
    QQmlEngine* engine = ui->transferQueueListView->engine();
    // Create New Image Provider
    FileListImageProvider* newImageProvider = new FileListImageProvider();
    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_FILE_ICON_PROVIDER_ID), newImageProvider);

    // Register Busy Indicator
    qmlRegisterType<BusyIndicator>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, DEFAULT_CUSTOM_COMPONENTS_BUSY_INDICATOR);

    // Set Resize Mode
    ui->transferQueueListView->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // Set Source
    ui->transferQueueListView->setSource(QUrl(DEFAULT_FILE_TRANSFER_LIST_QMLFILE_URL));

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
    }

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
// Launch Progress Dialog
//==============================================================================
void TransferProgressDialog::launch(const QString& aSourcePath, const QString& aTargetPath, const QStringList& aSelectedFiles, const int& aOptions)
{
    qDebug() << "TransferProgressDialog::launch - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - count: " << aSelectedFiles.count();

    // Set Source Path
    sourcePath = aSourcePath;
    // Set Target Path
    targetPath = aTargetPath;
    // Set Options
    options    = aOptions;

    // Set Archive Mode
    setArchiveMode(false);

    // Restore UI
    restoreUI();
    // Show
    show();

    // Build Queue
    if (buildQueue(aSourcePath, aTargetPath, aSelectedFiles)) {
        // Set Queue Index
        setQueueIndex(0);

        // Check File Util Client
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
void TransferProgressDialog::launch(const QString& aSourcePath, const QString& aTargetPath, const QString& aSourcePattern, const QString& aTargetPattern, const int& aOptions)
{
    qDebug() << "TransferProgressDialog::launch - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - aSourcePattern: " << aSourcePattern << " - aTargetPattern: " << aTargetPattern;

    // Set Source Path
    sourcePath = aSourcePath;
    // Set Target Path
    targetPath = aTargetPath;
    // Set Options
    options    = aOptions;
    // Set Source Pattern
    sourcePattern = aSourcePattern;
    // Set Target Pattern
    targetPattern = aTargetPattern;

    // Set Archive Mode
    setArchiveMode(false);

    // Restore UI
    restoreUI();
    // Show
    show();

    // Check File Util Client
    if (fileUtil) {
        // Check If Connected
        if (!fileUtil->isConnected()) {
            // Set Need To Build Queue
            needQueue = true;
            // Connect
            fileUtil->connectToFileServer();
        } else {
            // Clear Options
            fileUtil->clearFileTransferOptions();
            // Build Queue
            buildQueue(sourcePath, targetPath, sourcePattern, targetPattern);
        }
    }
}

//==============================================================================
// Launch Progress Dialog With Single Item
//==============================================================================
void TransferProgressDialog::launch(const QString& aSource, const QString& aTarget, const int& aOptions)
{
    qDebug() << "TransferProgressDialog::launch - aSource: " << aSource << " - aTarget: " << aTarget;

    // Init Source File Info
    QFileInfo sourceInfo(aSource);

    // Set Source Path
    sourcePath = sourceInfo.absolutePath();
    // Set Target Path
    targetPath = QFileInfo(aTarget).absolutePath();
    // Set Options
    options    = aOptions;

    // Set Archive Mode
    setArchiveMode(isFileArchiveByExt(aSource) && operation == DEFAULT_OPERATION_EXTRACT_ARCHIVE);

    // Restore UI
    restoreUI();
    // Show
    show();

    // Reset Need To Build Queue
    needQueue = false;

    // Check If File Exists
    if (sourceInfo.exists()) {

        // Compare Source And Target
        if (aSource == aTarget) {
            // Init Info Dialog
            InfoDialog infoDialog(tr(DEFAULT_WARNING_TEXT_INVALID_TARGET), EIDTWarning);
            // Exec Info Dialog
            infoDialog.exec();

            // close
            reject();

            return;
        }

        // Check If Source Is a Dir
        if (sourceInfo.isDir()) {
            // Get Source Path
            QString sourcePath = getDirPath(aSource);

            // Check Target
            if (/*aTarget.indexOf(sourcePath) >= 0 ||*/ aTarget.indexOf(aSource) >= 0) {
                // Init Info Dialog
                InfoDialog infoDialog(tr(DEFAULT_WARNING_TEXT_INVALID_TARGET), EIDTWarning);

                // Exec Info Dialog
                infoDialog.exec();

                // close
                reject();

                return;
            }
        }

        // Add To Queue Model
        queueModel->addItem(operation, aSource, aTarget);

        // Check If Is Dir
        if (!sourceInfo.isDir() && !sourceInfo.isSymLink() && !archiveMode) {
            // Add Size To Overall Size
            overallSize += sourceInfo.size();
            // Configure Overall Progress Bar
            configureOverallProgressBar(overallSize);
        }

        // Check Queue Count
        if (queueModel->rowCount() == 0) {
            // Close
            reject();

            return;
        }

        // Set Queue Index
        setQueueIndex(0);

        // Check File Util Client
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

        // Init Info Dialog
        InfoDialog infoDialog(tr(DEFAULT_WARNING_TEXT_INVALID_SOURCE), EIDTWarning);

        // Exec Info Dialog
        infoDialog.exec();

        // close
        reject();
    }
}

//==============================================================================
// Launch Progress Dialog
//==============================================================================
void TransferProgressDialog::launch(const QStringList& aSourceList, const QString& aTargetPath, const int& aOptions)
{
    // Set Source Path
    sourcePath = "";
    // Set Target Path
    targetPath = aTargetPath;
    // Set Options
    options    = aOptions;

    // Set Archive Mode
    setArchiveMode(false);

    // Restore UI
    restoreUI();
    // Show
    show();

    // Get Dropped Items List Count
    int dilCount = aSourceList.count();

    // Check Dropped Items List Count
    if (dilCount > 0) {
        // Init Default File URL Prefix
        QString fileURLPrefix(DEFAULT_URL_PREFIX_FILE);
        // Get Length
        int fupLength = fileURLPrefix.length();
        // Init Build Queue Options
        int bqOptions = 0;

        // Iterate Through Dropped Items List
        for (int i=0; i<dilCount; i++) {
            // Init Local Source File Name
            QString localSource = aSourceList[i];

            // Check Local Source
            if (localSource.startsWith(DEFAULT_URL_PREFIX_FILE)) {
                // Adjust Local Source
                localSource = localSource.right(localSource.length() - fupLength);
            }

            // Init Source Info
            QFileInfo sourceInfo(localSource);

            // Check If Source Info Exists
            if (sourceInfo.exists()) {

                // Get File Name
                QString fileName = sourceInfo.fileName();
                // Init Target File Path
                QString targetFilePath = aTargetPath;
                // Check Target Path
                if (!targetFilePath.endsWith("/")) {
                    // Adjust Target File Path
                    targetFilePath += "/";
                }

                // Add File Name To Target File Path
                targetFilePath += fileName;

                // Init Target File Info
                QFileInfo targetInfo(targetFilePath);

                // Compare Target File Info
                if (sourceInfo != targetInfo) {
                    // Check If Is Dir
                    if (!sourceInfo.isDir() && !sourceInfo.isSymLink()) {
                        // Add Size To Overall Size
                        overallSize += sourceInfo.size();
                        // Configure Overall Progress Bar
                        configureOverallProgressBar(overallSize);
                    }

                    // Add To Queue Model
                    queueModel->addItem(operation, sourceInfo.absoluteFilePath(), targetFilePath);

                } else {
                    // Check Options
                    if (bqOptions & DEFAULT_CONFIRM_SKIPALL) {
                        continue;
                    }

                    // Init Confirm Dialog
                    ConfirmDialog confirmDialog;

                    // Set Dialog Title
                    confirmDialog.setConfirmTitle(tr(DEFAULT_TITLE_WARNING));

                    // Set Error Text
                    confirmDialog.setConfirmText(tr(DEFAULT_WARNING_TEXT_SAME_SOURCE_TARGET));

                    // Configure Standard Buttons
                    confirmDialog.configureButtons(QDialogButtonBox::Abort);

                    // Add Button
                    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIP), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIP);
                    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIPALL);

                    // Exec Confirm Dialog
                    confirmDialog.exec();

                    // Get Action Index
                    int actionIndex = confirmDialog.getActionIndex();

                    // Switch Action Index
                    switch (actionIndex) {
                        case DEFAULT_CONFIRM_SKIPALL:
                            // Add Skip All To Options
                            bqOptions |= DEFAULT_CONFIRM_SKIPALL;
                        case DEFAULT_CONFIRM_SKIP:
                        break;

                        default:
                            // close
                            reject();

                            return;
                        break;
                    }
                }
            }
        }

        // Check Queue Count
        if (queueModel->rowCount() == 0) {
            // Close
            reject();

            return;
        }

        // Set Queue Index
        setQueueIndex(0);

        // Check File Util Client
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
// Build Queue
//==============================================================================
bool TransferProgressDialog::buildQueue(const QString& aSourcePath, const QString& aTargetPath, const QStringList& aSelectedFiles)
{
    // Get Selected Files Count
    int sfCount = aSelectedFiles.count();

    // Check Selected Files Count
    if (sfCount <= 0) {
        qDebug() << "TransferProgressDialog::buildQueue - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - NO SELECTED FILES TO TRANSFER!";

        return false;
    }

    // Check Queue Model
    if (!queueModel) {
        qDebug() << "TransferProgressDialog::buildQueue - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - NO QUEUE MODEL!";

        return false;
    }

    qDebug() << "TransferProgressDialog::buildQueue - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - count: " << aSelectedFiles.count();

    // Reset Overall Progress
    overallProgress = 0;
    // Reset Overall Size
    overallSize = 0;


    // Init Local Source Path
    QString localSourcePath = aSourcePath;

    // Check Local Source Path
    if (localSourcePath.endsWith("*")) {
        // Adjust Local Path
        localSourcePath.truncate(localSourcePath.length() - 1);
    }

    // Check Local localSourcePath Path
    if (!localSourcePath.endsWith("/")) {
        // Adjust Local Path
        localSourcePath += "/";
    }

    // Init Local Target Path
    QString localTargetPath = aTargetPath;

    // Check Local Target Path
    if (localTargetPath.endsWith("*")) {
        // Adjust Local Path
        localTargetPath.truncate(localTargetPath.length() - 1);
    }

    // Check Local Target Path
    if (!localTargetPath.endsWith("/")) {
        // Adjust Local Path
        localTargetPath += "/";
    }

    // Go Thru Selected Files
    for (int i=0; i<sfCount; ++i) {
        // Init Source File Path
        //QString sourceFilePath = localSourcePath + aSelectedFiles[i];
        QString sourceFilePath = aSelectedFiles[i];
        // Init Target File Path
        //QString targetFilePath = localTargetPath + aSelectedFiles[i];
        QString targetFilePath = localTargetPath + QFileInfo(aSelectedFiles[i]).fileName();

        // Init Source File Info
        QFileInfo sourceInfo(sourceFilePath);

        // Check If Source Exists
        if (sourceInfo.exists()) {
            // Check If Is Dir
            if (!sourceInfo.isDir() && !sourceInfo.isSymLink()) {
                // Add Size To Overall Size
                overallSize += sourceInfo.size();
                // Configure Overall Progress Bar
                configureOverallProgressBar(overallSize);
            }

            // Add To Queue Model
            queueModel->addItem(operation, sourceFilePath, targetFilePath);
        }
    }

    return true;
}

//==============================================================================
// Build Queue
//==============================================================================
bool TransferProgressDialog::buildQueue(const QString& aSourcePath, const QString& aTargetPath, const QString& aSourcePattern, const QString& aTargetPattern)
{
    // Check File Util & Queue Model
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::buildQueue - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - aSourcePattern: " << aSourcePattern << " - aTargetPattern: " << aTargetPattern;

        // File Util
        fileUtil->getDirList(aSourcePath);

        return true;
    }

    return false;
}

//==============================================================================
// Process Queue
//==============================================================================
void TransferProgressDialog::processQueue()
{
    // Check File Util & Queue Model
    if (fileUtil && queueModel) {
        // Check Queue Index
        if (queueIndex >= 0 && queueIndex < queueModel->rowCount()) {
            qDebug() << "TransferProgressDialog::processQueue - queueIndex: " << queueIndex;

            // Get Source File Name
            QString sourceFileName = queueModel->getSourceFileName(queueIndex);
            // Get Target File Name
            QString targetFileName = queueModel->getTargetFileName(queueIndex);
            // Get Operation
            operation = queueModel->getOperation(queueIndex);

            // Check Operation - Copy
            if (operation == DEFAULT_OPERATION_COPY_FILE) {
                // Copy File
                fileUtil->copyFile(sourceFileName, targetFileName);
            // Check Operation - Rename/Move
            } else if (operation == DEFAULT_OPERATION_MOVE_FILE) {
                // Move File
                fileUtil->moveFile(sourceFileName, targetFileName);
            } else if (operation == DEFAULT_OPERATION_EXTRACT_ARCHIVE) {
                // Extract File
                fileUtil->extractArchive(sourceFileName, targetFileName);
            } else {

                qDebug() << "TransferProgressDialog::processQueue - queueIndex: " << queueIndex << " - UNKNOWN OPERATION!!";

            }

            // Configure Buttons
            configureButtons(QDialogButtonBox::Abort);

        } else {
            qDebug() << "TransferProgressDialog::processQueue - FINISHED!";

            // Check Close When Finished
            if (closeWhenFinished) {
                // Close
                close();
            } else {
                // Configure Buttons
                configureButtons(QDialogButtonBox::Close);
                // Set Current File
                setCurrentFileName("");
                // Set Overall Progress Max Value
                ui->overallProgress->setMaximum(1);
                // Set Overall Progress Value
                ui->overallProgress->setValue(1);
            }
        }
    }
}

//==============================================================================
// Clear Queue
//==============================================================================
void TransferProgressDialog::clearQueue()
{
    // Check Queue
    if (queueModel && queueModel->rowCount() > 0) {
        qDebug() << "TransferProgressDialog::clearQueue";

        // Clear Model
        queueModel->clear();

        // Reset Queue Index
        setQueueIndex(-1);
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void TransferProgressDialog::restoreUI()
{
    qDebug() << "TransferProgressDialog::restoreUI";

    // Configure Buttons
    configureButtons(QDialogButtonBox::Close);

    // Check Operation
    if (operation == DEFAULT_OPERATION_COPY_FILE) {
        // Set Title
        setTitle(tr(DEFAULT_TITLE_COPY_FILES));
    } else if (operation == DEFAULT_OPERATION_MOVE_FILE) {
        // Set Title
        setTitle(tr(DEFAULT_TITLE_MOVE_FILES));
    } else if (operation == DEFAULT_OPERATION_EXTRACT_ARCHIVE) {
        // Set Title
        setTitle(tr(DEFAULT_TITLE_EXTRACT_FILE));
    } else {
        // ...
    }

    // Get Close When Finished
    closeWhenFinished = settings->getCloseWhenFinished();
    // Set Checkbox
    ui->closeWhenFinishedCheckBox->setChecked(closeWhenFinished);

    // Update Queue Column Sizes
    //updateQueueColumnSizes();

    // Reset Current Progress
    currentProgress = 0;
    // Reset Current Size
    currentSize = 0;

    // Reset Overall Progress
    overallProgress = 0;
    // Reset Overall Size
    overallSize = 0;

    // Chekc ARchive Mode
    if (archiveMode) {

        // Set Visibility
        //ui->currentFileNameLabel->setVisible(false);
        // Set Visibility
        //ui->currentFileTitleLabel->setVisible(false);
        // Set Visibility
        ui->currentProgress->setVisible(false);

    } else {
        // Set Visibility
        //ui->currentFileNameLabel->setVisible(true);
        // Set Visibility
        //ui->currentFileTitleLabel->setVisible(true);
        // Set Visibility
        ui->currentProgress->setVisible(true);
        // Set Minimum
        ui->currentProgress->setMinimum(0);
        // Set Maximum
        ui->currentProgress->setMaximum(0);
    }

    // Set Minimum
    ui->overallProgress->setMinimum(0);
    // Set Maximum
    ui->overallProgress->setMaximum(0);
}

//==============================================================================
// Save Settings
//==============================================================================
void TransferProgressDialog::saveSettings()
{
    // Check Settings
    if (settings) {
        return;
    }

    qDebug() << "TransferProgressDialog::saveSettings";

    // Get Close When Finished
    closeWhenFinished = ui->closeWhenFinishedCheckBox->checkState() == Qt::Checked;
    // Save Setting
    settings->setValue(SETTINGS_KEY_CLOSE_WHEN_FINISHED, closeWhenFinished);
}

//==============================================================================
// Configure Standard Buttons
//==============================================================================
void TransferProgressDialog::configureButtons(const QDialogButtonBox::StandardButtons& aButtons)
{
    // Configure Buttons
    ui->buttonBox->setStandardButtons(aButtons);
}

//==============================================================================
// Configure Current Progress Bar
//==============================================================================
void TransferProgressDialog::configureCurrentProgressBar(const quint64& aMax)
{
    // Reset Current Progress Scale
    currentProgressScale = 0;

    // Loop While Scale is OK
    while ((aMax >> currentProgressScale) > INT_MAX) {
        // In Current Progress Scale
        currentProgressScale++;
    }

    // Set Maximum
    ui->currentProgress->setMaximum(aMax >> currentProgressScale);

    //qDebug() << "TransferProgressDialog::configureCurrentProgressBar - maximum: " << ui->currentProgress->maximum();
}

//==============================================================================
// Configure Overall Progress Bar
//==============================================================================
void TransferProgressDialog::configureOverallProgressBar(const quint64& aMax)
{
    // Reset Overall Progress Scale
    overallProgressScale = 0;

    // Loop While Scale is OK
    while ((aMax >> overallProgressScale) > INT_MAX) {
        // In Current Progress Scale
        overallProgressScale++;
    }

    // Set Maximum
    ui->overallProgress->setMaximum(aMax >> overallProgressScale);

    //qDebug() << "TransferProgressDialog::configureOverallProgressBar - maximum: " << ui->overallProgress->maximum();
}

//==============================================================================
// Start Transfer Speed Timer
//==============================================================================
void TransferProgressDialog::startTransferSpeedTimer()
{
    // Check Transfer Speed Timer ID
    if (transferSpeedTimerID == -1) {
        //qDebug() <<"TransferProgressDialog::startTransferSpeedTimer";
        // Reset Last Speed Measure
        speedMeasureLastSize = 0;
        // Start Timer
        transferSpeedTimerID = startTimer(DEFAULT_ONE_SEC, Qt::PreciseTimer);
    }
}

//==============================================================================
// Stop Transfer Speed Timer
//==============================================================================
void TransferProgressDialog::stopTransferSpeedTimer()
{
    // Check Transfer Speed Timer ID
    if (transferSpeedTimerID != -1) {
        //qDebug() <<"TransferProgressDialog::stopTransferSpeedTimer";
        // Kill Timer
        killTimer(transferSpeedTimerID);
        // Reset Timer ID
        transferSpeedTimerID = -1;
        // Reset Last Speed Measure
        speedMeasureLastSize = 0;
    }
}

//==============================================================================
// Update Queue Column Sizes
//==============================================================================
void TransferProgressDialog::updateQueueColumnSizes()
{
    //qDebug() << "TransferProgressDialog::updateQueueColumnSizes";

    // ...
}

//==============================================================================
// Start Progress Refresh Timer
//==============================================================================
void TransferProgressDialog::startProgressRefreshTimer()
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
void TransferProgressDialog::stopProgressRefreshTimer()
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
// Set Queue Index
//==============================================================================
void TransferProgressDialog::setQueueIndex(const int& aQueueIndex)
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
void TransferProgressDialog::setArchiveMode(const bool& aArchiveMode)
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
// Suspend
//==============================================================================
void TransferProgressDialog::suspend()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::suspend";

        // Suspend
        fileUtil->suspend();

        // Stop Transfer Speed Timer
        stopTransferSpeedTimer();
        // Stop Refresh Progress Timer
        stopProgressRefreshTimer();
    }
}

//==============================================================================
// Resume
//==============================================================================
void TransferProgressDialog::resume()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::resume";

        // Resume
        fileUtil->resume();

        // Start Transfer Speed Timer
        startTransferSpeedTimer();
        // Start Progress Refresh Timer
        startProgressRefreshTimer();
    }
}

//==============================================================================
// Abort
//==============================================================================
void TransferProgressDialog::abort()
{
    // Check File Util
    if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTSuspended || fileUtil->getStatus() == ECSTWaiting)) {
        qDebug() << "TransferProgressDialog::abort";

        // Abort
        fileUtil->abort();
    }

    // Stop Transfer Speed Timer
    stopTransferSpeedTimer();
    // Stop Refresh Progress Timer
    stopProgressRefreshTimer();
}

//==============================================================================
// Get Supported Image Formats For Transfer Queue List
//==============================================================================
QStringList TransferProgressDialog::getSupportedImageFormats()
{
    return supportedImageFormats;
}

//==============================================================================
// Get Last Operation Target
//==============================================================================
QString TransferProgressDialog::getLastTarget()
{
    // Check Queue Model
    if (queueModel) {
        return queueModel->getTarget(queueModel->rowCount() - 1);
    }

    return "";
}

//==============================================================================
// Get Current Index
//==============================================================================
int TransferProgressDialog::getCurrentIndex()
{
    return queueIndex;
}

//==============================================================================
// Get Panel Focus
//==============================================================================
bool TransferProgressDialog::getPanelFocus()
{
    return panelHasFocus;
}

//==============================================================================
// Set Panel Focus
//==============================================================================
void TransferProgressDialog::setPanelFocus(const bool& aPanelFocus)
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
bool TransferProgressDialog::getArchiveMode()
{
    return archiveMode;
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
void TransferProgressDialog::setCurrentFileName(const QString& aCurrentFileName, const int& aSpeed)
{
    // Check Current File Name
    if (currentFileName != aCurrentFileName || transferSpeed != aSpeed) {
        //qDebug() << "TransferProgressDialog::setCurrentFileName - aCurrentFileName: " << aCurrentFileName << " - aSpeed: " << aSpeed;

        // Set Current File Name
        currentFileName = aCurrentFileName;
        // Set Transfer Speed
        transferSpeed = aSpeed;
        // Get Font Metrics
        QFontMetrics fontMetrics = ui->currentFileNameLabel->fontMetrics();

        // Check Speed
        if (transferSpeed > 0) {
            // Set Name Label Text
            nameLabelText = QString(DEFAULT_PROGRESS_DIALOG_FILENAME_TEXT_TEMPLATE).arg(currentFileName).arg(transferSpeed >> 10);
        } else {
            // Set Name Label Text
            nameLabelText = currentFileName;
        }

        // Set Text
        ui->currentFileNameLabel->setText(fontMetrics.elidedText(nameLabelText, Qt::ElideMiddle, ui->currentFileNameLabel->width()));

        // Check Current File Name
        if (currentFileName.isEmpty()) {
            // Set Label
            ui->currentFileTitleLabel->setText("");
        } else {
            // Set Label
            ui->currentFileTitleLabel->setText(tr(DEFAULT_LABEL_CURRENT_FILE_TITLE));
        }
    }
}

//==============================================================================
// Set Current File Progress
//==============================================================================
void TransferProgressDialog::setCurrentProgress(const quint64& aProgress)
{
    // Set Current Progress
    ui->currentProgress->setValue(aProgress >> currentProgressScale);
}

//==============================================================================
// Set Overall Progress
//==============================================================================
void TransferProgressDialog::setOverallProgress(const quint64& aProgress)
{
    // Set Overall Progress
    ui->overallProgress->setValue(aProgress >> overallProgressScale);
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void TransferProgressDialog::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "TransferProgressDialog::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected) {
        // Check File Util
        if (fileUtil) {
            // Clear Options
            fileUtil->clearFileTransferOptions();
        }

        // Check If Need Queue
        if (needQueue) {
            // Build Queue
            buildQueue(sourcePath, targetPath, sourcePattern, targetPattern);
        } else {
            // Process Queue
            QTimer::singleShot(1, this, SLOT(processQueue()));
        }
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void TransferProgressDialog::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    Q_UNUSED(aID);
    Q_UNUSED(aStatus);

    //qDebug() << "TransferProgressDialog::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...

}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void TransferProgressDialog::fileOpStarted(const unsigned int& aID,
                                           const QString& aOp,
                                           const QString& aPath,
                                           const QString& aSource,
                                           const QString& aTarget)
{
    Q_UNUSED(aPath);

    // Check Operation Parameter
    if (aOp == DEFAULT_OPERATION_COPY_FILE || aOp == DEFAULT_OPERATION_MOVE_FILE) {

        qDebug() << "TransferProgressDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

        // Configure Buttons
        configureButtons(QDialogButtonBox::Abort);

        // Set Current File Name
        setCurrentFileName(aSource);

        // Init File Info
        QFileInfo sourceInfo(aSource);

        // Reset Last Transfer Size
        lastTransferedSize = 0;
        // Reset Current Transfer Size
        currTransferedSize = 0;

        // Set Current Progress
        //setCurrentProgress(0, 0);
        currentProgress = 0;
        // Set Current Size
        currentSize = sourceInfo.size();

        // Configure Current Progress Bar
        configureCurrentProgressBar(currentSize);

        // Start Transfer Speed Timer
        startTransferSpeedTimer();

        // Start Progress Refresh Timer
        startProgressRefreshTimer();
    } else {

        qDebug() << "TransferProgressDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - WTF ?!?";

    }
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void TransferProgressDialog::fileOpProgress(const unsigned int& aID,
                                            const QString& aOp,
                                            const QString& aCurrFilePath,
                                            const quint64& aCurrProgress,
                                            const quint64& aCurrTotal)
{
    Q_UNUSED(aID);
    Q_UNUSED(aOp);
    Q_UNUSED(aCurrFilePath);
    Q_UNUSED(aCurrTotal);

    //qDebug() << "DeleteProgressDialog::fileOpProgress - aID: " << aID << " - aOp: " << aOp << " - aCurrFilePath: " << aCurrFilePath << " - aCurrProgress: " << aCurrProgress << " - aCurrTotal: " << aCurrTotal;

    // Check Queue Model
    if (queueModel) {
        // Set Done
        queueModel->setProgressStatus(queueIndex, ETPRunning);
    }

    // Set Current Progress
    currentProgress = aCurrProgress;

    // Set Current Transfer Size
    currTransferedSize = aCurrProgress;

    // Calculate Overall Progress
    overallProgress += (currTransferedSize - lastTransferedSize);

    // Set Last Transfer Size
    lastTransferedSize = currTransferedSize;

    // Set Current Progress
    //setCurrentProgress(currentProgress, currentSize);
    // Set Overall Progress
    //setOverallProgress(overallProgress, overallSize);
}

//==============================================================================
// File Operation Suspended Slot
//==============================================================================
void TransferProgressDialog::fileOpSuspended(const unsigned int& aID,
                                             const QString& aOp,
                                             const QString& aPath,
                                             const QString& aSource,
                                             const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "DeleteProgressDialog::fileOpSuspended - aID: " << aID << " - aOp: " << aOp;

    // ...
}

//==============================================================================
// File Operation Resumed Slot
//==============================================================================
void TransferProgressDialog::fileOpResumed(const unsigned int& aID,
                                           const QString& aOp,
                                           const QString& aPath,
                                           const QString& aSource,
                                           const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "DeleteProgressDialog::fileOpResumed - aID: " << aID << " - aOp: " << aOp;

    // ...

}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void TransferProgressDialog::fileOpSkipped(const unsigned int& aID,
                                           const QString& aOp,
                                           const QString& aPath,
                                           const QString& aSource,
                                           const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "TransferProgressDialog::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;
    //qDebug() << " ";
    //qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    //qDebug() << " ";

    // Check Operation - Copy/Move File
    if (aOp == DEFAULT_OPERATION_COPY_FILE || aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Check Queue Model
        if (queueModel) {
            // Set Progress State Skipped
            queueModel->setProgressStatus(queueIndex, ETPSkipped);
        }

        // Init Source Info
        QFileInfo sourceInfo(aSource);

        // Configure Current Progress
        configureCurrentProgressBar(1);
        // Set Current Progress
        setCurrentProgress(1);

        // Calculate Overall Progress
        overallProgress += sourceInfo.size();

        // Set Last Transfer Size
        lastTransferedSize = 0;

        // Set Overall Progress
        setOverallProgress(overallProgress);

        // Increase Current Queue Index
        setQueueIndex(queueIndex + 1);
    }

    // ...

    // Process Queue
    processQueue();
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void TransferProgressDialog::fileOpFinished(const unsigned int& aID,
                                            const QString& aOp,
                                            const QString& aPath,
                                            const QString& aSource,
                                            const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "TransferProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

    // Check Operation - Copy
    if (operation == DEFAULT_OPERATION_COPY_FILE) {

        // Check Finished Operation
        if (aOp == DEFAULT_OPERATION_QUEUE || aOp == DEFAULT_OPERATION_COPY_FILE) {

            // Check Queue Model
            if (queueModel) {
                // Set Progress State Finished
                queueModel->setProgressStatus(queueIndex, ETPFinished);
            }

            // Configure Current Progress
            configureCurrentProgressBar(1);
            // Set Current Progress
            setCurrentProgress(1);

            // Update Label
            ui->currentFileTitleLabel->setText(tr(DEFAULT_LABEL_CURRENT_FILE_TITLE_FINISHED));

            // Increase Current Queue Index
            setQueueIndex(queueIndex + 1);

            // Process Queue
            processQueue();

        } else {

            // Unhandled

            qDebug() << "TransferProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - WTF?!?";

            // ...
        }

    // Check Operation - Move
    } else if (operation == DEFAULT_OPERATION_MOVE_FILE) {

        // Check Finished Operation - Move
        if (aOp == DEFAULT_OPERATION_MOVE_FILE) {

            // Check Source and Target !!!!

            // ...

            // Check Queue Model
            if (queueModel) {
                // Set Done
                queueModel->setProgressStatus(queueIndex, ETPFinished);
            }

            // Configure Current Progress
            configureCurrentProgressBar(1);
            // Set Current Progress
            setCurrentProgress(1);

            // Update Label
            ui->currentFileTitleLabel->setText(tr(DEFAULT_LABEL_CURRENT_FILE_TITLE_FINISHED));

            // Increase Current Queue Index
            setQueueIndex(queueIndex + 1);

        // Check Finished Operation - Queue
        } else if (aOp == DEFAULT_OPERATION_QUEUE) {

            // Emit Current Index Changed to Reset after inserts
            emit currentIndexChanged(queueIndex);

        } else {

            // Unhandled
            qDebug() << "TransferProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - WTF?!?";

            // ...

            return;
        }

        // Process Queue
        processQueue();

    // Check Operation - List Dir
    } else if (operation == DEFAULT_OPERATION_EXTRACT_ARCHIVE) {

        // Increase Current Queue Index
        setQueueIndex(queueIndex + 1);

        // Process Queue
        processQueue();

    // Unhandled
    } else {

        qDebug() << "TransferProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - WTF?!?";

    }
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void TransferProgressDialog::fileOpAborted(const unsigned int& aID,
                                           const QString& aOp,
                                           const QString& aPath,
                                           const QString& aSource,
                                           const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "TransferProgressDialog::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

    // Check Close When Finished
    if (closeWhenFinished) {
        // Close
        close();
    } else {
        // Configure Buttons
        configureButtons(QDialogButtonBox::Close);
    }
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void TransferProgressDialog::fileOpError(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget,
                                         const int& aError)
{
    Q_UNUSED(aPath);

    qDebug() << "TransferProgressDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aError: " << aError;

    // ...

    // Init Confirmation Dialog
    ConfirmDialog confirmDialog;

    // Check Operation - Copy
    if (aOp == DEFAULT_OPERATION_COPY_FILE) {

        // Set Dialog Title
        confirmDialog.setConfirmTitle(tr(DEFAULT_ERROR_TITLE_COPY_FILE));

    // Check Operation - Move
    } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {

        // Set Dialog Title
        confirmDialog.setConfirmTitle(tr(DEFAULT_ERROR_TITLE_MOVE_FILE));
    }

    // Switch Error
    switch (aError) {
        default:
        case DEFAULT_ERROR_GENERAL: {
            // Check Operation - Copy
            if (aOp == DEFAULT_OPERATION_COPY_FILE) {

                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_COPY_FILE));

            // Check Operation - Move
            } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {

                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_MOVE_FILE));

            }

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

        case DEFAULT_ERROR_NOT_ENOUGH_SPACE: {

            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_NOT_ENOUGH_SPACE_ON_TARGET));

            // Set Path
            confirmDialog.setPath("");

            // Configure Standard Buttons
            confirmDialog.configureButtons(QDialogButtonBox::Abort);

            // Exec Confirm Dialog
            confirmDialog.exec();

            // Check File Util
            if (fileUtil) {
                // Send User Response
                fileUtil->sendUserResponse(DEFAULT_CONFIRM_ABORT, "");
            }

        } break;
    }
}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void TransferProgressDialog::fileOpNeedConfirm(const unsigned int& aID,
                                               const QString& aOp,
                                               const int& aCode,
                                               const QString& aPath,
                                               const QString& aSource,
                                               const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "TransferProgressDialog::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aCode: " << aCode;

    // Init Confirmation Dialog
    ConfirmDialog confirmDialog;

    // Set Dialog Title
    confirmDialog.setConfirmTitle(tr(DEFAULT_TITLE_CONFIRMATION));

    // Switch Code
    switch (aCode) {
        case DEFAULT_ERROR_EXISTS:
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_TARGET_FILE_EXISTS).arg(aTarget));
        break;

        case DEFAULT_ERROR_TARGET_DIR_NOT_EXISTS:
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_TARGET_DIR_DOESNT_EXIST).arg(aTarget));
        break;

        case DEFAULT_ERROR_TARGET_DIR_EXISTS:
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS_MERGE));
        break;
    }

    // Set Path
    confirmDialog.setPath(aPath);

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

    // ...

}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void TransferProgressDialog::fileOpQueueItemFound(const unsigned int& aID,
                                                  const QString& aOp,
                                                  const QString& aPath,
                                                  const QString& aSource,
                                                  const QString& aTarget)
{
    Q_UNUSED(aPath);

    // Check Queue
    if (queueModel) {
        qDebug() << "TransferProgressDialog::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

        // Check Operation
        if (aOp == DEFAULT_OPERATION_COPY_FILE) {
            // Add Item
            queueModel->addItem(aOp, aSource, aTarget);

        } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
            // Insert Item
            queueModel->insertItem(queueIndex, aOp, aSource, aTarget);
        }

        // Init Source File Info
        QFileInfo sourceInfo(aSource);

        // Check If Is Dir
        if (!sourceInfo.isDir() && !sourceInfo.isBundle() && !sourceInfo.isSymLink()) {
            // Add Size To Overall Size
            overallSize += sourceInfo.size();
            // Configure Overall Progress Bar
            configureOverallProgressBar(overallSize);
        }
    }
}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void TransferProgressDialog::dirListItemFound(const unsigned int& aID,
                                              const QString& aPath,
                                              const QString& aFileName)
{
    // Check Queue
    if (queueModel && QDir(aPath) == QDir(sourcePath)) {
        qDebug() << "TransferProgressDialog::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - aFileName: " << aFileName;

        // Init Dir
        QDir dir(sourcePath);

        // Init Source File Info
        QFileInfo sourceInfo(QString("%1%2").arg(!aPath.endsWith("/") ? (aPath + "/") : aPath).arg(aFileName));

        // Check If Matches Pattern
        if (dir.match(sourcePattern, sourceInfo.fileName())) {
            // Get Target File Name
            QString targetFileName = applyPattern(sourceInfo.fileName(), targetPattern);
            // Check Target File Name
            if (targetFileName.isEmpty()) {
                // No Valid Target File Name
                return;
            }

            // Init Target File Path
            QString targetFilePath = targetPath;

            // Check Target File Name
            if (!targetFilePath.endsWith("/")) {
                // Adjust Target File Name
                targetFilePath += "/";
            }

            // Add Item To Queue
            queueModel->addItem(operation, sourceInfo.absoluteFilePath(), targetFilePath + targetFileName);

            // Check If Is Dir
            if (!sourceInfo.isDir() && !sourceInfo.isBundle() && !sourceInfo.isSymLink()) {
                // Add Size To Overall Size
                overallSize += sourceInfo.size();
                // Configure Overall Progress Bar
                configureOverallProgressBar(overallSize);
            }

        }
    }
}

//==============================================================================
// Button Box Accepted Slot
//==============================================================================
void TransferProgressDialog::buttonBoxAccepted()
{
    //qDebug() << "TransferProgressDialog::buttonBoxAccepted";

    // ...

    // Emit Dialog Closed Signal
    //emit dialogClosed(this);
}

//==============================================================================
// Button Box Rejected Slot
//==============================================================================
void TransferProgressDialog::buttonBoxRejected()
{
    //qDebug() << "TransferProgressDialog::buttonBoxRejected";

    // ...

    // Emit Dialog Closed Signal
    //emit dialogClosed(this);
}

//==============================================================================
// Tab Changed Slot
//==============================================================================
void TransferProgressDialog::tabChanged(const int& aIndex)
{
    //qDebug() << "TransferProgressDialog::tabChanged - aIndex: " << aIndex;

    // Check Tab Index
    if (aIndex == 1) {
        // Update Queue Column Sizes
        updateQueueColumnSizes();
    }
}

//==============================================================================
// Resize Event
//==============================================================================
void TransferProgressDialog::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent && aEvent->size().width() > 0 && aEvent->size().height() > 0) {
        //qDebug() << "TransferProgressDialog::resizeEvent - aEvent: " << aEvent->size();

        // Update Queue Column Sizes
        updateQueueColumnSizes();

        // Get Font Metrics
        QFontMetrics fontMetrics = ui->currentFileNameLabel->fontMetrics();
        // Set Current File Name Label Text
        ui->currentFileNameLabel->setText(fontMetrics.elidedText(nameLabelText, Qt::ElideMiddle, ui->currentFileNameLabel->width()));
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void TransferProgressDialog::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Timer ID
        if (aEvent->timerId() == transferSpeedTimerID) {

            // Check File Util Client
            if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTWaiting)) {
                //qDebug() << "TransferProgressDialog::timerEvent - currTransferedSize: " << currTransferedSize << " - diff: " << (currTransferedSize - speedMeasureLastSize);
                // Set Current File name Label
                setCurrentFileName(currentFileName, currTransferedSize - speedMeasureLastSize);
                // Set Speed Measure Last Size
                speedMeasureLastSize = currTransferedSize;
            } else {
                // Set Current File Name
                setCurrentFileName("");
                // Stop Transfer Speed Timer
                stopTransferSpeedTimer();
            }

            // ...

        } else if (aEvent->timerId() == progressRefreshTimerID) {
            // Check File Util Client
            if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTWaiting)) {
                // Set Current Progress
                setCurrentProgress(currentProgress);
                // Set Overall Progress
                setOverallProgress(overallProgress);
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
void TransferProgressDialog::hideEvent(QHideEvent* aEvent)
{
    //qDebug() << "TransferProgressDialog::hideEvent";

    QDialog::hideEvent(aEvent);

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// On Close When Finished Check Box Clicked Slot
//==============================================================================
void TransferProgressDialog::on_closeWhenFinishedCheckBox_clicked()
{
    // Set Close When Finished
    closeWhenFinished = ui->closeWhenFinishedCheckBox->isChecked();
    // Set Settings Value
    settings->setCloseWhenFinished(closeWhenFinished);
}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressDialog::~TransferProgressDialog()
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

    // Delete Transfer Queue List View
    delete ui->transferQueueListView;

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

    //qDebug() << "TransferProgressDialog::~TransferProgressDialog";
}


