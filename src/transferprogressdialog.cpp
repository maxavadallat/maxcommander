#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QFileInfo>
#include <QDebug>

#include <mcwinterface.h>

#include "transferprogressdialog.h"
#include "transferprogressmodel.h"
#include "confirmdialog.h"
#include "ui_transferprogressdialog.h"
#include "remotefileutilclient.h"
#include "constants.h"




//==============================================================================
// Constructor
//==============================================================================
TransferProgressQueueItemDelegate::TransferProgressQueueItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
    , doneIcon(QImage(DEFAULT_ICON_PATH_OK).scaled(QSize(DEFAULT_ICON_WIDTH_16, DEFAULT_ICON_HEIGHT_16), Qt::KeepAspectRatio, Qt::SmoothTransformation))
    , errorIcon(QImage(DEFAULT_ICON_PATH_CANCEL).scaled(QSize(DEFAULT_ICON_WIDTH_16, DEFAULT_ICON_HEIGHT_16), Qt::KeepAspectRatio, Qt::SmoothTransformation))
    , progressIcon(QImage(DEFAULT_ICON_PATH_PROGRESS).scaled(QSize(DEFAULT_ICON_WIDTH_16, DEFAULT_ICON_HEIGHT_16), Qt::KeepAspectRatio, Qt::SmoothTransformation))
{
}

//==============================================================================
// Paint
//==============================================================================
void TransferProgressQueueItemDelegate::paint(QPainter* aPainter,
                                              const QStyleOptionViewItem& aOption,
                                              const QModelIndex& aIndex) const
{
    // Check Painter
    if (!aPainter) {
        return;
    }

    // Check State
    if (aOption.state & QStyle::State_Selected) {
        // Fill Selected Background
        aPainter->fillRect(aOption.rect, aOption.palette.highlight());
    }

    // Switch Column
    switch (aIndex.column()) {
        case 0:
            // Draw Operation Text
            aPainter->drawText(aOption.rect, Qt::AlignCenter, aIndex.model()->data(aIndex).toString());
        break;

        case 1: {
            // Init File Name
            QString fileName(aIndex.model()->data(aIndex).toString());

            // Check File
            if (!QFile::exists(fileName)) {
                // Save
                aPainter->save();
                // Set Brush
                aPainter->setBrush(QColor(128, 0, 0, 16));
                // Set Pen
                aPainter->setPen(Qt::NoPen);
                // Draw Background
                aPainter->drawRect(aOption.rect);
                // Restore
                aPainter->restore();
            }

            // Draw Source Text
            aPainter->drawText(aOption.rect.adjusted(DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN, 0, -DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN, 0), Qt::AlignVCenter, fileName);
        } break;

        case 2:
            // Draw Target Text
            aPainter->drawText(aOption.rect.adjusted(DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN, 0, -DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN, 0), Qt::AlignVCenter, aIndex.model()->data(aIndex).toString());
        break;

        case 3:
            // Switch State
            switch (aIndex.model()->data(aIndex).toInt()) {
                case ETPSkipped:
                case ETPIdle:
                    // Save
                    aPainter->save();
                    // Set Opacity
                    aPainter->setOpacity(DEFAULT_PROGRESS_DIALOG_SEMI_TRANSPARENCY);
                    // Draw Image
                    aPainter->drawImage(aOption.rect.center() - QPoint(progressIcon.width() / 2, progressIcon.height() / 2), progressIcon);
                    // Restore
                    aPainter->restore();
                break;

                case ETPRunning:
                    // Set Opacity
                    aPainter->setOpacity(1.0);
                    // Draw Image
                    aPainter->drawImage(aOption.rect.center() - QPoint(progressIcon.width() / 2, progressIcon.height() / 2), progressIcon);
                break;

                case ETPFinished:   aPainter->drawImage(aOption.rect.center() - QPoint(doneIcon.width() / 2, doneIcon.height() / 2), doneIcon);             break;
                case ETPError:      aPainter->drawImage(aOption.rect.center() - QPoint(errorIcon.width() / 2, errorIcon.height() / 2), errorIcon);          break;
            }
        break;
    }
}

