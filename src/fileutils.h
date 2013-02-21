#ifndef FILEUTILS_H
#define FILEUTILS_H

// INCLUDES

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDateTime>
#include <QDir>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QBuffer>
#include <QDataStream>
#include <QTimerEvent>
#include <QPixmap>
#include <QStringList>
#include <QEventLoop>
#include <QLocalServer>
#include <QLocalSocket>

#if defined (Q_OS_UNIX)

#include <errno.h>

#if defined (Q_OS_MAC)

#include <Carbon/Carbon.h>

#endif // Q_OS_MAC

#else // Q_OS_UNIX

#endif // Q_OS_UNIX

// CONSTANTS/DEFINES

//! Default Abort Check For Thread
#define DEFAULT_THREAD_ABORT_CHECK                  if (abort || restart) return
//! Default Abort Check For Thread Returning False
#define DEFAULT_THREAD_ABORT_CHECK_FALSE            if (abort || restart) return false
//! Default Abort Check For Functions
#define DEFAULT_FUNC_ABORT_CHECK                    if (aAbortSig) return


//! File Delete Options - Delete All Normal Files Without Confirmation
#define FILE_DELETE_OPTION_DELETE_NORMAL            0x0001
//! File Delete Options - Delete All Read Only Files Without Any Further Confirmation
#define FILE_DELETE_OPTION_DELETE_READONLY          0x0003

//! File Delete Options - Skip All Files
#define FILE_DELETE_OPTION_DELETE_SKIP_NORMAL       0x0010
//! File Delete Options - Skip All Read Only Files
#define FILE_DELETE_OPTION_DELETE_SKIP_READONLY     0x0020

//! File Delete Options - Ignore Deletion Error
#define FILE_DELETE_OPTION_IGNORE_DELETE_ERROR      0x0100
//! File Delete Options - Ignore All Deletion Error
#define FILE_DELETE_OPTION_IGNORE_ALL_DELETE_ERROR  0x0300

//! File Delete Options - Delete Non Empty Directories Without Any Further Confirmation
#define FILE_DELETE_OPTION_DELETE_NON_EMPTY_DIR     0x1000
//! File Delete Options - Delete Non Empty Directories Without Any Further Confirmation
#define FILE_DELETE_OPTION_SKIP_NON_EMPTY_DIR       0x3000


//! File Copy Options - Overwrite All Normal Files Without Confirmation
#define FILE_COPY_OPTION_OVERWRITE_NORMAL           0x0001
//! File Copy Options - Overwrite All Read Only Files Without Any Further Confirmation
#define FILE_COPY_OPTION_OVERWRITE_READONLY         0x0003

//! File Copy Options - Skip Overwrite All Files
#define FILE_COPY_OPTION_OVERWRITE_SKIP_NORMAL      0x0010
//! File Copy Options - Skip Overwrite Read Only Files
#define FILE_COPY_OPTION_OVERWRITE_SKIP_READONLY    0x0020


//! File Copy Options - Ignore Read Error
#define FILE_COPY_OPTION_IGNORE_READ_ERROR          0x0010
//! File Copy Options - Ignore All Read Errors
#define FILE_COPY_OPTION_IGNORE_ALL_READ_ERROR      0x0030
//! File Copy Options - Ignore Write Error
#define FILE_COPY_OPTION_IGNORE_WRITE_ERROR         0x0100
//! File Copy Options - Ignore All Write Errors
#define FILE_COPY_OPTION_IGNORE_WRITE_ALL_ERROR     0x0300




//! File Move Options - Overwrite All Normal Files Without Confirmation
#define FILE_MOVE_OPTION_OVERWRITE_NORMAL           0x0001
//! File Move Options - Overwrite All Read Only Files Without Any Further Confirmation
#define FILE_MOVE_OPTION_OVERWRITE_READONLY         0x0003

//! File Move Options - Delete Read Only Source
#define FILE_MOVE_OPTION_DELETE_READONLY_SOURCE     0x0004

//! File Move Options - Ignore Read Error
#define FILE_MOVE_OPTION_IGNORE_READ_ERROR          0x0010
//! File Move Options - Ignore All Read Errors
#define FILE_MOVE_OPTION_IGNORE_ALL_READ_ERROR      0x0030
//! File Move Options - Ignore Write Error
#define FILE_MOVE_OPTION_IGNORE_WRITE_ERROR         0x0100
//! File Move Options - Ignore All Write Errors
#define FILE_MOVE_OPTION_IGNORE_WRITE_ALL_ERROR     0x0300
//! File Move Options - Ignore Delete Error
#define FILE_MOVE_OPTION_IGNORE_DELETE_ERROR        0x1000
//! File Move Options - Ignore All Delete Errors
#define FILE_MOVE_OPTION_IGNORE_ALL_DELETE_ERROR    0x3000


