#include <QDir>
#include <QFileInfo>
#include <QPaintDevice>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QApplication>
#include <QThread>
#include <QSettings>
#include <QProcess>
#include <QStorageInfo>
#include <QColor>
#include <QDebug>
#include <QMimeDatabase>

#if defined(Q_OS_WIN)

#include <windows.h>
#include <shellapi.h

#elif defined(Q_OS_MAC)

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#include <sys/stat.h>
#include <sys/statvfs.h>

#elif defined(Q_OS_UNIX)

#undef signals
#include <gio/gio.h>
#include <glib.h>
//#include <sys/stat.h>
//#include <sys/statvfs.h>

#endif // Q_OS_UNIX

#include <mcwinterface.h>

#include "utility.h"
#include "constants.h"

// Icon Cache
static QHash<QString, QString> iconCache;

//==============================================================================
// Get Current User Name
//==============================================================================
QString getCurrentUserName()
{
    return QFileInfo(QDir::homePath()).owner();
}

//==============================================================================
// Get Total Space In Directory
//==============================================================================
qint64 getTotalSpace(const QString& aDirPath)
{
    // Init Storage Info
    QStorageInfo storageInfo(aDirPath);

    return storageInfo.bytesTotal();
}

//==============================================================================
// Get Free Space In Directory
//==============================================================================
qint64 getFreeSpace(const QString& aDirPath)
{
    // Init Storage Info
    QStorageInfo storageInfo(aDirPath);

    return storageInfo.bytesFree();
}

#if defined(Q_OS_MAC)

//==============================================================================
// Get Mac CG Context
//==============================================================================
CGContextRef getMacCGContext(const QPaintDevice* aPaintDevice)
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
OSStatus convertMacIcon(const IconRef& aMacIconref, QImage& aIconImage)
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

#elif defined(Q_OS_UNIX)


//==============================================================================
// Search Icon Path
//==============================================================================
QString searchIconPath(const QString& aFileName, const QString& aFileType, const int& aWidth, const int& aHeight)
{
    // Check File Name
    if (QFile::exists(aFileName) && !aFileType.isEmpty() && aWidth > 0 && aHeight > 0) {

        // Check If Dir

        // Check If It Is A Standard Path


        //qDebug() << "searchIconPath - aFileName: " << aFileName << " - type: " << aFileType << " - size: [" << aWidth << "x" << aHeight << "]";

        // Init Icon Path
        QString iconPath(DEFAULT_ICONS_PATH_LINUX);

        // Get Slash Pos
        int slashPos = aFileType.indexOf(QChar('/'));
        // Get Mime Main Type
        QString mimeMainType = slashPos >= 0 ? aFileType.left(slashPos) : aFileType;
        // Get Mime Sub Type
        QString mimeSubType = slashPos >= 0 ? aFileType.right(aFileType.length() - slashPos - 1) : QString("");
        // Get Mime File Name
        QString mimeFileName = convertMimeToFileName(aFileType);

        //qDebug() << "searchIconPath - aFileName: " << aFileName << " - mimeMainType: " << mimeMainType << " - mimeSubType: " << mimeSubType << " - mimeFileName: " << mimeFileName;

        // Ini Icon Filters
        QStringList iconFilters;

        // Adding Filters - ORDER IS IMPORTANT!!

        // Adding Mimetypes Dir Name
        iconFilters << QString(DEFAULT_ICONS_MIMETYPES_DIR);
        // Adding Mimes Dir Name
        iconFilters << QString(DEFAULT_ICONS_MIMES_DIR);
        // Adding Size
        iconFilters << QString("%1x%2").arg(aWidth).arg(aHeight);
        // Adding Full Mime
        //iconFilters << mimeFileName;
        // Adding Mime Sub Type
        iconFilters << mimeSubType;

        // Get Icon Paths
        QStringList iconPaths = searchDir(iconPath, iconFilters);

        int ipCount = iconPaths.count();

        //qDebug() << " ";
        //qDebug() << "searchIconPath - found: " << ipCount;

        // Go Thru First For Exact MIME Match
        for (int i=0; i<ipCount; i++) {
            //qDebug() << "    - " << iconPaths[i];

            // Check Paths - Not Checking Current Theme
            if ((iconPaths[i].indexOf(QString(DEFAULT_ICONS_MIMETYPES_DIR)) > 0 || iconPaths[i].indexOf(QString(DEFAULT_ICONS_MIMES_DIR)) > 0) &&   // mimetypes & mimes dir
                 iconPaths[i].indexOf(iconFilters[2]) > 0 &&                                                                                        // Exact Size
                 iconPaths[i].indexOf(mimeFileName)   > 0) {                                                                                        // Mime File Name

                return iconPaths[i];
            }
        }

        // Go Thru Second Time For Fuzzy MIME Match
        for (int j=0; j<ipCount; j++) {
            // Check Paths
            if ((iconPaths[j].indexOf(QString(DEFAULT_ICONS_MIMETYPES_DIR)) > 0 || iconPaths[j].indexOf(QString(DEFAULT_ICONS_MIMES_DIR)) > 0) &&   // mimetypes & mimes dir
                 iconPaths[j].indexOf(mimeFileName)   > 0) {                                                                                        // Mime File Name

                return iconPaths[j];
            }
        }

        // Check Mime Sub Type
        if (!mimeSubType.isEmpty()) {
            // Go Thru Second Time For Fuzzy MIME Match
            for (int k=0; k<ipCount; k++) {
                // Check Paths
                if (iconPaths[k].indexOf(mimeSubType)   > 0) {                                                                                      // Mime Sub Type

                    return iconPaths[k];
                }
            }
        }

        // ...

        //qDebug() << " ";

        // ...

        return iconPath;

    } else {

        qDebug() << "searchIconPath - aFileName: " << aFileName << " - type: " << aFileType << " - size: [" << aWidth << "x" << aHeight << "] - INVALID PARAMETER(S)!!";

    }

    return QString();
}


