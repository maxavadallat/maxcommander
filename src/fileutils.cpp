// INCLUDES

#include <QDebug>
#include <QProcess>
#include <QFileIconProvider>
#include <QTimer>
#include <QPainter>
#include <QRegExp>


#if defined(Q_OS_WIN)

#include <windows.h>
#include <shellapi.h>

#elif defined(Q_OS_MAC)

#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
//#include <Security/Authorization.h>
//#include <Security/AuthorizationTags.h>

#include <sys/stat.h>
#include <sys/statvfs.h>

/*
template <typename T>
class Q_CORE_EXPORT QCFType
{
public:
    inline QCFType(const T &t = 0) : type(t) {}
    inline QCFType(const QCFType &helper) : type(helper.type) { if (type) CFRetain(type); }
    inline ~QCFType() { if (type) CFRelease(type); }
    inline operator T() { return type; }
    inline QCFType operator =(const QCFType &helper)
    {
    if (helper.type)
        CFRetain(helper.type);
    CFTypeRef type2 = type;
    type = helper.type;
    if (type2)
        CFRelease(type2);
    return *this;
    }
    inline T *operator&() { return &type; }
    static QCFType constructFromGet(const T &t)
    {
        CFRetain(t);
        return QCFType<T>(t);
    }
protected:
    T type;
};
*/

// Global Authorization Reference
//static AuthorizationRef myAuthorizationRef;

#elif defined(Q_OS_UNIX)

#include <sys/stat.h>
#include <sys/statfs.h>

#endif // Q_OS_UNIX

#include "constants.h"
#include "settings.h"
#include "fileutils.h"
#include "fileoperations.h"
#include "fileutilsclient.h"


// GLOBAL VARIABLES

#ifdef Q_OS_WIN

//! Qt NTFS Permission Lookup
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

#endif // Q_OS_WIN


// IMPLEMENTATION

//==============================================================================
// Compare QStrings Case Insensitive
//==============================================================================
int fnstricmp(const QString& a, const QString& b) {
    return qstricmp(a.toAscii().data(), b.toAscii().data());
}

//==============================================================================
// Get File Attribute
//==============================================================================
int getFileAttr(const QString& aFileName) {
    // Init Attributes
    int attr = 0;

#ifdef Q_OS_WIN

    // Check If File Exists
    if (QFile::exists(aFileName)) {
        // Convert File Name To WCHAR
        const wchar_t* fn = reinterpret_cast<const wchar_t *>(aFileName.utf16());

        qDebug() << "getFileAttr - aFileName: " << QString::fromUtf16((const ushort*)fn).toAscii().data();

        // Get File Attributes
        attr = GetFileAttributes(fn);

    } else {
        qDebug() << "getFileAttr - aFileName: " << aFileName << " DOESN'T EXIST";
    }

#else // Q_OS_WIN

    Q_UNUSED(aFileName);

#endif // Q_OS_WIN

    return attr;
}

//==============================================================================
// Set File Attribute
//==============================================================================
bool setFileAttr(const QString& aFileName, const int& aAttributes) {

#ifdef Q_OS_WIN

    // Check If File Exists
    if (QFile::exists(aFileName)) {
        // Convert File Name To WCHAR
        const wchar_t* fn = reinterpret_cast<const wchar_t *>(aFileName.utf16());

        qDebug() << "setFileAttr - aFileName: " << QString::fromUtf16((const ushort*)fn).toAscii().data();

        // Set File Attribute
        return SetFileAttributes(fn, aAttributes);
    } else {
        qDebug() << "setFileAttr - aFileName: " << aFileName << " DOESN'T EXIST";
    }

#else // Q_OS_WIN

    Q_UNUSED(aFileName);
    Q_UNUSED(aAttributes);

#endif // Q_OS_WIN

    return false;
}

//==============================================================================
// Quick Sort
//==============================================================================
void quickSort(QFileInfoList& arr, int left, int right, SortingMethod sort, bool reverse) {

    // Check Indexes
    if (left < 0 || right < 0 || arr.count() <= 0)
        return;

    // Init First And Last Indexes
    int i = left, j = right;

    // Init Pivot
    QFileInfo pivot = arr[(left + right) / 2];

    // Partition
    while (i <= j) {
        //while (sort(arr[i], pivot, reverse) == 1)
        while (sort(arr[i], pivot, reverse) < 0)
            i++;

        //while (sort(arr[j], pivot, reverse) == -1)
        while (sort(arr[j], pivot, reverse) > 0)
            j--;

        if (i <= j) {
            arr.swap(i, j);
            i++;
            j--;
        }
    }

    // Recursion
    if (left < j) {
        quickSort(arr, left, j, sort, reverse);
    }

    // Recursion
    if (i < right) {
        quickSort(arr, i, right, sort, reverse);
    }
}

//==============================================================================
// Dir First
//==============================================================================
int dirFirst(const QFileInfo& a, const QFileInfo& b)
{
    // Check If Any File Is Dir
    if ((a.isDir() || a.isSymLink()) && !b.isDir() && !b.isSymLink())
        return -1;

    // Check If Any File Is Dir
    if (!a.isDir() && !a.isSymLink() && (b.isDir() || b.isSymLink()))
        return 1;

    // Check File Name
    if (a.fileName() == QString(".") && b.fileName() != QString("."))
        return -1;

    // Check File Name
    if (a.fileName() != QString(".") && b.fileName() == QString("."))
        return 1;

    // Check File Name
    if (a.fileName() == QString("..") && b.fileName() != QString(".."))
        return -1;

    // Check File Name
    if (a.fileName() != QString("..") && b.fileName() == QString(".."))
        return 1;

    return 0;
}


//==============================================================================
// Name Sort
//==============================================================================
int nameSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Case Insensitive Comparison
    dfr = fnstricmp(a.fileName(), b.fileName());

    return r ? -dfr : dfr;
}

//==============================================================================
// Extension Sort
//==============================================================================
int extSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Check If Both File Is a Dir Or Link
    if ((a.isDir() && b.isDir()) || (a.isSymLink() && b.isSymLink()) || (a.isDir() && b.isSymLink()) || (a.isSymLink() && b.isDir()))
        // Return Name Sort
        return nameSort(a, b, r);

    // Check Base Name
    if (a.baseName().isEmpty() && !b.baseName().isEmpty()) {
        // Adjust Result
        dfr = 1;
    } else if (!a.baseName().isEmpty() && b.baseName().isEmpty()) {
        // Adjust Result
        dfr = -1;
    } else {
        // Case Insensitive Comparison
        dfr = fnstricmp(a.suffix(), b.suffix());
    }

    // Check Result
    if (dfr)
        return r ? -dfr : dfr;

    // Return Name Sort Result
    return nameSort(a, b, r);
}

//==============================================================================
// Size Sort
//==============================================================================
int sizeSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Check If Both File Is a Dir Or Link
    if ((a.isDir() && b.isDir()) || (a.isSymLink() && b.isSymLink()))
        // Return Name Sort
        return nameSort(a, b, r);

    // Check File Size
    if (a.size() > b.size())
        return r ? -1 : 1;

    // Check File Size
    if (a.size() < b.size())
        return r ? 1 : -1;

    // Return Name Sort Result
    return nameSort(a, b, r);
}

//==============================================================================
// Date Sort
//==============================================================================
int dateSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Check File Date
    if (a.lastModified() < b.lastModified())
        return r ? -1 : 1;

    // Check File Date
    if (a.lastModified() > b.lastModified())
        return r ? 1 : -1;

    // Return Name Sort Result
    return nameSort(a, b, r);
}

//==============================================================================
// Owners Sort
//==============================================================================
int ownSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Case Insensitive Comparison
    dfr = fnstricmp(a.owner(), b.owner());

    // Check REsult
    if (dfr)
        // Return Result
        return r ? -dfr : dfr;

    // Return Name Sort Result
    return nameSort(a, b, r);
}

//==============================================================================
// Permission Sort
//==============================================================================
int permSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Check File Permissions
    if (a.permissions() < b.permissions())
        return r ? -1 : 1;

    // Check File Permissions
    if (a.permissions() > b.permissions())
        return r ? 1 : -1;

    // Return Name Sort Result
    return nameSort(a, b, r);
}

//==============================================================================
// Attributes Sort
//==============================================================================
int attrSort(const QFileInfo& a, const QFileInfo& b, const bool& r) {

    // Apply Dir First Filter
    int dfr = dirFirst(a, b);

    // Check Dir First Result
    if (dfr)
        return dfr;

    // Check File Attributes
    if (getFileAttr(a.absoluteFilePath()) < getFileAttr(b.absoluteFilePath()))
        return r ? -1 : 1;

    // Check File Attributes
    if (getFileAttr(a.absoluteFilePath()) > getFileAttr(b.absoluteFilePath()))
        return r ? 1 : -1;

    // Return Name Sort Result
    return nameSort(a, b, r);
}










//==============================================================================
// Constructor
//==============================================================================
FileUtils::FileUtils(QObject* aParent)
    : QObject(aParent)
{
}

//==============================================================================
// Get Dir Empty
//==============================================================================
bool FileUtils::isDirEmpty(const QString& aDirPath)
{
    // Init Temp Dir
    QDir tempDir(aDirPath);
    // Set Filter
    tempDir.setFilter(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    // Return Empty State According To Count
    return (tempDir.count() <= 0);
}

//==============================================================================
// Get File Size
//==============================================================================
qint64 FileUtils::getFileSize(const QString& aFilePath)
{
    // Init Dir Size
    qint64 dirSize = 0;
    // Init Abort Flag
    bool abortFlag = false;

    return getDirSize(aFilePath, dirSize, abortFlag);
}

//==============================================================================
// Get Dir Size
//==============================================================================
qint64 FileUtils::getDirSize(const QString& aDirPath, qint64& aDirSize, bool& aAbort)
{
    // Init File Info
    QFileInfo fileInfo(aDirPath);

    // Check If File Is A Dir
    if (fileInfo.isDir()) {
        // Init Dir
        QDir tempDir(aDirPath);
        // Init Filters
        QDir::Filters filters(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        // Init Sort Flags
        QDir::SortFlags sortFlags(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);
        // Get File Entry List
        QFileInfoList infoList = tempDir.entryInfoList(filters, sortFlags);

        // Get Info Count
        int fiCount = infoList.count();

        // Go Thru File Info List
        for (int i=0; i<fiCount; i++) {

            // Check Abort
            if (aAbort) {
                qDebug() << "FileUtils::getDirSize - ABORT";
                return aDirSize;
            }

            //qDebug() << infoList[i].absoluteFilePath();

            // Check File Info
            if (infoList[i].isDir()) {
                // Call Recursively
                getDirSize(infoList[i].absoluteFilePath(), aDirSize, aAbort);
            } else {
                // Add File Size To Total Size
                aDirSize += infoList[i].size();
            }

            //qDebug() << aDirSize;
        }

        return aDirSize;
    }

    return fileInfo.size();
}

//==============================================================================
// Create Directory
//==============================================================================
int FileUtils::createDir(const QString& aDirPath, int& aOptions, bool& aAbortSig, DirCreatorObserver* aObserver)
{
    // Init Dir Name
    QString dirName = QDir::cleanPath(aDirPath);

    qDebug() << "FileUtils::createDir - aDirPath: " << aDirPath;

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->createDirStarted(dirName);
    }

    // Init File Utils Client
    RemoteFileUtils* utilsClient = aObserver ? aObserver->getUtilsClient() : NULL;

    // Init Observer Response
    int observerResponse = 0;
    // Make Dir Result
    int mkdirResult = 0;
    // Init Last Error
    int lastError = 0;
    // Admin Mode
    bool adminMode = (aOptions & FILE_OPERATION_OPTION_ADMIN) && utilsClient;

    // Create
    for (int oldslash = -1, slash = 0; slash != -1; oldslash = slash) {
        // Get Slash Position
        slash = dirName.indexOf(QDir::separator(), oldslash + 1);

        // Check Slash Position
        if (slash == -1) {
            // Set Old Slasp Pos
            if (oldslash == dirName.length())
                break;
            // Set Slash Pos
            slash = dirName.length();
        }

        // Check Slash Position
        if (slash) {
            // Get Chunk
            QByteArray chunk = QFile::encodeName(dirName.left(slash));

            qDebug() << "FileUtils::createDir - chunk: " << chunk;

            do {
                // Reset Make Dir Result
                mkdirResult = 0;
                // Reset Last Error
                lastError = 0;

                // Check If Dir Exists
                if (!QFile::exists(QString(chunk))) {

                    // Check Admin Mode
                    if (adminMode && utilsClient) {

                        // Create Dir
                        //utilsClient->createDir(chunk);

                    } else {

#if defined (Q_OS_WIN)

                        // Create Directory
                        mkdirResult = CreateDirectory(chunk);

#elif defined (Q_OS_UNIX)

                        // Make Dir
                        mkdirResult = mkdir(chunk, 0777);

#endif  // Q_OS_MAC || Q_OS_UNIX

                        // Get Last Error
                        lastError = getLastError();
                    }
                }

                // Check Result
                if (mkdirResult != 0) {

                    // Check Observer
                    if (aObserver) {

                        // Get Response
                        observerResponse = aObserver->createDirError(dirName.left(slash), lastError);

                        // Check Observer Response & Admin Mode
                        if (observerResponse == FOORTAsRoot && !adminMode) {

                            // Get Admin Pass Query Provider
                            AdminPassQueryProvider* passQueryProvider = aObserver->passQueryProvider();

                            // Check Admin Pass Query Provider
                            if (passQueryProvider) {

                                // Launch Admin Pass Query And Get Admin Password
                                QString adminPass = passQueryProvider->launchAdminPassQuery();

                                // Get Utils Client
                                utilsClient = aObserver->getUtilsClient(adminPass);

                                // Check Utils Client
                                if (utilsClient) {
                                    // Set Options
                                    aOptions |= FILE_OPERATION_OPTION_ADMIN;
                                }
                            }

                            // Update Admin Mode
                            adminMode = (aOptions & FILE_OPERATION_OPTION_ADMIN) && utilsClient;

                            // Adjust Observer Response
                            observerResponse = FOORTRetry;
                        // Check Observer Response
                        } else if (observerResponse == FOORTAbort || observerResponse == FOORTCancel) {
                            // Set Abort Sig
                            aAbortSig = true;

                            return FILE_UTILS_RESPONSE_ABORT;
                        }

                    } else {
#if defined (Q_OS_WIN)
                        // Check Last Error
                        if (lastError != ERROR_ALREADY_EXISTS) {
#elif defined (Q_OS_UNIX)
                        // Check Err No
                        if (lastError != EEXIST) {
#endif // Q_OS_UNIX
                            qDebug() << "FileUtils::createDir - errno: " << lastError;

                            return FILE_UTILS_RESPONSE_ERROR;
                        }
                    }
                } else {
                    // Reset Last Error
                    lastError = 0;
                }
            } while (aObserver && observerResponse == FOORTRetry);
        }
    }

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->createDirFinished(dirName, lastError);
    }

    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Read Directory
//==============================================================================
int FileUtils::readDir(const QString& aDirName, int& aOptions, bool& aAbortSig, DirReaderObserver* aObserver)
{
    // Init Dir Name
    QString dirName = QDir::cleanPath(aDirName);

    // Check Dir Name
    if (dirName.isEmpty())
        return FILE_UTILS_RESPONSE_ERROR;

    // Init Dir Info
    QFileInfo dirInfo(dirName);

    // Init Observer Response
    int observerResponse = 0;
    // Init Last Error
    int lastError = 0;

    // Check If Dir Exists
    if (!dirInfo.exists()) {
        // Check Observer
        if (aObserver) {
#if defined (Q_OS_WIN)
            // Set Last Error
            lastError = ERROR_FILE_NOT_FOUND;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error
            lastError = ENOENT;
#endif // Q_OS_MAC || Q_OS_UNIX
            // Notify Observer
            aObserver->readDirError(aDirName, lastError);
        }

        return FILE_UTILS_RESPONSE_ERROR;
    }

    // Check Abort Signal
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->readDirStarted(dirName);
    }

    // ...

    // Check Abort Signal
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // ...


    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->readDirFinished(dirName, lastError);
    }

    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Scan Directory For Size
//==============================================================================
int FileUtils::scanDir(const QString& aDirName, int& aOptions, bool& aAbortSig, DirScannerObserver* aObserver)
{
    // Init Dir Name
    QString dirName = QDir::cleanPath(aDirName);
    // Init Dir Info
    QFileInfo dirInfo(dirName);

    // Init Observer Response
    int observerResponse = 0;
    // Init Last Error
    int lastError = 0;


    // Check Abort Signal
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }


    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->scanDirStarted(dirName);
    }

    // ...

    // Check Abort Signal
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // ...


    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->scanDirFinished(dirName, lastError);
    }

    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Delete File