//! File Operation Option - Admin Mode
#define FILE_OPERATION_OPTION_ADMIN                 0xFF00


//! File Utils Response - No Error
#define FILE_UTILS_RESPONSE_NOERROR                 0x0000
//! File Utils Response - Error
#define FILE_UTILS_RESPONSE_ERROR                   0x0001
//! File Utils Response - Skip
#define FILE_UTILS_RESPONSE_SKIP                    0x0002
//! File Utils Response - Abort
#define FILE_UTILS_RESPONSE_ABORT                   0x0004



// FORWARD DECLARATIONS

class FileOpQueueViewAPI;
class FileOpQueueHandler;
class FileOperationEntry;
class RemoteFileUtils;



// DECLARATIONS

//! @brief File Name Case InSensitive Compare
//! @param a File Name
//! @param b File Name
int fnstricmp(const QString& a, const QString& b);


//==============================================================================
//! @enum FileSortType File Sort Type Enum
//==============================================================================
enum FileSortType
{
    EFSTName        = 0x0000,
    EFSTExtension   = 0x0001,
    EFSTSize        = 0x0002,
    EFSTDate        = 0x0004,
    EFSTOwnership   = 0x0008,
    EFSTPermission  = 0x0010,
    EFSTAttributes  = 0x0020
};

//==============================================================================
//! @enum DriveType Drive Type Enum
//==============================================================================
enum DriveType
{
    DTUnknown       = 0x0000,
    DTNoRoot,
    DTRemoveable,
    DTFixed,
    DTRemote,
    DTCDRom,
    DTRamDisk
};


//==============================================================================
//! @enum FOORespType File Operation Observer Response Type Enum
//==============================================================================
enum FOORespType
{
    FOORTNo         = 0,
    FOORTYes,
    FOORTYesToAll,
    FOORTNoToAll,
    FOORTSkip,
    FOORTSkipAll,
    FOORTAbort,
    FOORTCancel,
    FOORTIgnore,
    FOORTIgnoreAll,
    FOORTRetry,
    FOORTRename,
    FOORTAsRoot
};




//==============================================================================
//! @typedef SortingMethod File Sort Algo Function Type
//==============================================================================
typedef int (*SortingMethod)(const QFileInfo&, const QFileInfo&, const bool&);




//==============================================================================
//! @class AdminPassQueryProvider Admin Password Query Provider Interface Class
//==============================================================================
class AdminPassQueryProvider
{
public:

    //! @brief Launch Admin Password Query Dialog
    //! @param none
    //! @return Admin Password
    virtual QString launchAdminPassQuery() = 0;
};




//==============================================================================
//! @class InfoDialogProvider Information Dialog Provider Interface Class
// To be able to launch Dialogs from different thread context
// Have to Emit a signal to Dialog Launcher To Call launchInfo.
// Caller Thread Must be blocked by an QEventLoop, which need to be exited by
// calling exitConfirm by Launcher
//==============================================================================
class InfoDialogProvider
{
public:

    //! @brief Launch Info Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
    //! @param aType Info Type
    //! @return Dialog Result
    virtual int launchInfo(const int& aType) = 0;

    //! @brief Exit Info Dialog
    //! @param aEventLoop Event Loop Blocking Confirm Dialog Provider
    //! @param aResult Dialog Result
    virtual void exitInfo(QEventLoop* aEventLoop, const int& aResult) = 0;
};






//==============================================================================
//! @enum ConfirmationType Confirmation Type Enum
//==============================================================================
enum ConfirmationType
{
    ECTDelete               = 0,
    ECTDeleteReadOnly,
    ECTDeleteNonEmpty,
    ECTOverWrite,
    ECTOverWriteReadOnly
};





//==============================================================================
//! @class ConfirmDialogProvider Confirmation Dialog Provider Interface Class
// To be able to launch Dialogs from different thread context
// Have to Emit a signal to Dialog Launcher To Call launchConfirm.
// Caller Thread Must be blocked by an QEventLoop, which need to be exited by
// calling exitConfirm by Launcher
//==============================================================================
class ConfirmDialogProvider
{
public:

    //! @brief Launch Confirmation Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
    //! @param aType Confirmation Type
    //! @return Dialog Result
    virtual int launchConfirm(const int& aType) = 0;

    //! @brief Exit Confirmation Dialog
    //! @param aEventLoop Event Loop Blocking Confirm Dialog Provider
    //! @param aResult Dialog Result
    virtual void exitConfirm(QEventLoop* aEventLoop, const int& aResult) = 0;
};








//==============================================================================
//! @class ErrorDialogProvider Error Dialog Provider Interface Class
// To be able to launch Dialogs from different thread context
// Have to Emit a signal to Dialog Launcher To Call launchError.
// Caller Thread Must be blocked by an QEventLoop, which need to be exited by
// calling exitError by Launcher
//==============================================================================
class ErrorDialogProvider
{
public:

