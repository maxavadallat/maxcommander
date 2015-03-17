#include <QDir>
#include <QSettings>
#include <QDebug>

#include "filelistmodel.h"
#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"



//==============================================================================
// Constructor
//==============================================================================
FileListModelItem::FileListModelItem()
    : selected(false)
{

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
    connect(fileUtil, SIGNAL(dirListItemFound(uint,QString,QString)), this, SLOT(dirListItemFound(uint,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,int)), this, SLOT(fileOpFinished(uint,QString,QString,QString,int)));
    connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,int)));

    // Connect To File Server
    //fileUtil->connectToFileServer();

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
        // Set Current Dir
        currentDir = aCurrentDir;

        // Clear
        clear();

        // Fetch Current Dir Items

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
    // Begin Reset Model
    beginResetModel();

    // Go Thru File Info List
    while (fileInfoList.count() > 0) {
        // Get File Info List Item
        FileListModelItem* item = fileInfoList.takeLast();
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

    // Fetch Dir Items
    fileUtil->getDirList(currentDir, QDir::AllEntries | QDir::NoDot | QDir::Hidden, QDir::DirsFirst);
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void FileListModel::fileOpProgress(const unsigned int& aID,
                                   const QString& aOp,
                                   const QString& aCurrFilePath,
                                   const quint64& aCurrProgress,
                                   const quint64& aCurrTotal,
                                   const quint64& aOverallProgress,
                                   const quint64& aOverallTotal,
                                   const int& aSpeed)
{
    qDebug() << "FileListModel::fileOpProgress - aID: " << aID << " - aOp: " << aOp;

    // ...
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void FileListModel::fileOpFinished(const unsigned int& aID,
                                   const QString& aOp,
                                   const QString& aSource,
                                   const QString& aTarget,
                                   const int& aError)
{
    qDebug() << "FileListModel::fileOpFinished - aID: " << aID << " - aOp: " << aOp;

    // ...
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void FileListModel::fileOpError(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aSource,
                                const QString& aTarget,
                                const int& aError)
{
    qDebug() << "FileListModel::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aError: " << aError;

    // ...
}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void FileListModel::fileOpNeedConfirm(const unsigned int& aID,
                                      const QString& aOp,
                                      const QString& aCode,
                                      const QString& aSource,
                                      const QString& aTarget)
{
    qDebug() << "FileListModel::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aCode: " << aCode;

    // ...
}

//==============================================================================
// Dir Size Scan Progress Slot
//==============================================================================
void FileListModel::dirSizeScanProgress(const unsigned int& aID,
                                        const QString& aPath,
                                        const quint64& aNumDirs,
                                        const quint64& aNumFiles,
                                        const quint64& aScannedSize)
{

}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void FileListModel::dirListItemFound(const unsigned int& aID,
                                     const QString& aPath,
                                     const QString& aFileName)
{
    qDebug() << "FileListModel::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - aFileName: " << aFileName;

    // ...

    // Create New File List Item

    // Begin Insert Row

    // Add Item

    // End Insert Row
}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void FileListModel::fileOpQueueItemFound(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aSource,
                                         const QString& aTarget)
{

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

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int FileListModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return fileInfoList.count();
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

            } break;

            default:
            break;
        }
    }

    return QVariant::fromValue(NULL);
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
        FileListModelItem* item = fileInfoList[aIndex.row()];

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

            default:
            break;
        }
    }

    return false;
}

//==============================================================================
// Destructor
//==============================================================================
FileListModel::~FileListModel()
{
    // Clear
    clear();

    // ...

    qDebug() << "FileListModel::~FileListModel";
}

