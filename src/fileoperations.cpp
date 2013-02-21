
// INCLUDES

#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "fileoperations.h"
#include "fileutils.h"
#include "mainwindow.h"


// IMPLEMENTAITON


//==============================================================================
// Constructor
//==============================================================================
FileOperationEntry::FileOperationEntry(FileOpQueueHandler* aHandler, const QString& aOpName, const QString& aSource, const QString& aTarget)
    : QObject(NULL)
    , queueHandler(aHandler)
    , queueIndex(-1)
    , opIndex(-1)
    , opName(aOpName)
    , source(aSource)
    , target(aTarget)
    , current(0)
    , total(0)
    , opState(FOSIdle)
//    , deleteDirAfter(false)
    , parentDeleteObserver(NULL)
    , parentCopyObserver(NULL)
    , parentMoveObserver(NULL)
    , parentRenameObserver(NULL)
{
    qDebug() << "Creating FileOperationEntry - aOpName: " << aOpName;

    // Check Operation Name
    if (opName == QString(PARAM_OPERATION_COPY)) {
        // Set Operation Index
        opIndex = OPERATION_ID_COPY;
    } else if (opName == QString(PARAM_OPERATION_MOVE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MOVE;
    } else if (opName == QString(PARAM_OPERATION_RENAME)) {
        // Set Operation Index
        opIndex = OPERATION_ID_RENAME;
    } else if (opName == QString(PARAM_OPERATION_DELETE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_DELETE;
    } else if (opName == QString(PARAM_OPERATION_MAKEDIR)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MAKEDIR;
    } else {
        // Set Operation Index
        opIndex = OPERATION_ID_NOOP;
    }

    qDebug() << "Creating FileOperationEntry...done";
}

//==============================================================================
// Constructor
//==============================================================================
FileOperationEntry::FileOperationEntry(FileOpQueueHandler* aHandler, const QString& aOperation)
    : QObject(NULL)
    , queueHandler(aHandler)
    , queueIndex(-1)
    , opIndex(-1)
    , opName(QString(""))
    , source(QString(""))
    , target(QString(""))
    , current(0)
    , total(0)
    , opState(FOSIdle)
{
    qDebug() << "Creating FileOperationEntry - aOperation: " << aOperation;

    // Split Operation Token
    QStringList opList = aOperation.split(QChar(FILE_OPERATIONS_TOKEN_SEPARATOR));

    // Set Operation Name
    opName = opList[0];

    // Check Operation Name
    if (opName == QString(PARAM_OPERATION_COPY)) {
        // Set Operation Index
        opIndex = OPERATION_ID_COPY;
    } else if (opName == QString(PARAM_OPERATION_MOVE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MOVE;
    } else if (opName == QString(PARAM_OPERATION_RENAME)) {
        // Set Operation Index
        opIndex = OPERATION_ID_RENAME;
    } else if (opName == QString(PARAM_OPERATION_DELETE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_DELETE;
    } else if (opName == QString(PARAM_OPERATION_MAKEDIR)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MAKEDIR;
    } else {
        // Set Operation Index
        opIndex = OPERATION_ID_NOOP;
    }

    // Set Operation Source
    source = opList[1];
    // set Operation Target
    target = opList[2];

    qDebug() << "Creating FileOperationEntry...done";
}

//==============================================================================
// Constructor
//==============================================================================
FileOperationEntry::FileOperationEntry(FileOpQueueHandler* aHandler, const int& aOpIndex, const QString& aSource, const QString& aTarget)
    : QObject(NULL)
    , queueHandler(aHandler)
    , queueIndex(-1)
    , opIndex(aOpIndex)
    , opName(QString(""))
    , source(aSource)
    , target(aTarget)
    , current(0)
    , total(0)
    , opState(FOSIdle)
{
    qDebug() << "Creating FileOperationEntry - aOpIndex: " << aOpIndex;

    // Switch Operation Index
    switch (opIndex) {
        default:
        case OPERATION_ID_NOOP: opName = QString(PARAM_OPERATION_NOOP); break;
        case OPERATION_ID_COPY: opName = QString(PARAM_OPERATION_COPY); break;
        case OPERATION_ID_MOVE: opName = QString(PARAM_OPERATION_MOVE); break;
        case OPERATION_ID_RENAME: opName = QString(PARAM_OPERATION_RENAME); break;
        case OPERATION_ID_DELETE: opName = QString(PARAM_OPERATION_DELETE); break;
        case OPERATION_ID_MAKEDIR: opName = QString(PARAM_OPERATION_MAKEDIR); break;
    }

    qDebug() << "Creating FileOperationEntry...done";
}

//==============================================================================
// Get Operation Index
//==============================================================================
int FileOperationEntry::getOperationIndex()
{
    return opIndex;
}

//==============================================================================
// Get Operation Name
//==============================================================================
QString FileOperationEntry::getOperationName()
{
    return opName;
}

//==============================================================================
// Get Operation Source
//==============================================================================
QString FileOperationEntry::getSource()
{
    return source;
}

//==============================================================================
// Get Operation Target
//==============================================================================
QString FileOperationEntry::getTarget()
{
    return target;
}

//==============================================================================
// Get Operation Token
//==============================================================================
QString FileOperationEntry::getOperation() const
{
    return QString("%1%2%3%4%5").arg(opName).arg(FILE_OPERATIONS_TOKEN_SEPARATOR).arg(source).arg(FILE_OPERATIONS_TOKEN_SEPARATOR).arg(target);
}

//==============================================================================
// Get Operation State
//==============================================================================
FileOperationState FileOperationEntry::getState()
{
    return opState;
}

//==============================================================================
// Set Observers
//==============================================================================
void FileOperationEntry::setObservers(FileDeleteObserver* aDeleteObserver,
                                      FileCopyObserver*   aCopyObserver,
                                      FileMoveObserver*   aMoveObserver,
                                      FileRenameObserver* aRenameObserver)
{
    qDebug() << "FileOperationEntry::setObservers - do: " << aDeleteObserver << " - co: " << aCopyObserver << " - mo: " << aMoveObserver << " - ro: " << aRenameObserver;
    // Set Delete Observer
    parentDeleteObserver = aDeleteObserver;
    // Set Copy Observer
    parentCopyObserver = aCopyObserver;
    // Set Move Observer
    parentMoveObserver = aMoveObserver;
    // Set Rename Observer
    parentRenameObserver = aRenameObserver;
}

//==============================================================================
// Set File Operation State By File Utils Response
//==============================================================================
void FileOperationEntry::setOpStateByFileUtilsResponse(const int& aFileUtilsResponse)
{
    // Switch File Utils Response
    switch (aFileUtilsResponse) {
        default:
        case FILE_UTILS_RESPONSE_NOERROR:   opState = FOSDone;      break;
        case FILE_UTILS_RESPONSE_ABORT:
        case FILE_UTILS_RESPONSE_ERROR:     opState = FOSFailed;    break;
        case FILE_UTILS_RESPONSE_SKIP:      opState = FOSSkipped;   break;
    }
}

//==============================================================================
// Is Entry Ready To Be Processed
//==============================================================================
bool FileOperationEntry::isEntryReadyToBeProcessed()
{
    return (opState != FOSDone && opState != FOSFailed && opState != FOSSkipped && opState != FOSRunning);
}

//==============================================================================
// Get Operation Progress - Current
//==============================================================================
qint64 FileOperationEntry::getCurrent()
{
    return current;
}

//==============================================================================
// Get Operation Progress - Total
//==============================================================================
qint64 FileOperationEntry::getTotal()
{
    return total;
}

//==============================================================================
// Confirm Deletion Callback
//==============================================================================
int FileOperationEntry::confirmDeletion(const QString& aFilePath, const bool& aReadOnly, const bool& aNonEmpty)
{
    // Check Parent Observer
    if (parentDeleteObserver)
        return parentDeleteObserver->confirmDeletion(aFilePath, aReadOnly, aNonEmpty);

    return (int)FOORTNo;
}

//==============================================================================
// Deletion Error Callback
//==============================================================================
int FileOperationEntry::deleteError(const QString& aFilePath, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentDeleteObserver)
        parentDeleteObserver->deleteError(aFilePath, aErrorCode);

    return (int)FOORTIgnore;
}

//==============================================================================
// Delete File Started Notification Callback
//==============================================================================
void FileOperationEntry::deleteFileStarted(const QString& aFilePath)
{
    // Check Parent Observer
    if (parentDeleteObserver)
        parentDeleteObserver->deleteFileStarted(aFilePath);
}

//==============================================================================
// Delete File Finished Notification Callback
//==============================================================================
void FileOperationEntry::deleteFileFinished(const QString& aFilePath, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentDeleteObserver)
        parentDeleteObserver->deleteFileFinished(aFilePath, aErrorCode);
}

//==============================================================================
// Confirm OverWrite Callback
//==============================================================================
int FileOperationEntry::confirmCopyOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly)
{
    // Check Parent Observer
    if (parentCopyObserver)
        return parentCopyObserver->confirmCopyOverWrite(aSource, aTarget, aReadOnly);

    return (int)FOORTNo;
}

//==============================================================================
// Copy File Error Callback
//==============================================================================
int FileOperationEntry::copyError(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentCopyObserver)
        return parentCopyObserver->copyError(aSource, aTarget, aErrorCode);

    return (int)FOORTIgnore;
}

