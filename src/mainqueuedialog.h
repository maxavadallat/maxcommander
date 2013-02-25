#ifndef MAINQUEUEDIALOG_H
#define MAINQUEUEDIALOG_H

// INCLUDES

#include <QDialog>
#include <QEventLoop>

#include "fileutils.h"


// FORWARD DECLARATIONS

namespace Ui {
class MainQueueDialog;
}

class InfoDialog;
class ConfirmationDialog;


// DECLARATIONS

//==============================================================================
//! @class MainQueueDialog Main Queue Dialog Class
//==============================================================================
class MainQueueDialog : public QDialog,
                        public FileOpQueueViewAPI,
                        public FileDeleteObserver,
                        public FileCopyObserver,
                        public FileMoveObserver,
                        public FileRenameObserver,
                        public InfoDialogProvider,
                        public ConfirmDialogProvider,
                        public ErrorDialogProvider
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit MainQueueDialog(QWidget* aParent = NULL);

    //! @brief Add File Operation Entry
    //! @param aEntry File Operation Entry
    virtual void addOperationEntry(FileOperationEntry* aEntry);

    //! @brief Remove Operation
    //! @param aIndex Operation Index
    virtual void removeOperationEntry(const int& aIndex);

    //! @brief Get Operations Count
    //! @param none
    //! @return Operations Count
    virtual int opEntryCount();

    //! @brief Get Operation
    //! @param aIndex Operation Index
    //! @return Operation
    virtual FileOperationEntry* getOperationEntry(const int& aIndex);

    //! @brief Process Queue
    //! @param none
    virtual void processQueue();

    //! @brief Show
    //! @param none
    virtual void show();

    //! @brief Hide
    //! @param none
    virtual void hide();

    //! @brief Set Modal
    //! @param aModal Modal Setting
    virtual void setModal(const bool& aModal);

    //! @brief Operation Added Callback - SIGNALS DON't WORK
    //! @param aIndex Inserted Index
    //! @param aCount Current Count
    virtual void operationEntryAdded(const int& aIndex, const int& aCount);

    //! @brief Operation Entry Removed Callback - SIGNALS DON'T WORK
    //! @param aIndex Removed Index
    //! @param aCount Count
    virtual void operationEntryRemoved(const int& aIndex, const int& aCount);

    //! @brief Operation Entry Updated Callback - SIGNALS DON'T WORK
    //! @param aIndex Updated Item Index
    virtual void operationEntryUpdated(const int& aIndex);

    //! @brief Get File Operations Queue Handler
    //! @param none
    //! @return File Operations Queue Handler
    virtual FileOpQueueHandler* queueHandler();

    //! @brief Reset All Count & Progress
    //! @param none
    void resetProgress();

    //! @brief Set Progress
    //! @param aProgress Progress Value
    void setProgress(const int& aProgress);

    //! @brief Add To Progress
    //! @param aValue Additional Progress Value
    void addToProgress(const int& aValue);

    //! @brief Get Progress
    //! @param none
    //! @brief Progress Value
    int getProgress();

    //! @brief Set Max Value
    //! @param aMaxValue Max Value
    void setMaxValue(const int& aMaxValue);

    //! @brief Add To Max Value
    //! @param aValue Additional Value To Max Value
    void addToMaxValue(const int& aValue);

    //! @brief Get Max Value
    //! @param none
    //! @brief Max Value
    int getMaxValue();

    //! @brief Destructor
    //! @param none
    virtual ~MainQueueDialog();

signals:

    //! @brief Show Info Signal
    //! @param aTitle Info Title
    //! @param aInfoMsg Info Message
    //! @param aModal Modal Flag
    void showInfo(const QString& aTitle, const QString& aInfoMsg, const bool& aModal);

    //! @brief Show Confirmation Signal
    //! @param aProvider Confrimation Dialog Provider
    //! @param aType Confirmation Type
    //! @param aLoop Blocking Event Loop
    void showConfirmation(ConfirmDialogProvider* aProvider, const int& aType, QEventLoop* aLoop);

    //! @brief Show Error Signal
    //! @param aProvider Error Dialog Provider
    //! @param aErrorCode Error Code
    //! @param aLoop Blocking Event Loop
    void showError(ErrorDialogProvider* aProvider, const int& aErrorCode, QEventLoop* aLoop);