//==============================================================================
// Create Edirot
//==============================================================================
QWidget* TransferProgressQueueItemDelegate::createEditor(QWidget* aParent,
                                                         const QStyleOptionViewItem& aOption,
                                                         const QModelIndex& aIndex) const
{
    Q_UNUSED(aParent);
    Q_UNUSED(aOption);
    Q_UNUSED(aIndex);

    return NULL;
}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressQueueItemDelegate::~TransferProgressQueueItemDelegate()
{
}



















//==============================================================================
// Constructor
//==============================================================================
TransferProgressDialog::TransferProgressDialog(const QString& aOperation, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferProgressDialog)
    , queueModel(NULL)
    , fileUtil(NULL)
    , operation(aOperation)
    , closeWhenFinished(false)
    , queueIndex(-1)
    , transferSpeedTimerID(-1)
    , lastTransferedSize(0)
    , currTransferedSize(0)
    , speedMeasureLastSize(0)
    , transferSpeed(0)
    , overallProgress(0)
    , overallSize(0)
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
    // Set Model
    ui->transferQueue->setModel(queueModel);
    // Set Item Delegate
    ui->transferQueue->setItemDelegate(new TransferProgressQueueItemDelegate());

    // Create File Util
    fileUtil = new RemoteFileUtilClient();

    // Check File Util
    if (fileUtil) {
        // Connect Signals
        connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpProgress(uint,QString,QString,quint64,quint64)), this, SLOT(fileOpProgress(uint,QString,QString,quint64,quint64)));
        connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
    }

    // Connect To File Server
    //fileUtil->connectToFileServer();

    // Connect Signals
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBoxAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(buttonBoxRejected()));

    // Connect Signals
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    // ...
}

//==============================================================================
// Launch Progress Dialog
//==============================================================================
void TransferProgressDialog::launch(const QString& aSourcePath, const QString& aTargetPath, const QStringList& aSelectedFiles)
{
    qDebug() << "TransferProgressDialog::launch - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - count: " << aSelectedFiles.count();

    // Set Source Path
    sourcePath = aSourcePath;
    // Set Target Path
    targetPath = aTargetPath;

    // Restore UI
    restoreUI();

    // Show
    show();

    // Build Queue
    if (buildQueue(aSourcePath, aTargetPath, aSelectedFiles)) {
        // Set Queue Index
        queueIndex = 0;

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
        queueIndex = -1;
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
    // Check Local Path
    if (!localSourcePath.endsWith("/")) {
        // Adjust Local Path
        localSourcePath += "/";
    }

    // Init Local Target Path
    QString localTargetPath = aTargetPath;
    // Check Local Path
    if (!localTargetPath.endsWith("/")) {
        // Adjust Local Path
        localTargetPath += "/";
    }

    // Go Thru Selected Files
    for (int i=0; i<sfCount; ++i) {
        // Init Source File Path
        QString sourceFilePath = localSourcePath + aSelectedFiles[i];
        // Init Target File Path
        QString targetFilePath = localTargetPath + aSelectedFiles[i];

        // Init Source File Info
        QFileInfo sourceInfo(sourceFilePath);
        // Check If Is Dir
        if (!sourceInfo.isDir() && !sourceInfo.isBundle() && !sourceInfo.isSymLink()) {
            // Add Size To Overall Size
            overallSize += sourceInfo.size();

            qDebug() << "TransferProgressDialog::buildQueue - aSourcePath: " << aSourcePath << " - overallSize: " << overallSize;

            // Set Overall Progress
            setOverallProgress(overallProgress, overallSize);
        }

        // Add To Queue Model
        queueModel->addItem(operation, sourceFilePath, targetFilePath);
    }

    return true;
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
            QString operation = queueModel->getOperation(queueIndex);

            // Check Operation - Copy
            if (operation == DEFAULT_OPERATION_COPY_FILE) {
                // Copy File
                fileUtil->copyFile(sourceFileName, targetFileName);
            // Check Operation - Rename/Move
            } else if (operation == DEFAULT_OPERATION_MOVE_FILE) {
                // Move File
                fileUtil->moveFile(sourceFileName, targetFileName);
            }

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

                // Check Overall Progress
                if (ui->overallProgress->value() == 0) {
                    // Set Overall Progress
                    setOverallProgress(1, 1);
                }
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
        queueIndex = -1;
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
        setTitle(DEFAULT_TITLE_COPY_FILES);
    } else {
        // Set Title
        setTitle(DEFAULT_TITLE_MOVE_FILES);
    }

    // Init Settings
    QSettings settings;
    // Get Close When Finished
    closeWhenFinished = settings.value(SETTINGS_KEY_CLOSE_WHEN_FINISHED, false).toBool();
    // Set Checkbox
    ui->closeWhenFinishedCheckBox->setChecked(closeWhenFinished);

    // Update Queue Column Sizes
    updateQueueColumnSizes();
}

