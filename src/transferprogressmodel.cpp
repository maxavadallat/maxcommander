#include <QDebug>

#include "transferprogressmodel.h"



//==============================================================================
// Constructor
//==============================================================================
TransferProgressModelItem::TransferProgressModelItem(const QString& aOp, const QString& aSource, const QString& aTarget)
    : op(aOp)
    , source(aSource)
    , target(aTarget)
    , done(false)
{

}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressModelItem::~TransferProgressModelItem()
{

}







//==============================================================================
// Constructor
//==============================================================================
TransferProgressModel::TransferProgressModel(QObject* aParent)
    : QAbstractListModel(aParent)
{
    qDebug() << "TransferProgressModel::TransferProgressModel";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void TransferProgressModel::init()
{
    qDebug() << "TransferProgressModel::init";

}

//==============================================================================
// Add Item
//==============================================================================
void TransferProgressModel::addItem(const QString& aOp, const QString& aSource, const QString& aTarget)
{
    qDebug() << "TransferProgressModel::init";

    // Create New Item
    TransferProgressModelItem* newItem = new TransferProgressModelItem(aOp, aSource, aTarget);

    // Begin Append Rows
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Append Item
    items << newItem;

    // End Insert Rows
    endInsertRows();
}

//==============================================================================
// Remove Item
//==============================================================================
void TransferProgressModel::removeItem(const int& aIndex)
{
    // Check Index
    if (aIndex >=0 && aIndex < rowCount()) {
        qDebug() << "TransferProgressModel::removeItem - aIndex: " << aIndex;
        // Begin REmove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);
        // Delete Item
        delete items.takeAt(aIndex);
        // End remove Rows
        endRemoveRows();
    }
}

//==============================================================================
// Set Done
//==============================================================================
void TransferProgressModel::setDone(const int& aIndex, const bool& aDone)
{
    // Check Index
    if (aIndex >=0 && aIndex < rowCount()) {
        qDebug() << "TransferProgressModel::setDone - aIndex: " << aIndex;
        // Set Data
        setData(createIndex(aIndex, 0), aDone, ERIDDone);
    }
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> TransferProgressModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File Operation
    roles[ERIDOp]       = "fileOp";
    // File Source
    roles[ERIDSource]   = "fileSource";
    // File Target
    roles[ERIDTarget]   = "fileTarget";
    // Operation Done
    roles[ERIDDone]     = "fileOpDone";

    return roles;

}

//==============================================================================
// Get Row Count
//==============================================================================
int TransferProgressModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return items.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int TransferProgressModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 4;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant TransferProgressModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        TransferProgressModelItem* item = items[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case ERIDOp:        return item->op;
            case ERIDSource:    return item->source;
            case ERIDTarget:    return item->target;

            default:
            break;
        }
    }

    return QVariant();
}

//==============================================================================
// Set Data
//==============================================================================
bool TransferProgressModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        TransferProgressModelItem* item = items[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case ERIDOp:
                // Set Operation
                item->op = aValue.toString();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            case ERIDSource:
                // Set Source
                item->source = aValue.toString();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            case ERIDTarget:
                // Set Target
                item->target = aValue.toString();
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
void TransferProgressModel::clear()
{
    qDebug() << "TransferProgressModel::clear";

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
TransferProgressModel::~TransferProgressModel()
{
    // Clear
    clear();

    qDebug() << "TransferProgressModel::~TransferProgressModel";
}

