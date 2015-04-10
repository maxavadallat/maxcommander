#include <QDebug>

#include "deleteprogressmodel.h"


//==============================================================================
// Constructor
//==============================================================================
DeleteProgressModelItem::DeleteProgressModelItem(const QString& aFileName)
    : fileName(aFileName)
    , done(false)
{

}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressModelItem::~DeleteProgressModelItem()
{

}






//==============================================================================
// Constructor
//==============================================================================
DeleteProgressModel::DeleteProgressModel(QObject* aParent)
    : QAbstractListModel(aParent)
{
    qDebug() << "DeleteProgressModel::DeleteProgressModel";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void DeleteProgressModel::init()
{
    qDebug() << "DeleteProgressModel::init";

    // ...
}

//==============================================================================
// Add Item
//==============================================================================
void DeleteProgressModel::addItem(const QString& aFileName)
{
    // Check File Name If Already Exists
    if (findIndex(aFileName) >= 0) {
        qDebug() << "DeleteProgressModel::addItem - aFileName: " << aFileName << " - ALREADY EXISTS!";

        return;
    }

    qDebug() << "DeleteProgressModel::addItem - aFileName: " << aFileName;

    // Create New Item
    DeleteProgressModelItem* newItem = new DeleteProgressModelItem(aFileName);

    // Begin Append Rows
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Append Item
    items << newItem;

    // End Insert Rows
    endInsertRows();
}

//==============================================================================
// Insert Item
//==============================================================================
void DeleteProgressModel::insertItem(const int& aIndex, const QString& aFileName)
{
    // Check File Name If Already Exists
    if (findIndex(aFileName) >= 0) {
        qDebug() << "DeleteProgressModel::insertItem - aFileName: " << aFileName << " - ALREADY EXISTS!";

        return;
    }

    qDebug() << "DeleteProgressModel::insertItem - aIndex: " << aIndex << " - aFileName: " << aFileName;

    // Create New Item
    DeleteProgressModelItem* newItem = new DeleteProgressModelItem(aFileName);

    // Init Insertion Index
    int insertIndex = qBound(0, aIndex, rowCount()-1);

    // Begin Append Rows
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);

    // Insert Item
    items.insert(insertIndex, newItem);

    // End Insert Rows
    endInsertRows();
}

//==============================================================================
// Remove Item
//==============================================================================
void DeleteProgressModel::removeItem(const int& aIndex)
{
    // Check Index
    if (aIndex >=0 && aIndex < rowCount()) {
        qDebug() << "DeleteProgressModel::removeItem - aIndex: " << aIndex;
        // Begin REmove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);
        // Delete Item
        items.removeAt(aIndex);
        // End remove Rows
        endRemoveRows();
    }
}

//==============================================================================
// Set Done
//==============================================================================
void DeleteProgressModel::setDone(const int& aIndex, const bool& aDone)
{
    // Check Index
    if (aIndex >=0 && aIndex < rowCount()) {
        qDebug() << "DeleteProgressModel::setDone - aIndex: " << aIndex;
        // Set Data
        setData(createIndex(aIndex, 0), aDone, ERIDDone);
    }
}

//==============================================================================
// Get File Name
//==============================================================================
QString DeleteProgressModel::getFileName(const int& aIndex)
{
    return data(createIndex(aIndex, 0), ERIDFileName).toString();
}

//==============================================================================
// Find Index
//==============================================================================
int DeleteProgressModel::findIndex(const QString& aFileName)
{
    // Get Items Count
    int iCount = items.count();

    // Go Thru Items
    for (int i=0; i<iCount; ++i) {
        // Get Item
        DeleteProgressModelItem* item = items[i];
        // Check File Name
        if (item && item->fileName == aFileName) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> DeleteProgressModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File Name
    roles[ERIDFileName] = "fileName";
    // Operation Done
    roles[ERIDDone]     = "done";

    return roles;

}

//==============================================================================
// Get Row Count
//==============================================================================
int DeleteProgressModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return items.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int DeleteProgressModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 2;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant DeleteProgressModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        DeleteProgressModelItem* item = items[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case ERIDFileName: return item->fileName;
            case ERIDDone:     return item->done;

            default:
            break;
        }
    }

    return QVariant();
}

//==============================================================================
// Set Data
//==============================================================================
bool DeleteProgressModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        DeleteProgressModelItem* item = items[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case ERIDFileName:
                // Set Operation
                item->fileName = aValue.toString();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            case ERIDDone:
                // Set Done
                item->done = aValue.toBool();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            default:
            break;
        }
    }

    return false;
}

//==============================================================================
// Clear
//==============================================================================
void DeleteProgressModel::clear()
{
    qDebug() << "DeleteProgressModel::clear";

    // Begin Reset Model
    beginResetModel();

    // Go Thru Items
    while (items.count() > 0) {
        // Take & Delete Last Item
        delete items.takeLast();
    }
    // Clear Items
    //items.clear();
    // End Reset Model
    endResetModel();
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressModel::~DeleteProgressModel()
{
    // Clear
    clear();

    qDebug() << "DeleteProgressModel::~DeleteProgressModel";
}

