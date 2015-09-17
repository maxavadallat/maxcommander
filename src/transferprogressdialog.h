#ifndef TRANSFERPROGRESSDIALOG_H
#define TRANSFERPROGRESSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QCloseEvent>
#include <QHideEvent>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QDialogButtonBox>
#include <QImage>

namespace Ui {
class TransferProgressDialog;
}

class TransferProgressModel;
class RemoteFileUtilClient;
class ConfirmDialog;


//==============================================================================
// Transfer Progress Dialog Queue Item Delegate Class
//==============================================================================
class TransferProgressQueueItemDelegate : public QStyledItemDelegate
{
public:
    // Constructor
    explicit TransferProgressQueueItemDelegate(QObject* aParent = NULL);

    // Paint
    virtual void paint(QPainter* aPainter,
                       const QStyleOptionViewItem& aOption,
                       const QModelIndex& aIndex) const;

    // Create Edirot
    virtual QWidget* createEditor(QWidget* aParent,
                                  const QStyleOptionViewItem& aOption,
                                  const QModelIndex& aIndex) const;

    // Destructor
    virtual ~TransferProgressQueueItemDelegate();

protected:
    // Done Icon
    QImage                  doneIcon;
    // Error Icon
    QImage                  errorIcon;
    // Progress Icon
    QImage                  progressIcon;
};









//==============================================================================
// Transfer Progress Dialog Class
//==============================================================================
class TransferProgressDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit TransferProgressDialog(const QString& aOperation, QWidget* aParent = NULL);

    // Set Title
    void setTitle(const QString& aTitle);

    // Set Current File Name
    void setCurrentFileName(const QString& aCurrentFileName, const int& aSpeed = 0);
    // Set Current File Progress
    void setCurrentProgress(const quint64& aProgress);
    // Set Overall Progress
    void setOverallProgress(const quint64& aProgress);

    // Launch Progress Dialog
    void launch(const QString& aSourcePath, const QString& aTargetPath, const QStringList& aSelectedFiles, const int& aOptions = 0);
    // Launch Progress Dialog
    void launch(const QString& aSourcePath, const QString& aTargetPath, const QString& aSourcePattern = "*.*", const QString& aTargetPattern = "", const int& aOptions = 0);
    // Launch Progress Dialog
    void launch(const QString& aSource, const QString& aTarget, const int& aOptions = 0);

    // Suspend
    void suspend();
    // Resume
    void resume();
    // Abort
    void abort();

    // Get Last Operation Target
    QString getLastTarget();

    // Destructor
    virtual ~TransferProgressDialog();

signals:

    // Dialog Closed
    void dialogClosed(TransferProgressDialog* aDialog);

protected slots:

    // Init
    void init();

    // Build Queue
    bool buildQueue(const QString& aSourcePath, const QString& aTargetPath, const QStringList& aSelectedFiles);
    // Build Queue
    bool buildQueue(const QString& aSourcePath, const QString& aTargetPath, const QString& aSourcePattern, const QString& aTargetPattern);
    // Process Queue
    void processQueue();
    // Clear Queue
    void clearQueue();

    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();

    // Configure Buttons
    void configureButtons(const QDialogButtonBox::StandardButtons& aButtons = QDialogButtonBox::Close);

    // Configure Current Progress Bar
    void configureCurrentProgressBar(const quint64& aMax);
    // Configure Overall Progress Bar
    void configureOverallProgressBar(const quint64& aMax);

    // Start Transfer Speed Timer
    void startTransferSpeedTimer();
    // Stop Transfer Speed Timer
    void stopTransferSpeedTimer();

    // Update Queue Column Sizes
    void updateQueueColumnSizes();

    // Start Progress Refresh Timer
    void startProgressRefreshTimer();
    // Stop Progress Refresh Timer
    void stopProgressRefreshTimer();

    // Set Archive Mode
    void setArchiveMode(const bool& aArchiveMode);

protected slots: // For RemoteFileUtilClient

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

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
                        const quint64& aCurrTotal);

    // File Operation Skipped Slot
    void fileOpSkipped(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

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

    // Dir List Item Found Slot
    void dirListItemFound(const unsigned int& aID,
                          const QString& aPath,
                          const QString& aFileName);

protected slots: // For QDialogButtonBox

    // Button Box Accepted Slot
    void buttonBoxAccepted();
    // Button Box Rejected Slot
    void buttonBoxRejected();

protected slots: // For QTabWidget

    // Tab Changed Slot
    void tabChanged(const int& aIndex);

protected: // From QDialog

    // Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);
    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);
    // Hide Event
    virtual void hideEvent(QHideEvent* aEvent);

protected slots:

    // On Close When Finished Check Box Clicked Slot
    void on_closeWhenFinishedCheckBox_clicked();

private:
    friend class MainWindow;

    // UI
    Ui::TransferProgressDialog*     ui;
    // Queue Model
    TransferProgressModel*          queueModel;
    // Remote File Util Client
    RemoteFileUtilClient*           fileUtil;
    // Operation
    QString                         operation;
    // CLose When Finished
    bool                            closeWhenFinished;
    // Queue Index
    int                             queueIndex;

    // Current File Name
    QString                         currentFileName;
    // Source Path
    QString                         sourcePath;
    // Target Path
    QString                         targetPath;
    // Options
    int                             options;
    // Source Pattern
    QString                         sourcePattern;
    // Target Pattern
    QString                         targetPattern;
    // Need To Build Queue
    bool                            needQueue;

    // Name Label Text
    QString                         nameLabelText;

    // Speed Timer ID
    int                             transferSpeedTimerID;
    // Last Transfered Size
    quint64                         lastTransferedSize;
    // Current Transfered Size
    quint64                         currTransferedSize;
    // Speed Measure Last Size
    quint64                         speedMeasureLastSize;
    // Transfer Speed
    int                             transferSpeed;

    // Current File Progress
    quint64                         currentProgress;
    // Current File Size
    quint64                         currentSize;

    // Current Progress Scale
    int                             currentProgressScale;

    // Overall Progress
    quint64                         overallProgress;
    // Overall Size
    quint64                         overallSize;

    // Overall Progress Scale
    int                             overallProgressScale;

    // Progress Refresh Timer ID
    int                             progressRefreshTimerID;

    // Archive Mode
    bool                            archiveMode;
};

#endif // TRANSFERPROGRESSDIALOG_H

