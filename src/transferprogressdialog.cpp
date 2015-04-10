#include <QTimer>
#include <QSettings>
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
TransferProgressDialog::TransferProgressDialog(const QString& aOperation, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferProgressDialog)
    , queueModel(NULL)
    , fileUtil(NULL)
    , operation(aOperation)
    , closeWhenFinished(false)
    , queueIndex(-1)
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

    // Create File Util
    fileUtil = new RemoteFileUtilClient();

    // Check File Util
    if (fileUtil) {
        // Connect Signals
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpProgress(uint,QString,QString,quint64,quint64,quint64,quint64,int)), this, SLOT(fileOpProgress(uint,QString,QString,quint64,quint64,quint64,quint64,int)));
        connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
    }
}

//==============================================================================
// Launch Progress Dialog
//==============================================================================
void TransferProgressDialog::launch(const QString& aSourcePath, const QString& aTargetPath, const QStringList& aSelectedFiles)
{
    qDebug() << "TransferProgressDialog::launch - aSourcePath: " << aSourcePath << " - aTargetPath: " << aTargetPath << " - count: " << aSelectedFiles.count();

    // Restore UI
    restoreUI();

    // ...

    // Show
    show();

    // Build Queue
    if (buildQueue(aSourcePath, aTargetPath, aSelectedFiles)) {
        // Set Queue Index
        queueIndex = 0;
        // Process Queue
        QTimer::singleShot(1, this, SLOT(processQueue()));
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
        // Add To Queue Model
        queueModel->addItem(operation, localSourcePath + aSelectedFiles[i], localTargetPath + aSelectedFiles[i]);
    }

    return true;
}

//==============================================================================
// Process Queue
//==============================================================================
void TransferProgressDialog::processQueue()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::processQueue";

    }
}

//==============================================================================
// Clear Queue
//==============================================================================
void TransferProgressDialog::clearQueue()
{
    // Check Queue
    if (queueModel) {
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

    // Init Settings
    QSettings settings;
    // Get Close When Finished
    closeWhenFinished = settings.value(SETTINGS_KEY_CLOSE_WHEN_FINISHED, false).toBool();
    // Set Checkbox
    ui->closeWhenFinishedCheckBox->setChecked(closeWhenFinished);
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
// Suspend
//==============================================================================
void TransferProgressDialog::suspend()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::suspend";

        // Suspend
        fileUtil->suspend();
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

    qDebug() << "DeleteProgressDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void TransferProgressDialog::fileOpProgress(const unsigned int& aID,
                                            const QString& aOp,
                                            const QString& aCurrFilePath,
                                            const quint64& aCurrProgress,
                                            const quint64& aCurrTotal,
                                            const quint64& aOverallProgress,
                                            const quint64& aOverallTotal,
                                            const int& aSpeed)
{
    qDebug() << "DeleteProgressDialog::fileOpProgress - aID: " << aID << " - aOp: " << aOp << " - aCurrFilePath: " << aCurrFilePath << " - aSpeed: " << aSpeed;

    // Set Current File Name
    setCurrentFileName(aCurrFilePath);
    // Set Current Progress
    setCurrentProgress(aCurrProgress, aCurrTotal);
    // Set Overall Progress
    setOverallProgress(aOverallProgress, aOverallTotal);

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

    qDebug() << "DeleteProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

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

    qDebug() << "DeleteProgressDialog::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

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

    qDebug() << "DeleteProgressDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aError: " << aError;

    // ...

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

    qDebug() << "DeleteProgressDialog::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aCode: " << aCode;

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

    qDebug() << "DeleteProgressDialog::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

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
