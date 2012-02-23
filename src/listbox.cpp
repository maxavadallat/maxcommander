// INCLUDES

#include <QtCore/qmath.h>
#include <QPainter>
#include <QDebug>
#include <QSet>

#include "constants.h"
#include "listbox.h"
#include "listmodel.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ItemDelegate::ItemDelegate(ListModelItemData* aData, QWidget* aParent)
    : QFrame(aParent)
    , parentList(NULL)
    , orientation(LBOVertical)
    , data(aData)
    , pressed(false)
    , moved(false)
    , keyPressed(false)
{
    // Check Parent
    if (parentWidget()) {
/*
        // Check Orientation
        if (orientation == LBOVertical) {
            // Resize
            setGeometry(0, 0, parentWidget()->width() - DEFAULT_LISTBOX_ITEM_SPACING * 2, DEFAULT_LISTBOX_ITEM_SIZE);
        } else {
            setGeometry(0, 0, DEFAULT_LISTBOX_ITEM_SIZE, qMin(DEFAULT_LISTBOX_ITEM_SIZE, parentWidget()->height() - DEFAULT_LISTBOX_ITEM_SPACING * 2));
        }
*/
        // Set Parent List Box
        parentList = reinterpret_cast<ListBox*>(parentWidget());
    }

    // Set Visibility
    setVisible(false);

    // Check Data
    if (data) {
        // Connect Signal
        connect(data, SIGNAL(dataUpdated()), this, SLOT(dataUpdated()));
    }

    // Set Focus Policy
    setFocusPolicy(Qt::NoFocus);

    //qDebug() << "Creating ItemDelegate...done";
}

//==============================================================================
// Duplicate
//==============================================================================
ItemDelegate* ItemDelegate::duplicate(ListModelItemData* aData)
{
    return new ItemDelegate(aData, parentWidget());
}

//==============================================================================
// Set Data
//==============================================================================
void ItemDelegate::setData(ListModelItemData* aData, const bool& aUpdate)
{
    // Check Data
    if (data) {
        // Disconnect Previous Signal
        disconnect(data, SIGNAL(dataUpdated()), this, SLOT(dataUpdated()));
    }

    //qDebug() << "ItemDelegate::setData - aUpdate: " << aUpdate;

    // Set Data
    data = aData;

    // Check Data
    if (data) {
        // Connect Signal
        connect(data, SIGNAL(dataUpdated()), this, SLOT(dataUpdated()));
    }
    // Check Update
    if (aUpdate) {
        // Update UI
        updateUI();
    }
}

//==============================================================================
// Get Data
//==============================================================================
ListModelItemData* ItemDelegate::getData()
{
    return data;
}

//==============================================================================
// Set Item Size
//==============================================================================
void ItemDelegate::setItemSize(const QSize& aSize)
{
    //qDebug() << "ItemDelegate::setItemSize - size: " << aSize;
    // Resize ?!?? Not sure if it works...
    resize(aSize);
}

//==============================================================================
// Get Item Size
//==============================================================================
QSize ItemDelegate::getItemSize()
{
    return size();
}

//==============================================================================
// Set Orientation
//==============================================================================
void ItemDelegate::setOrientation(const ListBoxOrientation& aOrientation)
{
    //qDebug() << "ItemDelegate::setOrientation - aOrientation: " << aOrientation;
    // Set Orientation
    orientation = aOrientation;
}

//==============================================================================
// Update UI
//==============================================================================
void ItemDelegate::updateUI()
{
    // ...

    // Update
    update();
}

//==============================================================================
// Paint Event
//==============================================================================
void ItemDelegate::paintEvent(QPaintEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Data
        if (data) {
            // Get Painter
            QPainter painter(this);

            // Set Render Hint
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::HighQualityAntialiasing);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);

            // Check If Has Focus
            if (hasFocus()) {
                // Set Pen
                painter.setPen(Qt::red);
            } else {
                // Set Pen
                painter.setPen(Qt::white);
            }

            // Check If Current Item
            if (data->current) {
                // Set Brush
                painter.setBrush(Qt::darkGray);
            } else {
                // Set Brush
                painter.setBrush(QColor(139, 158, 130));
            }

            //painter.drawRect(rect().adjusted(0,0,-1,-1));
            painter.drawRoundedRect(rect().adjusted(0,0,-1,-1), DEFAULT_FILELIST_ITEM_RADIUS, DEFAULT_FILELIST_ITEM_RADIUS);

            //qDebug() << "ItemDelegate::paintEvent - rect: " << rect();
            //qDebug() << "ItemDelegate::paintEvent - geometry: " << geometry();

            painter.setPen(Qt::white);
            //painter.drawText(rect(), QString("%1: %2").arg(data->index).arg(data->title), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
            painter.drawText(rect(), QString("%1").arg(data->title), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
        }
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void ItemDelegate::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent && data) {
        // Check Mouse Button
        if (aEvent->button() == Qt::LeftButton) {
            // Check Pressed State
            if (!pressed) {
                // Set Pressed State
                pressed = true;
                // Set Original Press Pos
                originalPressPos = aEvent->globalPos();
                // Set Previous Pos
                prevPos = aEvent->globalPos();
                // Reset Moved Flag
                moved = false;

                //qDebug() << "ItemDelegate::mousePressEvent - index: " << data->index;

                // ...

                // Emit Drag started Signal
                emit itemDragStarted(data->index);
            }
        }
    }
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void ItemDelegate::mouseMoveEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent && data) {
        // Check Mouse Button
        //if (aEvent->button() == Qt::LeftButton) {
            // Check Pressed
            if (pressed) {
                // Check Pos
                if (aEvent->globalPos() != prevPos) {
                    // Set Moved Flag
                    moved = true;
                    // Set Prev Pos
                    prevPos = aEvent->globalPos();
                    // Calculate Delta
                    QSize delta = QSize(aEvent->globalPos().x() - originalPressPos.x(), aEvent->globalPos().y() - originalPressPos.y());
                    //qDebug() << "ItemDelegate::mouseMoveEvent - index: " << data->index << " - delta: " << delta;
                    // Emit Item Dragged Signal
                    emit itemDragged(data->index, delta);
                }
            }
        //}
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void ItemDelegate::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent && data) {
        // Check Mouse Button
        if (aEvent->button() == Qt::LeftButton) {
            // Check Pressed
            if (pressed) {
                //qDebug() << "ItemDelegate::mouseReleaseEvent - index: " << data->index;
                // Reset Pressed State
                pressed = false;

                // ...

                // Emit Drag Finished Signal
                emit itemDragFinished(data->index);

                // Check Moved Flag
                if (!moved) {
                    // Emit Item Left Clicked Sizgnal
                    emit itemLeftClicked(data->index);
                }
            }
        } else if (aEvent->button() == Qt::RightButton) {
            // Emit Right Clicked Signal
            emit itemRightClicked(data->index);

            // ...
        }
    }
}

//==============================================================================
// Mouse Double Click Event
//==============================================================================
void ItemDelegate::mouseDoubleClickEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent && data) {
        // Check Mouse Button
        if (aEvent->button() == Qt::LeftButton) {
            //qDebug() << "ItemDelegate::mouseDoubleClickEvent - index: " << data->index;
            // Emit Item Double Clicked Sizgnal
            emit itemDoubleClicked(data->index);
        }
    }
}

