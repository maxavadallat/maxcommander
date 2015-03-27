#include <QDir>
#include <QFileInfo>
#include <QPaintDevice>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QProcess>
#include <QApplication>
#include <QThread>
#include <QSettings>
#include <QDebug>

#if defined(Q_OS_WIN)

#include <windows.h>
#include <shellapi.h

#elif defined(Q_OS_MAC)

#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>

#include <sys/stat.h>
#include <sys/statvfs.h>

#elif defined(Q_OS_UNIX)

#include <sys/stat.h>
#include <sys/statvfs.h>

#endif // Q_OS_UNIX

#include <mcwinterface.h>

#include "utility.h"
#include "constants.h"


//==============================================================================
// Get Total Space In Directory
//==============================================================================
qint64 getTotalSpace(const QString& aDirPath)
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
qint64 getFreeSpace(const QString& aDirPath)
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

#elif defined(Q_OS_WIN)


// ...


#else // Q_OS_UNIX


// ...


#endif // Q_OS_UNIX

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
            qDebug() << "### getFileIconImage - FSPathMakeRef: " << status;
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
        painter.drawImage(newImage.rect(), QImage(QString(":defaultIcon32x32")));
    }

#elif defined(Q_OS_WIN)

    // Init Painter
    QPainter painter(&newImage);

    newImage.fill(QColor(0, 0, 0, 0));

    //painter.drawImage(newImage.rect(), QImage(QString(":defaultIcon32x32")));

#else // Q_OS_UNIX

    // Init Painter
    QPainter painter(&newImage);

    newImage.fill(QColor(0, 0, 0, 0));

    //painter.drawImage(newImage.rect(), QImage(QString(":defaultIcon32x32")));

#endif // Q_OS_UNIX

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

    qDebug() << "launchRemoteFileServer - fileServerCommandLine: " << QDir::currentPath();

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

    return QString("");
}

//==============================================================================
// Get Dir Name
//==============================================================================
QString getDirName(const QString& aDirPath)
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
    settings.setValue(SETTINGS_KEY_EXEC_PATH, exeInfo.absolutePath());
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

    return settings.value(SETTINGS_KEY_EXEC_PATH).toString();
}





