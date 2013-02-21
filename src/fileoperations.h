
#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// INCLUDES

#include <QObject>

#include "constants.h"
#include "fileutils.h"


// FORWARD DECLARATIONS




// DECLARATIONS

//==============================================================================
//! @enum FileOperationState File Operation State Enum
//==============================================================================
enum FileOperationState
{
    FOSIdle         = 0x0000,
    FOSRunning,
    FOSPaused,
    FOSDone,
    FOSSkipped,
    FOSFailed,
    FOSRetry,
    FOSDeleteLater
};


//==============================================================================
//! @class FileOperationEntry File Operation Class For File Operation Queue
//==============================================================================
class FileOperationEntry : public QObject,
                           public FileDeleteObserver,
                           public FileCopyObserver,
                           public FileMoveObserver,
                           public FileRenameObserver
{
public:

    //! @brief Constructor
    //! @param aHandler File Operation Queue Handler
    //! @param aOperation Composit Operation Token
    FileOperationEntry(FileOpQueueHandler* aHandler, const QString& aOperation);

    //! @brief Constructor
    //! @param aHandler File Operation Queue Handler
    //! @param aOperation Operation Name
    //! @param aSource Operation Source Name
    //! @param aTarget Operation Target Name
    FileOperationEntry(FileOpQueueHandler* aHandler, const QString& aOperation, const QString& aSource, const QString& aTarget);

    //! @brief Constructor
    //! @param aHandler File Operation Queue Handler
    //! @param aOperation Operation Index
    //! @param aSource Operation Source Name
    //! @param aTarget Operation Target Name
    FileOperationEntry(FileOpQueueHandler* aHandler, const int& aOperation, const QString& aSource, const QString& aTarget);

    //! @brief Get Operation Index
    //! @param none
    //! @return Operation Index
    int getOperationIndex();

    //! @brief Get Operation Name
    //! @param none
    //! @return Operation Name
    QString getOperationName();

    //! @brief Get Operation Source
    //! @param none
    //! @return Operation Source
    QString getSource();

    //! @brief Get Operation Target
    //! @param none
    //! @return Operation Target
    QString getTarget();

    //! @brief Get Operation Token
    //! @param none
    //! @return Operation Token
    QString getOperation() const;

    //! @brief Get Operation State
    //! @param none
    //! @return Operation State
    FileOperationState getState();

    //! @brief Set Observers
    //! @param aDeleteObserver Delete Observer
    //! @param aCopyObserver Copy Observer
    //! @param aMoveObserver Move Observer
    //! @param aRenameObserver Rename Observer
    void setObservers(FileDeleteObserver* aDeleteObserver = NULL,
                      FileCopyObserver*   aCopyObserver = NULL,
                      FileMoveObserver*   aMoveObserver = NULL,
                      FileRenameObserver* aRenameObserver = NULL);

    //! @brief Set File Operation State By File Utils Response
    //! @param aFileUtilsResponse File Utils Response
    void setOpStateByFileUtilsResponse(const int& aFileUtilsResponse);

    //! @brief Is Entry Ready To Be Processed
    //! @param none
    //! @return Operation State
    bool isEntryReadyToBeProcessed();

    //! @brief Get Operation Progress - Current
    //! @param none
    //! @return Operation Progress - Current
    qint64 getCurrent();

    //! @brief Get Operation Progress - Total
    //! @param none
    //! @return Operation Progress - Total
    qint64 getTotal();

    //! @brief Destructor
    //! @param none
    virtual ~FileOperationEntry();

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

protected: // Data
    friend class FileOpQueueHandler;
    friend class FileOpQueueViewAPI;

    //! Queue Handler Parent
    FileOpQueueHandler*         queueHandler;

    //! Operation Queue Index
    int                         queueIndex;

    //! Operation Index
    int                         opIndex;
    //! Operation Name
    QString                     opName;
    //! Operation Source
    QString                     source;
    //! Operation Target
    QString                     target;

    //! Current Progress
    qint64                      current;
    //! Total Progress
    qint64                      total;

    //! File Operation State
    FileOperationState          opState;

    //! Delete Observer
    FileDeleteObserver*         parentDeleteObserver;
    //! Copy Observer
    FileCopyObserver*           parentCopyObserver;
    //! Move Observer
    FileMoveObserver*           parentMoveObserver;
    //! File Rename Observer
    FileRenameObserver*         parentRenameObserver;
};


#endif // FILEOPERATIONS_H