//==============================================================================
// Key Press Event
//==============================================================================
void ItemDelegate::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event & Data
    if (aEvent && data) {
        // Check Key Pressed Flag
        if (!keyPressed) {
            // Set Key Pressed Flag
            keyPressed = true;

            // ...

        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void ItemDelegate::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event & Data
    if (aEvent && data) {
        // Check Key Pressed Flag
        if (keyPressed) {
            // Reset Key Pressed Flag
            keyPressed = false;

            // ...

            // Emit Item Key Pressed Signal
            emit itemKeyPressed(data->index, aEvent->key(), aEvent->modifiers());
        }
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void ItemDelegate::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {

    }
}

//==============================================================================
// Data Updated Slot
//==============================================================================
void ItemDelegate::dataUpdated()
{
    // Update UI
    updateUI();
}

//==============================================================================
// Destructor
//==============================================================================
ItemDelegate::~ItemDelegate()
{
    // ...

    //qDebug() << "Deleting ItemDelegate...done";
}


















//==============================================================================
// Constructor
//==============================================================================
ListBox::ListBox(ListBoxOrientation aOrientation, QWidget* aParent)
    : QFrame(aParent)
    , model(NULL)
    , delegate(NULL)
    , orientation(aOrientation)
    , cacheOverhead(DEFAULT_LISTBOX_CACHE_OVERHEAD)
    , cacheCountMax(0)
    , spacing(DEFAULT_LISTBOX_ITEM_SPACING)
    , delegateSize(0)
    , fullItemSize(0)
    , visibleItemCount(0)
    , fullVisibleItemCount(0)
    , visibleItemCountMax(0)
    , firstCachedItem(-1)
    , lastCachedItem(-1)
    , firstVisibleItem(-1)
    , lastVisibleItem(-1)
    , orgScrollPos(0)
    , scrollPos(0)
    , minScrollPos(0)
    , maxScrollPos(0)
    , prevScrollPos(0)
    , currentIndex(-1)
    , easing(false)
    , easeSpeed(DEFAULT_LISTBOX_EASE_ACCELERATION)
    , addedIndex(-1)
    , updatedIndex(-1)
    , removedIndex(-1)
    , easeAnim(NULL)
    , reposAnim(NULL)
    , snapToAnim(NULL)
    , gotoAnim(NULL)
    , dragFilterID(-1)
    , snapToItem(true)
    , pressed(false)
    , keyPressed(false)
    , keyRepeat(false)
    , lastKey(-1)
    , itemsMoved(false)
    , wheelReposID(-1)
{
    qDebug() << "Creating ListBox...";

    // Create Model
    createModel();

    // Setup Ease Anim
    setupEaseAnim();
    // Setup Repos Anim
    setupReposAnim();
    // Setup Snap To Anim
    setupSnapToAnim();
    // Setup Go To Anim
    setupGotoAnim();

    // Set Focus Policy
    //setFocusPolicy(Qt::ClickFocus);
    setFocusPolicy(Qt::StrongFocus);

    qDebug() << "Creating ListBox...done";
}

//==============================================================================
// Create Model
//==============================================================================
void ListBox::createModel()
{
    // Check Model
    if (!model) {
        qDebug() << "ListBox::createModel";
        // Create Model
        model = new ListModel();

        // Connect Signals
        connect(model, SIGNAL(itemAdded(int)), this, SLOT(modelItemAdded(int)));
        connect(model, SIGNAL(itemRemoved(int)), this, SLOT(modelItemRemoved(int)));
        connect(model, SIGNAL(itemUpdated(int)), this, SLOT(modelItemUpdated(int)));
    }
}

//==============================================================================
// Destroy Model
//==============================================================================
void ListBox::destroyModel()
{
    // Check Model
    if (model) {
        qDebug() << "ListBox::destroyModel";
        // Disconnect Signals
        disconnect(model, SIGNAL(itemAdded(int)), this, SLOT(modelItemAdded(int)));
        disconnect(model, SIGNAL(itemRemoved(int)), this, SLOT(modelItemRemoved(int)));
        disconnect(model, SIGNAL(itemUpdated(int)), this, SLOT(modelItemUpdated(int)));
        // Delete Model
        delete model;
        model = NULL;
    }
}

//==============================================================================
// Append Item
//==============================================================================
void ListBox::appendItem(ListModelItemData* aItem, const bool& aUpdate)
{
    // Create Model If Needed
    createModel();

    // Check Model
    if (model) {
        //qDebug() << "ListBox::appendItem";
        // Append Item To Model
        model->appendItem(aItem, aUpdate);
    }
}

//==============================================================================
// Insert Item
//==============================================================================
void ListBox::insertItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate)
{
    // Create Model If Needed
    createModel();

    // Check Model
    if (model) {
        //qDebug() << "ListBox::insertItem - aIndex: " << aIndex;
        // Insert Item To Model
        model->insertItem(aIndex, aItem, aUpdate);
    }
}

//==============================================================================
// Replace Item
//==============================================================================
void ListBox::replaceItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate)
{
    // Create Model If Needed
    createModel();

    // Check Model
    if (model) {
        //qDebug() << "ListBox::replaceItem - aIndex: " << aIndex;
        // Insert Item To Model
        model->replaceItem(aIndex, aItem, aUpdate);
    }
}

//==============================================================================
// Remove Item
//==============================================================================
void ListBox::removeItem(const int& aIndex, const bool& aUpdate)
{
    // Check Model & Index
    if (model && aIndex >= 0 && aIndex < model->count()) {
        //qDebug() << "ListBox::removeItem - aIndex: " << aIndex;
        // Remove Item From Model
        model->removeItem(aIndex, aUpdate);
    }
}

//==============================================================================
// Get Count
//==============================================================================
int ListBox::count()
{
    // Check Model
    if (model) {
        return model->count();
    }

    return 0;
}

//==============================================================================
// Get Selected Item Count
//==============================================================================
int ListBox::selectedCount()
{
    // Check Model
    if (model) {
        // Get Item Count
        int iCount = model->count();
        // Init Selected Count
        int sCount = 0;
        // Go Thru Items
        for (int i=0; i<iCount; i++) {
            // Get Data
            ListModelItemData* itemData = model->data[i];
            // Check Item Data
            if (itemData && itemData->selected) {
                // Inc Selected Count
                sCount++;
            }
        }

        return sCount;
    }

    return 0;
}

//==============================================================================
// Get Current Index
//==============================================================================
int ListBox::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Clear
//==============================================================================
void ListBox::clear()
{
    qDebug() << "ListBox::clear";

    // Clear Cache
    clearCache();

    // Check Model
    if (model) {
        // Clear Model
        model->clear();
    }

    // Update Indexes
    updateIndexes();
    // Reset Indexes
    //resetIndexes();
}

//==============================================================================
// Update List - Full List Update
//==============================================================================
void ListBox::updateList()
{
    qDebug() << "ListBox::updateList";
    // Rebuild Cache
    rebuildCache();
}

//==============================================================================
// Set Model
//==============================================================================
void ListBox::setModel(ListModel* aModel)
{
    // Destroy Model
    destroyModel();
    qDebug() << "ListBox::setModel";
    // Set Model
    model = aModel;
    // Check Model
    if (model) {
        // Connect Signals
        connect(model, SIGNAL(itemAdded(int)), this, SLOT(modelItemAdded(int)));
        connect(model, SIGNAL(itemRemoved(int)), this, SLOT(modelItemRemoved(int)));
        connect(model, SIGNAL(itemUpdated(int)), this, SLOT(modelItemUpdated(int)));
    }
}

//==============================================================================
// Set Delegate
//==============================================================================
void ListBox::setDelegate(ItemDelegate* aDelegate)
{
    // Clear Cache
    clearCache();

    // Check Previous Delegate
    if (delegate) {
        // Delete Delegate
        delete delegate;
        delegate = NULL;
    }

    // Set Delegate
    delegate = aDelegate;

    // Check Delegate
    if (delegate) {
        // Get Delegate Size
        delegateSize = (orientation == LBOVertical) ? delegate->getItemSize().height() : delegate->getItemSize().width();
        // Set Full Item Size
        fullItemSize = delegateSize + spacing;
        // Rebuild Cache
        rebuildCache();

        qDebug() << "ListBox::setDelegate - delegateSize: " << delegateSize;
    }
}

//==============================================================================
// Set Item/Delegate Spacing
//==============================================================================
void ListBox::setSpacing(const int& aSpacing)
{
    // Check Spacing
    if (spacing != aSpacing) {
        qDebug() << "ListBox::setSpacing - aSpacing: " << aSpacing;
        // Set Spacing
        spacing = aSpacing;
        // Set Full Item Size
        fullItemSize = delegateSize + spacing;
        // Rebuild Cache
        rebuildCache();
    }
}

//==============================================================================
// Set Item/Delegate Cahce Overhead
//==============================================================================
void ListBox::setCacheOverhead(const int& aOverHead)
{
    // Check Cache Overhead
    if (cacheOverhead != aOverHead) {
        qDebug() << "ListBox::setCacheOverhead - aOverHead: " << aOverHead;
        // Set Caching Overhead
        cacheOverhead = aOverHead;
        // Rebuild Cache
        rebuildCache();
    }
}

