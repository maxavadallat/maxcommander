
// INCLUDES

#include "constants.h"
#include "customheader.h"
#include "ui_customheader.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
CustomHeader::CustomHeader(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::CustomHeader)
    , active(false)
    , index(-1)
    , prevSelIndex(-1)
    , reverse(false)
    , nextItemMaxSize(-1)
{
    // Setup UI
    ui->setupUi(this);

    // Set Up Header Items
    ui->filename->setTitle(QString("File name"));
    ui->filename->setTitleAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ui->extension->setTitle(QString("Ext"));
    ui->extension->setTitleAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ui->size->setTitle(QString("Size"));
    ui->size->setTitleAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->date->setTitle(QString("Date"));
    ui->date->setTitleAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->permissions->setTitle(QString("Perms"));
    ui->permissions->setTitleAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->ownership->setTitle(QString("Owner"));
    ui->ownership->setTitleAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    // Append Items To Item List
    appendItemToList(ui->filename);
    appendItemToList(ui->extension);
    appendItemToList(ui->size);
    appendItemToList(ui->date);
    appendItemToList(ui->ownership);
    appendItemToList(ui->permissions);

    // Set Focus Policy
    setFocusPolicy(Qt::NoFocus);
}

//==============================================================================
// Set Header Active
//==============================================================================
void CustomHeader::setActive(const bool& aActive, const bool& aRefresh)
{
    // Check Active State
    if (active != aActive) {
        // Set Active State
        active = aActive;
        // Get Header Items Count
        int hiCount = itemList.count();
        // Got Thru Items
        for (int i=0; i<hiCount; i++) {
            // Set Item Active
            itemList[i]->setActive(active, aRefresh);
        }
    }
}

//==============================================================================
// Set Sorting Order
//==============================================================================
void CustomHeader::setSorting(const int& aIndex, const bool& aReversed, const bool& aRefresh)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Set Index
        index = aIndex;
        // Set Reversed Order
        reverse = aReversed;
        // Set Header Item Selected
        itemList[index]->setSelected(true, aRefresh);
        // Set Reversed State
        itemList[index]->setReversed(reverse, aRefresh);
    }
}

//==============================================================================
// Get Sorting Order
//==============================================================================
int CustomHeader::getSorting()
{
    return index;
}

//==============================================================================
// Get Sorting Reversed Direction
//==============================================================================
bool CustomHeader::getReversed()
{
    return reverse;
}

//==============================================================================
// Get Header Active
//==============================================================================
bool CustomHeader::getActive()
{
    return active;
}

//==============================================================================
// Append Header Item
//==============================================================================
void CustomHeader::appendItem(HeaderItem* aItem)
{
    // Append Item To List
    appendItemToList(aItem);

    // ...
}

//==============================================================================
// Remove Header Item
//==============================================================================
void CustomHeader::removeItem(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Count
    if (aIndex >= 0 && aIndex < hiCount) {
        // Get Item
        HeaderItem* item = itemList[aIndex];
        // Remove Item From List
        removeItemFromList(aIndex);
        // Check Item
        if (item) {
            // Delete Item
            delete item;
        }
    }
}

//==============================================================================
// Get Header Item Count
//==============================================================================
int CustomHeader::getItemCount()
{
    return itemList.count();
}

//==============================================================================
// Set Header Item Width
//==============================================================================
void CustomHeader::setItemWidth(const int& aIndex, const int& aWidth)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Set Item Width
        itemList[aIndex]->setWidth(aWidth);
    }
}

//==============================================================================
// Get Header Item Width
//==============================================================================
int CustomHeader::getItemWidth(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Check Item In Use
        if (!itemList[aIndex]->getInUse())
            return -1;

        return itemList[aIndex]->getWidth();
    }

    return 0;
}

