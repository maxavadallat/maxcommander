#ifndef UTILITY
#define UTILITY

#include <QString>
#include <QFileInfo>


// File Sorting Type
enum FileSortType {
    EFSName  = 0,
    EFSExt,
    EFEType,
    EFSSize,
    EFSDate,
    EFSOwner,
    EFSPerms
};


// Get Total Space In Directory
qint64 getTotalSpace(const QString& aDirPath);
// Get Free Space In Directory
qint64 getFreeSpace(const QString& aDirPath);

// Get File Icon Image
QImage getFileIconImage(const QFileInfo& aInfo, const int& aWidth, const int& aHeight);

// Execute Shell Command
int execShellCommand(const QString& aCommand, const bool& asRoot = false, const QString& aRootPass = "");

// Check If File Server Running
bool checkRemoteFileServerRunning();

// Launch File Server
int launchRemoteFileServer(const bool& asRoot = false, const QString& aRootPass = "");

#endif // UTILITY