#else // Q_OS_WIN


// ...


#endif // Q_OS_WIN

//==============================================================================
// Get File Icon Pixmap
//==============================================================================
QImage getFileIconImage(const QString& aFilePath, const int& aWidth, const int& aHeight)
{
    // Init New Image
    QImage newImage(aWidth, aHeight, QImage::Format_ARGB32_Premultiplied);

#if defined(Q_OS_MAC)

    //qDebug() << "getFileIconImage MAC - size: " << newImage.size();

    // Init File System Reference
    FSRef macRef;

    // Init Status
    OSStatus status = noErr;

    // Init Safe Counter
    int safeCount = 0;

    do {
        // Get file System Reference
        status = FSPathMakeRef(reinterpret_cast<const UInt8*>(aFilePath.toUtf8().constData()), &macRef, 0);

        // Check Status
        if (status != noErr) {
            //qDebug() << "### getFileIconImage - FSPathMakeRef: " << status;
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
            qDebug() << "### getFileIconImage - FSGetCatalogInfo: " << status;
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
            qDebug() << "### getFileIconImage - GetIconRefFromFileInfo: " << status;
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
            qDebug() << "### getFileIconImage - convertMacIcon: " << status;

            // Release Icon Ref
            //ReleaseIconRef(iconRef);
            //return QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
            //continue;
        }

        // Release Icon Ref
        ReleaseIconRef(iconRef);
        // Increase Safe Count
        safeCount++;

    } while (status != noErr && safeCount < DEFAULT_ICOM_GET_RETRY_COUNT_MAX);

    // Check Safe Count
    if (safeCount >= DEFAULT_ICOM_GET_RETRY_COUNT_MAX) {
        // Init Painter
        QPainter painter(&newImage);
        // Fill Image
        newImage.fill(QColor(0, 0, 0, 0));
        // Draw Default Icon
        painter.drawImage(newImage.rect(), QImage(QString(":/resources/images/icons/default_file.png")));
    }

#elif defined(Q_OS_UNIX)

    // Init Error
    GError* error = NULL;
    // New File
    GFile* file = g_file_new_for_path (aFileName.toLocal8Bit().data());
    // Get File Info
    GFileInfo* file_info = g_file_query_info (file, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, &error);
    // Check Error
    if (error) {

        qDebug() << "### getFileIconImage - aFileName: " << aFileName << " - error: " << error->code << " : " << error->message;

        // Free Error
        g_free(error);
    }

    // Get Content Type
    const char* content_type = g_file_info_get_content_type(file_info);
    // Get Description
    char* description = g_content_type_get_description(content_type);
    // Get App Info
    //GAppInfo* app_info = g_app_info_get_default_for_type(content_type, FALSE);

    //QString fileName = QString(g_file_get_basename(file));
    QString fileType = QString(content_type);
    //QString fileDesc = QString(description);
    //QString fileAppName = app_info ? QString(g_app_info_get_name(app_info)) : QString();

    //qDebug() << "==============================================================================";
    //qDebug() << "File Name   : " << fileName;
    //qDebug() << "Type        : " << fileType;
    //qDebug() << "Description : " << fileDesc;
    //qDebug() << "App Name    : " << fileAppName;
    //qDebug() << "==============================================================================";

    // Check Description
    if (description) {
        // Free Description
        g_free(description);
    }

    // Get Icon Path From Icon Cache
    QString iconPath = iconCache.value(fileType);
    // Init Found In Cache
    bool foundInCache = false;

    // Check Icon Path
    if (iconPath.isNull()) {

        //qDebug() << "getFileIconImage - aFileName: " << aFileName << " - SEARCH";

        // Search For Icon Path
        iconPath = searchIconPath(aFileName, fileType, aWidth, aHeight);

    } else {

        //qDebug() << "getFileIconImage - aFileName: " << aFileName << " - CACHED";

        // Set Found In Cache
        foundInCache = true;
    }

    // Add Icon Path To Icon Path Cache
    if (QFile::exists(iconPath) && !foundInCache /*&& fileType != QString()*/ ) {

        //qDebug() << "getFileIconImage - CACHING - type: " << fileType << " - iconPath: " << iconPath;

        // Add Path To Cache
        iconCache[fileType] = iconPath;
    }

    // ...

    // Check Icon Path
    if (QFile::exists(iconPath)) {
        // Create New QImage
        return new QImage(iconPath);
    }

    // ...


    // Init Painter
    QPainter painter(&newImage);
    // Fill Image
    newImage.fill(QColor(0, 0, 0, 0));
    // Draw Default Icon
    painter.drawImage(newImage.rect(), QImage(QString(":/resources/images/icons/default_file.png")));

#else // Q_OS_WIN

    // Init Painter
    QPainter painter(&newImage);
    // Fill Image
    newImage.fill(QColor(0, 0, 0, 0));
    // Draw Default Icon
    painter.drawImage(newImage.rect(), QImage(QString(":/resources/images/icons/default_file.png")));

#endif // Q_OS_WIN

    return newImage;
}