//==============================================================================
// Set Snap To Item Mode
//==============================================================================
void ListBox::setSnapToItemMode(const bool& aSnap)
{
    // Check Snap To Item Mode
    if (snapToItem != aSnap) {
        // Set Snap To Item Mode
        snapToItem = aSnap;
        // Check Snap To Item Mode
        if (snapToItem) {
            // Adjust Scroll pos
            adjustScrollPos();
        }
    }
}

//==============================================================================
// Go To Index
//==============================================================================
void ListBox::gotoIndex(const int& aIndex, const bool& aSelection, const bool& aAnimated)
{
    // Get Item Count
    int iCount = count();
    // Get New Indedx
    int newIndex = qBound(0, aIndex, iCount-1);
    // Check New Index
    if (currentIndex != newIndex) {
        //qDebug() << "ListBox::gotoIndex - newIndex: " << newIndex;

        // Get Increment
        int increment = (newIndex >= currentIndex) ? 1 : -1;

        // Check Selection
        if (aSelection && increment > 0) {
            // Go Thru Items
            for (int i=currentIndex; i<newIndex && i<iCount; i+=increment) {
                // Toggle Item Selected
                toggleItemSelected(i);
            }
        } else if (aSelection && increment < 0) {
            // Go Thru Items
            for (int i=currentIndex; i>newIndex && i>=0; i+=increment) {
                // Toggle Item Selected
                toggleItemSelected(i);
            }
        }

        // Get Item Data
        ListModelItemData* itemData = model ? model->getItem(currentIndex) : NULL;
        // Check Item Data
        if (itemData) {
            // Set Current Flag
            itemData->setCurrent(false);
        }

        // Check New Index
        if (newIndex >= 0 && newIndex < iCount) {
            // Set Current Index
            currentIndex = newIndex;
            // Get Item Data
            itemData = model ? model->getItem(currentIndex) : NULL;
            // Check Item Data
            if (itemData) {
                // Set Current Flag
                itemData->setCurrent(true);
            }

            // Scroll To Index
            scrollToCurrentIndex(aAnimated);
        }
    } else {
        // Check Selection
        if (aSelection) {
            // Toggle Item Selected
            toggleItemSelected(currentIndex);
        }
    }
}

//==============================================================================
// Go To Next Item
//==============================================================================
void ListBox::gotoNextItem(const bool& aSelection, const bool& aAnimated)
{
    //qDebug() << "ListBox::gotoNextItem - aSelection: " << aSelection;
    // Go To Index
    gotoIndex(currentIndex + 1, aSelection, aAnimated);
}

//==============================================================================
// Go To Prev Item
//==============================================================================
void ListBox::gotoPrevItem(const bool& aSelection, const bool& aAnimated)
{
    //qDebug() << "ListBox::gotoPrevItem - aSelection: " << aSelection;
    // Go To Index
    gotoIndex(currentIndex - 1, aSelection, aAnimated);
}

//==============================================================================
// Go To Home
//==============================================================================
void ListBox::gotoHome(const bool& aSelection, const bool& aAnimated)
{
    //qDebug() << "ListBox::gotoHome - aSelection: " << aSelection;
    // Go To Index
    gotoIndex(0, aSelection, aAnimated);
}

//==============================================================================
// Go To End
//==============================================================================
void ListBox::gotoEnd(const bool& aSelection, const bool& aAnimated)
{
    //qDebug() << "ListBox::gotoEnd - aSelection: " << aSelection;
    // Go To Index
    gotoIndex(count() - 1, aSelection, aAnimated);
}

//==============================================================================
// Go One Page Up
//==============================================================================
void ListBox::pageUp(const bool& aSelection, const bool& aAnimated)
{
    //qDebug() << "ListBox::pageUp - aSelection: " << aSelection;
    // Go To Index
    gotoIndex(currentIndex - visibleItemCount, aSelection, aAnimated);
}

//==============================================================================
// Go One Page Down
//==============================================================================
void ListBox::pageDown(const bool& aSelection, const bool& aAnimated)
{
    //qDebug() << "ListBox::pageDown - aSelection: " << aSelection;
    // Go To Index
    gotoIndex(currentIndex + visibleItemCount, aSelection, aAnimated);
}

//==============================================================================
// Set Item Selected
//==============================================================================
void ListBox::setItemSelected(const int& aIndex, const int& aSelected)
{
    // Get Item Data
    ListModelItemData* itemData = model->getItem(aIndex);

    // Check Item Data
    if (itemData) {
        // Set Selected State
        itemData->setSelected(aSelected);
    }
}

//==============================================================================
// Get Item Selected
//==============================================================================
bool ListBox::getItemSelected(const int& aIndex)
{
    // Get Item Data
    ListModelItemData* itemData = model->getItem(aIndex);

    // Check Item Data
    if (itemData) {
        // Set Selected State
        return itemData->selected;
    }

    return false;
}

//==============================================================================
// Toggle Item Selected
//==============================================================================
void ListBox::toggleItemSelected(const int& aIndex)
{
    // Get Item Data
    ListModelItemData* itemData = model->getItem(aIndex);

    // Check Item Data
    if (itemData) {
        // Set Selected State
        itemData->setSelected(!itemData->selected);
    }
}

//==============================================================================
// Reset Indexes
//==============================================================================
void ListBox::resetIndexes()
{
    qDebug() << "ListBox::resetIndexes";

    // Reset Min Scroll Pos
    minScrollPos = 0;
    // Reset Scroll Pos
    scrollPos = 0;
    // Reset Prev Scroll Pos
    prevScrollPos = 0;
    // Reset Original Scroll Pos
    orgScrollPos = 0;
    // Reset Max Scroll Pos
    maxScrollPos = 0;
    // Reset Current Index
    currentIndex = -1;
    // Reset previous Index
    prevIndex = -1;
}

//==============================================================================
// Update Indexes
//==============================================================================
void ListBox::updateIndexes()
{
    // Get List Box Size
    int lbSize = (orientation == LBOVertical) ? height() : width();
    // Get Data Count
    int dCount = model ? model->count() : 0;
    // Check Delegate, Model, Item Count
    if (!delegate || !model || dCount <= 0 || delegateSize == 0) {
        // Reset Visible Item Count Max
        visibleItemCountMax = 0;
        // Reset Visible Item Count
        visibleItemCount = 0;
        // Reset Full Visible Item Count
        fullVisibleItemCount = 0;
        // Reset Cache Count Max
        cacheCountMax = 0;
        // Reset First Visible Index
        firstVisibleItem = -1;
        // Reset Last Visible Index
        lastVisibleItem = -1;
        // Reset First Cached Index
        firstCachedItem = -1;
        // Reset Last Cached Index
        lastCachedItem = -1;
        // Reset Indexes
        resetIndexes();
    } else {
        //qDebug() << "ListBox::updateIndexes";
        // Calculate Max Visible Item Count
        visibleItemCountMax = qMax(qCeil((qreal)lbSize / fullItemSize), 1);
        // Calculate Visible Item Count
        visibleItemCount = qMin(visibleItemCountMax, dCount);
        // Calculate Full Visible Item Count
        fullVisibleItemCount = qMin(qMax(qFloor((qreal)lbSize / fullItemSize), 1), dCount);
        // Calculate Cache Count Max
        cacheCountMax = qMin(visibleItemCount * cacheOverhead / 100, dCount);
        // Calculate First Visible Index
        firstVisibleItem = qMin(qMax((-scrollPos - spacing) / fullItemSize, 0), dCount-1);
        // Calculate Last Visible Index
        lastVisibleItem = qMin(qMax((-scrollPos + lbSize - spacing) / fullItemSize, 0), dCount-1);

        // Check Count
        if (dCount <= cacheCountMax) {
            // Set First Cached Index
            firstCachedItem = 0;
            // Set Last Cached Index
            lastCachedItem = dCount-1;
        } else {
            // Calculate First Cached Index
            firstCachedItem = qMax(qMin(firstVisibleItem - (cacheCountMax / 4), (dCount-1) - (cacheCountMax-1)), 0);
            // Calculate Last Cached Index
            lastCachedItem = qMin(firstCachedItem + (cacheCountMax-1), dCount-1);

            // Check First Cached Item Index
            if (firstCachedItem > lastCachedItem - (cacheCountMax-1)) {
                // Adjust First Cached Item Index
                firstCachedItem = lastCachedItem - (cacheCountMax-1);
            }
        }

        // Check Full Visible Item Count
        if (dCount <= fullVisibleItemCount) {
            // Reset Min Scroll Pos
            minScrollPos = 0;
            // Reset Max Scroll Pos
            maxScrollPos = 0;
            // Reset Scroll Pos
            scrollPos = 0;
            // Reset Previous Scroll Pos
            prevScrollPos = 0;
            // Reset Original Scroll Pos
            orgScrollPos = 0;
        } else {
            // Calculate Min Scroll Pos
            minScrollPos = lbSize - (dCount * fullItemSize + spacing + 1);
            // Calculate Max Scroll Pos
            maxScrollPos = 0;
        }

        // Check Current Index
        if (currentIndex < 0) {
            // Set Current Index
            currentIndex = 0;
            // Get Item Data
            ListModelItemData* itemData = model->getItem(currentIndex);
            // Check Item Data
            if (itemData) {
                // Set Current Flag
                itemData->current = true;
            }
        }
    }

    // Show Debug Info
    //showDebugInfo();
}