//==============================================================================
// Copy File Started Callback
//==============================================================================
void FileOperationEntry::copyStarted(const QString& aSource, const QString& aTarget)
{
    // Check Parent Observer
    if (parentCopyObserver)
        parentCopyObserver->copyStarted(aSource, aTarget);
}

//==============================================================================
// Copy File Progress Callback
//==============================================================================
void FileOperationEntry::copyProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize)
{
    // Set Current Progress
    current = aCurrentSize;
    // Set Total Progress
    total = aFullSize;

    // Check Parent Queue Handler
    if (queueHandler) {
        // Notify Queue Handler
        queueHandler->notifyEntryUpdated(queueIndex);
    }

    // Check Parent Observer
    if (parentCopyObserver) {
        parentCopyObserver->copyProgress(aSource, aTarget, aCurrentSize, aFullSize);
    }
}

//==============================================================================
// Copy File Finished Callback
//==============================================================================
void FileOperationEntry::copyFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentCopyObserver)
        parentCopyObserver->copyFinished(aSource, aTarget, aErrorCode);
}

//==============================================================================
// Confirm OverWrite Callback
//==============================================================================
int FileOperationEntry::confirmMoveOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly)
{
    // Check Parent Observer
    if (parentMoveObserver)
        return parentMoveObserver->confirmMoveOverWrite(aSource, aTarget, aReadOnly);

    return (int)FOORTNo;
}

