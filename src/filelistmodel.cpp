#include <QDir>
#include <QSettings>
#include <QDateTime>
#include <QDebug>

#include <mcwinterface.h>

#include "filelistmodel.h"
#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"



//==============================================================================
// Constructor
//==============================================================================
FileListModelItem::FileListModelItem(const QString& aPath, const QString& aFileName)
    : fileInfo(aPath + "/" + aFileName)
    , selected(false)
{
    // ...
}








//==============================================================================
// Constructor
//==============================================================================
FileListModel::FileListModel(QObject* aParent)
    : QAbstractListModel(aParent)
    , currentDir(QDir::homePath())
    , fileUtil(NULL)
{
    qDebug() << "FileListModel::FileListModel";

    // Init
    init();

    // ...
}

//==============================================================================
// Init
//==============================================================================
void FileListModel::init()
{
    qDebug() << "FileListModel::init";

    // Create File Utin
    fileUtil = new RemoteFileUtilClient();

    // Connect Signals
    connect(fileUtil, SIGNAL(clientConnectionChanged(int,bool)), this, SLOT(clientConnectionChanged(int,bool)));
    connect(fileUtil, SIGNAL(clientStatusChanged(int, int)), this, SLOT(clientStatusChanged(int, int)));
    connect(fileUtil, SIGNAL(dirListItemFound(uint,QString,QString)), this, SLOT(dirListItemFound(uint,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));

    // Connect To File Server
    fileUtil->connectToFileServer();

    // ...
}

//==============================================================================
// Get Current Dir
//==============================================================================
QString FileListModel::getCurrentDir()
{
    return currentDir;
}

//==============================================================================
// Set Current Dir
//==============================================================================
void FileListModel::setCurrentDir(const QString& aCurrentDir)
{
    // Check Current Dir
    if (currentDir != aCurrentDir) {
        qDebug() << "FileListModel::setCurrentDir - aCurrentDir: " << aCurrentDir;

        // Set Current Dir
        currentDir = aCurrentDir;

        // Clear
        clear();

        // Fetch Current Dir Items
        fetchDirItems();

        // ...

        // Emit Current dir Changed Signal
        currentDirChanged(currentDir);
    }
}

//==============================================================================
// Clear
//==============================================================================
void FileListModel::clear()
{
    //qDebug() << "FileListModel::clear";

    // Begin Reset Model
    beginResetModel();

    // Go Thru File Info List
    while (itemList.count() > 0) {
        // Get File Info List Item
        FileListModelItem* item = itemList.takeLast();
        // Check Item
        if (item) {
            // Delete Item
            delete item;
            item = NULL;
        }
    }

    // End Reset Model
    endResetModel();
}

//==============================================================================
// Reload
//==============================================================================
void FileListModel::reload()
{
    qDebug() << "FileListModel::reload";

    // Clear
    clear();

    // Fetch Dir Items
    fetchDirItems();
}

//==============================================================================
// Fetch Dir
//==============================================================================
void FileListModel::fetchDirItems()
{
    // Check File Util
    if (!fileUtil) {
        qWarning() << "FileListModel::fetchDirItems - NO FILE UTIL!!";
        return;
    }

    qDebug() << "FileListModel::fetchDirItems - currentDir: " << currentDir;

    // Init Settings
    QSettings settings;

    // Init Filters
    int filters = settings.value(SETTINGS_KEY_SHOW_HIDDEN_FILES, true).toBool() ? DEFAULT_FILTER_SHOW_HIDDEN : 0;

    // ...

    // Init Sort Flags
    int sortFlags = settings.value(SETTINGS_KEY_DIRFIRST, true).toBool() ? DEFAULT_SORT_DIRFIRST : 0;

    // ...

    // Fetch Dir Items
    fileUtil->getDirList(currentDir, filters, sortFlags);
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void FileListModel::clientConnectionChanged(const int& aID, const bool& aConnected)
{
    qDebug() << "FileListModel::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected) {
        // Fetch Dir Items
        fetchDirItems();
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void FileListModel::clientStatusChanged(const int& aID, const int& aStatus)
{
    qDebug() << "FileListModel::clientStatusChanged - aID: " << aID << " - aStatus: " << aStatus;

}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void FileListModel::fileOpFinished(const unsigned int& aID,
                                   const QString& aOp,
                                   const QString& aPath,
                                   const QString& aSource,
                                   const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

    // Emit Dir Fetch Finished Signal
    emit dirFetchFinished();
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void FileListModel::fileOpAborted(const unsigned int& aID,
                                  const QString& aOp,
                                  const QString& aPath,
                                  const QString& aSource,
                                  const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void FileListModel::fileOpError(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aPath,
                                const QString& aSource,
                                const QString& aTarget,
                                const int& aError)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

    // ...
}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void FileListModel::dirListItemFound(const unsigned int& aID,
                                     const QString& aPath,
                                     const QString& aFileName)
{
    Q_UNUSED(aID);

    //qDebug() << "FileListModel::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - aFileName: " << aFileName;

    // Check Current Dir vs Path
    if (currentDir != aPath) {
        qWarning() << "FileListModel::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - INVALID PATH!!";

        return;
    }

    // ...

    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(aPath, aFileName);

    // Get Count
    int count = rowCount();

    // Begin Insert Row
    beginInsertRows(QModelIndex(), count, count);

    // Add Item To Item List
    itemList << newItem;

    // End Insert Row
    endInsertRows();
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> FileListModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File Name
    roles[FileName]         = "fileName";
    // File Extension/Suffix
    roles[FileExtension]    = "fileExt";
    // File Type
    roles[FileType]         = "fileType";
    // File Name
    roles[FileAttributes]   = "fileAttrib";
    // File Name
    roles[FileSize]         = "fileSize";
    // File Name
    roles[FileDateTime]     = "fileDate";
    // File Name
    roles[FileOwner]        = "fileOwner";
    // File Permissions
    roles[FilePerms]        = "fileParms";
    // File Selected
    roles[FileSelected]     = "fileIsSelected";
    // File Is Hidden
    roles[FileIsHidden]     = "fileIsHidden";
    // File Is Link
    roles[FileIsLink]       = "fileIsLink";
    // Full File Name
    roles[FileFullName]     = "fileFullName";

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int FileListModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return itemList.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int FileListModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return FileRolesCount - Qt::UserRole;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant FileListModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {
        // Get Model Item
        FileListModelItem* item = itemList[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case FileName: {
                // Check File Info
                if (item->fileInfo.isBundle()) {
                    if (!item->fileInfo.bundleName().isEmpty())
                        return item->fileInfo.bundleName();

                    return item->fileInfo.fileName();
                }

                // Check Base Name
                if (item->fileInfo.baseName().isEmpty() || item->fileInfo.isDir()) {
                    return item->fileInfo.fileName();
                }

                return item->fileInfo.baseName();

            } break;

            case FileExtension: {
                // Check Base Name
                if (item->fileInfo.baseName().isEmpty() ||
                    item->fileInfo.isDir()              ||
                    item->fileInfo.isBundle()) {
                    return QString("");
                }

                return item->fileInfo.suffix();

            } break;

            case FileType: {
                return QString("");
            } break;

            case FileAttributes: {

            } break;

            case FileSize: {
                // Check File Info
                if (item->fileInfo.isBundle()) {
                    return QString("[BUNDLE]");
                }

                // Check File Info
                if (item->fileInfo.isDir()) {
                    return QString("[DIR]");
                }

                return item->fileInfo.size();
            } break;

            case FileDateTime:      return formatDateTime(item->fileInfo.lastModified());
            case FileOwner:         return item->fileInfo.owner();
            case FilePerms:         return (int)item->fileInfo.permissions();
            case FileSelected:      return item->selected;
            case FileFullName:      return item->fileInfo.fileName();
            case FileIsHidden:      return item->fileInfo.isHidden();
            case FileIsLink:        return item->fileInfo.isSymLink();

            default:
            break;
        }
    }

    return QString("");
}

//==============================================================================
// Set Data
//==============================================================================
bool FileListModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {
        //qDebug() << "FileListModel::setData - row: " << aIndex.row() << " - aValue: " << aValue << " - aRole: " << aRole;

        // Get Item
        FileListModelItem* item = itemList[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case FileName: {

            } break;

            case FileExtension: {

            } break;

            case FileType: {

            } break;

            case FileAttributes: {

            } break;

            case FileSize: {

            } break;

            case FileDateTime: {

            } break;

            case FileOwner: {

            } break;

            case FilePerms: {

            } break;

            case FileSelected: {
                // Check Item
                if (item) {
                    // Set Selected
                    item->selected = aValue.toBool();
                    // Emit Data Changed Signal
                    emit dataChanged(aIndex, aIndex);

                    return true;
                }
            } break;

            case FileIsHidden: {

            } break;

            case FileIsLink: {

            } break;

            default: {

            } break;
        }
    }

    return false;
}

//==============================================================================
// Find Index
//==============================================================================
int FileListModel::findIndex(const QString& aFileName)
{
    // Get Count
    int flmCount = itemList.count();

    // Go Thru List
    for (int i=0; i<flmCount; ++i) {
        // Get Item
        FileListModelItem* item = itemList[i];

        // Check Item
        if (item->fileInfo.fileName() == aFileName) {
            return i;
        }
    }

    return 0;
}

//==============================================================================
// Get File Info
//==============================================================================
QFileInfo FileListModel::getFileInfo(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];
        // Return File Info
        return item->fileInfo;
    }

    return QFileInfo("/");
}

//==============================================================================
// Check If Is Dir
//==============================================================================
bool FileListModel::isDir(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];
        // Return File Info
        return item->fileInfo.isDir();
    }

    return false;
}

//==============================================================================
// Check If Is Bundle
//==============================================================================
bool FileListModel::isBundle(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];
        // Return File Info
        return item->fileInfo.isBundle();
    }

    return false;
}

//==============================================================================
// Get Busy
//==============================================================================
bool FileListModel::getBusy()
{
    return fileUtil ? (fileUtil->getStatus() == ECSTBusy) || (fileUtil->getStatus() == ECSTAborting)
                    : false;
}

//==============================================================================
// Destructor
//==============================================================================
FileListModel::~FileListModel()
{
    // Clear
    clear();

    // Check File Util
    if (fileUtil) {
        fileUtil->close();

        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    // ...

    qDebug() << "FileListModel::~FileListModel";
}