//==============================================================================
int FileUtils::deleteFile(const QString& aFileName, int& aOptions, bool& aAbortSig, FileDeleteObserver* aObserver)
{
    // Init File Name
    QString fileName = QDir::cleanPath(aFileName);
    // Init File Info
    QFileInfo fileInfo(fileName);

    // Init Delete Files
    bool deleteFiles = aOptions & FILE_DELETE_OPTION_DELETE_NORMAL;
    // Init Delete All Read Only, System, Hidden Files
    bool deleteReadOnlyFiles = aOptions & FILE_DELETE_OPTION_DELETE_READONLY;
    // Init Skip Deleting All Normal Files
    bool skipFiles = aOptions & FILE_DELETE_OPTION_DELETE_SKIP_NORMAL;
    // Init Skip Deleting All Read Only  Files
    bool skipReadOnlyFiles = aOptions & FILE_DELETE_OPTION_DELETE_SKIP_READONLY;

    // Init Delete Non Empty Directories
    //bool deleteNonEmpty = aOptions & FILE_DELETE_OPTION_DELETE_NON_EMPTY_DIR;

    // Init Last Error
    int lastError = 0;

    // Check If File Exists
    if (!fileInfo.exists()) {
        // Check Observer
        if (aObserver) {
#if defined (Q_OS_WIN)
            // Set Last Error
            lastError = ERROR_FILE_NOT_FOUND;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error
            lastError = ENOENT;
#endif // Q_OS_MAC || Q_OS_UNIX
            // Notify
            aObserver->deleteError(fileName, lastError);
        }

        return FILE_UTILS_RESPONSE_ERROR;
    }

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->deleteFileStarted(fileName);
    }

    // Init Observer Response
    int observerResponse = 0;
    // Init Remove Result
    int removeResult = 0;

    // Init File Read Only
    bool fileReadOnly = false;

#if defined (Q_OS_WIN)

    // Get File Attributes
    int attrib =  FileUtils::getFileAttributes(fileName);

    // Check Attribute
    if ((attrib & FILE_ATTRIBUTE_HIDDEN) || (attrib & FILE_ATTRIBUTE_READONLY) || (attrib & FILE_ATTRIBUTE_SYSTEM)) {
        // Set Read Only Flag
        fileReadOnly = true;
    }

#else // Q_OS_WIN

    // ...

#endif // Q_OS_WIN

    // Check Observer
    if (aObserver) {
        // Check Delete Read Only Files Option
        if (fileReadOnly && !deleteReadOnlyFiles && !skipReadOnlyFiles) {
            // Get Confirmation Result
            observerResponse = aObserver->confirmDeletion(fileName, fileReadOnly);

            // Evaluate Observer Result
            if (observerResponse == FOORTSkipAll || observerResponse == FOORTNoToAll) {
                // Add To Options
                aOptions |= FILE_DELETE_OPTION_DELETE_SKIP_READONLY;

                return FILE_UTILS_RESPONSE_SKIP;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTYesToAll) {
                // Add To Options
                aOptions |= FILE_DELETE_OPTION_DELETE_READONLY;
                // Update Delete Read Only Files
                deleteReadOnlyFiles = true;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTAbort || observerResponse == FOORTCancel) {
                // Set Abort Sig
                aAbortSig = true;

                return FILE_UTILS_RESPONSE_ABORT;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTSkip || observerResponse == FOORTNo) {

                return FILE_UTILS_RESPONSE_SKIP;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTYes) {
                // Update Delete Read Only Files
                deleteReadOnlyFiles = true;
            }

        // Check Delete Normal Files Option
        } else if (!deleteFiles && !skipFiles) {
            // Get Confirmation Result
            observerResponse = aObserver->confirmDeletion(fileName, fileReadOnly);
            // Evaluate Observer Result
            if (observerResponse == FOORTSkipAll || observerResponse == FOORTNoToAll) {
                // Add To Options
                aOptions |= FILE_DELETE_OPTION_DELETE_SKIP_NORMAL;

                return FILE_UTILS_RESPONSE_SKIP;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTYesToAll) {
                // Add To Options
                aOptions |= FILE_DELETE_OPTION_DELETE_NORMAL;
                // Update Delete File
                deleteFiles = true;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTAbort || observerResponse == FOORTCancel) {
                // Set Abort Sig
                aAbortSig = true;

                return FILE_UTILS_RESPONSE_ABORT;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTSkip || observerResponse == FOORTNo) {

                return FILE_UTILS_RESPONSE_SKIP;

            // Evaluate Observer Result
            } else if (observerResponse == FOORTYes) {
                // Update Delete Files
                deleteFiles = true;
            }

        // Check Skip Read Only Files Option
        } else if (fileReadOnly && skipReadOnlyFiles) {

            return FILE_UTILS_RESPONSE_SKIP;

        // Check Skip Files Option
        } else if (!fileReadOnly && skipFiles) {

            return FILE_UTILS_RESPONSE_SKIP;

        }
    } else {
        // Check Options
        if ((fileReadOnly && skipReadOnlyFiles) || skipFiles || !deleteReadOnlyFiles && !deleteFiles) {

            return FILE_UTILS_RESPONSE_ERROR;
        }
    }

    qDebug() << "FileUtils::deleteFile - fileName: " << fileName;

    do {
        // Check Abort Signal
        if (aAbortSig) {
            return FILE_UTILS_RESPONSE_ABORT;
        }

        // Check File Info
        if (fileInfo.isDir()) {
#if defined (Q_OS_WIN)
            // Delete Directory
            removeResult = !RemoveDirectory(QFile::encodeName(fileName));
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Delete File
            removeResult = rmdir(QFile::encodeName(fileName));
#endif // Q_OS_MAC || Q_OS_UNIX
        } else {
#if defined (Q_OS_WIN)
            // Delete File
            removeResult = !DeleteFile(QFile::encodeName(fileName));
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Delete File
            removeResult = unlink(QFile::encodeName(fileName));
#endif // Q_OS_MAC || Q_OS_UNIX
        }

        // Check Remove Result
        if (removeResult && observerResponse != FOORTSkipAll && observerResponse != FOORTIgnoreAll) {
#if defined (Q_OS_WIN)
            // Get Last Error
            lastError = GetLastError();
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Get Last Error
            lastError = errno;

            // Check Last Error
            if (lastError == EEXIST) {
                // Reset Last Error
                lastError = 0;
            }
#endif // Q_OS_MAC || Q_OS_UNIX
            // Check Last Error
            if (lastError) {
                qDebug() << "FileUtils::deleteFile - lastError: " << lastError;
                // Check Observer
                if (aObserver) {
                    // Get Observer Response
                    observerResponse = aObserver->deleteError(fileName, lastError);

                    // Check Observer Error

                } else {
                    return FILE_UTILS_RESPONSE_ERROR;
                }
            }
        }
    } while (aObserver && observerResponse == FOORTRetry);

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->deleteFileFinished(fileName, lastError);
    }

    // Remove File
    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Copy File