//==============================================================================
// Execute Shell Command
//==============================================================================
int execShellCommand(const QString& aCommand, const bool& asRoot, const QString& aRootPass)
{
    // Init Result
    int result = 0;

#if defined(Q_OS_MAC) || defined (Q_OS_UNIX)

    // Init Command Line
    QString commandLine = asRoot ? QString(DEFAULT_ROOT_SHELL_COMMAND_TEMPLATE).arg(aRootPass).arg(aCommand) : aCommand;

    // Check If As Root
    if (asRoot) {
        // Run With System
        result = system(commandLine.toLocal8Bit().data());
    } else {
        // Init Process
        QProcess process;
        // Start New Process
        if (!process.startDetached(commandLine.toLocal8Bit().data())) {
            qDebug() << "utility::execShellCommand - ERROR STARTING commandLine: " << commandLine << " - error: " << process.errorString();
        }
    }

#elif defined(Q_OS_WIN)


#endif // Q_OS_WIN

    return result;
}

//==============================================================================
// Check If File Server Running
//==============================================================================
bool checkRemoteFileServerRunning()
{
#if defined(Q_OS_MAC) || defined (Q_OS_UNIX)

    // Init New PS Process
    QProcess psProcess;

    // Execute Process
    psProcess.start(QString(DEFAULT_PS_COMMAND_CHECK_FILESERVER));
    // Wait For Finished
    psProcess.waitForFinished();

    // Process Output
    QString output = psProcess.readAllStandardOutput();

    //qDebug() << "utility::checkRemoteFileServerRunning";
    //qDebug() << output;

    // Check Occurence
    if (output.indexOf(DEFAULT_FILE_SERVER_EXEC_NAME) > 0) {
        return true;
    }

#elif defined(Q_OS_WIN)

#endif // Q_OS_WIN

    return false;
}