//==============================================================================
// Rebuild Delegate Cache
//==============================================================================
void ListBox::rebuildCache()
{
    //qDebug() << "ListBox::rebuildCache";
    // Crear Cache
    clearCache();
    // Udpate Cache
    updateCache();
}

//==============================================================================
// Update Delegate Cache
//==============================================================================
void ListBox::updateCache()
{
    // Check Model
    if (!model || !delegate)
        return;

    //qDebug() << "ListBox::updateCache - dCount: " << model->count();

    // Updte Indexes
    updateIndexes();
    // Maintain Cached Items
    maintainCachedItems();
    // Update Cached Items Data
    updateCachedItemsData();
    // Update Cached Items Positions
    updateCachedItemsSizeAndPos();
}

//==============================================================================
// Maintain Cached Items - Create/Delete/Move Cached Items If Necessary
//==============================================================================
void ListBox::maintainCachedItems()
{
    // Check Model & Count & Delegate
    if (!model || !delegate)
        return;
    // Get Cached Items Count
    int ciCount = cache.count();

    // Check Cached Items Count - Items Added/List Scaled To Bigger
    if (ciCount < cacheCountMax) {
        //qDebug() << "ListBox::maintainCachedItems - Adding Cached Items.";

        // Create New Cached Items
        while (ciCount < cacheCountMax) {
            // Check Cache Count
            if (ciCount > 0) {
                // Check First Cached Item Index - Typically The Case When New Item Added To The Beginning Of The List
                if (cache[0]->data && cache[0]->data->index > firstCachedItem) {
                    // Create New Item
                    ItemDelegate* newItem = delegate->duplicate(getData(cache[0]->data->index-1));
                    // Connect Delegate Signals
                    connectDelegateSignals(newItem);
                    // Insert Item
                    cache.insert(0, newItem);
                // Check last Cached Item Index - Typically The Case When New Item Added To The End Of The List
                } else if (cache[ciCount-1]->data && cache[ciCount-1]->data->index < lastCachedItem) {
                    // Create New Item
                    ItemDelegate* newItem = delegate->duplicate(getData(cache[ciCount-1]->data->index+1));
                    // Connect Delegate Signals
                    connectDelegateSignals(newItem);
                    // Append Item
                    cache << newItem;
                // Typically The Case When New Item Inserted To The List
                } else {
                    // Go Thru Cache
                    for (int i=1; i<ciCount; i++) {
                        // Check Data
                        if (!cache[i]->data) {
                            // Set Data
                            cache[i]->setData(getData(cache[i-1]->data->index+1));
                        } else if (cache[i]->data->index != cache[i-1]->data->index+1) {
                            // Create New Item
                            ItemDelegate* newItem = delegate->duplicate(getData(cache[i-1]->data->index+1));
                            // Connect Delegate Signals
                            connectDelegateSignals(newItem);
                            // Insert Item
                            cache.insert(i, newItem);
                            // Update Cached Item Count
                            ciCount = cache.count();
                        }
                    }
                }
            } else {
                // Create New Item
                ItemDelegate* newItem = delegate->duplicate(getData(firstCachedItem));
                // Connect Delegate Signals
                connectDelegateSignals(newItem);
                // Append New Delegate
                cache << newItem;
            }
            // Update Cached Item Count
            ciCount = cache.count();
        }
    // Check Cached Items Count - Items Removed/List Scaled To Smaller
    } else if (ciCount > cacheCountMax) {
        //qDebug() << "ListBox::maintainCachedItems - Removing Cached Items.";
        // Init Used Indexes
        QSet<int> usedIndexes;
        // Go Thru Cache
        for (int i=ciCount-1; i>=0; i--) {
            // Get Cached Item
            ItemDelegate* item = cache[i];
            // Check Item
            if (item) {
                // Check Item Data
                if (item->data && item->data->index >= firstCachedItem && item->data->index <= lastCachedItem && !usedIndexes.contains(item->data->index)) {
                    // Add Item Index
                    usedIndexes << item->data->index;
                } else {
                    //qDebug() << "ListBox::maintainCachedItems - Removing Cached Item: " << i;
                    // Disconnect Delegate Signals
                    disconnectDelegateSignals(item);
                    // Delete Item
                    delete item;
                    item = NULL;
                    // Remove Cached Item
                    cache.removeAt(i);
                }
            }
        }
    // Check Cached Items Count - Items Scrolled
    } else if (ciCount > 1){

        //qDebug() << "ListBox::maintainCachedItems - Cached Items Count Match.";

        // Check First Cahced Item
        if (cache[0]->data->index > firstCachedItem) {
            //qDebug() << "ListBox::maintainCachedItems - First Cached Item Index Is Bigger Than First Cached Index.";
            // Init Loop Counter
            int n = 0;
            // Move Cached Items From The End Of The Cache
            while (cache[0]->data->index > firstCachedItem && n < ciCount) {
                // Move Cached Item From The
                cache.move(ciCount-1, 0);
                // Set Item Data
                cache[0]->setData(getData(cache[1]->data->index-1), true);
                // Clear Focus
                //cache[0]->clearFocus();
                // Inc Loop Counter
                n++;
            }
        // Check First Cached Item
        } else if (cache[0]->data->index < firstCachedItem) {
            //qDebug() << "ListBox::maintainCachedItems - First Cached Item Index Is Smaller Than First Cached Index.";
            // Init Loop Counter
            int n = 0;
            // Move Cached Items From The End Of The Cache
            while (cache[0]->data->index < firstCachedItem && n < ciCount) {
                // Move Cached Item From The
                cache.move(0, ciCount-1);
                // Set Item Data
                cache[ciCount-1]->setData(getData(cache[ciCount-2]->data->index+1), true);
                // Clear Focus
                //cache[ciCount-1]->clearFocus();
                // Inc Loop Counter
                n++;
            }
        // Check Last Cached Item
        } else if (cache[ciCount-1]->data->index < lastCachedItem) {
            //qDebug() << "ListBox::maintainCachedItems - Last Cached Item Index Is Smaller Than Last Cached Index.";
            // Init Loop Counter
            int n = 0;
            // Move Cached Items From The End Of The Cache
            while (cache[ciCount-1]->data->index < lastCachedItem && n < ciCount) {
                // Move Cached Item From The
                cache.move(0, ciCount-1);
                // Set Item Data
                cache[ciCount-1]->setData(getData(cache[ciCount-2]->data->index+1), true);
                // Clear Focus
                //cache[ciCount-1]->clearFocus();
                // Inc Loop Counter
                n++;
            }
        // Check Last Cached Item
        } else if (cache[ciCount-1]->data->index > lastCachedItem) {
            //qDebug() << "ListBox::maintainCachedItems - Last Cached Item Index Is Bigger Than Last Cached Index.";
            // Init Loop Counter
            int n = 0;
            // Move Cached Items From The End Of The Cache
            while (cache[ciCount-1]->data->index > lastCachedItem && n < ciCount) {
                // Move Cached Item From The
                cache.move(ciCount-1, 0);
                // Set Item Data
                cache[0]->setData(getData(cache[1]->data->index-1), true);
                // Clear Focus
                //cache[0]->clearFocus();
                // Inc Loop Counter
                n++;
            }
        } else {
            //qDebug() << "ListBox::maintainCachedItems - No Adjustment Needed.";

            // ...
        }
    } else {
        //qDebug() << "ListBox::maintainCachedItems - Cached Items Count Match - Single Item.";

        // ...
    }

    //qDebug() << "ListBox::maintainCachedItems - ciCount: " << cache.count();
}