//==============================================================================
int FileUtils::copyFile(const QString& aSource, const QString& aTarget, int& aOptions, bool& aAbortSig, FileCopyObserver* aObserver)
{
    // Get Source Name
    QString sourceName = QDir::cleanPath(aSource);
    // Get Target Name
    QString targetName = QDir::cleanPath(aTarget);
    // Init Source File Info
    QFileInfo sourceInfo(sourceName);
    // Init Target File Info
    QFileInfo targetInfo(targetName);

    // Init Overwrite Files
    bool overwriteFiles = aOptions & FILE_COPY_OPTION_OVERWRITE_NORMAL;
    // Init Overwrite All Read Only, System, Hidden Files
    bool overWriteReadOnlyFiles = aOptions & FILE_COPY_OPTION_OVERWRITE_READONLY;
    // Init Skip Overwriting All Normal Files
    bool skipFiles = aOptions & FILE_COPY_OPTION_OVERWRITE_SKIP_NORMAL;
    // Init Skip Overwrite All Read Only  Files
    bool skipReadOnlyFiles = aOptions & FILE_COPY_OPTION_OVERWRITE_SKIP_READONLY;

    // Init Last Error
    int lastError = 0;

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Init Observer Response
    int observerResponse = 0;
    // Init File Read Only
    bool fileReadOnly = false;


    // Check If File Exists
    if (!QFile::exists(sourceName)) {
        // Check Observer
        if (aObserver) {
#if defined (Q_OS_WIN)
            // Set Last Error
            lastError = ERROR_FILE_NOT_FOUND;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error
            lastError = ENOENT;
#endif // Q_OS_MAC || Q_OS_UNIX
            // Notify
            observerResponse = aObserver->copyError(sourceName, targetName, lastError);
        }

        return FILE_UTILS_RESPONSE_ERROR;
    }

    // Compare Source And Target File Name If They Are The Same
    if (sourceName == targetName) {
        // Check Observer
        if (aObserver) {
#if defined (Q_OS_WIN)
            // Set Last Error
            lastError = ERROR_INVALID_PARAMETER;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error Manually
            lastError = EINVAL;
#endif // Q_OS_MAC || Q_OS_UNIX
            // Notify
            observerResponse = aObserver->copyError(sourceName, targetName, lastError);
        }

        return FILE_UTILS_RESPONSE_ERROR;
    }

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Check Target File
    if (QFile::exists(targetName) && !targetInfo.isDir()) {

#if defined (Q_OS_WIN)

        // Get File Attributes
        int attrib =  FileUtils::getFileAttributes(targetName);

        // Check Attribute
        if ((attrib & FILE_ATTRIBUTE_HIDDEN) || (attrib & FILE_ATTRIBUTE_READONLY) || (attrib & FILE_ATTRIBUTE_SYSTEM)) {
            // Set Read Only Flag
            fileReadOnly = true;
        }

#else // Q_OS_WIN

    // ...

#endif // Q_OS_WIN

        // Check Observer
        if (!(overwriteFiles || overWriteReadOnlyFiles || skipFiles || skipReadOnlyFiles) && aObserver) {
            // Get Confirmation
            observerResponse = aObserver->confirmCopyOverWrite(sourceName, targetName);
            // Check Observer Response
            if (observerResponse == FOORTYes || observerResponse == FOORTYesToAll) {
                // Check Observer Response
                if (observerResponse == FOORTYesToAll) {
                    // Check Entry If Read Only
                    if (fileReadOnly) {
                        // Add To Options
                        aOptions |= FILE_COPY_OPTION_OVERWRITE_READONLY;
                    } else {
                        // Add To Options
                        aOptions |= FILE_COPY_OPTION_OVERWRITE_NORMAL;
                    }
                }

                do {
                    // Delete File
                    if (!QFile::remove(targetName)) {
                        // Get Last Error
                        lastError = getLastError();
                        // Check Last Error
                        if (aObserver && lastError) {
                            // Error
                            observerResponse = aObserver->copyError(sourceName, targetName, lastError);
                        }
                    }
                } while (aObserver && observerResponse == FOORTRetry);

            } else {

                // Check Observer Response
                if (observerResponse == FOORTNoToAll) {
                    // Check If Target File Is Read Only
                    if (fileReadOnly) {
                        // Add To Options
                        aOptions |= FILE_COPY_OPTION_OVERWRITE_SKIP_READONLY;
                    } else {
                        // Add To Options
                        aOptions |= FILE_COPY_OPTION_OVERWRITE_SKIP_NORMAL;
                    }
                // Evaluate Observer Result
                } else if (observerResponse == FOORTAbort || observerResponse == FOORTCancel) {
                    // Set Abort Sig
                    aAbortSig = true;

                    return FILE_UTILS_RESPONSE_ABORT;
                }

                return FILE_UTILS_RESPONSE_SKIP;
            }
        } else {
            // Check Options
            if ((!fileReadOnly && overwriteFiles) || (fileReadOnly && overWriteReadOnlyFiles)) {
                do {
                    // Delete File
                    if (!QFile::remove(targetName)) {
                        // Get Last Error
                        lastError = getLastError();
                        // Check Last Error
                        if (aObserver && lastError) {
                            // Error
                            observerResponse = aObserver->copyError(sourceName, targetName, lastError);
                        }
                    }
                } while (aObserver && observerResponse == FOORTRetry);

            } else {
                return FILE_UTILS_RESPONSE_ERROR;
            }
        }
    }

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->copyStarted(sourceName, targetName);
    }

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Get Source File Size
    qint64 sfSize = sourceInfo.size();
    // Get Settings Instance
    Settings* settings = Settings::getInstance();
    // Get Copy Buffer Size
    qint64 buffSize = settings ? settings->getValue(QString(SETTINGS_KEY_COPY_BUFFER_SIZE), DEFAULT_COPY_BUFFER_SIZE).toInt() : DEFAULT_COPY_BUFFER_SIZE;
    // Adjust Buff Size
    buffSize = qMin(buffSize, sfSize);

    // Alloc Main Buffer For Copy File
    char* buf = (char*)malloc(buffSize);

    // Init Source File
    QFile sf(sourceName);
    // Init Target File
    QFile tf(targetName);

    // Check Abort Sig
    if (aAbortSig) {
        // Free Copy Buffer
        free(buf);

        return FILE_UTILS_RESPONSE_ABORT;
    }

    do {// OPEN SOURCE
        // Open Source File
        if (sf.open(QIODevice::ReadOnly)) {
            do { // OPEN TARGET
                // Open Target File
                if (tf.open(QIODevice::WriteOnly)) {
                    // Source File Pos
                    qint64 spos = 0;
                    // Source File Size
                    qint64 ssize = sourceInfo.size();
                    // Go Thru Source File
                    while (!sf.atEnd()) {
                        do { // READ SOURCE

                            // Check Abort Signal
                            if (aAbortSig) {
                                // Close Source File
                                sf.close();
                                // Close Target File
                                tf.close();
                                // Free Copy Buffer
                                free(buf);

                                return FILE_UTILS_RESPONSE_ABORT;
                            }

                            // Read File Into Buff
                            qint64 byteRead = sf.read(buf, buffSize);
                            // Check Byte Read
                            if (byteRead < 0) {
                                // Get Last Error
                                lastError = getLastError();

                                // Check Observer
                                if (aObserver && lastError) {
                                    // Copy Error
                                    observerResponse = aObserver->copyError(sourceName, targetName, lastError);

                                    // Check Observer Response
                                    if (observerResponse == FOORTCancel) {
                                        // Close Source File
                                        sf.close();
                                        // Close Target File
                                        tf.close();
                                        // Free Copy Buffer
                                        free(buf);

                                        return FILE_UTILS_RESPONSE_ABORT;
                                    }
                                } else {
                                    // Close Source File
                                    sf.close();
                                    // Close Target File
                                    tf.close();
                                    // Free Copy Buffer
                                    free(buf);

                                    return FILE_UTILS_RESPONSE_ERROR;
                                }
                            } else {
                                // Set Source Pos
                                spos += byteRead;
                                do { // WRITE TARGET
                                    // Write To Target File
                                    if (tf.write(buf, byteRead) < 0) {
                                        // Get Last Error
                                        lastError = getLastError();

                                        // Check Observer
                                        if (aObserver && lastError) {
                                            // Copy Error
                                            observerResponse = aObserver->copyError(sourceName, targetName, lastError);

                                            // Check Observer Response
                                            if (observerResponse == FOORTCancel) {
                                                // Close Source File
                                                sf.close();
                                                // Close Target File
                                                tf.close();
                                                // Free Copy Buffer
                                                free(buf);

                                                return FILE_UTILS_RESPONSE_ABORT;
                                            }
                                        } else {
                                            // Close Source File
                                            sf.close();
                                            // Close Target File
                                            tf.close();
                                            // Free Copy Buffer
                                            free(buf);

                                            return FILE_UTILS_RESPONSE_ERROR;
                                        }
                                    } else {
                                        // Check Observer
                                        if (aObserver) {
                                            // Notify
                                            aObserver->copyProgress(sourceName, targetName, spos, ssize);
                                        }
                                    }
                                } while (aObserver && observerResponse == FOORTRetry);
                            }
                        } while (aObserver && observerResponse == FOORTRetry);
                    }
                    // Close Target File
                    tf.close();
                } else {
                    // Get Last Error
                    lastError = getLastError();
                    // Check Observer
                    if (aObserver && lastError) {
                        // Show Copy Error
                        observerResponse = aObserver->copyError(sourceName, targetName, lastError);

                        // Check Observer Response

                        // ...
                    }
                }
            } while (observerResponse == FOORTRetry);

            // Close Source File
            sf.close();
        } else {
            // Get Last Error
            lastError = getLastError();
            // Check Observer
            if (aObserver) {
                // Show Copy Error - Get Response
                observerResponse = aObserver->copyError(sourceName, targetName, lastError);

                // Check Observer Response

                // ...
            }
        }
    } while (aObserver && observerResponse == FOORTRetry);

    // Check Copy Buffer
    if (buf) {
        // Free Copy Buffer
        free(buf);
    }
    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->copyFinished(sourceName, targetName, lastError);
    }

    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Rename File
//==============================================================================
int FileUtils::renameFile(const QString& aSource, const QString& aTarget, int& aOptions, bool& aAbortSig, FileRenameObserver* aObserver)
{
    // Get Source Name
    QString sourceName = QDir::cleanPath(aSource);
    // Get Target Name
    QString targetName = QDir::cleanPath(aTarget);

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    qDebug() << "FileUtils::renameFile - sN: " << sourceName << " - tN: " << targetName << " - aOptions: " << aOptions << " - aObserver: " << aObserver;

    // Init Source File Info
    QFileInfo sourceInfo(sourceName);
    // Init Target File Info
    QFileInfo targetInfo(targetName);

    // Init Last Error
    int lastError = 0;

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }
    // Check If Source File Exists
    if (!sourceInfo.exists()) {
        // Check Observer
        if (aObserver) {
#if defined (Q_OS_WIN)
            // Set Last Error
            lastError = ERROR_FILE_NOT_FOUND;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error
            lastError = ENOENT;
#endif // Q_OS_MAC || Q_OS_UNIX
            // Notify
            aObserver->renameError(sourceName, targetName, lastError);
        }

        return FILE_UTILS_RESPONSE_ERROR;
    }

    // Init Observer Response
    int observerResponse = 0;

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Check Target File
    if (QFile::exists(targetName) && !targetInfo.isDir()) {
        // Check Observer
        if (aObserver) {
            // Get Confirmation
            observerResponse = aObserver->confirmRenameOverWrite(sourceName, targetName);
            // Check Observer Response
            if (observerResponse == FOORTYes || observerResponse == FOORTYesToAll) {
                do {
                    // Delete File
                    QFile::remove(targetName);
                    // Get Last Error
                    lastError = getLastError();
                    // Check Last Error
                    if (lastError) {
                        // Show Observer Error - Get Response
                        observerResponse = aObserver->renameError(sourceName, targetName, lastError);

                        // Check Observer Response

                        // ...
                    }
                } while (observerResponse == FOORTRetry);
            }
        } else {
            return FILE_UTILS_RESPONSE_ERROR;
        }
    }

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->renameStarted(sourceName, targetName);
    }

    do {
        // Check Abort Sig
        if (aAbortSig) {
            return FILE_UTILS_RESPONSE_ABORT;
        }

        // Rename File
        if (!QFile::rename(sourceName, targetName)) {
            // Get Last Error
            lastError = getLastError();
            // Check Last Error
            if (lastError && aObserver) {
                // Error
                observerResponse = aObserver->renameError(sourceName, targetName, lastError);
            }
        }
    } while (aObserver && observerResponse == FOORTRetry);

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->renameFinished(sourceName, targetName, lastError);
    }

    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Move File
//==============================================================================
int FileUtils::moveFile(const QString& aSource, const QString& aTarget, int& aOptions, bool& aAbortSig, FileMoveObserver* aObserver)
{
    // Get Source Name
    QString sourceName = QDir::cleanPath(aSource);
    // Get Target Name
    QString targetName = QDir::cleanPath(aTarget);

    qDebug() << "FileUtils::moveFile - sN: " << sourceName << " - tN: " << targetName << " - aOptions: " << aOptions << " - aObserver: " << aObserver;

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Init Source File Info
    QFileInfo sourceInfo(sourceName);
    // Init Target File Info
    QFileInfo targetInfo(targetName);

    // Init Last Error
    int lastError = 0;

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Check If Source File Exists
    if (!sourceInfo.exists()) {
        // Check Observer
        if (aObserver) {
#if defined (Q_OS_WIN)
            // Set Last Error
            lastError = ERROR_FILE_NOT_FOUND;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
            // Set Last Error
            lastError = ENOENT;
#endif // Q_OS_MAC || Q_OS_UNIX
            // Notify
            aObserver->moveError(sourceName, targetName, lastError);
        }

        return FILE_UTILS_RESPONSE_ERROR;
    }

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    // Init Observer Response
    int observerResponse = 0;

    // Check Target File
    if (QFile::exists(targetName) && !targetInfo.isDir()) {

        // Check Abort Sig
        if (aAbortSig) {
            return FILE_UTILS_RESPONSE_ABORT;
        }

        // Check Observer
        if (aObserver) {
            // Get Confirmation
            observerResponse = aObserver->confirmMoveOverWrite(sourceName, targetName);
            // Check Observer Response
            if (observerResponse == FOORTYes || observerResponse == FOORTYesToAll) {
                do {
                    // Delete File
                    QFile::remove(targetName);
                    // Get Last Error
                    lastError = getLastError();
                    // Check Last Error
                    if (lastError) {
                        // Show Observer Move Error - Get Response
                        observerResponse = aObserver->moveError(sourceName, targetName, lastError);

                        // Check Observer Response

                        // ...
                    }
                } while (observerResponse == FOORTRetry);
            }
        } else {
            return FILE_UTILS_RESPONSE_ERROR;
        }
    }

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->moveStarted(sourceName, targetName);
    }

    // Check Abort Sig
    if (aAbortSig) {
        return FILE_UTILS_RESPONSE_ABORT;
    }

    do {


    } while (aObserver && observerResponse == FOORTRetry);

    // Check Observer
    if (aObserver) {
        // Notify
        aObserver->moveFinished(sourceName, targetName, lastError);
    }

    return (lastError == 0) ? FILE_UTILS_RESPONSE_NOERROR : FILE_UTILS_RESPONSE_ERROR;
}

//==============================================================================
// Set File Attributes
//==============================================================================
bool FileUtils::setFileAttributes(const QString& aFilePath, const int& aAttributes)
{
    return setFileAttr(aFilePath, aAttributes);
}

