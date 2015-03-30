#ifndef DELETEPROGRESSDIALOG_H
#define DELETEPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteProgressDialog;
}

class DeleteProgressModel;
class RemoteFileUtilClient;


//==============================================================================
// Delete Progress Dialog Class
//==============================================================================
class DeleteProgressDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit DeleteProgressDialog(QWidget* aParent = NULL);

    // Set Current File Name
    void setCurrentFileName(const QString& aCurrentFileName);
    // Set Current File Progress
    void setCurrentProgress(const quint64& aProgress, const quint64& aTotal);

    // Abort
    void abort();

    // Destructor
    virtual ~DeleteProgressDialog();

signals:

    // Dialog Closed
    void dialogClosed(DeleteProgressDialog* aDialog);

protected slots:

    // Init
    void init();

    // Process Queue
    void processQueue();
    // Suspend
    void suspend();
    // Resume
    void resume();

protected slots: // for RemoteFileUtilClient

    // Client Status Changed Slot
    void clientStatusChanged(const int& aID, const int& aStatus);

    // File Operation Started Slot
    void fileOpStarted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Progress Slot
    void fileOpProgress(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aCurrFilePath,
                        const quint64& aCurrProgress,
                        const quint64& aCurrTotal,
                        const quint64& aOverallProgress,
                        const quint64& aOverallTotal,
                        const int& aSpeed);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Slot
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // Need Confirmation Slot
    void fileOpNeedConfirm(const unsigned int& aID,
                           const QString& aOp,
                           const int& aCode,
                           const QString& aPath,
                           const QString& aSource,
                           const QString& aTarget);

    // File Operation Queue Item Found Slot
    void fileOpQueueItemFound(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aPath,
                              const QString& aSource,
                              const QString& aTarget);

private:
    // UI
    Ui::DeleteProgressDialog*   ui;
    // Queue Model
    DeleteProgressModel*        queueModel;
    // Remote File Util Client
    RemoteFileUtilClient*       fileUtil;
};

#endif // DELETEPROGRESSDIALOG_H
