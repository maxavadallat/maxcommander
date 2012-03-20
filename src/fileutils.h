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


// CONSTANTS/DEFINES

//! Default Abort Check For Thread
#define DEFAULT_THREAD_ABORT_CHECK              if (abort || restart) return


// DECLARATIONS


//! @brief File Name In Case Sensitive Compare
//! @param a File Name
//! @param b File Name
int fnstricmp(const QString& a, const QString& b);


//==============================================================================
//! @enum FileSortType File Sort Type Enum
//==============================================================================
enum FileSortType
{
    EFSTName        = 0,
    EFSTExtension,
    EFSTSize,
    EFSTDate,
    EFSTOwnership,
    EFSTPermission,
    EFSTAttributes
};

//==============================================================================
//! @enum DriveType Drive Type Enum
//==============================================================================
enum DriveType
{
    DTUnknown       = 0,
    DTNoRoot,
    DTRemoveable,
    DTFixed,
    DTRemote,
    DTCDRom,
    DRRamDisk
};


//==============================================================================
//! @typedef SortingMethod File Sort Algo Function Type
//==============================================================================
typedef int (*SortingMethod)(const QFileInfo&, const QFileInfo&, const bool&);


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
    //! @return true If Successful
    static bool createDir(const QString& aDirName);

    //! @brief Delete File
    //! @param aFileName File Name
    //! @param aRecursive Recursive Flag
    //! @return true If Successful
    bool deleteFile(const QString& aFileName, const bool& aRecursive);

    //! @brief Copy File
    //! @param aSource Source File Name
    //! @param aTarget Target File Name
    //! @return true If Successful
    bool copyFile(const QString& aSource, const QString& aTarget);

    //! @brief Rename File
    //! @param aSource Source File Name
    //! @param aTarget Target File Name
    //! @return true If Successful
    bool renameFile(const QString& aSource, const QString& aTarget);

    //! @brief Move File
    //! @param aSource Source File Name
    //! @param aTarget Target File Name
    //! @return true If Successful
    bool moveFile(const QString& aSource, const QString& aTarget);

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
    static QImage convertMacIcon(const IconRef& aMacIconref, QImage& aIconImage);

#elif defined(Q_OS_WIN)

#else

#endif

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

    //! @brief Destructor
    //! @param none
    virtual ~FileUtils();

signals:

    //! @brief Copy File Progress Signal
    //! @param aFilePath Target File Path
    //! @param aFullSize Full Size
    //! @param aCopied Copied Size
    void copyFileProgress(const QString& aFilePath, const qint64& aFullSize, const qint64 aCopied);

    //! @brief File Entry Found Signal
    //! @param aInfo File Info
    void fileEntryFound(const QFileInfo& aInfo);

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
    void pause();

    //! @brief Resume
    //! @param none
    void resume();

    //! @brief Stop
    //! @param none
    virtual void stop();

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


#endif // FILEUTILS_H