    //! @brief Launch Confirmation Dialog - MUST BE CALLED FROM GUI THREAD CONTEXT
    //! @param aErrorCode Error Code
    //! @return Dialog Result
    virtual int launchError(const int& aErrorCode) = 0;

    //! @brief Exit Error Dialog
    //! @param aEventLoop Event Loop Blocking Error Dialog Provider
    //! @param aResult Dialog Result
    virtual void exitError(QEventLoop* aEventLoop, const int& aResult) = 0;
};






//==============================================================================
//! @class DirReaderObserver Dir Reader UI Observer Interface Class
//==============================================================================
class DirReaderObserver
{
public:

    //! @brief Read Dir Error Callback
    //! @param aDirPath Directory Path
    //! @param aErrorCode Error Code
    //! @return Directory Read Error Response
    virtual int readDirError(const QString& aDirPath, const int& aErrorCode) = 0;

    //! @brief Read Dir Started
    //! @param aDirPath Directory Path
    virtual void readDirStarted(const QString& aDirPath) = 0;

    //! @brief Read Dir Entry Found
    //! @param aFilePath Directory Entry File Path
    virtual void readDirEntryFount(const QString& aFilePath) = 0;

    //! @brief Read Dir Finished
    //! @param aDirPath Directory Path
    //! @param aErrorCode Directory Read Error Code
    virtual void readDirFinished(const QString& aDirPath, const int& aErrorCode) = 0;
};








//==============================================================================
//! @class DirScannerObserver Dir Size Scanner UI Observer Interface Class
//==============================================================================
class DirScannerObserver
{
public:

    //! @brief Read Dir Error Callback
    //! @param aDirPath Directory Path
    //! @param aErrorCode Error Code
    //! @return Directory Read Error Response
    virtual int scanDirError(const QString& aDirPath, const int& aErrorCode) = 0;

    //! @brief Read Dir Started
    //! @param aDirPath Directory Path
    virtual void scanDirStarted(const QString& aDirPath) = 0;

    //! @brief Read Dir Entry Found
    //! @param aFilePath Directory Entry File Path
    //! @param aTotalSize Directory Size
    virtual void scanDirSizeUpdated(const QString& aFilePath, const quint64& aTotalSize) = 0;

    //! @brief Read Dir Finished
    //! @param aDirPath Directory Path
    //! @param aErrorCode Directory Read Error Code
    virtual void scanDirFinished(const QString& aDirPath, const int& aErrorCode) = 0;
};







//==============================================================================
//! @class DirCreatorObserver Dir Creator UI Observer Interface Class
//==============================================================================
class DirCreatorObserver
{
public:
    //! @brief Create Dir Error Callback
    //! @param aDirPath Directory Path
    //! @param aErrorCode Error Code
    //! @return Creation Error Response
    virtual int createDirError(const QString& aDirPath, const int& aErrorCode) = 0;

    //! @brief Create Dir Started
    //! @param aDirPath Directory Path
    virtual void createDirStarted(const QString& aDirPath) = 0;

    //! @brief Create Dir Finished
    //! @param aDirPath Directory Path
    //! @param aErrorCode Directory Creation Error Code
    virtual void createDirFinished(const QString& aDirPath, const int& aErrorCode) = 0;

    //! @brief Get Admin Password Query Provider - Observer/Owner Has To Delete After Finished!!
    //! @param none
    //! @return Admin Password Query Provider
    virtual AdminPassQueryProvider* passQueryProvider() = 0;

    //! @brief Get A Connected File Utils Client - Observer/Owner Has To Delete After Finished!!
    //! @param aAdminPass Admin Password
    //! @return Connected File Utils Client
    virtual RemoteFileUtils* getUtilsClient(const QString& aAdminPass = QString("")) = 0;
};










//==============================================================================
//! @class FileDeleteObserver File Deletion UI Observer Interface Class
//==============================================================================
class FileDeleteObserver
{
public:
    //! @brief Confirm Deletion Callback
    //! @param aFilePath File Path
    //! @param aReadOnly File Read Only
    //! @param aNonEmpty Directory Empty
    //! @return File Delete Observer Delete Confirmation Response
    virtual int confirmDeletion(const QString& aFilePath, const bool& aReadOnly = false, const bool& aNonEmpty = false) = 0;

    //! @brief Deletion Error Callback
    //! @param aFilePath File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return Deletion Error Code Response
    virtual int deleteError(const QString& aFilePath, const int& aErrorCode) = 0;

    //! @brief Delete File Started Notification
    //! @param aFilePath File Path
    virtual void deleteFileStarted(const QString& aFilePath) = 0;

    //! @brief Delete File Finished Notification
    //! @param aFilePath File Path
    //! @param aErrorCode File Deletion Error Code
    virtual void deleteFileFinished(const QString& aFilePath, const int& aErrorCode) = 0;
};