//==============================================================================
// Set Header Item Useage
//==============================================================================
void CustomHeader::setItemUseage(const int& aIndex, const bool& aInuse)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex > 0 && aIndex < hiCount) {
        // Check Item Usage
        if (itemList[aIndex]->getInUse() != aInuse) {
            // Set In Use
            itemList[aIndex]->setInUse(aInuse);
            // Check Index
            if (index == aIndex && !aInuse) {
                // Set Sorting
                setSorting(0, reverse);
            }
            // Check Item 0 Width
            if (getItemSize(0) != getItemMaxSize(0, false)) {
                // Set Item Width
                setItemWidth(0, getItemMaxSize(0, false));
            }
        }
    }

    // Go Thru Items To Set Resize Areas Visibility
    for (int i=0; i<hiCount; i++) {
        // Set Item Resize Area Visibility
        itemList[i]->setResizeAreaVisible(true);
    }

    // Go Thru Items Backwards To Find Last Used Item
    for (int i=hiCount-1; i>=0; i--) {
        // Check Item In use
        if (itemList[i]->getInUse()) {
            // Set Item Resize Area Visibility
            itemList[i]->setResizeAreaVisible(false);
            return;
        }
    }
}

//==============================================================================
// Get Header Item Useage
//==============================================================================
bool CustomHeader::getItemUseage(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        return itemList[aIndex]->getInUse();
    }

    return false;
}

//==============================================================================
// Header Selection Changed Slot
//==============================================================================
void CustomHeader::headerSelectionChanged(const int& aIndex, const bool& aSelected, const bool& aActivation)
{
    //qDebug() << "CustomHeader::headerSelectionChanged - aIndex: " << aIndex << " - aSelected: " << aSelected;
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Selected
    if (aSelected) {
        // Check Previous Selection
        if (prevSelIndex >= 0) {
            // Deselect Previous Selection
            if (prevSelIndex >= 0 && prevSelIndex < hiCount) {
                // Set Selected State
                itemList[prevSelIndex]->setSelected(false);
            }
        }
        // Set Previous Selection Index
        prevSelIndex = aIndex;
        // Check Activation
        if (aActivation) {
            // Set Active State
            setActive(true);
            // Update
            update();
        }
        // Set Sorting Index
        index = aIndex;
        // Set Reverse
        reverse = itemList[aIndex]->getReversed();
        //qDebug() << "CustomHeader::headerSelectionChanged - orderChanged index: " << index << " - order: " << reverse;
        // Relay Item Selected Signal
        emit orderChanged(index, reverse);
    } else {
        // Reset previous Selected Index
        prevSelIndex = -1;
    }
}

//==============================================================================
// Header Order Changed Slot
//==============================================================================
void CustomHeader::headerOrderChanged(const int& aIndex, const bool& aReversed, const bool& aActivation)
{
    //qDebug() << "CustomHeader::headerOrderChanged - aIndex: " << aIndex << " - aReversed: " << aReversed;
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount && (index != aIndex || reverse != aReversed || aActivation)) {
        qDebug() << "CustomHeader::headerOrderChanged - orderChanged index: " << aIndex << " - order: " << aReversed;
        // Set Order Index
        index = aIndex;
        // Set Reversed State
        reverse = aReversed;
        // Check Activation
        if (aActivation) {
            // Set Active State
            setActive(true);
            // Update
            update();
        }
        // Relay Item Selected Signal
        emit orderChanged(index, reverse);
    }
}

//==============================================================================
// Header Item Size Changed Slot
//==============================================================================
void CustomHeader::headerItemSizeChanged(const int& aIndex, const int& aWidth, const int& aDelta)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount && aDelta) {
        //qDebug() << "CustomHeader::headerItemSizeChanged - index: " << aIndex << " - aWidth: " << aWidth << " - aDelta: " << aDelta;
        // Adjust Items Size And Pos
        adjustItemsSizeAndPos(aIndex, aDelta);
        // Relay Header Size Changed Signal
        emit itemSizeChanged(aIndex, aWidth);
    }
}

