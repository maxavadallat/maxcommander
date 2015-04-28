#ifndef UTILITY
#define UTILITY

#include <QString>
#include <QFileInfo>
#include <QDateTime>


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
QImage getFileIconImage(const QString& aFilePath, const int& aWidth, const int& aHeight);

// Execute Shell Command
int execShellCommand(const QString& aCommand, const bool& asRoot = false, const QString& aRootPass = "");

// Check If File Server Running
bool checkRemoteFileServerRunning();

// Launch File Server
int launchRemoteFileServer(const bool& asRoot = false, const QString& aRootPass = "");

// Format Data Time
QString formatDateTime(const QDateTime& aDateTime);

// Has Parent Dir
bool hasParentDir(const QString& aDirPath);

// Get Parent Dir
QString getParentDir(const QString& aDirPath);

// Get Dir Name
QString getDirName(const QString& aDirPath);
// Get Dir Path
QString getDirPath(const QString& aDirPath);

// Check If Have Access To List Dir
bool haveAccessToDir(const QString& aDirPath);

// Store App Exec Path
void storeAppExecPath(const char* aPath);

// Get App Exec Path
QString getAppExecPath();

// Get File Name WithOut Extension From Full File Name
QString getFileNameFromFullName(const QString& aFullFileName);
// Get File Extension From Full File Name
QString getExtensionFromFullName(const QString& aFullFileName);

// Check If Dir Is Empty
bool isDirEmpty(const QString& aDirPath);

// Launch App
bool launchApp(const QString& aAppName, const QStringList& aArgs, const QString& aWorkingDir = "");


#endif // UTILITY

