#include <QSettings>
#include <QDebug>

#include "filelistmodel.h"


FileListModelItem::FileListModelItem()
    : selected(false)
{

}








//==============================================================================
// Constructor
//==============================================================================
FileListModel::FileListModel(QObject* aParent)
    : QAbstractListModel(aParent)
{
    qDebug() << "FileListModel::FileListModel";

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