protected:

    //! @brief Abort
    //! @param none
    void abort();

    //! @brief Pause
    //! @param none
    void pause();

    //! @brief Resume
    //! @param none
    void resume();

    //! @brief Clear
    //! @param none
    void clear();

    //! @brief Clear
    //! @param none
    void configureButtons();

protected: // From FileDeleteObserver

    //! @brief Confirm Deletion Callback
    //! @param aFilePath File Path
    //! @param aReadOnly File Read Only
    //! @param aNonEmpty Directory Empty
    //! @return File Delete Observer Delete Confirmation Response
    virtual int confirmDeletion(const QString& aFilePath, const bool& aReadOnly, const bool& aNonEmpty);

    //! @brief Deletion Error Callback
    //! @param aFilePath File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return Deletion Error Code Response
    virtual int deleteError(const QString& aFilePath, const int& aErrorCode);

    //! @brief Delete File Started Notification Callback
    //! @param aFilePath File Path
    virtual void deleteFileStarted(const QString& aFilePath);

    //! @brief Delete File Finished Notification Callback
    //! @param aFilePath File Path
    //! @param aErrorCode File Deletion Error Code
    virtual void deleteFileFinished(const QString& aFilePath, const int& aErrorCode);

protected: // From FileCopyObserver

    //! @brief Confirm OverWrite Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aReadOnly Target File Is Read Only
    //! @return File Copy Observer Response
    virtual int confirmCopyOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly);

    //! @brief Copy File Error Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return File Copy Error Code
    virtual int copyError(const QString& aSource, const QString& aTarget, const int& aErrorCode);

    //! @brief Copy File Started Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    virtual void copyStarted(const QString& aSource, const QString& aTarget);

    //! @brief Copy File Progress Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aCurrentSize Copy File Current Size/Progress
    //! @param aFullSize Copy File Full Size
    virtual void copyProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize);

    //! @brief Copy File Finished Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Copy Error Code
    virtual void copyFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode);

protected: // From FileMoveObserver

    //! @brief Confirm OverWrite Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aReadOnly Target File Is Read Only
    //! @return File Rename/Move Observer Response
    virtual int confirmMoveOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly);

    //! @brief Confirm Source Deletion Callback
    //! @param aSource Source File Path
    //! @param aReadOnly Source File Is Read Only
    //! @param aNonEmpty Source File Is A Non Empty Directory
    //! @return File Rename/Move Observer Response
    virtual int confirmMoveDeletion(const QString& aSource, const bool& aReadOnly, const bool& aNonEmpty);

    //! @brief Rename/Move File Error Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return File Rename/Move Error Code
    virtual int moveError(const QString& aSource, const QString& aTarget, const int& aErrorCode);

    //! @brief Rename/Move File Started Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    virtual void moveStarted(const QString& aSource, const QString& aTarget);

    //! @brief Renamve/Move File Progress Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aCurrentSize Move File Current Size/Progress
    //! @param aFullSize Move File Full Size
    virtual void moveProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize);

    //! @brief Rename/Move File Finished Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Move Error Code
    virtual void moveFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode);

protected: // From FileRenameObserver

    //! @brief Confirm OverWrite Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aReadOnly Target File Is Read Only
    //! @return File Rename Observer Response
    virtual int confirmRenameOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly);

    //! @brief Confirm Source Deletion Callback
    //! @param aSource Source File Path
    //! @param aReadOnly Source File Is Read Only
    //! @param aNonEmpty Source File Is A Non Empty Directory
    //! @return File Rename Observer Response
    virtual int confirmRenameDeletion(const QString& aSource, const bool& aReadOnly, const bool& aNonEmpty);

    //! @brief Rename File Error Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return File Rename Error Code
    virtual int renameError(const QString& aSource, const QString& aTarget, const int& aErrorCode);

    //! @brief Rename File Started Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    virtual void renameStarted(const QString& aSource, const QString& aTarget);

    //! @brief Rename File Finished Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Rename Error Code
    virtual void renameFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode);