//==============================================================================
// Confirm Source Deletion Callback
//==============================================================================
int FileOperationEntry::confirmMoveDeletion(const QString& aSource, const bool& aReadOnly, const bool& aNonEmpty)
{
    // Check Parent Observer
    if (parentMoveObserver)
        return parentMoveObserver->confirmMoveDeletion(aSource, aReadOnly, aNonEmpty);

    return (int)FOORTNo;
}

//==============================================================================
// Rename/Move File Error Callback
//==============================================================================
int FileOperationEntry::moveError(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentMoveObserver)
        return parentMoveObserver->moveError(aSource, aTarget, aErrorCode);

    return (int)FOORTIgnore;
}

//==============================================================================
// Rename/Move File Started Callback
//==============================================================================
void FileOperationEntry::moveStarted(const QString& aSource, const QString& aTarget)
{
    // Check Parent Observer
    if (parentMoveObserver)
        parentMoveObserver->moveStarted(aSource, aTarget);
}

//==============================================================================
// Renamve/Move File Progress Callback
//==============================================================================
void FileOperationEntry::moveProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize)
{
    // Set Current Progress
    current = aCurrentSize;
    // Set Total Progress
    total = aFullSize;

    // Check Parent Queue Handler
    if (queueHandler) {
        // Notify Queue Handler
        queueHandler->notifyEntryUpdated(queueIndex);
    }

    // Check Parent Observer
    if (parentMoveObserver) {
        parentMoveObserver->moveProgress(aSource, aTarget, aCurrentSize, aFullSize);
    }
}

//==============================================================================
// Rename/Move File Finished Callback
//==============================================================================
void FileOperationEntry::moveFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentMoveObserver)
        parentMoveObserver->moveFinished(aSource, aTarget, aErrorCode);
}

//==============================================================================
// Confirm OverWrite Callback
//==============================================================================
int FileOperationEntry::confirmRenameOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly)
{
    // Check Parent Observer
    if (parentRenameObserver)
        return parentRenameObserver->confirmRenameOverWrite(aSource, aTarget, aReadOnly);

    return (int)FOORTNo;
}

//==============================================================================
// Confirm Source Deletion Callback
//==============================================================================
int FileOperationEntry::confirmRenameDeletion(const QString& aSource, const bool& aReadOnly, const bool& aNonEmpty)
{
    // Check Parent Observer
    if (parentRenameObserver)
        parentRenameObserver->confirmRenameDeletion(aSource, aReadOnly, aNonEmpty);

    return (int)FOORTNo;
}

//==============================================================================
// Rename File Error Callback
//==============================================================================
int FileOperationEntry::renameError(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentRenameObserver)
        return parentRenameObserver->renameError(aSource, aTarget, aErrorCode);

    return (int)FOORTIgnore;
}

//==============================================================================
// Rename File Started Callback
//==============================================================================
void FileOperationEntry::renameStarted(const QString& aSource, const QString& aTarget)
{
    // Check Parent Observer
    if (parentRenameObserver)
        parentRenameObserver->renameStarted(aSource, aTarget);
}

//==============================================================================
// Rename File Finished Callback
//==============================================================================
void FileOperationEntry::renameFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode)
{
    // Check Parent Observer
    if (parentRenameObserver)
        parentRenameObserver->renameFinished(aSource, aTarget, aErrorCode);
}

//==============================================================================
// Destructor
//==============================================================================
FileOperationEntry::~FileOperationEntry()
{
    // ...

    //qDebug() << "Deleting FileOperationEntry...done";
}