//==============================================================================
// Launch File Server
//==============================================================================
int launchRemoteFileServer(const bool& asRoot, const QString& aRootPass)
{
    QString fileServerCommandLine = asRoot ? QString(getAppExecPath() + "/%1 %2").arg(DEFAULT_FILE_SERVER_EXEC_NAME)
                                                                                 .arg(DEFAULT_OPTION_RUNASROOT)
                                           : QString(getAppExecPath() + "/%1").arg(DEFAULT_FILE_SERVER_EXEC_NAME);

    qDebug() << "launchRemoteFileServer - fileServerCommandLine: " << fileServerCommandLine;

    // Exec Shell Command
    int result = execShellCommand(fileServerCommandLine, asRoot, aRootPass);

    // Wait a bit
    QThread::msleep(DEFAULT_FILE_SERVER_LAUNCH_DELAY);

    return result;
}

//==============================================================================
// Format Data Time
//==============================================================================
QString formatDateTime(const QDateTime& aDateTime)
{
    return QString(DEFAULT_DATE_FORMAT_STRING).arg(aDateTime.date().year())
                                              .arg(aDateTime.date().month(), 2, 10, QChar('0'))
                                              .arg(aDateTime.date().day(), 2, 10, QChar('0'))
                                              .arg(aDateTime.time().hour(), 2, 10, QChar('0'))
                                              .arg(aDateTime.time().minute(), 2, 10, QChar('0'))
                                              .arg(aDateTime.time().second(), 2, 10, QChar('0'));
}

//==============================================================================
// Has Parent Dir
//==============================================================================
bool hasParentDir(const QString& aDirPath)
{
    // Init File Info
    QFileInfo dirInfo(aDirPath);
    // Check If Exists
    if (dirInfo.exists()) {
        // Check Canonical Path
        return dirInfo.canonicalPath() != aDirPath;
    }

    return false;
}

//==============================================================================
// Get Parent Dir
//==============================================================================
QString getParentDir(const QString& aDirPath)
{
    // Init File Info
    QFileInfo dirInfo(aDirPath);
    // Check If Exists
    if (dirInfo.exists()) {
        // Return Canonical Path
        return dirInfo.absolutePath();
    }

    return QString(DEFAULT_ROOT_DIR);
}

//==============================================================================
// Get Parent Dir From Path
//==============================================================================
QString getParentDirFromPath(const QString& aDirPath)
{
    // Check Local Path
    if (aDirPath == QString(DEFAULT_ROOT_DIR)) {
        return QString(DEFAULT_ROOT_DIR);
    }

    // Get Local Path
    QString localPath = aDirPath;

    // Check Local Path
    if (localPath.endsWith("/")) {
        // Adjust Local Path
        localPath = localPath.left(localPath.length()-1);
    }

    // Get Last Index Of /
    int liSlash = localPath.lastIndexOf("/");

    // Check Last Index Of Slash
    if (liSlash > 0) {
        return localPath.left(liSlash);
    }

    return QString(DEFAULT_ROOT_DIR);
}

//==============================================================================
// Get File Name
//==============================================================================
QString getFileName(const QString& aDirPath)
{
    // Init File Info
    QFileInfo dirInfo(aDirPath);

    // Check If Exists
    if (dirInfo.exists()) {
        // Return Canonical Path
        return dirInfo.fileName();
    }

    return QString("");
}

//==============================================================================
// Get Dir Path
//==============================================================================
QString getDirPath(const QString& aDirPath)
{
    // Init File Info
    QFileInfo dirInfo(aDirPath);
    // Check If Exists
    if (dirInfo.exists()) {
        // Return Canonical Path
        return dirInfo.absolutePath();
    }

    return QString("");
}

