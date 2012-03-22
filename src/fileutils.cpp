// INCLUDES

#include <QDebug>
#include <QProcess>
#include <QFileIconProvider>
#include <QTimer>
#include <QPainter>

#if defined(Q_OS_WIN)

#include <windows.h>
#include <shellapi.h>

#elif defined(Q_OS_MAC)

#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>
//#include <CoreFoundation/CoreFoundation.h>

#include <sys/stat.h>
#include <sys/statvfs.h>

#elif defined(Q_OS_UNIX)

#include <sys/stat.h>
#include <sys/statvfs.h>

#endif // Q_OS_UNIX

#include "constants.h"
#include "fileutils.h"


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
        // Init Total Size
        //qint64 totalSize = 0;
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
/*
        if (fiCount) {
            qDebug() << "====================================================";
            qDebug() << "FileUtils::getDirSize - aDirPath: " << aDirPath;
        }
*/
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
/*
        if (fiCount) {
            qDebug() << "====================================================";
        }
*/
        return aDirSize;
    }

    return fileInfo.size();
}

//==============================================================================
// Create Directory
//==============================================================================
bool FileUtils::createDir(const QString& aDirName)
{
    // Init Temp Dir
    QDir tempDir(QDir::root());
    // Make Path
    return tempDir.mkpath(aDirName);
}

//==============================================================================
// Delete File
//==============================================================================
bool FileUtils::deleteFile(const QString& aFileName, const bool& aRecursive)
{
    // Init Temp Dir
    QDir tempDir(QDir::root());
    // Check If Is A Empty Dir
    if (!isDirEmpty(aFileName) && aRecursive) {
        // Delete Dir

        // ...
    }

    // Remove File
    return tempDir.remove(aFileName);
}

//==============================================================================
// Copy File
//==============================================================================
bool FileUtils::copyFile(const QString& aSource, const QString& aTarget)
{
    // Check If Source Exists And Target Is Not Empty
    if (QFile::exists(aSource) && !aTarget.isEmpty()) {

    }

    return false;
}

//==============================================================================
// Rename File
//==============================================================================
bool FileUtils::renameFile(const QString& aSource, const QString& aTarget)
{
    // Check If Source Exists And Target Is Not Empty
    if (QFile::exists(aSource) && !aTarget.isEmpty()) {

    }

    return false;
}

//==============================================================================
// Move File
//==============================================================================
bool FileUtils::moveFile(const QString& aSource, const QString& aTarget)
{
    // Check If Source Exists And Target Is Not Empty
    if (QFile::exists(aSource) && !aTarget.isEmpty()) {

    }

    return false;
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
QImage FileUtils::convertMacIcon(const IconRef& aMacIconref, QImage& aIconImage)
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
    // Plot Icon Ref In Context
    OSStatus status = PlotIconRefInContext(ctx, &rect, kAlignNone, kTransformNone, &color, kPlotIconRefNormalFlags, aMacIconref);
    // Doing Twice To Make Sure It Is Drawn
    status = PlotIconRefInContext(ctx, &rect, kAlignNone, kTransformNone, &color, kPlotIconRefNormalFlags, aMacIconref);
    // Check Status
    if (status != noErr) {
        qDebug() << "#### convertMacIcon - PlotIconRefInContext: " << status;
    }
    // Release Context
    CGContextRelease(ctx);
    // Return Icon Image
    return aIconImage;
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
    // Get file System Reference
    OSStatus status = FSPathMakeRef(reinterpret_cast<const UInt8*>(aInfo.canonicalFilePath().toUtf8().constData()), &macRef, 0);

    // Check Status
    if (status != noErr) {
        qDebug() << "### FileUtils::getFileIconImage - FSPathMakeRef: " << status;
        return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
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
        return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
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
        return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
    }

    // Convert Mac Icon
    convertMacIcon(iconRef, newImage);

    // Release Icon Ref
    ReleaseIconRef(iconRef);

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

        return fileInfo.fileName();
    } else {
        qDebug() << "FileUtils::getDirName - aDirPath: " << aDirPath << " - NOT A DIR!";
    }

    return QString("");
}

//==============================================================================
// Format File Size
//==============================================================================
QString FileUtils::formatFileSize(const QFileInfo& aInfo)
{
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
            // Emit Operatoin Stopped Signal
            emit opStopped();
            // Set Abort
            abort = true;
            // Reset Restart
            restart = false;
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