//==============================================================================
// Get File Attribute
//==============================================================================
int FileUtils::getFileAttributes(const QString& aFilePath)
{
    return getFileAttr(aFilePath);
}

//==============================================================================
// Set File Premissions
//==============================================================================
bool FileUtils::setPermissions(const QString& aFilePath, const QFile::Permissions& aPermissions)
{
    return QFile::setPermissions(aFilePath, aPermissions);
}

//==============================================================================
// Get File Permissions
//==============================================================================
QFile::Permissions FileUtils::permissions(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

#ifdef Q_OS_WIN

    // Turn On NTFS Permission Lookup
    qt_ntfs_permission_lookup++;

#endif // Q_OS_WIN

    // Get File Permission
    QFile::Permissions result = fileInfo.permissions();

#ifdef Q_OS_WIN

    // Turn Off NTFS Permission Lookup
    qt_ntfs_permission_lookup--;

#endif // Q_OS_WIN

    return result;
}

//==============================================================================
// Get File Owner
//==============================================================================
QString FileUtils::getOwner(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

    return fileInfo.owner();
}

//==============================================================================
// Set File Owner
//==============================================================================
bool FileUtils::setOwner(const QString& aFilePath, const QString& aOwner)
{
    // Check If File Exists & Date Is Valid
    if (!QFile::exists(aFilePath) || aOwner.isEmpty())
        return false;

    qDebug() << "FileUtils::setOwner - aFilePath: " << aFilePath << " - aOwner: " << aOwner;


    return false;
}

//==============================================================================
// Set File Last Modified Date
//==============================================================================
bool FileUtils::setLastModified(const QString& aFilePath, const QDateTime& aDate)
{
    // Check If File Exists & Date Is Valid
    if (!QFile::exists(aFilePath) || !aDate.isValid())
        return false;

    qDebug() << "FileUtils::setLastModified - aFilePath: " << aFilePath << " - mt: " << aDate.toString();

#ifdef Q_OS_WIN

    // http://msdn.microsoft.com/en-us/library/ms724205.aspx

    OFSTRUCT ofs;

    HFILE winFile = OpenFile(aFilePath.toAscii().data(), &ofs, OF_READWRITE);

    // Check File Handle
    if (winFile) {
        FILETIME ft;

        ft.dwLowDateTime = (aDate.toMSecsSinceEpoch() << 32) >> 32;
        ft.dwHighDateTime = aData.toMSecsSinceEpoch() >> 32;

        SetFileTime(winFile, NULL, NULL, &ft);

        // Close File
        FindClose(winFile);
    }

#else // Q_OS_WIN

    // Init Params
    QStringList params;
    // Add Modification Date Options Parameter
    params << QString("-m");
    // Add Date Parameter
    params << QString("%1%2%3%4%5%6").arg(aDate.date().year())
                                     .arg(aDate.date().month())
                                     .arg(aDate.date().day())
                                     .arg(aDate.time().hour())
                                     .arg(aDate.time().minute())
                                     .arg(aDate.time().second());
    // Add File Path Parameter
    params << aFilePath;
    // Execute Touch
    return (QProcess::execute(QString("touch"), params) == 0);

#endif // Q_OS_WIN

    return false;
}

//==============================================================================
// Get File Last Modified Date
//==============================================================================
QDateTime FileUtils::getLastModified(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

    return fileInfo.lastModified();
}

//==============================================================================
// Get Directory File List
//==============================================================================
QFileInfoList FileUtils::getDirList(const QString& aDirName, const FileSortType& aSortType, const bool& aHidden, const bool& aReverse, const QStringList& aFilters)
{
    // Check Dir Name
    if (aDirName.isEmpty()) {
        return QFileInfoList();
    }

    //qDebug() << "FileUtils::getDirList - aDirName: " << aDirName;

    // Init File Filters
    QDir::Filters filters(QDir::AllDirs | QDir::Files | QDir::NoDot);

    // Check Hidden
    if (aHidden) {
        // Add Hidden Flag
        filters |= QDir::Hidden;
        // Add System Flag
        filters |= QDir::System;
    }

    // Init Sort Flags
    QDir::SortFlags sortFlags = QDir::NoSort;

/*
    // Init Sort Flags
    QDir::SortFlags sortFlags(QDir::DirsFirst | QDir::IgnoreCase);

    // Switch Sort Type
    switch (aSortType) {
        default:
        case EFSTName:  sortFlags |= QDir::Name;    break;
        case EFSTSize:  sortFlags |= QDir::Size;    break;
        case EFSTDate:  sortFlags |= QDir::Time;    break;
    }

    // Check Reverse Order
    if ((aReverse && aSortType == EFSTName) || ((aSortType == EFSTSize || aSortType == EFSTDate) && !aReverse)) {
        // Add Reverse Flag
        sortFlags |= QDir::Reversed;
    }
*/
    // Init Dir
    QDir tempDir(aDirName);

    // Check If Root Dir
    if (tempDir.isRoot()) {
        // Adjust Filters
        filters |= QDir::NoDotAndDotDot;
    }

    // Init File List
    QFileInfoList fileList;

    // Check If Directory Readable
    if (tempDir.isReadable()) {
        //qDebug() << "FileUtils::getDirList - aDirName: " << aDirName << " - permissions: " << QFileInfo(aDirName).permissions();
        // Get Initial List
        fileList = tempDir.entryInfoList(aFilters, filters, sortFlags);
        // Check Count
        if (fileList.count() > 0) {
            qDebug() << "FileUtils::getDirList - aDirName: " << aDirName << " - count: " << fileList.count();

            // Switch Sort Type
            switch (aSortType) {
                default:
                    // Properly Sort List
                    sortFileList(fileList, aSortType, aReverse);
                break;
/*
                case EFSTName:
                case EFSTSize:
                case EFSTDate:
                    break;
*/
            }
        } else {
            qDebug() << "FileUtils::getDirList - aDirName: " << aDirName << " - Could Not Find Items!";
        }
    } else {
        qDebug() << "FileUtils::getDirList - aDirName: " << aDirName << " - Dir Not Readable!";
    }

    // Return Properly Sorted List
    return fileList;
}

//==============================================================================
// Get Dirve Type
//==============================================================================
int FileUtils::getDriveType(const int& aDriveIndex)
{
    // Init Drive Type
    int driveType = DTUnknown;

    // Check Drive Index
    if (aDriveIndex >= 0 && aDriveIndex < DEFAULT_MAX_DRIVES) {

#ifdef Q_OS_WIN

        // Init Drive Root Path
        QString driveRoot = QString("%1:\\").arg((char)('a' + aDriveIndex));

        //qDebug() << "FileUtils::getDriveType - driveRoot: " << driveRoot;

        // Get Drive Type
        driveType = GetDriveType(reinterpret_cast<const wchar_t *>(driveRoot.utf16()));

#endif // Q_OS_WIN
    }

    return driveType;
}

//==============================================================================
// Get File Icon
//==============================================================================
QPixmap FileUtils::getFileIconPixmap(const QFileInfo& aInfo, const int& aWidth, const int& aHeight)
{
    // Init File Icon Provider
    QFileIconProvider iconProvider;

    // Get File Icon
    QIcon fileIcon = iconProvider.icon(aInfo);

    // Check File Icon
    if (!fileIcon.isNull()) {

        //qDebug() << "FileListDelegate::updateIcon - availableSizes: " << fileIcon.availableSizes();

        // Get Icon Pixmap
        QPixmap iconPixmap = fileIcon.pixmap(QSize(aWidth, aHeight), QIcon::Normal, QIcon::On);

        //qDebug() << "FileUtils::getFileIcon - size: " << iconPixmap.size() << " - depth: " << iconPixmap.depth();

        // Check Icon
        if (iconPixmap.isNull()) {
            qDebug() << "### FileUtils::getFileIcon - NULL PIXMAP!";

            return QPixmap(0, 0);
        }

        return iconPixmap;
    }

    qDebug() << "### FileUtils::getFileIcon - NULL ICON! ###";

    return QPixmap(0, 0);
}

#if defined(Q_OS_MAC)

//==============================================================================
// Get Mac CG Context
//==============================================================================
CGContextRef FileUtils::getMacCGContext(const QPaintDevice* aPaintDevice)
{
    // Init Flags
    uint flags = kCGImageAlphaPremultipliedFirst;

#ifdef kCGBitmapByteOrder32Host
    // Adjust Flags
    flags |= kCGBitmapByteOrder32Host;
#endif

    // Init Context Ref
    CGContextRef contextRef = NULL;

    // Check Paint Device Type - Works Only With QImage!
    if (aPaintDevice && aPaintDevice->devType() == QInternal::Image) {
        // Get Image
        QImage* image = (QImage*)aPaintDevice;
        // Get CG Context
        contextRef = CGBitmapContextCreate(image->bits(), image->width(), image->height(), DEFAULT_BITS_PER_COMPONENTS, image->bytesPerLine(), CGColorSpaceCreateDeviceRGB(), flags);
    }

    return contextRef;
}

//==============================================================================
// Get/Convert Mac Icon To QImage
//==============================================================================
OSStatus FileUtils::convertMacIcon(const IconRef& aMacIconref, QImage& aIconImage)
{
    // Fill Icon Image
    aIconImage.fill(QColor(0, 0, 0, 0));
    // Create Rect
    CGRect rect = CGRectMake(0, 0, aIconImage.width(), aIconImage.height());
    // Get Graphics Context Ref
    CGContextRef ctx = getMacCGContext(&aIconImage);
    // Init Color
    RGBColor color;
    // Reset Colors
    color.blue = color.green = color.red = 255 * 255;
    //color.blue = color.green = color.red = 0;

    // Init Status
    OSStatus status = noErr;

    // Plot Icon Ref In Context
    status = PlotIconRefInContext(ctx, &rect, kAlignNone, kTransformNone, &color, kPlotIconRefNormalFlags, aMacIconref);

    // Release Context
    CGContextRelease(ctx);

    // Return Icon Image
    return status;
}

#elif defined(Q_OS_WIN)


// ...


#else // Q_OS_UNIX


// ...


#endif // Q_OS_UNIX

//==============================================================================
// Get File Icon Pixmap
//==============================================================================
QImage FileUtils::getFileIconImage(const QFileInfo& aInfo, const int& aWidth, const int& aHeight)
{
    // Init New Image
    QImage newImage(aWidth, aHeight, QImage::Format_ARGB32_Premultiplied);

#if defined(Q_OS_MAC)

    //qDebug() << "FileUtils::getFileIconImage MAC - size: " << newImage.size();

    // Init File System Reference
    FSRef macRef;

    // Init Status
    OSStatus status = noErr;

    // Init Safe Counter
    int safeCount = 0;

    do {
        // Get file System Reference
        status = FSPathMakeRef(reinterpret_cast<const UInt8*>(aInfo.canonicalFilePath().toUtf8().constData()), &macRef, 0);

        // Check Status
        if (status != noErr) {
            qDebug() << "### FileUtils::getFileIconImage - FSPathMakeRef: " << status;
            // Increase Safe Count
            safeCount++;
            //return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
            continue;
        }

        // Init file System Catalog Info
        FSCatalogInfo info;

        // Init Mac Name
        HFSUniStr255 macName;

        // Get Catalog Info
        status = FSGetCatalogInfo(&macRef, kIconServicesCatalogInfoMask, &info, &macName, 0, 0);

        // Check Status
        if (status != noErr) {
            qDebug() << "### FileUtils::getFileIconImage - FSGetCatalogInfo: " << status;
            // Increase Safe Count
            safeCount++;
            //return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
            continue;
        }

        // Init Icon Reference
        IconRef iconRef;
        // Init Icon Label
        SInt16 iconLabel;

        // Get Icon Reference
        status = GetIconRefFromFileInfo(&macRef, macName.length, macName.unicode, kIconServicesCatalogInfoMask, &info, kIconServicesNormalUsageFlag, &iconRef, &iconLabel);

        // Check Status
        if (status != noErr) {
            qDebug() << "### FileUtils::getFileIconImage - GetIconRefFromFileInfo: " << status;
            // Release Icon Ref
            ReleaseIconRef(iconRef);
            // Increase Safe Count
            safeCount++;
            //return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
            continue;
        }

        // Convert Mac Icon
        status = convertMacIcon(iconRef, newImage);

        // Check Status
        if (status != noErr) {
            qDebug() << "### FileUtils::getFileIconImage - convertMacIcon: " << status;

            // Release Icon Ref
            //ReleaseIconRef(iconRef);
            //return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
            //continue;
        }

        // Release Icon Ref
        ReleaseIconRef(iconRef);
        // Increase Safe Count
        safeCount++;

    } while (status != noErr && safeCount < DEFAULT_LISTBOX_ICOM_GET_RETRY_COUNT_MAX);

    // Check Safe Count
    if (safeCount >= DEFAULT_LISTBOX_ICOM_GET_RETRY_COUNT_MAX) {
        // Init Painter
        QPainter painter(&newImage);
        // Fill Image
        newImage.fill(QColor(0, 0, 0, 0));
        // Draw Default Icon
        painter.drawImage(newImage.rect(), QImage(QString(":defaultIcon32x32")));
    }

#elif defined(Q_OS_WIN)

    // Init Painter
    QPainter painter(&newImage);

    newImage.fill(QColor(0, 0, 0, 0));

    painter.drawImage(newImage.rect(), QImage(QString(":defaultIcon32x32")));

#else // Q_OS_UNIX

    // Init Painter
    QPainter painter(&newImage);

    newImage.fill(QColor(0, 0, 0, 0));

    painter.drawImage(newImage.rect(), QImage(QString(":defaultIcon32x32")));

#endif // Q_OS_UNIX

    return newImage;
}