//==============================================================================
// Header Item Pos Changed Slot
//==============================================================================
void CustomHeader::headerItemPosChanged(const int& aIndex, const int& aDelta)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount && aDelta) {
        //qDebug() << "CustomHeader::headerItemPosChanged - index: " << aIndex << " - aDelta: " << aDelta;
        // Adjust Items Size And Pos
        adjustItemsSizeAndPos(aIndex, aDelta);
        // Relay Header pos Changed Signal
        emit itemPosChanged(aIndex, itemList[aIndex]->geometry().x());
        // Get Prev Item
        HeaderItem* prevItem = aIndex > 0 ? itemList[aIndex-1] : NULL;
        // Check Prev Item
        if (prevItem) {
            // Get Previous Item Right
            int prevItemRight = getItemRight(aIndex-1);
            // Get Current Pos X
            int currPosX = itemList[aIndex]->geometry().x();

            //qDebug() << "CustomHeader::headerItemPosChanged - prevRight: " << prevItemRight;
            //qDebug() << "CustomHeader::headerItemPosChanged - currPosX: " << currPosX;

            // Check Prev Item Right
            if (prevItemRight > currPosX) {
                qDebug() << "CustomHeader::headerItemPosChanged - Previous Item Right is OFF!";
                // Adjust Prev Item Width
                prevItem->setWidth(prevItem->width() - (prevItemRight - currPosX));
            }
        }
    }
}

//==============================================================================
// Header Item Resize Started Slot
//==============================================================================
void CustomHeader::headerItemResizeStarted(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        //qDebug() << "CustomHeader::headerItemResizeStarted - index: " << aIndex;
/*
        // Check Index
        if ((aIndex + 1) < hiCount) {
            // Get Next Item Max Size
            nextItemMaxSize = itemList[aIndex+1]->getWidth();

            // ...
        }
*/
    }
}

//==============================================================================
// Header Item Resize Finished Slot
//==============================================================================
void CustomHeader::headerItemResizeFinished(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        qDebug() << "CustomHeader::headerItemResizeFinished - index: " << aIndex;
/*
        // Check Next Item Max Size
        if (nextItemMaxSize >= 0 && (aIndex + 1) < hiCount) {

            // ...
        }
*/
    }
}

//==============================================================================
// Header Item Resize Area Double CLicked Slot
//==============================================================================
void CustomHeader::headerItemResizeAreaDoubleClicked(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        //qDebug() << "CustomHeader::headerItemResizeAreaDoubleClicked - index: " << aIndex;
        // Check Index
        if (aIndex < (hiCount-1)) {
            // Get Next Header Item
            HeaderItem* nextHeaderItem = itemList[aIndex+1];
            // Check Next Header Item
            if (nextHeaderItem && nextHeaderItem->minimumWidth() == 0) {
/*
                // Set Minimum Size
                nextHeaderItem->setMinimumWidth(DEFAULT_LISTBOX_ITEM_HEADER_MINIMUM_WIDTH);
                // Set Maximum Width
                nextHeaderItem->setMaximumWidth(DEFAULT_LISTBOX_ITEM_HEADER_MINIMUM_WIDTH);
*/
                // Set Default Width
                nextHeaderItem->setWidth(DEFAULT_LISTBOX_ITEM_HEADER_MINIMUM_WIDTH);
            }
        }
    }
}

//==============================================================================
// Header Item Right Clicked
//==============================================================================
void CustomHeader::headerItemRightClicked(const int& aIndex, const QPoint& aPos)
{
    qDebug() << "CustomHeader::headerItemRightClicked - aIndex: " << aIndex;
/*
    if (aIndex == 0) {
        // Get Header Items Count
        int hiCount = itemList.count();
        // Go Thru Items
        for (int i=1; i<hiCount; i++) {
            // Set Item Usage
            setItemUseage(i, true);
        }
    } else {
        // Set Header Item In Use
        setItemUseage(aIndex, false);
    }
*/
    // Emit Header Options Signal
    emit headerOptions(aPos);
}

//==============================================================================
// Get Item Size
//==============================================================================
int CustomHeader::getItemSize(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Check Visibility
        if (itemList[aIndex]->getInUse()) {
            return itemList[aIndex]->width();
        }
    }

    return 0;
}

//==============================================================================
// Get Item Minimum Size
//==============================================================================
int CustomHeader::getItemMinSize(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Check Visibility
        if (itemList[aIndex]->getInUse()) {
            return itemList[aIndex]->minimumWidth();
        }
    }

    return 0;
}