//==============================================================================
// Updating Cached Items Data
//==============================================================================
void ListBox::updateCachedItemsData(const bool& aUpdateUI)
{
    // Get Cached Item Count
    int ciCount = cache.count();
    // Get Data Count
    int dCount = model ? model->data.count() : 0;

    //qDebug() << "ListBox::updateCachedItemsData - ciCount: " << ciCount;

    // Go Thru Cache
    for (int i=0, n=firstCachedItem; i<ciCount && n<=lastCachedItem && n<dCount; i++, n++) {
        // Get Item
        ItemDelegate* item = cache[i];
        // Check Item
        if (item && item->data->index != model->data[n]->index) {
            //qDebug() << "ListBox::updateCachedItemsData - index: " << n;
            // Set Item Data
            item->setData(model->data[n], aUpdateUI);
        }
    }
}

//==============================================================================
// Updating Cached Items Size & Pos
//==============================================================================
void ListBox::updateCachedItemsSizeAndPos()
{
    // Get Cached Item Count
    int ciCount = cache.count();

    //qDebug() << "ListBox::updateCachedItemsSizeAndPos - ciCount: " << ciCount;
    //qDebug() << "ListBox::updateCachedItemsSizeAndPos - scrollPos: " << scrollPos;

    // Calculate Start X
    int startX = spacing;
    // Calculate Start X
    int startY = spacing;

    // Go Thru Cached Items
    for (int i=0; i<ciCount; i++) {
        // Get Item
        ItemDelegate* item = cache[i];
        // Check Item
        if (item && item->data) {
            // Init New Geometry
            QRect newGeometry = QRect(0, 0, 0, 0);
            // Check Orientation
            if (orientation == LBOVertical) {
                newGeometry = QRect((width() - item->getItemSize().width()) / 2,
                                    startY + fullItemSize * item->data->index + scrollPos,
                                    item->getItemSize().width(),
                                    item->getItemSize().height());
            } else {
                // Set Item Geometry
                newGeometry = QRect(startX + fullItemSize * item->data->index + scrollPos,
                                    (height() - item->getItemSize().height()) / 2,
                                    item->getItemSize().width(),
                                    item->getItemSize().height());
            }

            // Set visible
            item->setVisible(true);
            // Check Item Width & Height
            if (item->width() == newGeometry.width() && item->height() == newGeometry.height()) {
                // Set Item Geometry
                item->move(newGeometry.topLeft());
            } else {
                // Set Item Geometry
                item->setGeometry(newGeometry);
            }

            //qDebug() << "ListBox::updateCachedItemsSizeAndPos - item["<< i <<"]: " << newGeometry << " - visible: " << item->isVisible();
        }
    }
}

//==============================================================================
// Connect Delegate Signals
//==============================================================================
void ListBox::connectDelegateSignals(ItemDelegate* aItem)
{
    // Check Item
    if (aItem) {
        // Connect Signals
        connect(aItem, SIGNAL(itemLeftClicked(int)), this, SLOT(delegateLeftClicked(int)));
        connect(aItem, SIGNAL(itemRightClicked(int)), this, SLOT(delegateRightClicked(int)));
        connect(aItem, SIGNAL(itemDoubleClicked(int)), this, SLOT(delegateDoubleClicked(int)));
        connect(aItem, SIGNAL(itemDragStarted(int)), this, SLOT(delegateDragStarted(int)));
        connect(aItem, SIGNAL(itemDragged(int,QSize)), this, SLOT(delegateDragged(int,QSize)));
        connect(aItem, SIGNAL(itemDragFinished(int)), this, SLOT(delegateDragFinished(int)));
        connect(aItem, SIGNAL(itemKeyPressed(int,int,Qt::KeyboardModifiers)), this, SLOT(delegateKeyPressed(int,int,Qt::KeyboardModifiers)));
        connect(aItem, SIGNAL(sizeChanged(int,QSize)), this, SLOT(delegateSizeChanged(int,QSize)));
    }
}

//==============================================================================
// Disconnect Delegate Signals
//==============================================================================
void ListBox::disconnectDelegateSignals(ItemDelegate* aItem)
{
    // Check Item
    if (aItem) {
        // Disconnect Signals
        disconnect(aItem, SIGNAL(itemLeftClicked(int)), this, SLOT(delegateLeftClicked(int)));
        disconnect(aItem, SIGNAL(itemRightClicked(int)), this, SLOT(delegateRightClicked(int)));
        disconnect(aItem, SIGNAL(itemDoubleClicked(int)), this, SLOT(delegateDoubleClicked(int)));
        disconnect(aItem, SIGNAL(itemDragStarted(int)), this, SLOT(delegateDragStarted(int)));
        disconnect(aItem, SIGNAL(itemDragged(int,QSize)), this, SLOT(delegateDragged(int,QSize)));
        disconnect(aItem, SIGNAL(itemDragFinished(int)), this, SLOT(delegateDragFinished(int)));
        disconnect(aItem, SIGNAL(itemKeyPressed(int,int,Qt::KeyboardModifiers)), this, SLOT(delegateKeyPressed(int,int,Qt::KeyboardModifiers)));
        disconnect(aItem, SIGNAL(sizeChanged(int,QSize)), this, SLOT(delegateSizeChanged(int,QSize)));
    }
}

//==============================================================================
// Clear Delegate Cache
//==============================================================================
void ListBox::clearCache()
{
    // Get Cached Items Count
    int ciCount = cache.count();

    qDebug() << "ListBox::clearCache - ciCount: " << ciCount;

    // Go Thru Cache
    for (int i=ciCount-1; i>=0; i--) {
        // Get Cache Item
        ItemDelegate* item = cache[i];
        // Check Item
        if (item) {
            // Delete Item
            delete item;
            item = NULL;
        }
    }

    // Clear Cache
    cache.clear();
}

//==============================================================================
// Show Debug Info
//==============================================================================
void ListBox::showDebugInfo()
{
    // Get List Box Visible Area Size
    int lbSize = (orientation == LBOVertical) ? height() : width();

    qDebug() << "ListBox::updateIndexes - mip: " << minScrollPos;
    qDebug() << "ListBox::updateIndexes - map: " << maxScrollPos;
    qDebug() << "ListBox::updateIndexes - scp: " << scrollPos;
    qDebug() << "ListBox::updateIndexes - lbs: " << lbSize;
    qDebug() << "ListBox::updateIndexes - dis: " << delegateSize;
    qDebug() << "ListBox::updateIndexes - spc: " << spacing;
    qDebug() << "ListBox::updateIndexes - vic: " << visibleItemCount;
    qDebug() << "ListBox::updateIndexes - vcm: " << visibleItemCountMax;
    qDebug() << "ListBox::updateIndexes - ccm: " << cacheCountMax;
    qDebug() << "ListBox::updateIndexes - fvi: " << firstVisibleItem;
    qDebug() << "ListBox::updateIndexes - lvi: " << lastVisibleItem;
    qDebug() << "ListBox::updateIndexes - fci: " << firstCachedItem;
    qDebug() << "ListBox::updateIndexes - lci: " << lastCachedItem;

}

//==============================================================================
// Set Current Index
//==============================================================================
void ListBox::setCurrentIndex(const int& aIndex)
{
    // Check Current Index
    if (currentIndex != aIndex && model) {
        // Get Model Item Data
        ListModelItemData* itemData = NULL;
        // Check Current Index
        if (currentIndex >= 0) {
            // Get Model Item Data
            itemData = model->getItem(currentIndex);
            // Check Mdoel Item Data
            if (itemData) {
                // Set Current Flag
                itemData->setCurrent(false);
            }
        }

        // Set Current Index
        currentIndex = aIndex;

        // Get Model Item Data
        itemData = model->getItem(currentIndex);
        // Check Mdoel Item Data
        if (itemData) {
            // Set Current Flag
            itemData->setCurrent(true);
        }
    }
}

//==============================================================================
// Set Ease Pos
//==============================================================================
void ListBox::setEasePos(const int& aPos)
{
    // Set Scroll Pos
    scrollPos = aPos;
    // Reset Prev Scroll Pos
    prevScrollPos = scrollPos;
    // Update Cache
    updateCache();
}

//==============================================================================
// Get Ease Pos
//==============================================================================
int ListBox::easePos() const
{
    return scrollPos;
}

