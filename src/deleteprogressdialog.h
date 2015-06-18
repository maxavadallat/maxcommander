#ifndef DELETEPROGRESSDIALOG_H
#define DELETEPROGRESSDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QDialogButtonBox>
#include <QImage>

namespace Ui {
class DeleteProgressDialog;
}

class DeleteProgressModel;
class RemoteFileUtilClient;
class ConfirmDialog;


//==============================================================================
// Delete Progress Dialog Queue Item Delegate Class
//==============================================================================
class DeleteProgressQueueItemDelegate : public QStyledItemDelegate
{
public:
    // Constructor
    explicit DeleteProgressQueueItemDelegate(QObject* aParent = NULL);

    // Paint
    virtual void paint(QPainter* aPainter,
                       const QStyleOptionViewItem& aOption,
                       const QModelIndex& aIndex) const;

    // Create Edirot
    virtual QWidget* createEditor(QWidget* aParent,
                                  const QStyleOptionViewItem& aOption,
                                  const QModelIndex& aIndex) const;

    // Destructor
    virtual ~DeleteProgressQueueItemDelegate();

protected:

    // Done Icon
    QImage              doneIcon;
    // Error Icon
    QImage              errorIcon;
    // Progress Icon
    QImage              progressIcon;
};







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
    void setCurrentProgress(const int& aProgress);

    // Launch Progress Dialog
    void launch(const QString& aDirPath, const QStringList& aSelectedFiles);
    // Launch Progress Dialog
    void launch(const QString& aDirPath, const QString& aPattern);
    // Get Dir Path
    QString getDirPath();

    // Suspend
    void suspend();
    // Resume
    void resume();
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

    // Build Queue
    bool buildQueue(const QString& aDirPath, const QStringList& aSelectedFiles);
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

    // Update Queue Column Sizes
    void updateQueueColumnSizes();

    // Start Progress Refresh Timer
    void startProgressRefreshTimer();
    // Stop Progress Refresh Timer
    void stopProgressRefreshTimer();

protected slots: // for RemoteFileUtilClient

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

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);
    // Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);
    // Hide Event
    virtual void hideEvent(QHideEvent* aEvent);

private slots:

    // On Close When Finished Check Box Clicked
    void on_closeWhenFinishedCheckBox_clicked();

private:
    friend class MainWindow;

    // UI
    Ui::DeleteProgressDialog*   ui;
    // Queue Model
    DeleteProgressModel*        queueModel;
    // Remote File Util Client
    RemoteFileUtilClient*       fileUtil;
    // CLose When Finished
    bool                        closeWhenFinished;
    // Queue Index
    int                         queueIndex;
    // Dir Path
    QString                     dirPath;
    // Selection Pattern
    QString                     pattern;
    // Need Queue
    bool                        needQueue;

    // Progress Refresh Timer ID
    int                         progressRefreshTimerID;
};

#endif // DELETEPROGRESSDIALOG_H