//==============================================================================
// Get Item Maximum Size
//==============================================================================
int CustomHeader::getItemMaxSize(const int& aIndex, const bool& useMinWidth)
{
    // Get Header Items Count
    int hiCount = itemList.count();

    // Check Index
    if (aIndex < 0 && aIndex >= hiCount) {
        //qDebug() << "CustomHeader::getItemMaxSize - aIndex: " << aIndex << " - INDEX IS OUTSIDE OF BOUNDS";
        return 0;
    }
/*
    // Check Visibility
    if (!itemList[aIndex]->isVisible()) {
        //qDebug() << "CustomHeader::getItemMaxSize - aIndex: " << aIndex << " - ITEM IS NOT VISIBLE";
        //return 0;
    }
*/
    // Check Item In use
    if (!itemList[aIndex]->getInUse()) {
        //qDebug() << "CustomHeader::getItemMaxSize - aIndex: " << aIndex << " - ITEM IS NOT IN USE";
        return 0;
    }

    // Init Max Size
    int maxSize = geometry().width();

    //qDebug() << "#### CustomHeader::getItemMaxSize - aIndex: " << aIndex << " - maxSize: " << maxSize;

    // Init Items Size
    int itemsSize = 0;

    // Go Thru Items
    for (int i=0; i<hiCount; i++) {

        // Check Index
        if (i != aIndex && itemList[i]->getInUse()) {
            // Check Use Min Width
            if (useMinWidth) {
                //qDebug() << "CustomHeader::getItemMaxSize - item[" << i << "] minWidth: " << itemList[i]->minimumWidth();
                // Add To Items Size Size
                itemsSize += itemList[i]->minimumWidth();
            } else {
                //qDebug() << "CustomHeader::getItemMaxSize - item[" << i << "] width: " << itemList[i]->geometry().width();
                // Add To Items Size Size
                itemsSize += itemList[i]->geometry().width();
            }
        }
    }

    //qDebug() << "CustomHeader::getItemMaxSize - aIndex: " << aIndex << " - maxSize: " << maxSize - itemsSize;

    return qMax(0, maxSize - itemsSize);
}

//==============================================================================
// Append Header Item To Item List
//==============================================================================
void CustomHeader::appendItemToList(HeaderItem* aItem)
{
    // Check Item
    if (aItem) {
        qDebug() << "CustomHeader::appendItemToList";
        // Get Last Item
        HeaderItem* lastItem = (itemList.count() > 0) ? itemList[itemList.count()-1] : NULL;
        // Check Last Item
        if (lastItem) {
            // Set Resize Area Visibility
            lastItem->setResizeAreaVisible(true);
        }

        // Set Parent Header
        aItem->setParentHeader(this);
        // Set Index
        aItem->setIndex(itemList.count());
        // Set Focus Policy
        aItem->setFocusPolicy(Qt::NoFocus);

        // Connect Signals
        connect(aItem, SIGNAL(selectionChanged(int,bool,bool)), this, SLOT(headerSelectionChanged(int,bool,bool)));
        connect(aItem, SIGNAL(orderChanged(int,bool,bool)), this, SLOT(headerOrderChanged(int,bool,bool)));
        connect(aItem, SIGNAL(sizeChanged(int,int,int)), this, SLOT(headerItemSizeChanged(int,int,int)));
        connect(aItem, SIGNAL(posChanged(int,int)), this, SLOT(headerItemPosChanged(int,int)));

        connect(aItem, SIGNAL(resizeStarted(int)), this, SLOT(headerItemResizeStarted(int)));
        connect(aItem, SIGNAL(resizeFinished(int)), this, SLOT(headerItemResizeFinished(int)));
        connect(aItem, SIGNAL(resizeDoubleClicked(int)), this, SLOT(headerItemResizeAreaDoubleClicked(int)));

        connect(aItem, SIGNAL(rightClicked(int, QPoint)), this, SLOT(headerItemRightClicked(int, QPoint)));

        // Set Resize Area Visibility
        aItem->setResizeAreaVisible(false);
        // Append Item
        itemList << aItem;
        // Update Minimum Size
        updateMinSize();
    }
}

