
// INCLUDES

#include <QDebug>

#include "listmodel.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ListModelItemData::ListModelItemData( QObject* aParent)
    : QObject(aParent)
    , index(-1)
    , selected(false)
    , current(false)
{
}

//==============================================================================
// Set Title
//==============================================================================
void ListModelItemData::setTitle(const QString& aTitle)
{
    // Set Title
    title = aTitle;
}

//==============================================================================
// Get Title
//==============================================================================
QString ListModelItemData::getTitle()
{
    return title;
}

//==============================================================================
// Set Current Flag
//==============================================================================
void ListModelItemData::setCurrent(const bool& aCurrent)
{
    // Set Current Flag
    current = aCurrent;
    // Emit Data Updated
    emit dataUpdated();
}

//==============================================================================
// Set Selected Flag
//==============================================================================
void ListModelItemData::setSelected(const bool& aSelected)
{
    // Set Selected Flag
    selected = aSelected;
    // Emit Data Updated
    emit dataUpdated();
}

//==============================================================================
// Toggle Selected Flag
//==============================================================================
void ListModelItemData::toggleSelected()
{
    // Set Selected Flag
    selected = !selected;
    // Emit Data Updated
    emit dataUpdated();
}

//==============================================================================
// Trigger Update
//==============================================================================
void ListModelItemData::triggerUpdate()
{
    //qDebug() << "ListModelItemData::triggerUpdate";
    // Emit Data Updated
    emit dataUpdated();
}

//==============================================================================
// Destructor
//==============================================================================
ListModelItemData::~ListModelItemData()
{
}







//==============================================================================
// Constructor
//==============================================================================
ListModel::ListModel(QObject* aParent)
    : QObject(aParent)
{
    qDebug() << "Creating ListModel...done";
}

//==============================================================================
// Append Item
//==============================================================================
void ListModel::appendItem(ListModelItemData* aItem, const bool& aUpdate)
{
    // Check Item
    if (aItem) {
        //qDebug() << "ListModel::appendItem";
        // Set Item Index
        aItem->index = data.count();
        // Append Item
        data << aItem;
        // Check Update
        if (aUpdate) {
            // Emit Item Added Signal
            emit itemAdded(aItem->index);
        }
    }
}

//==============================================================================
// Insert Item
//==============================================================================
void ListModel::insertItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate)
{
    // Check Item
    if (!aItem)
        return;
    //qDebug() << "ListModel::insertItem - aIndex: " << aIndex;
    // Get Count
    int dCount = data.count();
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        // Set Index
        aItem->index = aIndex;
        // Insert New Item
        data.insert(aIndex, aItem);
        // Update Count
        dCount = data.count();
        // Update Indexes
        for (int i=aIndex+1; i<dCount; i++) {
            // Get Data
            ListModelItemData* item = data[i];
            // Check Item
            if (item) {
                // Set Index
                item->index = i;
            }
        }
    } else {
        // Set Index
        aItem->index = dCount-1;
        // Append Item
        data << aItem;
    }

    // Check Update
    if (aUpdate) {
        // Emit Item Updated Signal
        emit itemAdded(aItem->index);
    }
}

//==============================================================================
// Replace Item
//==============================================================================
void ListModel::replaceItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate)
{
    // Check Item
    if (!aItem)
        return;
    //qDebug() << "ListModel::replaceItem - aIndex: " << aIndex;
    // Get Count
    int dCount = data.count();
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        // Get Item Data
        ListModelItemData* item = data[aIndex];
        // Delete Item Data
        delete item;
        item = NULL;
        // Remove Index
        data.removeAt(aIndex);
        // Set Index
        aItem->index = aIndex;
        // Insert New Item
        data.insert(aIndex, aItem);
        // Check Update
        if (aUpdate) {
            // Emit Item Updated Signal
            emit itemUpdated(aItem->index);
        }
    } else {
        // Set Index
        aItem->index = dCount-1;
        // Append Item
        data << aItem;
        // Check Update
        if (aUpdate) {
            // Emit Item Added Signal
            emit itemAdded(aItem->index);
        }
    }
}

//==============================================================================
// Remove Item
//==============================================================================
void ListModel::removeItem(const int& aIndex, const bool& aUpdate)
{
    // Get Count
    int dCount = data.count();
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        //qDebug() << "ListModel::removeItem - aIndex: " << aIndex;
        // Get Item Data
        ListModelItemData* item = data[aIndex];
        // Delete Item Data
        delete item;
        item = NULL;
        // Remove Index
        data.removeAt(aIndex);
        // Update Count
        dCount = data.count();
        // Update Indexes
        for (int i=aIndex; i<dCount; i++) {
            // Get Item
            item = data[i];
            // Check Item
            if (item) {
                // Set Index
                item->index = i;
            }
        }
        // Check Update
        if (aUpdate) {
            // Emit Item Removed Signal
            emit itemRemoved(aIndex);
        }
    }
}

//==============================================================================
// Clear
//==============================================================================
void ListModel::clear()
{
    //qDebug() << "ListModel::clear";
    // Get Count
    int dCount = data.count();
    // Go Thru Items
    for (int i=dCount-1; i>=0; i--) {
        // Get Item Data
        ListModelItemData* item = data[i];
        // Check Item
        if (item) {
            // Delete Item Data
            delete item;
            item = NULL;
        }
    }

    // Clear Data
    data.clear();
}

//==============================================================================
// Get Count
//==============================================================================
int ListModel::count()
{
    return data.count();
}

//==============================================================================
// Get Item
//==============================================================================
ListModelItemData* ListModel::getItem(const int& aIndex)
{
    // Get Count
    int dCount = data.count();
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        return data[aIndex];
    }

    return NULL;
}

//==============================================================================
// Destructor
//==============================================================================
ListModel::~ListModel()
{
    // Clear
    clear();
}

