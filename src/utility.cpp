#include <QFileInfo>
#include <QDebug>

#if defined(Q_OS_WIN)

#include <windows.h>
#include <shellapi.h

#elif defined(Q_OS_MAC)

#include <sys/stat.h>
#include <sys/statvfs.h>

#elif defined(Q_OS_UNIX)

#include <sys/stat.h>
#include <sys/statvfs.h>

#endif // Q_OS_UNIX


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


#include "utility.h"