//==============================================================================
// Remove Header Item From Item List
//==============================================================================
void CustomHeader::removeItemFromList(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        //qDebug() << "CustomHeader::removeItemFromList";
        // Remove Item
        itemList.removeAt(aIndex);
    }
}

//==============================================================================
// Can Header Item Shrink
//==============================================================================
bool CustomHeader::canItemShrink(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Check Item Width
        if (itemList[aIndex]->width() > itemList[aIndex]->minimumWidth())
            return true;
    }

    return false;
}

//==============================================================================
// Can Header Item Grow
//==============================================================================
bool CustomHeader::canItemGrow(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Go Thru Items
        for (int i=aIndex+1; i<hiCount; i++) {
            // Check Item Width
            if (itemList[i]->width() > itemList[i]->minimumWidth()) {
                return true;
            }
        }
    }

    return false;
}

//==============================================================================
// Adjust Items Size And Position
//==============================================================================
void CustomHeader::adjustItemsSizeAndPos(const int& aIndex, const int& aDelta)
{
    Q_UNUSED(aDelta);
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        //qDebug() << "CustomHeader::adjustItemsSizeAndPos - aIndex: " << aIndex;
        // Get Current Item
        HeaderItem* currItem = itemList[aIndex];
        // Get Next Item Index
        int nextIndex = getNextItemIndex(aIndex);
        // Get Next Item
        HeaderItem* nextItem = getItem(nextIndex);
        // Check Next Item
        if (nextItem) {
            // Set Next Header Item Geometry
            nextItem->setGeometry(qMin(currItem->geometry().right() + 1, getItemMaxPos(nextIndex)), nextItem->geometry().y(), getItemMaxSize(nextIndex, false), nextItem->geometry().height());
        }
    }
}

//==============================================================================
// Get Item Right
//==============================================================================
int CustomHeader::getItemRight(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        //qDebug() << "CustomHeader::getItemRight - aIndex: " << aIndex;
        return itemList[aIndex]->geometry().x() + itemList[aIndex]->geometry().width();
    }

    return 0;
}

//==============================================================================
// Get Item Max Pos
//==============================================================================
int CustomHeader::getItemMaxPos(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        // Init Max Pos
        int maxPos = width();
        // Go Thru Items
        for (int i=hiCount-1; i>=0 && i>=aIndex; i--) {
            // Adjust Max Pos
            maxPos -= itemList[i]->getInUse() ?  itemList[i]->minimumWidth() : 0;
        }

        return maxPos;
    }

    return 0;
}

//==============================================================================
// Update Minimum Size
//==============================================================================
void CustomHeader::updateMinSize()
{
    //qDebug() << "CustomHeader::updateMinSize";
    // Get Header Items Count
    int hiCount = itemList.count();
    // Init New Min Size
    int newMinSize = 0;
    // Go Thru Items
    for (int i=0; i<hiCount; i++) {
        // Update New Min Size
        newMinSize += itemList[i]->minimumWidth();
    }
    // Set Minimum Size
    setMinimumWidth(newMinSize);
}

//==============================================================================
// Get Next In Use Item Index
//==============================================================================
int CustomHeader::getNextItemIndex(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Go Thru Items
    for (int i=aIndex+1; i<hiCount; i++) {
        // Get Item
        HeaderItem* headerItem = itemList[i];
        // Check Header Item
        if (headerItem && headerItem->getInUse()) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Get Item
//==============================================================================
HeaderItem* CustomHeader::getItem(const int& aIndex)
{
    // Get Header Items Count
    int hiCount = itemList.count();
    // Check Index
    if (aIndex >= 0 && aIndex < hiCount) {
        return itemList[aIndex];
    }

    return NULL;
}

//==============================================================================
// Resize Event
//==============================================================================
void CustomHeader::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Get Header Items Count
        int hiCount = itemList.count();
        // Check Count
        if (hiCount > 0) {
            //qDebug() << "CustomHeader::resizeEvent - size: " << aEvent->size();
            // Set First Item Width
            itemList[0]->setWidth(getItemMaxSize(0, false));
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
CustomHeader::~CustomHeader()
{
    // Delete UI
    delete ui;
}