//==============================================================================
//! @class FileCopyObserver File Copy UI Observer Interface Class
//==============================================================================
class FileCopyObserver
{
public:
    //! @brief Confirm OverWrite Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aReadOnly Target File Is Read Only
    //! @return File Copy Observer Response
    virtual int confirmCopyOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly = false) = 0;

    //! @brief Copy File Error Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return File Copy Error Code
    virtual int copyError(const QString& aSource, const QString& aTarget, const int& aErrorCode) = 0;

    //! @brief Copy File Started Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    virtual void copyStarted(const QString& aSource, const QString& aTarget) = 0;

    //! @brief Copy File Progress Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aCurrentSize Copy File Current Size/Progress
    //! @param aFullSize Copy File Full Size
    virtual void copyProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize) = 0;

    //! @brief Copy File Finished Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Copy Error Code
    virtual void copyFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode) = 0;
};







//==============================================================================
//! @class FileMoveObserver File Move UI Observer Interface Class
//==============================================================================
class FileMoveObserver
{
public:
    //! @brief Confirm OverWrite Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aReadOnly Target File Is Read Only
    //! @return File Rename/Move Observer Response
    virtual int confirmMoveOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly = false) = 0;

    //! @brief Confirm Source Deletion Callback
    //! @param aSource Source File Path
    //! @param aReadOnly Source File Is Read Only
    //! @param aNonEmpty Source File Is A Non Empty Directory
    //! @return File Rename/Move Observer Response
    virtual int confirmMoveDeletion(const QString& aSource, const bool& aReadOnly = false, const bool& aNonEmpty = false) = 0;

    //! @brief Rename/Move File Error Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return File Rename/Move Error Code
    virtual int moveError(const QString& aSource, const QString& aTarget, const int& aErrorCode) = 0;

    //! @brief Rename/Move File Started Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    virtual void moveStarted(const QString& aSource, const QString& aTarget) = 0;

    //! @brief Renamve/Move File Progress Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aCurrentSize Move File Current Size/Progress
    //! @param aFullSize Move File Full Size
    virtual void moveProgress(const QString& aSource, const QString& aTarget, const qint64& aCurrentSize, const qint64& aFullSize) = 0;

    //! @brief Rename/Move File Finished Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Move Error Code
    virtual void moveFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode) = 0;
};















//==============================================================================
//! @class FileRenameObserver File Rename UI Observer Interface Class
//==============================================================================
class FileRenameObserver
{
public:
    //! @brief Confirm Rename OverWrite Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aReadOnly Target File Is Read Only
    //! @return File Rename Observer Response
    virtual int confirmRenameOverWrite(const QString& aSource, const QString& aTarget, const bool& aReadOnly = false) = 0;

    //! @brief Confirm Source Deletion Callback
    //! @param aSource Source File Path
    //! @param aReadOnly Source File Is Read Only
    //! @param aNonEmpty Source File Is A Non Empty Directory
    //! @return File Rename Observer Response
    virtual int confirmRenameDeletion(const QString& aSource, const bool& aReadOnly = false, const bool& aNonEmpty = false) = 0;

    //! @brief Rename File Error Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Deletion Error Code
    //! @return File Rename Error Code
    virtual int renameError(const QString& aSource, const QString& aTarget, const int& aErrorCode) = 0;

    //! @brief Rename File Started Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    virtual void renameStarted(const QString& aSource, const QString& aTarget) = 0;

    //! @brief Rename File Finished Callback
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    //! @param aErrorCode File Rename Error Code
    virtual void renameFinished(const QString& aSource, const QString& aTarget, const int& aErrorCode) = 0;
};