//==============================================================================
// Get Parent Dir Path
//==============================================================================
QString FileUtils::getParentDirPath(const QString& aDirPath)
{
    // Init File Info
    QFileInfo fileInfo(aDirPath.trimmed());

    // Check File Info
    if (fileInfo.isDir()) {
        // Check Dir Path
        if (aDirPath == QString("/") || aDirPath == QString("\\")) {
            return QString("/");
        }

        //qDebug() << "FileUtils::getParentDirPath - aDirPath: " << aDirPath;

        // Init Temp Path
        QString tempPath = aDirPath;

        // Check Temp Path
        if (!tempPath.endsWith(QString("/")) && !tempPath.endsWith(QString("\\"))) {
            // Add Slash
            tempPath += QString("/");
        }

        // Set File
        fileInfo.setFile(tempPath + QString(".."));

        // Set Temp Path
        tempPath = QDir::cleanPath(fileInfo.absoluteFilePath());
/*
        // Check Temp Path
        if (!tempPath.endsWith(QString("/")) && !tempPath.endsWith(QString("\\"))) {
            // Add Slash
            tempPath += QString("/");
        }
*/
        return tempPath;
    } else {
        qDebug() << "FileUtils::getParentDirPath - aDirPath: " << aDirPath << " - NOT A DIR!";
    }

    return QString("");
}

//==============================================================================
// Get Parent Dir Name
//==============================================================================
QString FileUtils::getParentDirName(const QString& aDirPath)
{
    // Get Parent Path
    QString tempPath = getParentDirPath(aDirPath);

    qDebug() << "FileUtils::getParentDirName - tempPath: " << tempPath;

    return getDirName(tempPath);
}

//==============================================================================
// Get Dir Name
//==============================================================================
QString FileUtils::getDirName(const QString& aDirPath)
{
    if (aDirPath == QString("/") || aDirPath == QString("\\")) {
        return QString("/");
    }

    // Init File Info
    QFileInfo fileInfo(aDirPath);

    //qDebug() << "FileUtils::getDirName - aDirPath: " << aDirPath;

    if (fileInfo.exists()) {
        // Check File Info
        if (fileInfo.isDir()) {
            // Init Temp Path
            QString tempPath = aDirPath;
            // Check Temp Path
            if (tempPath.endsWith(QString("/")) || tempPath.endsWith(QString("\\"))) {
                // Adjust Temp Path
                tempPath = tempPath.left(tempPath.length()-1);
                // Set File
                fileInfo.setFile(tempPath);
            }
        } else {
            qDebug() << "FileUtils::getDirName - aDirPath: " << aDirPath << " - NOT A DIR!";
        }
    } else {
        qDebug() << "FileUtils::getDirName - aDirPath: " << aDirPath << " - DOES NOT EXIST!";
    }

    return fileInfo.fileName();
}

//==============================================================================
// Get File Directory/Path
//==============================================================================
QString FileUtils::getFilePath(const QString& aFilePath)
{
    if (aFilePath == QString("/") || aFilePath == QString("\\")) {
        return QString("/");
    }

    // Init File Info
    QFileInfo fileInfo(aFilePath);

    return fileInfo.path();
}

//==============================================================================
// Format File Size
//==============================================================================
QString FileUtils::formatFileSize(const QFileInfo& aInfo)
{
    // Bundle Type
    //const int BundleType = 0x00080000;
/*
    if (aInfo.exists() && aInfo.isDir()) {
        // Get Path
        QCFType<CFStringRef> path = CFStringCreateWithBytes(0,
                (const UInt8*)aInfo.filePath().toAscii().data(), aInfo.filePath().length(), kCFStringEncodingUTF8, false);
        // Get URL
        QCFType<CFURLRef> url = CFURLCreateWithFileSystemPath(0, path, kCFURLPOSIXPathStyle, true);

        char type[4], creator[4];

        // Try To Get Bundle Package Info
        if (CFBundleGetPackageInfoInDirectory(url, (UInt32*)&type, (UInt32*)&creator)) {
            qDebug() << "FileUtils::formatFileSize - fileName: " << aInfo.fileName() << " - BUNDLE - type: " << QString(type).left(4);
        }
    }
*/

    // Check File Info
    if (aInfo.fileName() == QString("..")) {
        return QString(DEFAULT_FILE_SIZE_DIR);
    }

    // Check File Info
    if (aInfo.isBundle()) {
        // Check File Info
        if (aInfo.isExecutable()) {
            return QString(DEFAULT_FILE_SIZE_APP);
        }

        return QString(DEFAULT_FILE_SIZE_BUNDLE);
    }

    // Check File Info
    if (aInfo.isDir()) {
        return QString(DEFAULT_FILE_SIZE_DIR);
    }

    if (aInfo.isSymLink()) {
        return QString(DEFAULT_FILE_SIZE_LINK);
    }

    return formatSize(aInfo.size());
}

//==============================================================================
// Format Size
//==============================================================================
QString FileUtils::formatSize(const qint64& aSize)
{
    // Init Size String
    QString sizeString = QString("%L1").arg(aSize);

    // Check Size
    if (aSize > ONE_GIGA) {
        // Adjust Size String
        sizeString = QString("%L1M").arg(aSize / ONE_MEGA);
        return sizeString;
    }

    // Check Size
    if (aSize > ONE_MEGA) {
        // Adjust Size String
        sizeString = QString("%L1K").arg(aSize / ONE_KILO);
    }

    // Return Size String
    return sizeString;
}

//==============================================================================
// Format File Permissions
//==============================================================================
QString FileUtils::formatPermissions(const QFileInfo& aFileInfo)
{
    // Init Permissions String
    QString permsString = QString(DEFAULT_FILE_PERMISSION_NONE);

    QString dirPerm = QString("-");
    QString readPerm = QString("-");
    QString writePerm = QString("-");
    QString execPerm = QString("-");

    // Check File Info
    if (aFileInfo.isDir() && aFileInfo.isReadable() && aFileInfo.permission(QFile::ReadUser))
        // Set Dir Perm
        dirPerm = QString("d");

    // Check File Info
    if (aFileInfo.permission(QFile::ReadUser))
        // Set Read Perm
        readPerm = QString("r");

    // Check File Info
    if (aFileInfo.permission(QFile::WriteUser))
        // Set Write Perm
        writePerm = QString("w");

    // Check File Info
    if (aFileInfo.permission(QFile::ExeUser))
        // Set Write Perm
        execPerm = QString("x");

    // Set Permissions String
    permsString = QString(DEFAULT_PERMISSIONS_TEXT_TEMPLATE).arg(dirPerm)
                                                            .arg(readPerm)
                                                            .arg(writePerm)
                                                            .arg(execPerm);

    return permsString;
}

//==============================================================================
// Get Total Space In Directory
//==============================================================================
qint64 FileUtils::getTotalSpace(const QString& aDirPath)
{
    // Init File Info
    QFileInfo fileInfo(aDirPath);
    // Check Dir Path
    if (fileInfo.isDir()) {

        // Init Total Space
        quint64 fFree = 0;
        quint64 fTotal = 0;

#if defined(Q_OS_WIN)

        // Init User Free Space
        quint64 fUserFree = 0;

        // Get Free Space
        if (GetDiskFreeSpaceEx(aDirPath.toAscii().data(), &fUserFree, &fTotal, &fFree)) {
            return fTotal;
        }

#elif defined(Q_OS_MAC) || defined(Q_OS_UNIX)

        // Init Stats
        struct stat stst;
        struct statvfs stfs;

        // Get State
        if ( ::stat(aDirPath.toLocal8Bit(), &stst) == -1 )
            return 0;

        // Get Stat FS
        if ( ::statvfs(aDirPath.toLocal8Bit(), &stfs) == -1 )
            return 0;

        // Set Free Blocks
        fFree = stfs.f_bavail;
        // Set Total Blocks
        fTotal = stfs.f_blocks;

        return fTotal * stst.st_blksize;

#endif // Q_OS_WIN

    }

    return 0;
}

//==============================================================================
// Get Free Space In Directory
//==============================================================================
qint64 FileUtils::getFreeSpace(const QString& aDirPath)
{
    // Init File Info
    QFileInfo fileInfo(aDirPath);
    // Check Dir Path
    if (fileInfo.isDir()) {

        // Init Free Space
        quint64 fFree = 0;
        // Init Total Space
        quint64 fTotal = 0;

#if defined(Q_OS_WIN)

        // Init User Free Space
        quint64 fUserFree = 0;

        // Get Free Space
        if (GetDiskFreeSpaceEx(aDirPath.toAscii().data(), &fUserFree, &fTotal, &fFree)) {
            return fFree;
        }

#elif defined(Q_OS_MAC) || defined(Q_OS_UNIX)

        // Init Stats
        struct stat stst;
        struct statvfs stfs;

        // Get State
        if ( ::stat(aDirPath.toLocal8Bit(), &stst) == -1 )
            return 0;

        // Get Stat FS
        if ( ::statvfs(aDirPath.toLocal8Bit(), &stfs) == -1 )
            return 0;

        // Set Free Blocks
        fFree = stfs.f_bavail;
        // Set Total Blocks
        fTotal = stfs.f_blocks;

        return fFree * stst.st_blksize;

#endif // Q_OS_WIN

    }

    return 0;
}

//==============================================================================
// Get Last Error
//==============================================================================
int FileUtils::getLastError()
{
#if defined(Q_OS_WIN)
    return GetLastError();
#elif defined(Q_OS_UNIX)
    return errno;
#endif // Q_OS_UNIX
}

//==============================================================================
// Wild Card Matching
//==============================================================================
QString FileUtils::wildCardMatching(const QString& aSource, const QString& aPattern)
{
    qDebug() << "FileUtils::wildCardMatching - aSource: " << aSource << " - aPattern: " << aPattern;

    // Init Target Temp
    QString tTemp = QString("");
    // Init Target Source Pattern Length
    QString tsTemp = aPattern;

    // Get Target Pattern Length
    int tLength = tsTemp.length();
    // Get Source Name Length
    int sLength = aSource.length();

    // Go Thru Target Pattern
    for (int i=0, j=0; i<tLength && j<sLength; ++i) {
        // Check Current Char
        if (tsTemp[i] == '?') {
            // Add Char From Source
            tTemp += aSource[j];
            // Inc Source Pos
            j++;
        // Check Current Char
        } else if (tsTemp[i] == '*') {
            // Check Target Pattern Char Index
            if (i == tLength-1) {
                // Check Source Dot Pos
                int dPos = aSource.indexOf(QChar('.'), j);
                // Check Dot Pos
                if (dPos >= 0) {
                    // Adding The Rest Of The Source
                    tTemp += aSource.right(aSource.length()-dPos-1);
                } else {
                    // Adding The Rest Of The Source
                    tTemp += aSource.right(aSource.length()-j);
                }
            // Check Target Pattern Char Index And Source Char Index
            } else if (/*i == 0 &&*/ i<(tLength-1) && j<sLength) {
                // Get Next Char
                QChar nextChar = tsTemp[i+1];
                // Check Dot Pos
                int dPos = aSource.indexOf(QChar('.'), j);
                // Check Next Char
                if (nextChar == QChar('.') && dPos >= 0) {
                    // Copy Source While Not Matching Dot
                    while (aSource[j] != QChar('.') && j<sLength) {
                        // Add Source Char
                        tTemp += aSource[j];
                        // Inc Source Index
                        j++;
                    }
                } else {
                    // Copy Source While Not Matching Next Char
                    while (aSource[j] != nextChar && j<sLength) {
                        // Add Source Char
                        tTemp += aSource[j];
                        // Inc Source Index
                        j++;
                    }
                }
            }
        } else {
            // Add Char From Target
            tTemp += tsTemp[i];
            // Inc Source Pos
            j++;
        }
    }

    return tTemp;
}

//==============================================================================
// Parse Target File Name
//==============================================================================
QString FileUtils::parseTargetFileName(const QString& aSourceName, const QString& aTargetName)
{
    //qDebug() << "FileUtils::parseTargetFileName - aSN: " << aSourceName << " - aTN: " << aTargetName;

    // Init Target Temp
    QString tTemp = QString("");
    // Init Target Source Temp
    QString tsTemp = aTargetName;

    // Get Slash Last Pos
    int slp = aTargetName.lastIndexOf('/');
    // Get Back Slash Last Pos
    int bslp = aTargetName.lastIndexOf('\\');

    // Check Last Slash Pos And Last Back Slash Pos
    if (slp >= 0 || bslp >= 0) {
        // Add Partial Target Name
        tTemp += aTargetName.left(qMax(slp, bslp) + 1);
        // Set Target Source Temp
        tsTemp = aTargetName.right(aTargetName.length() - tTemp.length());
    }

    // Check Target Name
    if (tsTemp.isEmpty() || tsTemp == QString("*.*") || tsTemp == QString("*")) {
        // Add Source Name
        tTemp += aSourceName;
    // Check Target Name
    } else if (!tsTemp.isEmpty() && !tsTemp.contains(QString("*")) && !tsTemp.contains(QString("?"))) {
        // Add Target Name
        tTemp += tsTemp;
    } else {
        // Add Wild Card Matching String To Target
        tTemp += FileUtils::wildCardMatching(aSourceName, tsTemp);
    }

    return tTemp;
}

