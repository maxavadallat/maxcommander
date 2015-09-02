#ifndef UTILITY
#define UTILITY

#include <QString>
#include <QFileInfo>
#include <QDateTime>
#include <QRgb>

//==============================================================================
// File Sorting Type
//==============================================================================
enum FileSortType {
    EFSName  = 0,
    EFSExt,
    EFEType,
    EFSSize,
    EFSDate,
    EFSOwner,
    EFSPerms
};

//==============================================================================
// Archive File Info For File Util Client
//==============================================================================
struct ArchiveFileInfo {
    // Full File Path
    QString     filePath;
    // File Name
    QString     fileName;
    // File Size
    qint64      fileSize;
    // File Date
    QDateTime   fileDate;
    // File Attribs
    QString     fileAttribs;
    // Is Dir
    bool        fileIsDir;
    // Is Link
    bool        fileIsLink;
};


// Get Current User Name
QString getCurrentUserName();

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

// Has Parent Dir Of An Existing File/Dir
bool hasParentDir(const QString& aDirPath);

// Get Parent Dir Of An Existing File/Dir
QString getParentDir(const QString& aDirPath);

// Get Parent Dir From Path
QString getParentDirFromPath(const QString& aDirPath);

// Get File Name
QString getFileName(const QString& aDirPath);
// Get Dir Path
QString getDirPath(const QString& aDirPath);

// Check If Have Access To List Dir
bool haveAccessToDir(const QString& aDirPath);

// Store App Exec Path
void storeAppExecPath(const char* aPath);

// Get App Exec Path
QString getAppExecPath();

// Get File Name From Full File Name
QString getFileNameFromFullName(const QString& aFullFileName);
// Get File Name WithOut Extension From Full File Name
QString getBaseNameFromFullName(const QString& aFullFileName);
// Get File Extension From Full File Name
QString getExtensionFromFullName(const QString& aFullFileName);

// Check If Dir Is Empty
bool isDirEmpty(const QString& aDirPath);

// Launch App
bool launchApp(const QString& aAppName, const QStringList& aArgs, const QString& aWorkingDir = "");

// RGB To String - #AARRGGBB
QString rgb2string(const QRgb& aColor);
// String To RGB - #AARRGGBB
QRgb string2Rgb(const QString& aColor);

// Formatted Size
QString formattedSize(const qint64& aSize);

// Is Path Relative
bool isPathRelative(const QString& aPath);

// Split Path & File Name
QStringList splitPath(const QString& aPath);

// Compare Files By Name
int compareFileNames(const QFileInfo& aFileInfoA, const QFileInfo& aFileInfoB);

// Apply File name Pattern
QString applyPattern(const QString& aSourceFileName, const QString& aPattern);

// Is Volume Path
bool isVolumePath(const QString& aFilePath);

// Set Desktop Wallpaper
void setDesktopWallpaper(const QString& aFilePath, const int& aDesktop = 0);

// Check If File Archive/Compressed
bool isFileArchiveByExt(const QString& aFilePath);

// Get Permissions Text
QString getPermsText(const QFileInfo& aFileInfo);

// Get Archive Current Dir From Full Current Dir
QString getArchiveCurrentDir(const QString& aFileName, const QString& aCurrentDir);

#endif // UTILITY