//==============================================================================
// Save Settings
//==============================================================================
void TransferProgressDialog::saveSettings()
{
    qDebug() << "TransferProgressDialog::saveSettings";

    // Init Settings
    QSettings settings;
    // Get Close When Finished
    closeWhenFinished = ui->closeWhenFinishedCheckBox->checkState() == Qt::Checked;
    // Save Setting
    settings.setValue(SETTINGS_KEY_CLOSE_WHEN_FINISHED, closeWhenFinished);
    // Sync
    settings.sync();
}

//==============================================================================
// Configure Buttons
//==============================================================================
void TransferProgressDialog::configureButtons(const QDialogButtonBox::StandardButtons& aButtons)
{
    // Configure Buttons
    ui->buttonBox->setStandardButtons(aButtons);
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

    // Set Column Width
    ui->transferQueue->setColumnWidth(0, DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_OP);
    // Set Column Width
    ui->transferQueue->setColumnWidth(1, (ui->transferQueue->width() - DEFAULT_PROGRESS_DIALOG_SCROLLBAR_WIDTH - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_OP - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE) / 2);
    // Set Column Width
    ui->transferQueue->setColumnWidth(2, (ui->transferQueue->width() - DEFAULT_PROGRESS_DIALOG_SCROLLBAR_WIDTH - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_OP - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE) / 2);
    // Set Column Width
    ui->transferQueue->setColumnWidth(3, DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
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
    }
}