//==============================================================================
//! @class FileUtils File Utils Class
//==============================================================================
class FileUtils : public QObject
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    FileUtils(QObject* aParent = NULL);

    //! @brief Get Dir Empty
    //! @param aDirPath Directory Path
    //! @return Dir Empty State
    static bool isDirEmpty(const QString& aDirPath);

    //! @brief Get File Size
    //! @param aFilePath File Path
    //! @return File Size
    qint64 getFileSize(const QString& aFilePath);

    //! @brief Get Directory Size
    //! @param aDirPath Directory Path
    //! @param aDirSize Directory Size In/Out Parameter
    //! @param aAbort Abort Flag
    //! @return Directory Size
    qint64 getDirSize(const QString& aDirPath, qint64& aDirSize, bool& aAbort);

    //! @brief Create Directory
    //! @param aDirName Directory Path
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Directory Create Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Successful
    static int createDir(const QString& aDirName, int& aOptions, bool& aAbortSig, DirCreatorObserver* aObserver = NULL);

    //! @brief Read Directory
    //! @param aDirName Directory Path
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Directory Read Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Started Successful
    static int readDir(const QString& aDirName, int& aOptions, bool& aAbortSig, DirReaderObserver* aObserver = NULL);

    //! @brief Scan Directory For Size
    //! @param aDirName Directory Path
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Directory Scan Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Started Successful
    static int scanDir(const QString& aDirName, int& aOptions, bool& aAbortSig, DirScannerObserver* aObserver = NULL);

    //! @brief Delete File
    //! @param aFileName File Name
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Delete Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Successful
    static int deleteFile(const QString& aFileName, int& aOptions, bool& aAbortSig, FileDeleteObserver* aObserver = NULL);

    //! @brief Copy File
    //! @param aSource Source File Name
    //! @param aTarget Target File Name
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Copy Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Successful
    static int copyFile(const QString& aSource, const QString& aTarget, int& aOptions, bool& aAbortSig, FileCopyObserver* aObserver = NULL);

    //! @brief Rename File
    //! @param aSource Source File Name
    //! @param aTarget Target File Name
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Rename Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Successful
    static int renameFile(const QString& aSource, const QString& aTarget, int& aOptions, bool& aAbortSig, FileRenameObserver* aObserver = NULL);

    //! @brief Move File
    //! @param aSource Source File Name
    //! @param aTarget Target File Name
    //! @param aOptions Options/Flags
    //! @param aAbortSig Abort Signal Flag
    //! @param aObserver Move Observer Interface
    //! @return FILE_UTILS_RESPONSE_NOERROR If Successful
    static int moveFile(const QString& aSource, const QString& aTarget, int& aOptions, bool& aAbortSig, FileMoveObserver* aObserver = NULL);

    //! @brief Set File Attributes
    //! @param aFilePath File Name
    //! @param aAttributes Attributes
    //! @return true If Successful
    static bool setFileAttributes(const QString& aFilePath, const int& aAttributes);

    //! @brief Get File Attribute
    //! @param aFilePath File Name
    //! @return File Attributes
    static int getFileAttributes(const QString& aFilePath);

    //! @brief Set File Premissions
    //! @param aFilePath File Name
    //! @param aPermissions Permissions
    //! @return true If Successful
    static bool setPermissions(const QString& aFilePath, const QFile::Permissions& aPermissions);

    //! @brief Get File Permissions
    //! @param aFilePath File Name
    //! @return File Premissions
    static QFile::Permissions permissions(const QString& aFilePath);

    //! @brief Get File Owner
    //! @param aFilePath File Name
    //! @return File Owner
    static QString getOwner(const QString& aFilePath);

    //! @brief Set File Owner
    //! @param aFilePath File Name
    //! @param aOwner File Owner
    //! @return true If Successfule, false Otherwise
    static bool setOwner(const QString& aFilePath, const QString& aOwner);

    //! @brief Set File Last Modified Date
    //! @param aFilePath File Name
    //! @param aDate Modified Date
    //! @return true If Successful
    static bool setLastModified(const QString& aFilePath, const QDateTime& aDate);

    //! @brief Get File Last Modified Date
    //! @param aFilePath File Name
    //! @return File Last Modified Date
    static QDateTime getLastModified(const QString& aFilePath);

    //! @brief Get Directory File List
    //! @param aDirName Directory Name
    //! @param aSortType Sorting Type
    //! @param aHidden Show Hidden
    //! @param aReverse Reverse Order
    //! @param aFilters Filters
    QFileInfoList getDirList(const QString& aDirName, const FileSortType& aSortType, const bool& aHidden = true, const bool& aReverse = false, const QStringList& aFilters = QStringList());

    //! @brief Get Dirve Type
    //! @param aDriveIndex Drive Index
    //! @return Dirve Type
    static int getDriveType(const int& aDriveIndex);

    //! @brief Get File Icon Pixmap
    //! @param aInfo File Info
    //! @param aWidth Requested Width
    //! @param aHeight Requested Height
    //! @return File Icon Pixmap
    static QPixmap getFileIconPixmap(const QFileInfo& aInfo, const int& aWidth, const int& aHeight);

#if defined(Q_OS_MAC)

    //! @brief Get Mac CG Context
    //! @param aPaintDevice Paint Device
    //! @return CG Context
    static CGContextRef getMacCGContext(const QPaintDevice* aPaintDevice);

    //! @brief Get/Convert Mac Icon To QImage
    //! @param aMacIconref Mac Icon Ref
    //! @param aIconImage Image In/Out Param
    //! @return a QImage
    static OSStatus convertMacIcon(const IconRef& aMacIconref, QImage& aIconImage);

#elif defined(Q_OS_WIN)

    static convertWinIcon(... , QImage& aIconImage);

#else // Q_OS_UNIX

    static convertLinuxIcon(... , QImage& aIconImage);