//==============================================================================
// Scroll To Current Index
//==============================================================================
void ListBox::scrollToCurrentIndex(const bool& aAnimated)
{
    // Check Current Index
    if (currentIndex >= 0 && currentIndex < count()) {
        //qDebug() << "ListBox::scrollToCurrentIndex - FVI: " << firstVisibleItem;
        //qDebug() << "ListBox::scrollToCurrentIndex - LVI: " << lastVisibleItem;

        // Check Current Index
        if (currentIndex > firstVisibleItem && currentIndex < lastVisibleItem)
            return;

        // Init New Scroll Pos
        int newScrollPos = scrollPos;

        // Get List Box Size
        int lbSize = (orientation == LBOVertical) ? height() : width();

        // Check Current Index
        if (currentIndex <= firstVisibleItem) {
            // Calculate New Scroll Pos
            newScrollPos = qBound(minScrollPos, -(currentIndex * fullItemSize), maxScrollPos);
        // Check Current Index
        } else if (currentIndex >= lastVisibleItem) {
            // Calculate New Scroll Pos
            newScrollPos = qBound(minScrollPos, -((currentIndex + 1) * fullItemSize + spacing) + lbSize, maxScrollPos);
        }

        // Check Animated Flag
        if (aAnimated) {
            // Set Scroll Pos
            startGotoAnim(newScrollPos);
        } else {
            // Set Scroll Pos
            scrollPos = newScrollPos;
            // Set Prev Scroll Pos
            prevScrollPos = newScrollPos;
            // set Original Scroll Pos
            orgScrollPos = newScrollPos;
            // Update Cache
            updateCache();
            // Rebuild Cache
            //rebuildCache();
        }
    }
}

//==============================================================================
// Adjust Scroll Pos
//==============================================================================
void ListBox::adjustScrollPos(const bool& aAnimated)
{
    // Check Prev Scroll Pos
    if (prevScrollPos != scrollPos) {
        //qDebug() << "ListBox::adjustScrollPos - ease";
        // Calculate New Scroll Pos
        int newScrollPos = scrollPos + (scrollPos - prevScrollPos) * -easeSpeed;
        // Check Animated Flag
        if (aAnimated) {
            // Start Ease Anim
            startEaseAnim(newScrollPos);
        } else {
            // Set Scroll Pos
            scrollPos = newScrollPos;
            // Update Cache
            updateCache();
        }
    // Check Scroll Pos
    } else if (scrollPos < minScrollPos) {
        //qDebug() << "ListBox::adjustScrollPos - repos";
        // Check Animated Flag
        if (aAnimated) {
            // Start Repos Anim
            startReposAnim(minScrollPos);
        } else {
            // Set Scroll Pos
            scrollPos = minScrollPos;
            // Update Cache
            updateCache();
        }
    } else if (scrollPos > maxScrollPos) {
        //qDebug() << "ListBox::adjustScrollPos - repos";
        // Check Animated Flag
        if (aAnimated) {
            // Start Repos Anim
            startReposAnim(maxScrollPos);
        } else {
            // Set Scroll Pos
            scrollPos = maxScrollPos;
            // Update Cache
            updateCache();
        }
    } else if (snapToItem && !easing) {
        //qDebug() << "ListBox::adjustScrollPos - snap";
        // Check Current Index
        if (currentIndex >= 0) {
            // Init New Scroll Pos
            int newScrollPos = 0;
            // Check Current Index
            if (currentIndex == lastVisibleItem) {
                // Get List Box Size
                int lbSize = (orientation == LBOVertical) ? height() : width();
                // Adjust Scroll Pos
                newScrollPos = qBound(minScrollPos, -((currentIndex + 1) * fullItemSize + spacing) + lbSize, maxScrollPos);
            } else {
                // Calculate Scroll Pos
                newScrollPos = qBound(minScrollPos, -qRound((qreal)(-scrollPos - spacing) / fullItemSize) * fullItemSize, maxScrollPos);
            }
            // Check Animated Flag
            if (aAnimated) {
                // Start Snap To Item Pos Anim
                startSnapToAnim(newScrollPos);
            } else {
                // Set Scroll Pos
                scrollPos = newScrollPos;
                // Update Cache
                updateCache();
            }
        }
    }
}

//==============================================================================
// Setup Ease Anim
//==============================================================================
void ListBox::setupEaseAnim()
{
    // Check Ease Anim
    if (!easeAnim) {
        // Create Ease Anim
        easeAnim = new QPropertyAnimation(this, DEFAULT_LISTBOX_EASPOS_PROPERTY_NAME);
        // Set Easing Curve
        easeAnim->setEasingCurve(QEasingCurve::OutQuad);
        // Set Duration
        easeAnim->setDuration(DEFAULT_LISTBOX_EASEANIM_DURATION);
        // Connect Finished Signal
        connect(easeAnim, SIGNAL(finished()), this, SLOT(easeAnimFinished()));
    }
}

//==============================================================================
// Start Ease Anim
//==============================================================================
void ListBox::startEaseAnim(const int& aEndValue)
{
    // Check Anim
    if (easeAnim && scrollPos != aEndValue) {
        // Stop Anim First
        easeAnim->stop();
        //qDebug() << "ListBox::startEaseAnim - aEndValue: " << aEndValue;
        // Set End Value
        easeAnim->setEndValue(aEndValue);
        // Start Anim
        easeAnim->start();
        // Set Easing
        easing = true;
    }
}

//==============================================================================
// Delete Ease Anim
//==============================================================================
void ListBox::deleteEaseAnim()
{
    // Check Anim
    if (easeAnim) {
        // Stop Anim First
        easeAnim->stop();
        //qDebug() << "ListBox::deleteEaseAnim";
        // Delete Anim
        delete easeAnim;
        easeAnim = NULL;
    }
}

//==============================================================================
// Setup Repos Anim
//==============================================================================
void ListBox::setupReposAnim()
{
    // Check Repos Anim
    if (!reposAnim) {
        // Create Repos Anim
        reposAnim = new QPropertyAnimation(this, DEFAULT_LISTBOX_EASPOS_PROPERTY_NAME);
        // Set Easing Curve
        reposAnim->setEasingCurve(QEasingCurve::OutBack);
        // Set Duration
        reposAnim->setDuration(DEFAULT_LISTBOX_REPOSANIM_DURATION);
        // Connect Finished Signal
        connect(reposAnim, SIGNAL(finished()), this, SLOT(reposAnimFinished()));
    }
}

//==============================================================================
// Start Repos Anim
//==============================================================================
void ListBox::startReposAnim(const int& aEndValue)
{
    // Check Anim
    if (reposAnim && scrollPos != aEndValue) {
        // Stop Anim
        reposAnim->stop();
        //qDebug() << "ListBox::startReposAnim - aEndValue: " << aEndValue;
        // Set End Value
        reposAnim->setEndValue(aEndValue);
        // Start Anim
        reposAnim->start();
    }
}

//==============================================================================
// Delete Repos Anim
//==============================================================================
void ListBox::deleteReposAnim()
{
    // Check Anim
    if (reposAnim) {
        // Stop Anim First
        reposAnim->stop();
        //qDebug() << "ListBox::deleteReposAnim";
        // Delete Anim
        delete reposAnim;
        reposAnim = NULL;
    }
}

//==============================================================================
// Setup SnapTo Anim
//==============================================================================
void ListBox::setupSnapToAnim()
{
    // Check SnapTo Anim
    if (!snapToAnim) {
        // Create Snap To Anim
        snapToAnim = new QPropertyAnimation(this, DEFAULT_LISTBOX_EASPOS_PROPERTY_NAME);
        // Set Easing Curve
        snapToAnim->setEasingCurve(QEasingCurve::OutBack);
        // Set Duration
        snapToAnim->setDuration(DEFAULT_LISTBOX_SNAPTOANIM_DURATION);
        // Connect Finished Signal
        connect(snapToAnim, SIGNAL(finished()), this, SLOT(snaptoAnimFinished()));
    }
}

//==============================================================================
// Start Snap to Anim
//==============================================================================
void ListBox::startSnapToAnim(const int& aEndValue)
{
    // Check Anim
    if (snapToAnim && scrollPos != aEndValue) {
        // Stop Anim
        snapToAnim->stop();
        //qDebug() << "ListBox::startSnapToAnim - aEndValue: " << aEndValue;
        // Set End Value
        snapToAnim->setEndValue(aEndValue);
        // Start Anim
        snapToAnim->start();
    }
}