//==============================================================================
// Set Current File Progress
//==============================================================================
void TransferProgressDialog::setCurrentProgress(const quint64& aProgress, const quint64& aTotal)
{
    // Set Minimum
    ui->currentProgress->setMinimum(0);

    // Check Total
    if (aTotal > UINT_MAX) {
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
    if (aTotal > UINT_MAX) {
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

        // Process Queue
        QTimer::singleShot(1, this, SLOT(processQueue()));
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void TransferProgressDialog::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    qDebug() << "TransferProgressDialog::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

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


    qDebug() << " ";
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << " ";
    qDebug() << "TransferProgressDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Configure Buttons
    configureButtons(QDialogButtonBox::Abort);

    // Set Current File Name
    setCurrentFileName(aSource);

    // Reset Last Transfer Size
    lastTransferedSize = 0;
    // Reset Current Transfer Size
    currTransferedSize = 0;

    // Set Current Progress
    setCurrentProgress(0, 0);

    // Start Transfer Speed Timer
    startTransferSpeedTimer();
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

    //qDebug() << "TransferProgressDialog::fileOpProgress - p: " << aCurrProgress << " - t: " << aCurrTotal;

    // Check Queue Model
    if (queueModel) {
        // Set Done
        queueModel->setProgressState(queueIndex, ETPRunning);
    }

    // Set Current Progress
    setCurrentProgress(aCurrProgress, aCurrTotal);

    // Set Current Transfer Size
    currTransferedSize = aCurrProgress;

    // Calculate Overall Progress
    overallProgress += (currTransferedSize - lastTransferedSize);

    // Set Last Transfer Size
    lastTransferedSize = currTransferedSize;

    // Set Overall Progress
    setOverallProgress(overallProgress, overallSize);
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
    qDebug() << " ";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    qDebug() << " ";

    // Check Operation - Copy/Move File
    if (aOp == DEFAULT_OPERATION_COPY_FILE || aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Check Queue Model
        if (queueModel) {
            // Set Done
            queueModel->setProgressState(queueIndex, ETPSkipped);
        }

        // Init Source Info
        QFileInfo sourceInfo(aSource);

        // Set Current Progress
        setCurrentProgress(sourceInfo.size(), sourceInfo.size());

        // Calculate Overall Progress
        overallProgress += sourceInfo.size();

        // Set Last Transfer Size
        lastTransferedSize = 0;

        // Set Overall Progress
        setOverallProgress(overallProgress, overallSize);

        // Increase Current Queue Index
        queueIndex++;
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

    qDebug() << "TransferProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;
    qDebug() << " ";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    qDebug() << " ";

    // ...

    // Check Operation - Copy
    if (operation == DEFAULT_OPERATION_COPY_FILE) {

        // Check Finished Operation
        if (aOp == DEFAULT_OPERATION_QUEUE || aOp == DEFAULT_OPERATION_COPY_FILE) {
            // Check Queue Model
            if (queueModel) {
                // Set Done
                queueModel->setProgressState(queueIndex, ETPFinished);
            }

            // Increase Current Queue Index
            queueIndex++;

            // Process Queue
            processQueue();

        } else {

            // Unhandled

            // ...
        }

    // Check Operation - Move
    } else if (operation == DEFAULT_OPERATION_MOVE_FILE) {

        // Check Finished Operation - Move
        if (aOp == DEFAULT_OPERATION_MOVE_FILE) {

            // Check Queue Model
            if (queueModel) {
                // Set Done
                queueModel->setProgressState(queueIndex, ETPFinished);
            }

            // Set Current Progress
            setCurrentProgress(1, 1);

            // Increase Current Queue Index
            queueIndex++;

        // Check Finished Operation - Queue
        } else if (aOp == DEFAULT_OPERATION_QUEUE) {

            // Do Nothing, Process Queue

        // Check Finished Operation - Copy
        } else if (aOp == DEFAULT_OPERATION_COPY_FILE) {

            // Return, Delete Suppose to Follow

            return;

        } else if (aOp == DEFAULT_OPERATION_DELETE_FILE) {

            // Check Queue Model
            if (queueModel) {
                // Set Done
                queueModel->setProgressState(queueIndex, ETPFinished);
            }

            // Increase Current Queue Index
            queueIndex++;

        } else {

            // Unhandled

            qDebug() << "TransferProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - WTF?!?";

            // ...

        }

        // Process Queue
        processQueue();
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
                confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_CANT_COPY_FILE));

            // Check Operation - Move
            } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {

                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_CANT_MOVE_FILE));

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

        // ...

        // Check Operation
        if (aOp == DEFAULT_OPERATION_COPY_FILE) {
            // Add Item
            queueModel->addItem(aOp, aSource, aTarget);

            // Init Source File Info
            QFileInfo sourceInfo(aSource);

            // Check If Is Dir
            if (!sourceInfo.isDir() && !sourceInfo.isBundle() && !sourceInfo.isSymLink()) {

                // Add Size To Overall Size
                overallSize += sourceInfo.size();

                // Set Overall Progress
                setOverallProgress(overallProgress, overallSize);
            }
        } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
            // Insert Item
            queueModel->insertItem(queueIndex, aOp, aSource, aTarget);

            // Init Source File Info
            QFileInfo sourceInfo(aSource);

            // Check If Is Dir
            if (!sourceInfo.isDir() && !sourceInfo.isBundle() && !sourceInfo.isSymLink()) {

                // Add Size To Overall Size
                overallSize += sourceInfo.size();

                // Set Overall Progress
                setOverallProgress(overallProgress, overallSize);
            }
        }
    }
}

//==============================================================================
// Button Box Accepted Slot
//==============================================================================
void TransferProgressDialog::buttonBoxAccepted()
{
    qDebug() << "TransferProgressDialog::buttonBoxAccepted";

    // ...

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// Button Box Rejected Slot
//==============================================================================
void TransferProgressDialog::buttonBoxRejected()
{
    qDebug() << "TransferProgressDialog::buttonBoxRejected";

    // ...

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
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
        }
    }
}

//==============================================================================
// Close Event
//==============================================================================
void TransferProgressDialog::closeEvent(QCloseEvent* aEvent)
{
    QDialog::closeEvent(aEvent);

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressDialog::~TransferProgressDialog()
{
    // Save Settings
    saveSettings();

    // Abort
    abort();

    // Clear Queue
    clearQueue();

    delete ui;

    // Check Qeuue Model
    if (queueModel) {
        // Delete Queue
        delete queueModel;
        queueModel = NULL;
    }

    // Check File Util
    if (fileUtil) {
        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    qDebug() << "TransferProgressDialog::~TransferProgressDialog";
}