#endif // Q_OS_UNIX

    //! @brief Get File Icon Pixmap
    //! @param aInfo File Info
    //! @param aWidth Requested Width
    //! @param aHeight Requested Height
    //! @return File Icon Pixmap
    static QImage getFileIconImage(const QFileInfo& aInfo, const int& aWidth, const int& aHeight);

    //! @brief Get Parent Dir Path
    //! @param aDirPath Dir Path
    //! @return Parent Dir Path
    static QString getParentDirPath(const QString& aDirPath);

    //! @brief Get Parent Dir Name
    //! @param aDirPath Dir Path
    //! @return Parent Dir Name
    static QString getParentDirName(const QString& aDirPath);

    //! @brief Get Dir Name
    //! @param aDirPath Dir Path
    //! @return Dir Name
    static QString getDirName(const QString& aDirPath);

    //! @brief Get File Directory/Path
    //! @param aFilePath File Path
    //! @return File Path
    static QString getFilePath(const QString& aFilePath);

    //! @brief Format File Size
    //! @param aInfo File Info
    //! @return Formatted File Size
    static QString formatFileSize(const QFileInfo& aInfo);

    //! @brief Format Size
    //! @param aSize Size
    //! @return Formatted Size String
    static QString formatSize(const qint64& aSize);

    //! @brief Format File Permissions
    //! @param aInfo File Permissions
    //! @return Formatted File Permissions String
    static QString formatPermissions(const QFileInfo& aInfo);

    //! @brief Get Total Space In Directory
    //! @param aDirPath Directory Path
    //! @return Total Space in Directory
    static qint64 getTotalSpace(const QString& aDirPath);

    //! @brief Get Free Space In Directory
    //! @param aDirPath Directory Path
    //! @return Free Space in Directory
    static qint64 getFreeSpace(const QString& aDirPath);

    //! @brief Get Last Error
    //! @param none
    //! @return Last Error Code
    static int getLastError();

    //! @brief Wild Card Matching
    //! @param aSource Source String
    //! @param aPattern Matching Pattern
    //! @return Parsed Wild Card Matching String
    static QString wildCardMatching(const QString& aSource, const QString& aPattern);

    //! @brief Parse Target File Name
    //! @param aSourceName Source Name
    //! @param aTargetName Target Name/Pattern
    //! @return Parsed File Name
    static QString parseTargetFileName(const QString& aSourceName, const QString& aTargetName);

    //! @brief Create File Operation Entry
    //! @param aQueueHandler Operation Queue Handler
    //! @param aOperation Operation Index
    //! @param aSourceDir Source Directory
    //! @param aSourceName Source Name
    //! @param aTargetDir Target Directory
    //! @param aTargetName Target Name/Pattern
    //! @return New File OperationEntry
    static FileOperationEntry* createFileOperationEntry(FileOpQueueHandler* aQueueHandler,
                                                        const int& aOperation,
                                                        const QString& aSourceDir,
                                                        const QString& aSourceName,
                                                        const QString& aTargetDir,
                                                        const QString& aTargetName);

    //! @brief Is Full Path
    //! @param aFilePath File Path
    //! @return true If It's A Full Path
    static bool isFullPath(const QString& aFilePath);
/*
    //! @brief Get Authorization
    //! @param none
    //! @return true If Authorization Successful, false Otherwise
    static bool getAuthorization();

    //! @brief Release Authorization
    //! @param none
    static void releaseAuthorization();
*/
    //! @brief Destructor
    //! @param none
    virtual ~FileUtils();

protected:

    //! @brief Sort File Info List
    //! @param aFileList File Info List
    //! @param aSortType File Sorting Method
    //! @param aReverse Reverse Sorting Order
    void sortFileList(QFileInfoList& aFileList, const FileSortType& aSortType, const bool& aReverse);



};




















//==============================================================================
//! @class FileUtilThreadBase File Async Thread Base
//==============================================================================
class FileUtilThreadBase : public QThread
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    FileUtilThreadBase(QObject* aParent = NULL);

    //! @brief Start Operation
    //! @param none
    virtual void startOperation();

    //! @brief Restart Operation
    //! @param none
    virtual void restartOperation();

    //! @brief Pause
    //! @param none
    virtual void pause();

    //! @brief Resume
    //! @param none
    virtual void resume();

    //! @brief Stop
    //! @param none
    virtual void stop();

    //! @brief Is Stopping
    //! @param none
    //! @return true if Aborting/Stopping, false Otherwise
    virtual bool stopping();

    //! @brief Destructor
    //! @param none
    virtual ~FileUtilThreadBase();

signals:

    //! @brief Operation Started Signal
    //! @param none
    void opStarted();

    //! @brief Operation Stopped Signal
    //! @param none
    void opStopped();

    //! @brief Operation Finished Signal
    //! @param none
    void opFinished();

protected:

    //! @brief Do Operation
    //! @param none
    virtual void doOperation() = 0;

protected: // From QThread

    //! @brief Run
    //! @param none
    virtual void run();