//==============================================================================
// Check If Have Access To List Dir
//==============================================================================
bool haveAccessToDir(const QString& aDirPath)
{
    // Init Dir Info
    QFileInfo dirInfo(aDirPath);

    // Check If Exists
    if (dirInfo.exists() && (dirInfo.isDir() || dirInfo.isBundle())) {
        return dirInfo.permissions() & QFileDevice::ReadUser;
    }

    return false;
}

//==============================================================================
// Store App Exec Path
//==============================================================================
void storeAppExecPath(const char* aPath)
{
    // Init Exe Info
    QFileInfo exeInfo(aPath);
    // Init Settings
    QSettings settings;
    // Set Value
    settings.setValue(SETTINGS_KEY_MAIN_EXEC_PATH, exeInfo.absolutePath());
    // Sync/Save Settings
    settings.sync();
}

//==============================================================================
// Get App Exec Path
//==============================================================================
QString getAppExecPath()
{
    // Init Settings
    QSettings settings;

    return settings.value(SETTINGS_KEY_MAIN_EXEC_PATH).toString();
}

//==============================================================================
// Get File Name From Full File Name
//==============================================================================
QString getFileNameFromFullName(const QString& aFullFileName)
{
    // Get Last Pos Of Slash
    int lastSlashPos = aFullFileName.lastIndexOf("/");

    // Check Last Slash Pos
    if (lastSlashPos < 0)
        return aFullFileName;

    return aFullFileName.right(aFullFileName.length() - lastSlashPos - 1);
}

//==============================================================================
// Get File Name WithOut Extension From Full File Name
//==============================================================================
QString getBaseNameFromFullName(const QString& aFullFileName)
{
    // Get Last Dot Pos
    int lastDotPos = aFullFileName.lastIndexOf(".");

    // Check Last Dot Pos
    if (lastDotPos <= 0) {
        return aFullFileName;
    }

    // Check File Name
    if (aFullFileName.endsWith(".")) {
        return aFullFileName;
    }

    // Get File Name
    QString fileName = aFullFileName.left(lastDotPos);

    // Init Tar Extension
    QString tarExt = QString(".%1").arg(DEFAULT_EXTENSION_TAR);

    // Check File Name
    if (fileName.endsWith(tarExt)) {
        // Get Tar Extension Pos
        int tarExtensionPos = fileName.indexOf(tarExt);

        return aFullFileName.left(tarExtensionPos);
    }

    return aFullFileName.left(lastDotPos);
}

//==============================================================================
// Get File Extension From Full File Name
//==============================================================================
QString getExtensionFromFullName(const QString& aFullFileName)
{
    // Get Last Dot Pos
    int lastDotPos = aFullFileName.lastIndexOf(".");

    // Check Last Dot Pos
    if (lastDotPos <= 0) {
        return "";
    }

    // Check File Name
    if (aFullFileName.endsWith(".")) {
        return "";
    }

    // Get File Name
    QString fileName = aFullFileName.left(lastDotPos);

    // Init Tar Extension
    QString tarExt = QString(".%1").arg(DEFAULT_EXTENSION_TAR);

    // Check File Name
    if (fileName.endsWith(tarExt)) {
        // Get Tar Extension Pos
        int tarExtensionPos = fileName.indexOf(tarExt);

        return aFullFileName.right(aFullFileName.length() - tarExtensionPos - 1);
    }

    return aFullFileName.right(aFullFileName.length() - lastDotPos - 1);
}

//==============================================================================
// Check If Dir Is Empty
//==============================================================================
bool isDirEmpty(const QString& aDirPath)
{
    // Init Dir
    QDir dir(aDirPath);

    // Get Entry List
    QStringList entryList = dir.entryList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

    return entryList.count() == 0;
}

//==============================================================================
// Launch App
//==============================================================================
bool launchApp(const QString& aAppName, const QStringList& aArgs, const QString& aWorkingDir)
{
    return QProcess::startDetached(aAppName, aArgs, aWorkingDir);
}


