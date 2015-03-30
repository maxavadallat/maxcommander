#include <QDebug>
#include <mcwinterface.h>

#include "deleteprogressdialog.h"
#include "deleteprogressmodel.h"
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

    // Check File Util
    if (fileUtil) {
        // Connect Signals


    }
}

//==============================================================================
// Process Queue
//==============================================================================
void DeleteProgressDialog::processQueue()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "DeleteProgressDialog::processQueue";

    }
}

//==============================================================================
// Suspend
//==============================================================================
void DeleteProgressDialog::suspend()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "DeleteProgressDialog::suspend";

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

    }
}

//==============================================================================
// Abort
//==============================================================================
void DeleteProgressDialog::abort()
{
    // Check File Util
    if (fileUtil) {
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
void DeleteProgressDialog::clientStatusChanged(const int& aID, const int& aStatus)
{

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

}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressDialog::~DeleteProgressDialog()
{
    // Abort
    abort();

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