//==============================================================================
// Create File Operation Entry
//==============================================================================
FileOperationEntry* FileUtils::createFileOperationEntry(FileOpQueueHandler* aQueueHandler,
                                                        const int& aOperation,
                                                        const QString& aSourceDir,
                                                        const QString& aSourceName,
                                                        const QString& aTargetDir,
                                                        const QString& aTargetName)
{
    if (!aSourceDir.isEmpty() && !aTargetDir.isEmpty()) {
        //qDebug() << "FileUtils::createFileOperationEntry - aOperation: " << aOperation << " - aSD: " << aSourceDir << " - aSN: " << aSourceName << " - aTD: " << aTargetDir << " - aTN: " << aTargetName;
        // Init Source Temp
        QString sTemp = aSourceDir;

        // Check Source Temp
        if (!sTemp.endsWith(QDir::separator())) {
            // Add Slash
            sTemp += QDir::separator();
        }

        // Add Source Name
        sTemp += aSourceName;

        // Init Target Temp
        QString tTemp = aTargetDir;

        // Check Target Temp
        if (!tTemp.endsWith(QDir::separator())) {
            // Add Slash
            tTemp += QDir::separator();
        }

        // Check If Target Name Is Relative
        if (QDir::isAbsolutePath(aTargetName)) {
            // Set Parsed Target Name
            tTemp = FileUtils::parseTargetFileName(aSourceName, aTargetName);
        } else {
            // Add Parsed Target Name
            tTemp += FileUtils::parseTargetFileName(aSourceName, aTargetName);
        }

        // Check Source And Target
        if (!sTemp.isEmpty() && !tTemp.isEmpty()) {
            // Create New File Operation Entry
            return new FileOperationEntry(aQueueHandler, aOperation, sTemp, tTemp);
        }
    }

    return NULL;
}

//==============================================================================
// Is Full Path
//==============================================================================
bool FileUtils::isFullPath(const QString& aFilePath)
{
    // Check File Path
    if (!aFilePath.isEmpty()) {

#if defined (Q_OS_WIN)

        // Check File Path
        if (aFilePath.at(0).toLower().toAscii() >= 'a' && aFilePath.at(0).toLower().toAscii() <= 'z' &&
            aFilePath.at(1).toAscii() == ':' && (aFilePath.at(2).toAscii() == '\\' || aFilePath.at(2).toAscii() == '/')) {

            return true;
        }

        return false;

#else // MAC || UNIX
        // Check The Beginning
        if (aFilePath.startsWith(QChar('/')) || aFilePath.startsWith(QChar('\\'))) {
            return true;
        }
#endif // MAC || UNIX
    }

    return false;
}

/*
//==============================================================================
// Get Authorization
//==============================================================================
bool FileUtils::getAuthorization()
{
#if defined (Q_OS_MAC)

    //AuthorizationRef myAuthorizationRef;

    // Create Authorization
    OSStatus myStatus = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &myAuthorizationRef);

    // Check Status
    if (myStatus == errAuthorizationSuccess) {

        // Init Right
        AuthorizationItem right = { "com.mycompany.myapplication.whatever", 0, NULL, 0 };
        AuthorizationItem admin = { kAuthorizationRightExecute, 0, NULL, 0};
        // Init Rights
        AuthorizationItem rights[2];
        // Set Rights
        rights[0] = right;
        rights[1] = admin;
        // Init Rights
        AuthorizationRights rightSet = { 2, rights };
        // Init Flags
        AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagPreAuthorize | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagExtendRights;

        // Adding Rights
        myStatus = AuthorizationCopyRights(myAuthorizationRef, &rightSet, kAuthorizationEmptyEnvironment, flags, NULL);
    }

    return (myStatus == errAuthorizationSuccess);

#elif defined (Q_OS_UNIX)

#else // Q_OS_WIN

#endif // Q_OS_WIN

    return false;
}

//==============================================================================
// Release Authorization
//==============================================================================
void FileUtils::releaseAuthorization()
{
#if defined (Q_OS_MAC)

    // Free Authorization
    AuthorizationFree(myAuthorizationRef, kAuthorizationFlagDestroyRights);

#elif defined (Q_OS_UNIX)

#else // Q_OS_WIN

#endif
}
*/