//==============================================================================
// RGB To String
//==============================================================================
QString rgb2string(const QRgb& aColor)
{
    // Convert To Result
    QString result = QString("#%1%2%3%4").arg(qAlpha(aColor), 2, 16, QChar('0'))
                                         .arg(qRed(aColor), 2, 16, QChar('0'))
                                         .arg(qGreen(aColor), 2, 16, QChar('0'))
                                         .arg(qBlue(aColor), 2, 16, QChar('0'));

    return result;
}

//==============================================================================
// String To RGB
//==============================================================================
QRgb string2Rgb(const QString& aColor)
{
    // Convert To Result
    QRgb result = qRgba(aColor.mid(3, 2).toInt(NULL, 16),
                        aColor.mid(5, 2).toInt(NULL, 16),
                        aColor.mid(7, 2).toInt(NULL, 16),
                        aColor.mid(1, 2).toInt(NULL, 16));

    return result;
}

//==============================================================================
// Formatted Size
//==============================================================================
QString formattedSize(const qint64& aSize)
{
    // Check Size
    if (aSize <= 0) {
        return "0 B";
    }

    // Init Units
    QStringList units = QString("B,kB,MB,GB,TB,EB").split(",");

    // Calculate Digit Group
    //int digitGroups = (int) (log10(aSize) / log10(DEFAULT_ONE_KILO));
    int digitGroups = qMax((int) (log10(aSize) / log10(DEFAULT_ONE_KILO)) - 1, 0);

    // Get Result
    QString result = QString("%L1 %2").arg((qlonglong)(aSize / pow(DEFAULT_ONE_KILO, digitGroups)), 0, 10, QChar('0')).arg(units[digitGroups]);

    return result;
}

//==============================================================================
// Is Path Relative
//==============================================================================
bool isPathRelative(const QString& aPath)
{
    if (aPath.startsWith("/")) {
        return false;
    }

    return true;
}

//==============================================================================
// Split Path & File Name
//==============================================================================
QStringList splitPath(const QString& aPath)
{
    // Init Result
    QStringList result;
    // Get Last Index Of Slash
    int lastSlashIndex = aPath.lastIndexOf("/");

    // Check Last Index Of Slash
    if (lastSlashIndex == -1) {

        result << aPath;

        return result;
    }

    // Add Path
    result << aPath.left(lastSlashIndex + 1);

    // Add File/Pattern
    result << aPath.right(aPath.length() - lastSlashIndex - 1);

    return result;
}

//==============================================================================
// Compare Files By Name
//==============================================================================
int compareFileNames(const QFileInfo& aFileInfoA, const QFileInfo& aFileInfoB)
{
    // Check File  Infos
    if (aFileInfoA.isDir() && !aFileInfoB.isDir())
        return 1;

    if (!aFileInfoA.isDir() && aFileInfoB.isDir())
        return -1;

    return qstricmp(aFileInfoB.absoluteFilePath().toLocal8Bit().data(), aFileInfoA.absoluteFilePath().toLocal8Bit().data());
}

//==============================================================================
// Apply File name Pattern
//==============================================================================
QString applyPattern(const QString& aSourceFileName, const QString& aPattern)
{
    // Check Pattern
    if (aPattern == QString("*") || (aPattern.indexOf("*.*") >= 0) || aPattern.isEmpty() || aPattern.isNull()) {
        return aSourceFileName;
    }

    // Check Pattern
    if (aPattern.startsWith("*.")) {
        return QString("%1.%2").arg(getBaseNameFromFullName(aSourceFileName)).arg(getExtensionFromFullName(aPattern));
    }

    // Check Pattern
    if (aPattern.endsWith(".*")) {
        return QString("%1.%2").arg(getBaseNameFromFullName(aPattern)).arg(getExtensionFromFullName(aSourceFileName));
    }

    // ...

    return "";
}