protected: // Data

    //! Mutex
    QMutex          mutex;
    //! Wait condition
    QWaitCondition  condition;
    //! Internal Running State
    bool            running;
    //! Restart
    bool            restart;
    //! Abort
    bool            abort;
};


















//==============================================================================
//! @class DirReader Directory Reader Thread
//==============================================================================
class DirReader : public FileUtilThreadBase
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    DirReader(QObject* aParent = NULL);

    //! @brief Is Dir Readable
    //! @param aDirPath Dir Path
    //! @return true if Dir Is Readable, false Otherwise
    bool isReadable(const QString& aDirPath);

    //! @brief Read Dir
    //! @param aDirPath Dir Path
    //! @param aSortType Sort Type
    //! @param aReverse Reverse Order
    //! @param aHidden Show Hidden
    //! @param aNameFilters Name Filters
    void readDir(const QString& aDirPath, const FileSortType& aSortType, const bool& aReverse, const bool& aHidden, const QStringList& aNameFilters);

    //! @brief Destructor
    //! @param none
    virtual ~DirReader();

signals:

    //! @brief Entry Found Signal
    //! @param aFilePath File Item/Entry Info
    void entryFound(const QString& aFilePath);

protected: // From FileUtilThreadBase

    //! @brief Do Operation
    //! @param none
    virtual void doOperation();

protected: // Data

    //! Dir Path
    QString         dirPath;
    //! Sort Order
    FileSortType    sortType;
    //! Reverse Order
    bool            reversed;
    //! Show Hidden
    bool            showHidden;
    //! Name Filters
    QStringList     nameFilters;
};



















//==============================================================================
//! @class DirSizeScanner Directory Size Scanner Thread
//==============================================================================
class DirSizeScanner : public FileUtilThreadBase
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    DirSizeScanner(QObject* aParent = NULL);

    //! @brief Scan Dir Size
    //! @param aDirPath Dir Path
    void scanDirSize(const QString& aDirPath);

    //! @brief Get Dir Size
    //! @param none
    //! @return Dir Size
    qint64 getDirSize();

    //! @brief Stop
    //! @param none
    virtual void stop();

    //! @brief Destructor
    //! @param none
    virtual ~DirSizeScanner();

signals:

    //! @brief Size Update Signal
    //! @param aSize Dir Size
    void sizeUpdate(const qint64& aSize);

protected:

    //! @brief Start Update Timer
    //! @param none
    void startUpdateTimer();

    //! @brief Stop Update Timer
    //! @param none
    void stopUpdateTimer();

    //! @brief Notify Size Changed
    //! @param none
    void notifySizeChanged();

protected: // From FileUtilThreadBase

    //! @brief Do Operation
    //! @param none
    virtual void doOperation();

protected: // From QOject

    //! @brief Timer Event
    //! @param aEvent Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

protected: // Data

    //! Dir Path
    QString         dirPath;
    //! Previous Dir Size
    qint64          prevDirSize;
    //! Dir Size
    qint64          dirSize;
    //! Update Timer ID
    int             updateTimerID;
    //! File Util
    FileUtils       fileUtils;
};








































//==============================================================================
//! @class FileOpQueueViewAPI File Operation Queue Handler Interface Class
//==============================================================================
class FileOpQueueViewAPI
{
public:

    //! @brief Add Operation
    //! @param aEntry Operation Entry
    virtual void addOperationEntry(FileOperationEntry* aEntry) = 0;

    //! @brief Remove Operation
    //! @param aIndex Operation Index
    virtual void removeOperationEntry(const int& aIndex) = 0;

    //! @brief Get Operations Count
    //! @param none
    //! @return Operations Count
    virtual int opEntryCount() = 0;

    //! @brief Get Operation
    //! @param aIndex Operation Index
    //! @return Operation
    virtual FileOperationEntry* getOperationEntry(const int& aIndex) = 0;

    //! @brief Process Queue
    //! @param none
    virtual void processQueue() = 0;

    //! @brief Show
    //! @param none
    virtual void show() = 0;

    //! @brief Hide
    //! @param none
    virtual void hide() = 0;

    //! @brief Set Modal
    //! @param aModal Modal Setting
    virtual void setModal(const bool& aModal) = 0;

    //! @brief Operation Entry Added Callback - SIGNALS DON'T WORK
    //! @param aIndex Inserted Index
    //! @param aCount Count
    virtual void operationEntryAdded(const int& aIndex, const int& aCount) = 0;

    //! @brief Operation Entry Removed Callback - SIGNALS DON'T WORK
    //! @param aIndex Removed Index
    //! @param aCount Count
    virtual void operationEntryRemoved(const int& aIndex, const int& aCount) = 0;

    //! @brief Operation Entry Updated Callback - SIGNALS DON'T WORK
    //! @param aIndex Updated Item Index
    virtual void operationEntryUpdated(const int& aIndex) = 0;