//==============================================================================
// Delete Snap To Anim
//==============================================================================
void ListBox::deleteSnapToAnim()
{
    // Check Anim
    if (snapToAnim) {
        // Stop Anim First
        snapToAnim->stop();
        //qDebug() << "ListBox::deleteSnapToAnim";
        // Delete Anim
        delete snapToAnim;
        snapToAnim = NULL;
    }
}

//==============================================================================
// Setup Goto Anim
//==============================================================================
void ListBox::setupGotoAnim()
{
    // Check SnapTo Anim
    if (!gotoAnim) {
        // Create Snap To Anim
        gotoAnim = new QPropertyAnimation(this, DEFAULT_LISTBOX_EASPOS_PROPERTY_NAME);
        // Set Easing Curve
        gotoAnim->setEasingCurve(QEasingCurve::Linear);
        // Set Duration
        gotoAnim->setDuration(DEFAULT_LISTBOX_GOTOANIM_DURATION);
        // Connect Finished Signal
        connect(gotoAnim, SIGNAL(finished()), this, SLOT(gotoAnimFinished()));
    }
}

//==============================================================================
// Start Goto Anim
//==============================================================================
void ListBox::startGotoAnim(const int& aEndValue)
{
    // Check Anim
    if (gotoAnim && scrollPos != aEndValue) {
        // Stop Anim
        gotoAnim->stop();
        //qDebug() << "ListBox::startGotoAnim - aEndValue: " << aEndValue;
        // Set End Value
        gotoAnim->setEndValue(aEndValue);
        // Start Anim
        gotoAnim->start();
    }
}

//==============================================================================
// Delete Goto Anim
//==============================================================================
void ListBox::deleteGotoAnim()
{
    // Check Anim
    if (gotoAnim) {
        // Stop Anim First
        gotoAnim->stop();
        //qDebug() << "ListBox::deleteGotoAnim";
        // Delete Anim
        delete gotoAnim;
        gotoAnim = NULL;
    }
}

//==============================================================================
// Get Model Item Data Safe
//==============================================================================
ListModelItemData* ListBox::getData(const int& aIndex)
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        // Get Model Data
        return model->data[aIndex];
    }

    return NULL;
}

//==============================================================================
// Select All Items
//==============================================================================
void ListBox::selectAll()
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Go Thru Items
    for (int i=0; i<dCount; i++) {
        // Toggle Selected
        model->data[i]->toggleSelected();
    }
}

//==============================================================================
// Trigger Item Update
//==============================================================================
void ListBox::triggerItemUpdate(const int& aIndex)
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        //qDebug() << "ListBox::triggerItemUpdate - aIndex: " << aIndex;
        // Get Model Data
        model->data[aIndex]->triggerUpdate();
    }
}

//==============================================================================
// Start Drag Filter Timer
//==============================================================================
void ListBox::startDragFilterTimer()
{
    // Stop Drag Filter Timer
    stopDragFilterTimer();
    // Check Timer ID
    if (dragFilterID == -1) {
        // Start Timer
        dragFilterID = startTimer(DEFAULT_LISTBOX_DRAGFILTER_TIMEOUT);
    }
}

//==============================================================================
// Stop Drag Filter Timer
//==============================================================================
void ListBox::stopDragFilterTimer()
{
    // Check Timer ID
    if (dragFilterID != -1) {
        // Kill Timer
        killTimer(dragFilterID);
        // Reset Timer ID
        dragFilterID = -1;
    }
}

//==============================================================================
// Start Wheel Repositioning Timer
//==============================================================================
void ListBox::startWheelReposTimer()
{
    // Stop Timer First
    stopWheelReposTimer();
    // Check Timer ID
    if (wheelReposID == -1) {
        //qDebug() << "ListBox::startWheelReposTimer";
        // Start Timer
        wheelReposID = startTimer(DEFAULT_LISTBOX_WHEEL_REPOS_TIMEOUT);
    }
}

//==============================================================================
// Stop Wheel Repositioning Timer
//==============================================================================
void ListBox::stopWheelReposTimer()
{
    // Check Timer ID
    if (wheelReposID != -1) {
        //qDebug() << "ListBox::stopWheelReposTimer";
        // Kill Timer
        killTimer(wheelReposID);
        // Reset Timer ID
        wheelReposID = -1;
    }
}

//==============================================================================
// Handle Key Event
//==============================================================================
void ListBox::handleKeyEvent(const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    // Get Shift Pressed
    bool shiftPressed = aModifiers & Qt::ShiftModifier;

    // Switch Key
    switch (aKey) {
        // Previous Item
        case Qt::Key_Up: {          gotoPrevItem(shiftPressed);         } break;
        // Next Item
        case Qt::Key_Down: {        gotoNextItem(shiftPressed);         } break;
        // Page Up
        case Qt::Key_PageUp: {      pageUp(shiftPressed);               } break;
        // Page Down
        case Qt::Key_PageDown: {    pageDown(shiftPressed);             } break;
        // Goto Home
        case Qt::Key_Home: {        gotoHome(shiftPressed);             } break;
        // Goto End
        case Qt::Key_End: {         gotoEnd(shiftPressed);              } break;
        // Insett Key
        case Qt::Key_Insert: {      gotoNextItem(shiftPressed);         } break;
        // Enter Pressed
        case Qt::Key_Enter:
        case Qt::Key_Return: {      emit itemSelected(currentIndex);    } break;

        default:
            //qDebug() << "ListBox::handleKeyEvent - aKey: " << QString("0x%1").arg(aKey, 8, 16, QChar('0'));

            // ...
        break;
    }
}

//==============================================================================
// Model Item Added Slot
//==============================================================================
void ListBox::modelItemAdded(const int& aIndex)
{
    // Check Index
    //if ((aIndex >= firstCachedItem && aIndex <= lastCachedItem) || (firstCachedItem == -1) || (visibleItemCount << visibleItemCountMax)) {
        //qDebug() << "ListBox::modelItemAdded - aIndex: " << aIndex;
        // Set Added Index
        addedIndex = aIndex;
        // Update Cache
        updateCache();
        // Adjust Scroll Pos
        adjustScrollPos();
    //}
}

//==============================================================================
// Model Item Removed Slot
//==============================================================================
void ListBox::modelItemRemoved(const int& aIndex)
{
    // Check Index
    //if (aIndex >= firstCachedItem && aIndex <= lastCachedItem) {
        //qDebug() << "ListBox::modelItemRemoved - aIndex: " << aIndex;
        // Set Removed Index
        removedIndex = aIndex;
        // Update Cache
        updateCache();
        // Adjust Scroll Pos
        adjustScrollPos();
    //}
}

//==============================================================================
// Model Item Updated Slot
//==============================================================================
void ListBox::modelItemUpdated(const int& aIndex)
{
    // Check Index
    if (aIndex >= firstCachedItem && aIndex <= lastCachedItem) {
        //qDebug() << "ListBox::modelItemUpdated - aIndex: " << aIndex;
        // Set Updated Index
        updatedIndex = aIndex;
        // Update Cache
        updateCache();
    }
}

//==============================================================================
// Delegate Left Clicked Slot
//==============================================================================
void ListBox::delegateLeftClicked(const int& aIndex)
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Check Count
    if (dCount <= 0)
        return;
    //qDebug() << "ListBox::delegateLeftClicked - aIndex: " << aIndex;
    // Set Current Index
    setCurrentIndex(aIndex);
    // Check If has Focus
    if (!hasFocus())
        // Set Focus
        setFocus();
}

//==============================================================================
// Delegate Right Clicked Slot
//==============================================================================
void ListBox::delegateRightClicked(const int& aIndex)
{
    //qDebug() << "ListBox::delegateRightClicked - aIndex: " << aIndex;
    // Set Current Index
    setCurrentIndex(aIndex);
    // Check If has Focus
    if (!hasFocus())
        // Set Focus
        setFocus();
    // Emit Item Options Signal For Launch Popup Menu
    emit itemOptions(aIndex);
}

//==============================================================================
// Delegate Double Clicked Slot
//==============================================================================
void ListBox::delegateDoubleClicked(const int& aIndex)
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Check Count
    if (dCount <= 0)
        return;
    //qDebug() << "ListBox::delegateDoubleClicked - aIndex: " << aIndex;
    // Emit Item Selected Signal
    emit itemSelected(aIndex);
}