//==============================================================================
// Is Volume Path
//==============================================================================
bool isVolumePath(const QString& aFilePath)
{
    // Check Path
    if (!QFile::exists(aFilePath)) {
        return false;
    }

#if defined(Q_OS_MAC)

    //qDebug() << "utility::isVolumePath - aFilePath: " << aFilePath;

    // Check Path
    if (!aFilePath.startsWith(DEFAULT_VOLUMES_PATH_MAC))
        return false;
/*
    // Check Path if Link
    if (QFileInfo(aFilePath).isSymLink()) {
        return false;
    }
*/
    // Get Volume Name
    QString volumeName = aFilePath.right(aFilePath.length() - QString(DEFAULT_VOLUMES_PATH_MAC).length());

    // Check Volume Name
    while (volumeName.endsWith("/")) {
        // Adjust Volume Name
        volumeName.chop(1);
    }

    // Check Volume Name
    if (volumeName.indexOf("/") < 0) {
        return true;
    }

#elif defined(Q_OS_LINUX)

    // Check Path
    if (!aFilePath.startsWith(DEFAULT_MEDIA_PATH_LINUX))
        return false;

#elif // Q_OS_WIN


#endif // Q_OS_WIN

    return false;
}

//==============================================================================
// Set Desktop Wallpaper
//==============================================================================
void setDesktopWallpaper(const QString& aFilePath, const int& aDesktop)
{
    Q_UNUSED(aFilePath);
    Q_UNUSED(aDesktop);

    // Init Command Line
    QString commandLine = "";

#if defined(Q_OS_MAC)

    // Set Command Line
    commandLine = QString(DEFAULT_CHANGE_DESKTOP_BACKGROUND_COMMAND_MAC_OSX).arg(aFilePath);

#else // Q_OS_MACX


#endif // Q_OS_MACX

    // System Command
    system(commandLine.toLocal8Bit().data());

}

//==============================================================================
// Check If File Compressed
//==============================================================================
bool isFileArchiveByExt(const QString& aFilePath)
{
    // Get File Extension
    QString ext = getExtensionFromFullName(aFilePath);

    // TODO: Make Better Implementation For Evaluation!

    // Check Extension
    if (ext.toLower() == DEFAULT_EXTENSION_RAR)
        return true;

    if (ext.toLower() == DEFAULT_EXTENSION_ZIP)
        return true;

    if (ext.toLower() == DEFAULT_EXTENSION_GZIP)
        return true;

    if (ext.toLower() == DEFAULT_EXTENSION_ARJ)
        return true;

    if (ext.toLower() == DEFAULT_EXTENSION_ACE)
        return true;

    if (ext.toLower() == DEFAULT_EXTENSION_TAR)
        return true;

    if (ext.startsWith(QString("%1.").arg(DEFAULT_EXTENSION_TAR)))
        return true;

    return false;
}

//==============================================================================
// Get Permissions Text
//==============================================================================
QString getPermsText(const QFileInfo& aFileInfo)
{
    // Init Perms Text
    QString permsText = DEFAULT_PERMISSIONS_TEXT;

    // Check File Info
    if (aFileInfo.isSymLink()) {
        // Adjust Perms Text
        permsText[0] = 'l';
    } else if (aFileInfo.isDir()) {
        // Adjust Perms Text
        permsText[0] = 'd';
    }

    // Get Permissions
    QFile::Permissions perms = aFileInfo.permissions();

    // Check Perms
    if (perms & QFile::ReadUser)    { permsText[1] = 'r'; }
    if (perms & QFile::WriteUser)   { permsText[2] = 'w'; }
    if (perms & QFile::ExeUser)     { permsText[3] = 'x'; }

    if (perms & QFile::ReadGroup)   { permsText[4] = 'r'; }
    if (perms & QFile::WriteGroup)  { permsText[5] = 'w'; }
    if (perms & QFile::ExeGroup)    { permsText[6] = 'x'; }

    if (perms & QFile::ReadOther)   { permsText[7] = 'r'; }
    if (perms & QFile::WriteOther)  { permsText[8] = 'w'; }
    if (perms & QFile::ExeOther)    { permsText[9] = 'x'; }

    return permsText;
}

//==============================================================================
// Get Archive Current Dir From Full Current Dir
//==============================================================================
QString getArchiveCurrentDir(const QString& aFilePath, const QString& aCurrentDir)
{
    // Get Archive Name
    QString archiveName = getFileNameFromFullName(aFilePath);

    // Check Current Dir
    if (!aCurrentDir.startsWith(archiveName)) {
        return "/";
    }

    // Init Result
    QString result = aCurrentDir.right(aCurrentDir.length() - archiveName.length() - 3);

    return result;
}