protected: // From InfoDialogProvider

    //! @brief Launch Info Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
    //! @param aType Info Type
    //! @return Dialog Result
    virtual int launchInfo(const int& aType);

    //! @brief Exit Info Dialog
    //! @param aEventLoop Event Loop Blocking Confirm Dialog Provider
    //! @param aResult Dialog Result
    virtual void exitInfo(QEventLoop* aEventLoop, const int& aResult);

protected: // From ConfirmDialogProvider

    //! @brief Launch Confirmation Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
    //! @param aType Confirmation Type
    //! @return Dialog Result
    virtual int launchConfirm(const int& aType);

    //! @brief Exit Confirmation Dialog
    //! @param aEventLoop Event Loop Blocking Confirm Dialog Provider
    //! @param aResult Dialog Result
    virtual void exitConfirm(QEventLoop* aEventLoop, const int& aResult);

protected: // From ErrorDialogProvider

    //! @brief Launch Confirmation Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
    //! @param aErrorCode Error Code
    //! @return Dialog Result
    virtual int launchError(const int& aErrorCode);

    //! @brief Exit Error Dialog
    //! @param aEventLoop Event Loop Blocking Error Dialog Provider
    //! @param aResult Dialog Result
    virtual void exitError(QEventLoop* aEventLoop, const int& aResult);

protected slots:

    //! @brief Pause Button Pressed Slot
    //! @param none
    void pauseButtonPressed();

    //! @brief Abort Button Pressed Slot
    //! @param none
    void abortButtonPressed();

    //! @brief Clear Button Pressed Slot
    //! @param none
    void clearButtonPressed();

    //! @brief Operation Queue Processing Started Slot
    //! @param none
    void opQueueStarted();

    //! @brief Operation Queue Processing Stopped Slot
    //! @param none
    void opQueueStopped();

    //! @brief Operation Queue Processing Finished Slot
    //! @param none
    void opQueueFinished();

    //! @brief Operation Added Slot
    //! @param aIndex Operation Index
    //! @param aCount New Operations Count
    void operationAdded(const int& aIndex, const int& aCount);

    //! @brief Operation Removed Slot
    //! @param aIndex Operation Index
    //! @param aCount Remaining Operations Count
    void operationRemoved(const int& aIndex, const int& aCount);

    //! @brief Operation Started Slot
    //! @param aIndex Operation Index
    void operationStarted(const int& aIndex);

    //! @brief Operation Stopped Slot
    //! @param aIndex Operation Index
    void operationStopped(const int& aIndex);

    //! @brief Operation Updated Slot
    //! @param aIndex Operation Index
    void operationUpdated(const int& aIndex);

    //! @brief Operation Completed Slot
    //! @param aIndex Operation Index
    void operationCompleted(const int& aIndex);

    //! @brief Operation Aborted Slot
    //! @param aIndex Operation Index
    void operationAborted(const int& aIndex);

protected:

    //! @brief Update Operation Item Text
    //! @param aIndex Operation Index
    void updateQueueItemText(const int& aIndex);

protected: // Data

    //! UI
    Ui::MainQueueDialog*    ui;

    //! Pause Button
    QPushButton*            pauseButton;
    //! Abort Button
    QPushButton*            abortButton;
    //! Clear Button
    QPushButton*            clearButton;

    //! Paused
    bool                    paused;
    //! Finished
    bool                    qFinished;

    //! Operation Queue
    FileOpQueueHandler*     opQueueHandler;

    //! Info Dialog
    InfoDialog*             infoDialog;
    //! Delete Confirmation Dialog
    ConfirmationDialog*     deleteConfirmDialog;
    //! OverWrite Confirmation Dialog
    ConfirmationDialog*     overWriteConfirmDialog;
    //! Error Dialog
    ConfirmationDialog*     errorDialog;

};

#endif // MAINQUEUEDIALOG_H