    //! @brief Get File Operations Queue Handler
    //! @param none
    //! @return File Operations Queue Handler
    virtual FileOpQueueHandler* queueHandler() = 0;
};















//==============================================================================
//! @class FileOperationQueue File Operation Queue Class
//==============================================================================
class FileOpQueueHandler : public FileUtilThreadBase
{

    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aOpHandler File Operation Queue View Api
    //! @param aParent Parent
    FileOpQueueHandler(FileOpQueueViewAPI* aOpQViewAPI,
                       DirCreatorObserver* aDirCreatorObserver = NULL,
                       FileDeleteObserver* aDeleteObserver = NULL,
                       FileCopyObserver* aCopyObserver = NULL,
                       FileMoveObserver* aMoveObserver = NULL,
                       FileRenameObserver* aRenameObserver = NULL,
                       QObject* aParent = NULL);

    //! @brief Add Operation Entry
    //! @param aEntry Operation Entry
    void addOperation(FileOperationEntry* aEntry);

    //! @brief Insert Operation Entry
    //! @param aEntry Operation Entry
    //! @param aIndex Operation Entry Index
    void insertOperation(FileOperationEntry* aEntry, const int& aIndex);

    //! @brief Remove Operation
    //! @param aIndex Operation Index
    void removeOperation(const int& aIndex);

    //! @brief Process Queue
    //! @param none
    void processQueue();

    //! @brief Get Operation Entry
    //! @param aIndex Operation Index
    //! @return Operation Entry
    FileOperationEntry* getOperation(const int& aIndex);

    //! @brief Get Operations Count
    //! @param none
    //! @return Operations Count
    int count();

    //! @brief Get Operations Index
    //! @param none
    //! @return Operations Index
    int getOperationIndex(FileOperationEntry* aEntry);

    //! @brief Clear
    //! @param none
    void clear();

    //! @brief Export Operations List Into a String List
    //! @param none
    //! @return Operations List
    QStringList exportList();

    //! @brief Destructor
    //! @param none
    virtual ~FileOpQueueHandler();

signals:

    //! @brief Operation Added Signal
    //! @param aIndex Operation Index
    //! @param aCount Operations Count
    void operationAdded(const int& aIndex, const int& aCount);

    //! @brief Operation Removed Signal
    //! @param aIndex Operation Index
    //! @param aCount Operations Count
    void operationRemoved(const int& aIndex, const int& aCount);

    //! @brief Operation Started Signal
    //! @param aIndex Operation Index
    void operationStarted(const int& aIndex);

    //! @brief Operation Stopped Signal
    //! @param aIndex Operation Index
    void operationStopped(const int& aIndex);

    //! @brief Operation Completed Signal
    //! @param aIndex Operation Index
    void operationCompleted(const int& aIndex);

    //! @brief Operation Updated/Progress Changed Signal
    //! @param aIndex Operation Index
    void operationUpdated(const int& aIndex);

    //! @brief Operation Aborted Signal
    //! @param aIndex Operation Index
    void operationAborted(const int& aIndex);

protected: // From FileUtilThreadBase

    //! @brief Do Operation
    //! @param none
    virtual void doOperation();

    //! @brief Reset Flags
    //! @param none
    void resetFlags();

    //! @brief Process Single Entry
    //! @param aEntry File Operation Entry
    //! @param aIndex File Operation Entry Index
    //! @return true If Entry Is Processed, false Otherwise
    virtual bool processEntry(FileOperationEntry* aEntry, const int& aIndex);

    //! @brief Process Directory Entry
    //! @param aEntry File Operation Entry
    //! @param aIndex File Operation Entry Index
    //! @return true If Entry Is Processed, false Otherwise
    virtual bool processDirEntry(FileOperationEntry* aEntry, const int& aIndex);

protected:
    friend class FileOperationEntry;

    //! @brief Notify View About Entry Updated
    //! @param aIndex File operation Entry Index
    virtual void notifyEntryUpdated(const int& aIndex);

protected: // Data

    //! File Operations List
    QList<FileOperationEntry*>  operations;
    //! Operations Queue View API
    FileOpQueueViewAPI*         opQView;

    //! Current Operation Index
    int                         currOp;

    //! Dir Creator Observer
    DirCreatorObserver*         dirCreatorObserver;
    //! Delete Observer
    FileDeleteObserver*         deleteObserver;
    //! Copy Observer
    FileCopyObserver*           copyObserver;
    //! Move Observer
    FileMoveObserver*           moveObserver;
    //! File Rename Observer
    FileRenameObserver*         renameObserver;

    //! Create Dir Flags
    int                         createDirFlags;
    //! Copy Flags
    int                         copyFlags;
    //! Move Flags
    int                         moveFlags;
    //! Rename Flags
    int                         renameFlags;
    //! Delete Flags
    int                         deleteFlags;
};











#endif // FILEUTILS_H
