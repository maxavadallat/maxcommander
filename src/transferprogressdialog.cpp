#include <QDebug>

#include "transferprogressdialog.h"
#include "transferprogressmodel.h"
#include "ui_transferprogressdialog.h"
#include "remotefileutilclient.h"


//==============================================================================
// Constructor
//==============================================================================
TransferProgressDialog::TransferProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TransferProgressDialog)
    , queueModel(NULL)
    , fileUtil(NULL)
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
// Suspend
//==============================================================================
void TransferProgressDialog::suspend()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::suspend";

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

    }
}

//==============================================================================
// Abort
//==============================================================================
void TransferProgressDialog::abort()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "TransferProgressDialog::abort";

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
void TransferProgressDialog::clientStatusChanged(const int& aID, const int& aStatus)
{

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

}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressDialog::~TransferProgressDialog()
{
    // Abort
    abort();

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