//==============================================================================
// Delegate Drag Started Slot
//==============================================================================
void ListBox::delegateDragStarted(const int& aIndex)
{
    Q_UNUSED(aIndex);
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Check Count
    if (dCount <= 0)
        return;
    //qDebug() << "ListBox::delegateDragStarted - aIndex: " << aIndex;
    // Check Ease Anim
    if (easeAnim) {
        // Stop Ease Anim
        easeAnim->stop();
    }
    // Check Repos Anim
    if (reposAnim) {
        // Stop Repos Anim
        reposAnim->stop();
    }
    // Check Snap To Anim
    if (snapToAnim) {
        // Stop Snam To Anim
        snapToAnim->stop();
    }
    // Save Prev Scroll Pos
    orgScrollPos = scrollPos;
    // Reset Items Moved
    itemsMoved = false;
    // Check If has Focus
    if (!hasFocus())
        // Set Focus
        setFocus();
}

//==============================================================================
// Delegate Dragged Slot
//==============================================================================
void ListBox::delegateDragged(const int& aIndex, const QSize& aDelta)
{
    //qDebug() << "ListBox::delegateDragged - aIndex: " << aIndex << " - delta: " << aDelta;
    Q_UNUSED(aIndex);
    // Start Drag Filter Timer
    startDragFilterTimer();
    // Set Previous Scroll Pos
    prevScrollPos = scrollPos;
    // Set Scroll Pos
    scrollPos = orgScrollPos + ((orientation == LBOVertical) ? aDelta.height() : aDelta.width());
    //qDebug() << "ListBox::delegateDragged - aIndex: " << aIndex << " - scrollPos: " << scrollPos;
    // Set Items Moved
    itemsMoved = true;
    // Update Cache
    updateCache();
}

//==============================================================================
// Delegate Drag Finshed Slot
//==============================================================================
void ListBox::delegateDragFinished(const int& aIndex)
{
    //qDebug() << "ListBox::delegateDragFinished - aIndex: " << aIndex;
    // Check Items Moved
    if (!itemsMoved) {
        // Set Current Index
        setCurrentIndex(aIndex);
    }
    // Save Prev Scroll Pos
    orgScrollPos = scrollPos;
    // Stop Drag Filter Timer
    stopDragFilterTimer();
    // Adjust Scroll Pos
    adjustScrollPos();
    // Reset Items Moved
    itemsMoved = false;
}

//==============================================================================
// Delegate Size Changed Slot
//==============================================================================
void ListBox::delegateSizeChanged(const int& aIndex, const QSize& aSize)
{
    qDebug() << "ListBox::delegateSizeChanged - aIndex: " << aIndex << " - size: " << aSize;
    // Check Index
    if (aIndex <= lastCachedItem) {
        // Update Cache
        updateCache();
    }
}

//==============================================================================
// Delegate Key Pressed Slot
//==============================================================================
void ListBox::delegateKeyPressed(const int& aIndex, const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    //qDebug() << "ListBox::delegateDragFinished - aIndex: " << aIndex << " - key: " << aKey;
/*
    // Switch Key
    switch (aKey) {
        case Qt::Key_Down: {
            // Next Item
            gotoNextItem();
        } break;

        case Qt::Key_Up : {
            // Go To Previous Item
            gotoPrevItem();
        } break;

        default: {
            qDebug() << "ListBox::delegateKeyPressed - Unhandled Key";
        } break;
    }
*/
    Q_UNUSED(aIndex);
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiers);
}

//==============================================================================
// Ease Anim Finished Slot
//==============================================================================
void ListBox::easeAnimFinished()
{
    //qDebug() << "ListBox::easeAnimFinished";
    // Reset Previus Scroll Pos
    prevScrollPos = scrollPos;
    // Reset Easing
    easing = false;
    // Adjust Scroll Pos
    adjustScrollPos();
}

//==============================================================================
// Repos Anim Finished Slot
//==============================================================================
void ListBox::reposAnimFinished()
{
    //qDebug() << "ListBox::reposAnimFinished";

    // ...
}

//==============================================================================
// SnapTo Anim Finished Slot
//==============================================================================
void ListBox::snaptoAnimFinished()
{
    //qDebug() << "ListBox::snaptoAnimFinished";

    // ...
}

//==============================================================================
// Goto Anim Finished Slot
//==============================================================================
void ListBox::gotoAnimFinished()
{
    //qDebug() << "ListBox::gotoAnimFinished";

    // ...
}

//==============================================================================
// Resize Event
//==============================================================================
void ListBox::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        //qDebug() << "ListBox::resizeEvent - size: " << aEvent->size();
        // Update Cache
        updateCache();
        // Adjust Scroll Pos
        adjustScrollPos();
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void ListBox::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Timer ID
        if (aEvent->timerId() == dragFilterID) {
            //qDebug() << "ListBox::timerEvent - dragFilterID";
            // Stop Drag Filter Timer
            stopDragFilterTimer();
            // Reset Previous Pos
            prevScrollPos = scrollPos;
        } else if (aEvent->timerId() == wheelReposID) {
            qDebug() << "ListBox::timerEvent - wheelReposID";
            // Stop Timer
            stopWheelReposTimer();
            // Reset Previous Pos
            prevScrollPos = scrollPos;
            // Reset Scroll Pos
            adjustScrollPos();
        }
    }
}

//==============================================================================
// Key Press Event
//==============================================================================
void ListBox::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Key Pressed
        if (!keyPressed) {
            // Set Key Pressed
            keyPressed = true;

            // ...

            // Set Last Key
            lastKey = aEvent->key();
            // Reset Key Repeat
            keyRepeat = false;
        } else {
            // Check Last Key
            if (aEvent->key() == lastKey) {
                // Set Key Repeat
                keyRepeat = true;
                // Handle Key Event
                handleKeyEvent(aEvent->key(), aEvent->modifiers());
            }

            // Set Last Key
            lastKey = aEvent->key();
        }

        // Emit List Box Key Pressed Signal
        emit listBoxKeyPressed(aEvent->key(), aEvent->modifiers());
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void ListBox::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Key Pressed
        if (keyPressed) {
            // Set Key Pressed
            keyPressed = false;

            // Check Key Repeat
            if (!keyRepeat) {
                // Handle Keys Event
                handleKeyEvent(aEvent->key(), aEvent->modifiers());
            }
        }

        // Emit List Box Key Released Signal
        emit listBoxKeyReleased(aEvent->key(), aEvent->modifiers());
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void ListBox::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent) {
        // Check Pressed State
        if (!pressed) {
            // Set Pressed State
            pressed = true;

            // Check If has Focus
            if (!hasFocus())
                // Set Focus
                setFocus();

            // ...
        }
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void ListBox::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent) {
        // Check Pressed
        if (pressed) {
            //qDebug() << "ListBox::mouseReleaseEvent";
            // Reset Pressed State
            pressed = false;

            // ...

        }
    }
}

//==============================================================================
// Wheel Event
//==============================================================================
void ListBox::wheelEvent(QWheelEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Set Previous Scroll Pos
        prevScrollPos = scrollPos;
        //qDebug() << "ListBox::wheelEvent - delta: " << aEvent->delta();
        // Set Scroll Pos
        scrollPos = scrollPos + aEvent->delta() * DEFAULT_LISTBOX_WHEEL_SPEED;
        // Set Items Moved
        itemsMoved = true;

        // Update Cache
        updateCache();
        // Start Wheel Repos Timer
        startWheelReposTimer();

        // Adjust Scroll Pos
        //adjustScrollPos();

        // Accept Event
        aEvent->accept();
    }
}

//==============================================================================
// Focus In Event
//==============================================================================
void ListBox::focusInEvent(QFocusEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        qDebug() << "ListBox::focusInEvent";

        // ...
    }
}

//==============================================================================
// Destructor
//==============================================================================
ListBox::~ListBox()
{
    // Delete Ease Anim
    deleteEaseAnim();
    // Delete Repos Anim
    deleteReposAnim();
    // Delete SnapTo Anim
    deleteSnapToAnim();
    // Delete GoTo Anim
    deleteGotoAnim();
    // Clear
    clear();
    // Destroy Model
    destroyModel();
    // Clear Delegate
    setDelegate(NULL);
}

