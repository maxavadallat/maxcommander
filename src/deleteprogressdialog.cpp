#include <QTimer>
#include <QSettings>
#include <QDebug>

#include <mcwinterface.h>

#include "deleteprogressdialog.h"
#include "deleteprogressmodel.h"
#include "confirmdialog.h"
#include "ui_deleteprogressdialog.h"
#include "remotefileutilclient.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
DeleteProgressDialog::DeleteProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteProgressDialog)
    , queueModel(NULL)
    , fileUtil(NULL)
    , closeWhenFinished(false)
    , queueIndex(-1)
    , dirPath("")
{
    qDebug() << "DeleteProgressDialog::DeleteProgressDialog";

    // Setup UI
    ui->setupUi(this);

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
    // Set Model
    ui->deleteQueue->setModel(queueModel);

    // Create File Util
    fileUtil = new RemoteFileUtilClient();
    // Connect To File Server
    fileUtil->connectToFileServer();

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

    // Init Local Path
    QString localPath = aDirPath;
    // Check Local Path
    if (!localPath.endsWith("/")) {
        // Adjust Local Path
        localPath += "/";
    }

    // Go Thru Selected Files
    for (int i=0; i<sfCount; ++i) {
        // Add To Queue Model
        queueModel->addItem(localPath + aSelectedFiles[i]);
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
        qDebug() << "DeleteProgressDialog::processQueue";

        // Check Queue Index
        if (queueIndex >= 0 && queueIndex < queueModel->rowCount()) {
            // Get File Name
            QString fileName = queueModel->getFileName(queueIndex);
            // Delete File
            fileUtil->deleteFile(fileName);
        } else {
            // Check Close When Finished
            if (closeWhenFinished) {
                // Close
                close();
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
        queueIndex = -1;
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void DeleteProgressDialog::restoreUI()
{
    qDebug() << "DeleteProgressDialog::restoreUI";

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
void DeleteProgressDialog::saveSettings()
{
    qDebug() << "DeleteProgressDialog::saveSettings";

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
// Launch Progress Dialog
//==============================================================================
void DeleteProgressDialog::launch(const QString& aDirPath, const QStringList& aSelectedFiles)
{
    qDebug() << "DeleteProgressDialog::launch - aDirPath: " << aDirPath << " - count: " << aSelectedFiles.count();

    // Set Dir Path
    dirPath = aDirPath;

    // Restore UI
    restoreUI();

    // ...

    // Show
    show();

    // Build Queue
    if (buildQueue(aDirPath, aSelectedFiles)) {
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
// Set Current File Name
//==============================================================================
void DeleteProgressDialog::setCurrentFileName(const QString& aCurrentFileName)
{
    // Set Text
    ui->currentFileNameLabel->setText(aCurrentFileName);
}

//==============================================================================
// Set Current File Progress
//==============================================================================
void DeleteProgressDialog::setCurrentProgress(const quint64& aProgress, const quint64& aTotal)
{
    // Set Minimum
    ui->currentProgress->setMinimum(0);

    // Check Total
    if (aTotal > INT_MAX) {
        // Set Maximum
        ui->currentProgress->setMaximum(aTotal >> 32);
        // Set Value
        ui->currentProgress->setValue(aProgress >> 32);
    } else {
        // Set Maximum
        ui->currentProgress->setMaximum(aTotal);
        // Set Value
        ui->currentProgress->setValue(aProgress);
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

    // Set Current Progress
    setCurrentProgress(queueIndex, queueModel->rowCount());

    // ...
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void DeleteProgressDialog::fileOpProgress(const unsigned int& aID,
                                          const QString& aOp,
                                          const QString& aCurrFilePath,
                                          const quint64& aCurrProgress,
                                          const quint64& aCurrTotal,
                                          const quint64& aOverallProgress,
                                          const quint64& aOverallTotal,
                                          const int& aSpeed)
{
    Q_UNUSED(aCurrProgress);
    Q_UNUSED(aCurrTotal);
    Q_UNUSED(aSpeed);

    qDebug() << "DeleteProgressDialog::fileOpProgress - aID: " << aID << " - aOp: " << aOp;

    // Set Current File Name
    setCurrentFileName(aCurrFilePath);
    // Set Current Progress
    setCurrentProgress(aOverallProgress, aOverallTotal);
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

    // Check Queue Model
    if (queueModel) {
        // Set Done
        queueModel->setDone(queueIndex);
    }

    // Increase Current Queue Index
    queueIndex++;

    // Set Current Progress
    setCurrentProgress(queueIndex, queueModel->rowCount());

    // Process Queue
    processQueue();
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

    // ...
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

    qDebug() << "DeleteProgressDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aCode: " << aCode;

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

        // Set Current Progress
        setCurrentProgress(queueIndex, queueModel->rowCount());
    }
}

//==============================================================================
// Close Event
//==============================================================================
void DeleteProgressDialog::closeEvent(QCloseEvent* aEvent)
{
    qDebug() << "DeleteProgressDialog::closeEvent";

    QDialog::closeEvent(aEvent);

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressDialog::~DeleteProgressDialog()
{
    // Save Settings
    saveSettings();

    // Abort
    abort();

    // Clear Queue
    clearQueue();

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
        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    qDebug() << "DeleteProgressDialog::~DeleteProgressDialog";
}