//==============================================================================
// Sort File Info List
//==============================================================================
void FileUtils::sortFileList(QFileInfoList& aFileList, const FileSortType& aSortType, const bool& aReverse)
{
    // Get File List Count
    int flCount = aFileList.count();

    qDebug() << "FileUtils::sortFileList - aSortType: " << aSortType;

    // Switch Sorting Method
    switch (aSortType) {
        default:
        case EFSTName:          quickSort(aFileList, 0, flCount-1, nameSort, aReverse); break;
        case EFSTExtension:     quickSort(aFileList, 0, flCount-1, extSort,  aReverse); break;
        case EFSTSize:          quickSort(aFileList, 0, flCount-1, sizeSort, aReverse); break;
        case EFSTDate:          quickSort(aFileList, 0, flCount-1, dateSort, aReverse); break;
        case EFSTOwnership:     quickSort(aFileList, 0, flCount-1, ownSort,  aReverse); break;
        case EFSTPermission:    quickSort(aFileList, 0, flCount-1, permSort, aReverse); break;
        case EFSTAttributes:    quickSort(aFileList, 0, flCount-1, attrSort, aReverse); break;
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileUtils::~FileUtils()
{
    // ...
}






















//==============================================================================
// Constructor
//==============================================================================
FileUtilThreadBase::FileUtilThreadBase(QObject* aParent)
    : QThread(aParent)
    , running(false)
    , restart(false)
    , abort(false)
{
    qDebug() << "Creating FileUtilThreadBase...done";
}

//==============================================================================
// Start Operation
//==============================================================================
void FileUtilThreadBase::startOperation()
{
    // Lock Mutex
    mutex.lock();
    // Check If Running
    if (!isRunning()) {
        qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::startOperation - THREAD START";
        // Reset Restart
        restart = false;
        // Reset Abort
        abort = false;
        // Start
        start(QThread::LowestPriority);
    } else {
        qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::startOperation - THREAD RESTART";
        // Set Restart
        restart = true;
        // Reset Abort
        abort = false;
        // Wake Condition
        condition.wakeOne();
    }
    // Unlock Mutex
    mutex.unlock();
}

//==============================================================================
// Restart Operation
//==============================================================================
void FileUtilThreadBase::restartOperation()
{
    qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::restartOperation";

    // Original Start Operation
    FileUtilThreadBase::startOperation();
}

//==============================================================================
// Pause
//==============================================================================
void FileUtilThreadBase::pause()
{
    qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::pause";

    // Lock
    mutex.lock();
}

//==============================================================================
// Resume
//==============================================================================
void FileUtilThreadBase::resume()
{
    qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::resume";

    // Unlock
    mutex.unlock();
}

//==============================================================================
// Run
//==============================================================================
void FileUtilThreadBase::run()
{
    //qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::run";

    // 4 Ever Loop
    forever {
        // Check Abort
        if (abort)
            return;

        // Emit Operation Started
        emit opStarted();

        qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::run - started";
        // Set Internal Running State
        running = true;
        // Do Operation
        doOperation();
        // Check Abort
        if (abort)
            return;

        qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::run - finished";

        // Emit Operation Finished
        emit opFinished();
        // Check Restart
        if (restart) {
            qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::run - restarting";
            // Reset Restart
            restart = false;
            //break;
            continue;
        }

        qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::run - idle";
        // Lock Mutext
        mutex.lock();
        // Check Restart
        if (!restart)
            // Wait Condition
            condition.wait(&mutex);
        // Reset Restart
        restart = false;
        // Reset Running
        running = false;
        // Unlock Mutex
        mutex.unlock();
    }

    qWarning() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::run - OUTSIDE LOOP";
}

//==============================================================================
// Stop
//==============================================================================
void FileUtilThreadBase::stop()
{
    // Check Internal Running State
    if (!running)
        return;

    // Lock Mutex
    mutex.lock();
    // Check If Running
    if (isRunning()) {
        // Check Abort
        if (!abort) {
            qDebug() << (metaObject() ? metaObject()->className() : "FileUtilThreadBase") << "::stop";
            // Set Abort
            abort = true;
            // Reset Restart
            restart = false;
            // Emit Operatoin Stopped Signal
            emit opStopped();
            // Wake Up
            condition.wakeOne();
        }
    }
    // Set Internal Running State
    running = false;
    // Unlock Mutex
    mutex.unlock();
}

//==============================================================================
// Is Stopping
//==============================================================================
bool FileUtilThreadBase::stopping()
{
    return ((running || isRunning()) && abort);
}

//==============================================================================
// Destructor
//==============================================================================
FileUtilThreadBase::~FileUtilThreadBase()
{
    qDebug() << "Deleting FileUtilThreadBase...";
    // Lock Mutex
    mutex.lock();
    // Set Abort
    abort = true;
    // Wake Up
    condition.wakeOne();
    // Unlock Mutex
    mutex.unlock();
    // Wait
    wait();

    qDebug() << "Deleting FileUtilThreadBase... Done";
}























//==============================================================================
// Constructor
//==============================================================================
DirReader::DirReader(QObject* aParent)
    : FileUtilThreadBase(aParent)
    , sortType(EFSTName)
    , reversed(false)
    , showHidden(false)
{
    qDebug() << "Creating DirReader...Done";
}

//==============================================================================
// Is Dir Readable
//==============================================================================
bool DirReader::isReadable(const QString& aDirPath)
{
    // Init File Info
    QFileInfo fileInfo(aDirPath);

    return fileInfo.isReadable();
}

//==============================================================================
// Read Dir
//==============================================================================
void DirReader::readDir(const QString& aDirPath, const FileSortType& aSortType, const bool& aReverse, const bool& aHidden, const QStringList& aNameFilters)
{
    // Check Current Dir Path, Sort Type, etc...
    //if (dirPath != aDirPath || sortType != aSortType || reversed != aReverse || showHidden != aHidden || nameFilters != aNameFilters)
    {
        // Stop
        stop();
        // Set Dir Path
        dirPath = aDirPath;
        // Set Sort Type
        sortType = aSortType;
        // Set Revese Order
        reversed = aReverse;
        // Set Show Hidden
        showHidden = aHidden;
        // Set Name Filters
        nameFilters = aNameFilters;

        // Check Dir Path
        if (!dirPath.isEmpty()) {
            qDebug() << "DirReader::readDir - aDirPath: " << aDirPath;
            // Start New Operation
            startOperation();
        } else {
            qDebug() << "DirReader::readDir - aDirPath: " << aDirPath << " - EMPTY!";
        }
    }
}

//==============================================================================
// Do Operation
//==============================================================================
void DirReader::doOperation()
{
    //qDebug() << "DirReader::doOperation - dirPath: " << dirPath;
    DEFAULT_THREAD_ABORT_CHECK;

    // Init File Util
    FileUtils fileUtil;

    DEFAULT_THREAD_ABORT_CHECK;

    // Get File List
    QFileInfoList fileList = fileUtil.getDirList(dirPath, sortType, showHidden, reversed, nameFilters);

    DEFAULT_THREAD_ABORT_CHECK;

    // Get File List Count
    int flCount = fileList.count();

    DEFAULT_THREAD_ABORT_CHECK;

    // Go Thru File List
    for (int i=0; i<flCount; i++) {

        DEFAULT_THREAD_ABORT_CHECK;
        //qDebug() << fileList[i].absoluteFilePath();

        // Emit Entry Found Signal
        emit entryFound(fileList[i].absoluteFilePath());

        DEFAULT_THREAD_ABORT_CHECK;
    }

    //qDebug() << "DirReader::doOperation - dirPath: " << dirPath << " done.";
}

//==============================================================================
// Destructor
//==============================================================================
DirReader::~DirReader()
{
    qDebug() << "Deleting DirReader...Done";
}























//==============================================================================
// Constructor
//==============================================================================
DirSizeScanner::DirSizeScanner(QObject* aParent)
    : FileUtilThreadBase(aParent)
    , updateTimerID(-1)
{
    qDebug() << "Creating DirSizeScanner...Done";
}

//==============================================================================
// Scan Dir Size
//==============================================================================
void DirSizeScanner::scanDirSize(const QString& aDirPath)
{
    // Stop
    stop();
    // Set Dir Path
    dirPath = aDirPath;
    // Check Dir Path
    if (!dirPath.isEmpty()) {
        qDebug() << "DirSizeScanner::scanDirSize - aDirPath: " << aDirPath;
        // Reset Dir Size
        dirSize = 0;
        // Reset Previous Dir size
        prevDirSize = 0;
        // Start New Operation
        startOperation();
        // Start Update Timer
        startUpdateTimer();
    } else {
        qDebug() << "DirSizeScanner::scanDirSize - aDirPath: " << aDirPath << " - EMPTY!";
    }
}

//==============================================================================
// Get Dir Size
//==============================================================================
qint64 DirSizeScanner::getDirSize()
{
    return dirSize;
}

//==============================================================================
// Stop
//==============================================================================
void DirSizeScanner::stop()
{
    // Stop Update Timer
    stopUpdateTimer();

    // Stop
    FileUtilThreadBase::stop();
}

//==============================================================================
// Start Update Timer
//==============================================================================
void DirSizeScanner::startUpdateTimer()
{
    // Stop Update Timer First
    stopUpdateTimer();

    // Check Timer ID
    if (updateTimerID == -1) {
        //qDebug() << "DirSizeScanner::startUpdateTimer";
        // Start Timer
        updateTimerID = startTimer(DEFAULT_DIR_SIZE_SCANNER_UPDATE_TIMER_INTERVAL);
    }
}

//==============================================================================
// Stop Update Timer
//==============================================================================
void DirSizeScanner::stopUpdateTimer()
{
    // Check Timer ID
    if (updateTimerID != -1) {
        //qDebug() << "DirSizeScanner::stopUpdateTimer";
        // Kill Tiemr
        killTimer(updateTimerID);
        // Reset Timer ID
        updateTimerID = -1;
    }
}

//==============================================================================
// Notify Size Changed
//==============================================================================
void DirSizeScanner::notifySizeChanged()
{
    // Check Prev Dir Size
    if (prevDirSize != dirSize) {
        //qDebug() << "DirSizeScanner::notifySizeChanged - dirSize: " << dirSize;
        // Set Prev Dir Size
        prevDirSize = dirSize;
        // Emit Size Update Signal
        emit sizeUpdate(dirSize);
    }
}

//==============================================================================
// Do Operation
//==============================================================================
void DirSizeScanner::doOperation()
{
    qDebug() << "DirSizeScanner::doOperation - dirPath: " << dirPath;
    DEFAULT_THREAD_ABORT_CHECK;
    // Get Dir Size
    fileUtils.getDirSize(dirPath, dirSize, abort);
    DEFAULT_THREAD_ABORT_CHECK;
    // Stop Update Timer
    stopUpdateTimer();
    // Notify Size Changed
    notifySizeChanged();
    qDebug() << "DirSizeScanner::doOperation - dirPath: " << dirPath << " done.";
}

//==============================================================================
// Timer Event
//==============================================================================
void DirSizeScanner::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Event's Timer ID
        if (aEvent->timerId() == updateTimerID) {
            // Notify Size Changed
            notifySizeChanged();
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
DirSizeScanner::~DirSizeScanner()
{
    // Stop Update Timer
    stopUpdateTimer();

    qDebug() << "Deleting DirSizeScanner...Done";
}


































//==============================================================================
// Constructor
//==============================================================================
FileOpQueueHandler::FileOpQueueHandler(FileOpQueueViewAPI* aOpQViewAPI,
                                       DirCreatorObserver* aDirCreatorObserver,
                                       FileDeleteObserver* aDeleteObserver,
                                       FileCopyObserver* aCopyObserver,
                                       FileMoveObserver* aMoveObserver,
                                       FileRenameObserver* aRenameObserver,
                                       QObject* aParent)
    : FileUtilThreadBase(aParent)
    , opQView(aOpQViewAPI)
    , dirCreatorObserver(aDirCreatorObserver)
    , deleteObserver(aDeleteObserver)
    , copyObserver(aCopyObserver)
    , moveObserver(aMoveObserver)
    , renameObserver(aRenameObserver)
    , createDirFlags(0)
    , copyFlags(0)
    , moveFlags(0)
    , renameFlags(0)
    , deleteFlags(0)
{
    qDebug() << "Creating FileOperationQueue...";

    // ...

    qDebug() << "Creating FileOperationQueue...done";
}

//==============================================================================
// Add Operation Entry
//==============================================================================
void FileOpQueueHandler::addOperation(FileOperationEntry* aEntry)
{
    // Check Entry
    if (aEntry) {
        qDebug() << "FileOperationQueue::addOperation - aEntry: " << aEntry->getSource();

        // Set Observers
        aEntry->setObservers(deleteObserver, copyObserver, moveObserver, renameObserver);

        // Add To Operations
        operations << aEntry;

        // Update Entry's Index
        aEntry->queueIndex = operations.indexOf(aEntry);

        // Check Entry Queue Handler
        if (aEntry->queueHandler != this) {
            // Set Entry's Queue handler
            aEntry->queueHandler = this;
        }

        // Get Operations
        int opCount = operations.count();

        // Signal Operation Added
        emit operationAdded(opCount-1, opCount);

        msleep(100);
    }
}

//==============================================================================
// Insert Operation Entry
//==============================================================================
void FileOpQueueHandler::insertOperation(FileOperationEntry* aEntry, const int& aIndex)
{
    // Check Entry
    if (aEntry) {
        // Get Entry Count
        int oCount = operations.count();

        // Init insertion Index
        int insertedIndex = aIndex;

        // Check Index
        if (aIndex < 0) {
            // Set Inserted Index
            insertedIndex = 0;
        }
        // Check _Index
        if (aIndex >= oCount) {
            // Set Inserted Index
            insertedIndex = oCount;
        }

        qDebug() << "FileOperationQueue::insertOperation - aEntry: " << aEntry->getSource() << " - insertedIndex: " << insertedIndex;

        // Set Observers
        aEntry->setObservers(deleteObserver, copyObserver, moveObserver, renameObserver);

        // Add To Operations
        operations.insert(insertedIndex, aEntry);

        // Update Entry's Index
        aEntry->queueIndex = operations.indexOf(aEntry);

        // Check Entry Queue Handler
        if (aEntry->queueHandler != this) {
            // Set Entry's Queue handler
            aEntry->queueHandler = this;
        }

        // Get Operations
        int opCount = operations.count();

        // Signal Operation Added
        emit operationAdded(insertedIndex, opCount);

        msleep(100);
    }
}

//==============================================================================
// Remove Operation
//==============================================================================
void FileOpQueueHandler::removeOperation(const int& aIndex)
{
    // Get Entry Count
    int oCount = operations.count();
    // Check Index
    if (aIndex >= 0 && aIndex < oCount) {
        qDebug() << "FileOperationQueue::removeOperation - aIndex: " << aIndex;
        // Get Entry
        FileOperationEntry* op = operations[aIndex];
        // Check Entry
        if (op) {
            // Delete Operation
            delete op;
            op = NULL;
        }

        // Remove
        operations.removeAt(aIndex);

        // Get Operations
        int opCount = operations.count();

        // Emit Operation Removed
        emit operationRemoved(aIndex, opCount);

        msleep(100);
    }
}

//==============================================================================
// Process Queue
//==============================================================================
void FileOpQueueHandler::processQueue()
{
    qDebug() << "FileOperationQueue::processQueue";
    // Start Operation
    startOperation();
}

//==============================================================================
// Get Operation Entry
//==============================================================================
FileOperationEntry* FileOpQueueHandler::getOperation(const int& aIndex)
{
    // Get Entry Count
    int oCount = operations.count();
    // Check Index
    if (aIndex >= 0 && aIndex < oCount) {
        return operations[aIndex];
    }

    return NULL;
}

//==============================================================================
// Get Operations Count
//==============================================================================
int FileOpQueueHandler::count()
{
    return operations.count();
}

//==============================================================================
// Get Operations Index
//==============================================================================
int FileOpQueueHandler::getOperationIndex(FileOperationEntry* aEntry)
{
    return operations.indexOf(aEntry);
}

//==============================================================================
// Clear
//==============================================================================
void FileOpQueueHandler::clear()
{
    qDebug() << "FileOperationQueue::clear";

    // Stop
    stop();

    // Get Entry Count
    int oCount = operations.count();
    // Go Thru Operations List
    for (int i=oCount-1; i>=0; --i) {
        // Get Entry
        FileOperationEntry* op = operations[i];
        // Check Entry
        if (op) {
            // Delete Operation
            delete op;
            op = NULL;
        }
    }

    // Clear
    operations.clear();

    // Reset Flags
    resetFlags();
}

//==============================================================================
// Export Operations List Into a String List
//==============================================================================
QStringList FileOpQueueHandler::exportList()
{
    qDebug() << "FileOperationQueue::exportList";
    // Init Result List
    QStringList result;
    // Get Entry Count
    int oCount = operations.count();
    // Go Thru Operations List
    for (int i=0; i<oCount; ++i) {
        // Get Entry
        FileOperationEntry* op = operations[i];
        // Check Entry
        if (op) {
            // Add Entry To Result
            result << op->getOperation();
        }
    }

    return result;
}

//==============================================================================
// Do Operation
//==============================================================================
void FileOpQueueHandler::doOperation()
{
    qDebug() << "FileOperationQueue::doOperation";

    DEFAULT_THREAD_ABORT_CHECK;

    // Get Operations Count
    //int qCount = operations.count();

    DEFAULT_THREAD_ABORT_CHECK;

    // Go Through Operations List
    for (int i=0; i<operations.count(); ) {

        DEFAULT_THREAD_ABORT_CHECK;

        // Set Current Operation Index
        currOp = i;

        // Get File Operation Entry
        FileOperationEntry* entry = operations[i];

        // Check File Operation Entry
        if (entry && ((entry->opState != FOSDone && entry->opState != FOSFailed && entry->opState != FOSRunning) /*|| (entry->opState == FOSRunning && entry->deleteDirAfter)*/)) {
            // Emit Operation Started Signal
            emit operationStarted(currOp);

            DEFAULT_THREAD_ABORT_CHECK;

            // Process Entry
            if (processEntry(entry, i)) {

                // Success

            } else {

                // No Success

            }

            // Check If Entry Delete Dir After Flag Has Been Set - Deleting Directory
            //if (!entry->deleteDirAfter) {
            if (entry->opState != FOSDeleteLater) {
                // Emit Operation Completed
                emit operationCompleted(currOp);

                // Increase Loop counter
                i++;
            }

            DEFAULT_THREAD_ABORT_CHECK;

        } else {
            // Increase Loop counter
            i++;
        }

        DEFAULT_THREAD_ABORT_CHECK;

        // Reset Current Operation Index
        currOp = -1;

        // Update Operation Count
        //qCount = operations.count();
    }

    DEFAULT_THREAD_ABORT_CHECK;

    qDebug() << "FileOperationQueue::doOperation...done";
}

//==============================================================================
// Reset Flags
//==============================================================================
void FileOpQueueHandler::resetFlags()
{
    qDebug() << "FileOperationQueue::resetFlags";

    createDirFlags = 0;
    copyFlags = 0;
    moveFlags = 0;
    renameFlags = 0;
    deleteFlags = 0;
}

//==============================================================================
// Process Entry
//==============================================================================
bool FileOpQueueHandler::processEntry(FileOperationEntry* aEntry, const int& aIndex)
{
    // Check Entry
    if (!aEntry) {
        qDebug() << "FileOpQueueHandler::processEntry - NO ENTRY!";
        return false;
    }

    // Check If Entry Is ready To Be Processed
    if (!aEntry->isEntryReadyToBeProcessed()) {
        qDebug() << "FileOpQueueHandler::processEntry - ENTRY [" << aIndex << "] IS NOT READY!";
        return false;
    }

    // Init Result
    bool result = false;

    // Init File Utils Response
    int fuResp = FILE_UTILS_RESPONSE_NOERROR;

    // Switch Entry Operation Index
    switch (aEntry->opIndex) {
        default:
        case OPERATION_ID_NOOP:
            qDebug() << "FileOperationQueue::processEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - NOOP";

            // Do Nothing

            // Set Result
            result = true;
        break;

        case OPERATION_ID_MAKEDIR:
            qDebug() << "FileOperationQueue::processEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - MAKEDIR";
            // Process Dir Entry
            result = processDirEntry(aEntry, aIndex);
        break;

        case OPERATION_ID_COPY: {
            // Init Source Info
            QFileInfo sourceInfo(aEntry->source);
            // Check Source Info If It's a Dir
            if (sourceInfo.isDir() && !FileUtils::isDirEmpty(aEntry->source)) {
                // Process Dir Entry
                result = processDirEntry(aEntry, aIndex);
            } else {
                qDebug() << "FileOperationQueue::processEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - COPY";
                // Set Entry Running State
                aEntry->opState = FOSRunning;
                // Copy File
                //fuResp = FileUtils::copyFile(aEntry->source, aEntry->target, copyFlags, abort, copyObserver);
                fuResp = FileUtils::copyFile(aEntry->source, aEntry->target, copyFlags, abort, aEntry);
                // Set Result
                result = (fuResp == FILE_UTILS_RESPONSE_NOERROR);
                // Set Entry State
                aEntry->setOpStateByFileUtilsResponse(fuResp);
            }
        } break;

        case OPERATION_ID_MOVE: {
            // Init Source Info
            QFileInfo sourceInfo(aEntry->source);
            // Check Source Info If It's a Dir
            if (sourceInfo.isDir() && !FileUtils::isDirEmpty(aEntry->source)) {
                // Process Dir Entry
                result = processDirEntry(aEntry, aIndex);
            } else {
                qDebug() << "FileOperationQueue::processEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - MOVE";
                // Set Entry Running State
                aEntry->opState = FOSRunning;
                // Move File
                fuResp = FileUtils::moveFile(aEntry->source, aEntry->target, moveFlags, abort, aEntry);
                // Set Result
                result = (fuResp == FILE_UTILS_RESPONSE_NOERROR);
                // Set Entry State
                aEntry->setOpStateByFileUtilsResponse(fuResp);
            }
        } break;

        case OPERATION_ID_RENAME: {
            qDebug() << "FileOperationQueue::processEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - RENAME";
            // Set Entry Running State
            aEntry->opState = FOSRunning;
            // Rename File
            fuResp = FileUtils::renameFile(aEntry->source, aEntry->target, moveFlags, abort, aEntry);
            // Set Result
            result = (fuResp == FILE_UTILS_RESPONSE_NOERROR);
            // Set Entry State
            aEntry->setOpStateByFileUtilsResponse(fuResp);
        } break;

        case OPERATION_ID_DELETE: {
            // Init Source Info
            QFileInfo sourceInfo(aEntry->source);
            // Check Source Info If It's a Dir
            if (sourceInfo.isDir() && !FileUtils::isDirEmpty(aEntry->source)) {
                // Process Dir Entry
                result = processDirEntry(aEntry, aIndex);
            } else {
                qDebug() << "FileOperationQueue::processEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - DELETE";
                // Set Entry Running State
                aEntry->opState = FOSRunning;
                // Delete File
                fuResp = FileUtils::deleteFile(aEntry->source, deleteFlags, abort, aEntry);
                // Set Result
                result = (fuResp == FILE_UTILS_RESPONSE_NOERROR);
                // Set Entry State
                aEntry->setOpStateByFileUtilsResponse(fuResp);
            }
        } break;
    }

    qDebug() << "FileOperationQueue::processEntry...done";

    msleep(200);    // For Debugging

    return result;
}

//==============================================================================
// Process Directory Entry
//==============================================================================
bool FileOpQueueHandler::processDirEntry(FileOperationEntry* aEntry, const int& aIndex)
{
    // Check Entry
    if (!aEntry) {
        qDebug() << "FileOpQueueHandler::processDirEntry - NO ENTRY!";
        return false;
    }

    // Check If Entry Is ready To Be Processed
    if (!aEntry->isEntryReadyToBeProcessed()) {
        qDebug() << "FileOpQueueHandler::processDirEntry - ENTRY [" << aIndex << "] IS NOT READY!";
        return false;
    }

    // Init Result
    bool result = false;

    // Init File Utils Response
    int fuResp = FILE_UTILS_RESPONSE_NOERROR;

    DEFAULT_THREAD_ABORT_CHECK_FALSE;

    // Switch Entry Operation Index
    switch (aEntry->opIndex) {
        default:
        case OPERATION_ID_NOOP:
            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - NOOP";

            // Do Nothing

            // Set Result
            result = true;
        break;

        case OPERATION_ID_MAKEDIR:
            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - MAKEDIR";
            // Set Entry Operation State
            aEntry->opState = FOSRunning;
            // Create  Dir
            fuResp = FileUtils::createDir(aEntry->source, createDirFlags, abort, dirCreatorObserver);
            // Set Result
            result = (fuResp == FILE_UTILS_RESPONSE_NOERROR);
            // Set Entry State
            aEntry->setOpStateByFileUtilsResponse(fuResp);
        break;

        case OPERATION_ID_COPY: {
            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - COPY";
            // Set Entry Operation State
            aEntry->opState = FOSRunning;
            // Init Source Dir To Read Entries
            QDir sourceDir(aEntry->source);
            // Init Source Dir Temp
            QString sourceDirTemp = aEntry->source;
            // Init Target Dir Temp
            QString targetDirTemp = aEntry->target;

            // Compare Source And Target Dir File Names
            if (sourceDirTemp == targetDirTemp) {
                // Check Copy Observer
                if (copyObserver) {
#if defined (Q_OS_WIN)
                    // Set Last Error
                    int lastError = ERROR_INVALID_PARAMETER;
#elif defined (Q_OS_MAC) || defined (Q_OS_UNIX)
                    // Set Last Error Manually
                    int lastError = EINVAL;
#endif // Q_OS_MAC || Q_OS_UNIX
                    // Notify Observer For Now  - TODO: Add Possibility For Rename
                    copyObserver->copyError(sourceDirTemp, targetDirTemp, lastError);
                }

                // Set Entry Operation State
                aEntry->opState = FOSFailed;

                return false;
            }

            // Check Source Dir Temp Path
            if (!sourceDirTemp.endsWith(QString("/")) && !sourceDirTemp.endsWith(QString("\\"))) {
                // Add Slash
                sourceDirTemp += QDir::separator();
            }

            // Check Target Dir Temp Path
            if (!targetDirTemp.endsWith(QString("/")) && !targetDirTemp.endsWith(QString("\\"))) {
                // Add Slash
                targetDirTemp += QDir::separator();
            }

            // Get Entry Info List
            QFileInfoList sourceEntryInfoList = sourceDir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

            DEFAULT_THREAD_ABORT_CHECK_FALSE;

            // Get Source Entry List Count
            int seilCount = sourceEntryInfoList.count();

            // Go Through Source Entry Info List
            for (int i=0; i<seilCount; ++i) {

                DEFAULT_THREAD_ABORT_CHECK_FALSE;

                // Create new Entry
                FileOperationEntry* newEntry = FileUtils::createFileOperationEntry(this, aEntry->opIndex, sourceDirTemp, sourceEntryInfoList[i].fileName(), targetDirTemp, sourceEntryInfoList[i].fileName());
                // Add Operation
                addOperation(newEntry);
            }

            // Create Target Dir
            fuResp = FileUtils::createDir(aEntry->target, createDirFlags, abort, dirCreatorObserver);
            // Set Entry State
            aEntry->setOpStateByFileUtilsResponse(fuResp);
        } break;

        case OPERATION_ID_MOVE: {
            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - MOVE";
            // Set Entry Operation State
            aEntry->opState = FOSRunning;
            // Init Source Dir To Read Entries
            QDir sourceDir(aEntry->source);
            // Init Source Dir Temp
            QString sourceDirTemp = aEntry->source;
            // Init Target Dir Temp
            QString targetDirTemp = aEntry->target;

            // Check Source Dir Temp Path
            if (!sourceDirTemp.endsWith(QString("/")) && !sourceDirTemp.endsWith(QString("\\"))) {
                // Add Slash
                sourceDirTemp += QDir::separator();
            }

            // Check Target Dir Temp Path
            if (!targetDirTemp.endsWith(QString("/")) && !targetDirTemp.endsWith(QString("\\"))) {
                // Add Slash
                targetDirTemp += QDir::separator();
            }

            // Get Entry Info List
            QFileInfoList sourceEntryInfoList = sourceDir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

            DEFAULT_THREAD_ABORT_CHECK_FALSE;

            // Get Source Entry List Count
            int seilCount = sourceEntryInfoList.count();

            // Go Through Source Entry Info List
            for (int i=0; i<seilCount; ++i) {

                DEFAULT_THREAD_ABORT_CHECK_FALSE;

                // Create new Entry
                FileOperationEntry* newEntry = FileUtils::createFileOperationEntry(this, aEntry->opIndex, sourceDirTemp, sourceEntryInfoList[i].fileName(), targetDirTemp, sourceEntryInfoList[i].fileName());
                // Add Operation
                insertOperation(newEntry, aIndex + i);
            }

            // Create Target Dir
            fuResp = FileUtils::createDir(aEntry->target, createDirFlags, abort, dirCreatorObserver);
            // Set Entry State
            aEntry->setOpStateByFileUtilsResponse(fuResp);
        } break;

        case OPERATION_ID_RENAME: {
            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - RENAME";
            // Set Entry Operation State
            aEntry->opState = FOSRunning;
            // Rename Dir
            fuResp = FileUtils::renameFile(aEntry->source, aEntry->target, renameFlags, abort, aEntry);
            // Set Entry State
            aEntry->setOpStateByFileUtilsResponse(fuResp);
        } break;

        case OPERATION_ID_DELETE: {
            // Check Entry State
            if (aEntry->opState == FOSDeleteLater) {
                qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - DELETE DIR AFTER ALL ITEMS DELETED";
/*
                // Set Entry Operation State
                aEntry->opState = FOSDone;
                // Remove Directory
                return QFile::remove(sourceDirTemp);
*/
                // Set Entry Running State
                aEntry->opState = FOSRunning;
                // Delete File
                fuResp = FileUtils::deleteFile(aEntry->source, deleteFlags, abort, aEntry);
                // Set Result
                result = (fuResp == FILE_UTILS_RESPONSE_NOERROR);
                // Set Entry State
                aEntry->setOpStateByFileUtilsResponse(fuResp);

                return result;
            }

            // Init Source Dir To Read Entries
            QDir sourceDir(aEntry->source);
            // Init Source Dir Temp
            QString sourceDirTemp = aEntry->source;
            // Set Entry Operation State
            aEntry->opState = FOSRunning;

            // Check Delete Observer & Deletion Options
            if (deleteObserver && !((deleteFlags & FILE_DELETE_OPTION_DELETE_NON_EMPTY_DIR) || (deleteFlags & FILE_DELETE_OPTION_SKIP_NON_EMPTY_DIR))) {
                // Confirm Not Empty Dir Deletion
                int observerResponse = deleteObserver->confirmDeletion(sourceDirTemp, false, true);

                // Check Observer Response
                if (observerResponse == FOORTYesToAll) {
                    // Add Delete Non Empty Dirs Option
                    deleteFlags |= FILE_DELETE_OPTION_DELETE_NON_EMPTY_DIR;

                // Check Observer Response
                } else if (observerResponse == FOORTNoToAll) {

                    // Add Delete Non Empty Dirs Option
                    deleteFlags |= FILE_DELETE_OPTION_SKIP_NON_EMPTY_DIR;

                    // Set Entry State
                    aEntry->opState = FOSSkipped;

                    return result;

                // Check Observer Response
                } else if (observerResponse == FOORTNo) {
                    // Set Entry State
                    aEntry->opState = FOSSkipped;

                    return result;
                }

                // Falling Thru - Non Empty Dir Deletion Confirmed

            // Check Deletion Options
            } else if (!(deleteFlags & FILE_DELETE_OPTION_DELETE_NON_EMPTY_DIR)) {
                // Check Delete Options
                if (deleteFlags & FILE_DELETE_OPTION_SKIP_NON_EMPTY_DIR) {
                    // Set Entry State
                    aEntry->opState = FOSSkipped;
                } else {
                    // Set Entry State
                    aEntry->opState = FOSFailed;
                }

                return result;
            }

            // Falling Thru - Non Empty Dir Deletion Confirmed

            // Check Source Dir Temp Path
            if (!sourceDirTemp.endsWith(QString("/")) && !sourceDirTemp.endsWith(QString("\\"))) {
                // Add Slash
                sourceDirTemp += QDir::separator();
            }

            // Get Entry Info List
            QFileInfoList sourceEntryInfoList = sourceDir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

            DEFAULT_THREAD_ABORT_CHECK_FALSE;

            // Get Source Entry List Count
            int seilCount = sourceEntryInfoList.count();

            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << aIndex << "]: " << aEntry->getSource() << " - DELETE" << " - seilCount: " << seilCount;

            // Go Through Source Entry Info List
            for (int i=0; i<seilCount; ++i) {

                DEFAULT_THREAD_ABORT_CHECK_FALSE;

                // Create new Entry
                FileOperationEntry* newEntry = FileUtils::createFileOperationEntry(this, aEntry->opIndex, sourceDirTemp, sourceEntryInfoList[i].fileName(), sourceDirTemp, sourceEntryInfoList[i].fileName());

                qDebug() << "FileOperationQueue::processDirEntry - newEntry[" << i << "]: " << newEntry->getSource() << " - DELETE";

                // Add Operation
                insertOperation(newEntry, aIndex + i);
            }

            qDebug() << "FileOperationQueue::processDirEntry - aEntry[" << (aIndex + seilCount) << "]: " << aEntry->getSource() << " - DELETE AFTER ALL ITEMS DELETED SET";

            // Set Operation State
            aEntry->opState = FOSDeleteLater;
            // Set Result
            result = true;
        } break;
    }

    qDebug() << "FileOperationQueue::processDirEntry...done";

    return result;
}

//==============================================================================
// Notify View About Entry Updated
//==============================================================================
void FileOpQueueHandler::notifyEntryUpdated(const int& aIndex)
{
    qDebug() << "FileOperationQueue::notifyEntryUpdated - aIndex: " << aIndex;

    // Emit Entry Operation Updated Signal
    emit operationUpdated(aIndex);
}

//==============================================================================
// Destructor
//==============================================================================
FileOpQueueHandler::~FileOpQueueHandler()
{
    qDebug() << "Deleting FileOperationQueue...";

    // Clear
    clear();

    qDebug() << "Deleting FileOperationQueue...done";
}


