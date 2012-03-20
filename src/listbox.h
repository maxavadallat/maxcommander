#ifndef LISTBOX_H
#define LISTBOX_H

// INCLUDES

#include <QFrame>
#include <QSize>
#include <QList>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QWheelEvent>
#include <QFocusEvent>
#include <QPropertyAnimation>

#include "listmodel.h"


// DECLARATIONS

//==============================================================================
//! @enum ListBoxOrientation List Box Orientation Enum
//==============================================================================
enum ListBoxOrientation
{
    LBOVertical = 0,
    LBOHorizontal
};



//==============================================================================
//! @class ItemDelegate List Box Item Delegate Class
//==============================================================================
class ItemDelegate : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aData Item Data
    //! @param aParent Parent Widget
    ItemDelegate(ListModelItemData* aData, QWidget* aParent = NULL);

    //! @brief Create Delegate Duplicate
    //! @param aData Item Data
    //! @return New Item Delegate
    virtual ItemDelegate* duplicate(ListModelItemData* aData = NULL);

    //! @brief Set Data
    //! @param aData Item Data
    //! @param aUpdate Update
    virtual void setData(ListModelItemData* aData, const bool& aUpdate = false);

    //! @brief Get Data
    //! @param none
    //! @return Item Data
    virtual ListModelItemData* getData();

    //! @brief Set Item Size
    //! @param aSize Item Size
    virtual void setItemSize(const QSize& aSize);

    //! @brief Get Item Size
    //! @param none
    //! @return Item Size
    virtual QSize getItemSize();

    //! @brief Set Orientation
    //! @param aOrientation Parent List Box Orientation
    virtual void setOrientation(const ListBoxOrientation& aOrientation);

    //! @brief Destructor
    //! @param none
    virtual ~ItemDelegate();

signals:

    //! @brief Size Changed Signal
    //! @param aIndex Delegate Index
    //! @param aSize Delegate Size
    void sizeChanged(const int& aIndex, const QSize& aSize);

    //! @brief Item Left Clicked Signal
    //! @param aIndex Delegate Index
    void itemLeftClicked(const int& aIndex);

    //! @brief Item Right Clicked Signal
    //! @param aIndex Delegate Index
    void itemRightClicked(const int& aIndex);

    //! @brief Item Double Clicked Signal
    //! @param aIndex Delegate Index
    void itemDoubleClicked(const int& aIndex);

    //! @brief Item Drag Started Signal
    //! @param aIndex Delegate Index
    void itemDragStarted(const int& aIndex);

    //! @brief Item Dragged Signal
    //! @param aIndex Delegate Index
    //! @param aDelta Dragged Delta
    void itemDragged(const int& aIndex, const QSize& aDelta);

    //! @brief Item Drag Finished Signal
    //! @param aIndex Delegate Index
    void itemDragFinished(const int& aIndex);

    //! @brief Item Key Pressed Signal
    //! @param aIndex Delegate Index
    //! @param aKey Key Code
    //! @param aModifiers Modifiers (Shift, Control, Alt)
    void itemKeyPressed(const int& aIndex, const int& aKey, const Qt::KeyboardModifiers& aModifiers);

protected:

    //! @brief Update UI
    //! @param none
    virtual void updateUI();

protected: // From QFrame

    //! @brief Paint Event
    //! @param aEvent Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

    //! @brief Mouse Press Event
    //! @param aEvent Mouse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);

    //! @brief Mouse Move Event
    //! @param aEvent Mouse Event
    virtual void mouseMoveEvent(QMouseEvent* aEvent);

    //! @brief Mouse Release Event
    //! @param aEvent Mouse Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    //! @brief Mouse Double Click Event
    //! @param aEvent Mouse Event
    virtual void mouseDoubleClickEvent(QMouseEvent* aEvent);

    //! @brief Key Press Event
    //! @param aEvent Key Event
    virtual void keyPressEvent(QKeyEvent* aEvent);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

    //! @brief Timer Event
    //! @param aEvent Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

protected slots:

    //! @brief Data Updated Slot
    //! @param none
    void dataUpdated();

protected: // Data
    friend class ListBox;
    friend class FileListBox;

    //! Parent List Box
    ListBox*            parentList;
    //! Orientation
    ListBoxOrientation  orientation;
    //! Item Data
    ListModelItemData*  data;
    //! Pressed
    bool                pressed;
    //! Moved
    bool                moved;
    //! Original Press Pos
    QPoint              originalPressPos;
    //! Previous Pos
    QPoint              prevPos;
    //! Key Pressed
    bool                keyPressed;
};









//==============================================================================
//! @typedef CacheType List Box Cache List Type Def
//==============================================================================
typedef QList<ItemDelegate*> CacheType;


//==============================================================================
//! @class ListBox List Box Class
//==============================================================================
class ListBox : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(int easePos READ easePos WRITE setEasePos)

public:

    //! @brief Constructor
    //! @param aOrientation Orientation
    //! @param aParent Parent Widget
    ListBox(ListBoxOrientation aOrientation = LBOVertical, QWidget* aParent = NULL);

    //! @brief Append Item
    //! @param aItem New Item
    //! @param aUpdate Update
    void appendItem(ListModelItemData* aItem, const bool& aUpdate = true);

    //! @brief Insert Item
    //! @param aIndex New Item Index
    //! @param aItem New Item
    //! @param aUpdate Update
    void insertItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate = true);

    //! @brief Replace Item
    //! @param aIndex New Item Index
    //! @param aItem New Item
    //! @param aUpdate Update
    void replaceItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate = true);

    //! @brief Remove Item
    //! @param aIndex New Item Index
    //! @param aUpdate Update
    void removeItem(const int& aIndex, const bool& aUpdate = true);

    //! @brief Get Item Count
    //! @param none
    //! @return Item Count
    int count();

    //! @brief Get Selected Item Count
    //! @param none
    //! @return Selected Item Count
    int selectedCount();

    //! @brief Get Current Index
    //! @param none
    //! @return Current Index
    int getCurrentIndex();

    //! @brief Clear
    //! @param none
    void clear();

    //! @brief Update List - Full List Update
    //! @param none
    void updateList();

    //! @brief Set Model
    //! @param aModel New Model
    void setModel(ListModel* aModel);

    //! @brief Set Delegate
    //! @param aDelegate New Delegate
    void setDelegate(ItemDelegate* aDelegate);

    //! @brief Get Delegate
    //! @param none
    //! @return Delegate
    ItemDelegate* getDelegate();

    //! @brief Set Item/Delegate Spacing
    //! @param aSpacing Item/Delegate Spacing
    void setSpacing(const int& aSpacing);

    //! @brief Get Item/Delegate Spacing
    //! @param none
    //! @return Item/Delegate Spacing
    int getSpacing();

    //! @brief Set Item/Delegate Cahce Overhead
    //! @param aOverHead Cache Overhead
    void setCacheOverhead(const int& aOverHead);

    //! @brief Set Snap To Item Mode
    //! @param aSnap Snap To Item Mode
    void setSnapToItemMode(const bool& aSnap);

    //! @brief Go To Index
    //! @param aIndex Item Index
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void gotoIndex(const int& aIndex, const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Go To Next Item
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void gotoNextItem(const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Go To Prev Item
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void gotoPrevItem(const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Go To Home
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void gotoHome(const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Go To End
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void gotoEnd(const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Go One Page Up
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void pageUp(const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Go One Page Down
    //! @param aSelection Selection
    //! @param aAnimated Animated
    void pageDown(const bool& aSelection = false, const bool& aAnimated = true);

    //! @brief Set Item Selected
    //! @param aIndex Item Index
    //! @param aSelected Selected State
    void setItemSelected(const int& aIndex, const int& aSelected);

    //! @brief Get Item Selected
    //! @param aIndex Item Index
    //! @return Item Selected State
    bool getItemSelected(const int& aIndex);

    //! @brief Toggle Item Selected
    //! @param aIndex Item Index
    void toggleItemSelected(const int& aIndex);

    //! @brief Get Model Item Data Safe
    //! @param aIndex Item Index
    //! @return Model Item Data
    ListModelItemData* getData(const int& aIndex);

    //! @brief Select All Items
    //! @param none
    virtual void selectAll();

    //! @brief Adjust Scroll Pos
    //! @param aAnimated Animated
    void adjustScrollPos(const bool& aAnimated = true);

    //! @brief Destructor
    //! @param none
    virtual ~ListBox();

signals:

    //! @brief List Box Key Pressed Signal
    //! @param aKey Key
    //! @param aModifiers Key Modifiers
    void listBoxKeyPressed(const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief List Box Key Released Signal
    //! @param aKey Key
    //! @param aModifiers Key Modifiers
    void listBoxKeyReleased(const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief List Box Item Selected Signal
    //! @param aIndex Item Index
    void itemSelected(const int& aIndex);

    //! @brief List Box Item Options Signal
    //! @param aIndex Item Index
    void itemOptions(const int& aIndex);

protected slots:

    //! @brief Model Item Added Slot
    //! @param aIndex Item Index
    void modelItemAdded(const int& aIndex);

    //! @brief Model Item Removed Slot
    //! @param aIndex Item Index
    void modelItemRemoved(const int& aIndex);

    //! @brief Model Item Updated Slot
    //! @param aIndex Item Index
    void modelItemUpdated(const int& aIndex);

    //! @brief Delegate Left Clicked Slot
    //! @param aIndex Item Index
    void delegateLeftClicked(const int& aIndex);

    //! @brief Delegate Right Clicked Slot
    //! @param aIndex Item Index
    void delegateRightClicked(const int& aIndex);

    //! @brief Delegate Double Clicked Slot
    //! @param aIndex Item Index
    void delegateDoubleClicked(const int& aIndex);

    //! @brief Delegate Drag Started Slot
    //! @param aIndex Item Index
    void delegateDragStarted(const int& aIndex);

    //! @brief Delegate Dragged Slot
    //! @param aIndex Item Index
    void delegateDragged(const int& aIndex, const QSize& aDelta);

    //! @brief Delegate Drag Finshed Slot
    //! @param aIndex Item Index
    void delegateDragFinished(const int& aIndex);

    //! @brief Delegate Size Changed Slot
    //! @param aIndex Item Index
    //! @param aSize New Delegate Size
    void delegateSizeChanged(const int& aIndex, const QSize& aSize);

    //! @brief Delegate Key Pressed Slot
    //! @param aIndex Delegate Index
    //! @param aKey Key Code
    //! @param aModifiers Modifiers (Shift, Control, Alt)
    void delegateKeyPressed(const int& aIndex, const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief Ease Anim Finished Slot
    //! @param none
    void easeAnimFinished();

    //! @brief Repos Anim Finished Slot
    //! @param none
    void reposAnimFinished();

    //! @brief SnapTo Anim Finished Slot
    //! @param none
    void snaptoAnimFinished();

    //! @brief Goto Anim Finished Slot
    //! @param none
    void gotoAnimFinished();

protected:

    //! @brief Create Model
    //! @param none
    void createModel();

    //! @brief Destroy Model
    //! @param none
    void destroyModel();

    //! @brief Reset Indexes
    //! @param none
    void resetIndexes();

    //! @brief Update Indexes
    //! @param none
    void updateIndexes();

    //! @brief Rebuild Delegate Cache
    //! @param none
    void rebuildCache();

    //! @brief Update Delegate Cache
    //! @param none
    void updateCache();

    //! @brief Clear Delegate Cache
    //! @param none
    virtual void clearCache();

    //! @brief Maintain Cached Items - Create/Delete/Move Cached Items If Necessary
    //! @param none
    void maintainCachedItems();

    //! @brief Updating Cached Items Data
    //! @param aUpdateUI Update UI
    virtual void updateCachedItemsData(const bool& aUpdateUI = true);

    //! @brief Updating Cached Items Size And Pos
    //! @param none
    virtual void updateCachedItemsSizeAndPos();

    //! @brief Connect Delegate Signals
    //! @param aItem Item Delegate
    virtual void connectDelegateSignals(ItemDelegate* aItem);

    //! @brief Disconnect Delegate Signals
    //! @param aItem Item Delegate
    virtual void disconnectDelegateSignals(ItemDelegate* aItem);

    //! @brief Show Debug Info
    //! @param none
    void showDebugInfo();

    //! @brief Set Current Index
    //! @param aIndex New Current Index
    void setCurrentIndex(const int& aIndex);

    //! @brief Set Ease Pos
    //! @param aPos
    void setEasePos(const int& aPos);

    //! @brief Get Ease Pos
    //! @param none
    //! @return Ease Pos
    int easePos() const;

    //! @brief Scroll To Current Index
    //! @param aAnimated Animated
    void scrollToCurrentIndex(const bool& aAnimated = true);

    //! @brief Setup Ease Anim
    //! @param none
    void setupEaseAnim();

    //! @brief Start Ease Anim
    //! @param aEndValue End Value
    void startEaseAnim(const int& aEndValue);

    //! @brief Delete Ease Anim
    //! @param none
    void deleteEaseAnim();

    //! @brief Setup Repos Anim
    //! @param none
    void setupReposAnim();

    //! @brief Start Repos Anim
    //! @param aEndValue End Value
    void startReposAnim(const int& aEndValue);

    //! @brief Delete Repos Anim
    //! @param none
    void deleteReposAnim();

    //! @brief Setup Snap to Anim
    //! @param none
    void setupSnapToAnim();

    //! @brief Start Snap to Anim
    //! @param aEndValue End Value
    void startSnapToAnim(const int& aEndValue);

    //! @brief Delete Snap To Anim
    //! @param none
    void deleteSnapToAnim();

    //! @brief Setup Goto Anim
    //! @param none
    void setupGotoAnim();

    //! @brief Start Goto Anim
    //! @param none
    void startGotoAnim(const int& aEndValue);

    //! @brief Delete Goto Anim
    //! @param none
    void deleteGotoAnim();

    //! @brief Trigger Item Update
    //! @param aIndex Item Index
    void triggerItemUpdate(const int& aIndex);

    //! @brief Start Drag Filter Timer
    //! @param none
    void startDragFilterTimer();

    //! @brief Stop Drag Filter Timer
    //! @param none
    void stopDragFilterTimer();

    //! @brief Start Wheel Repositioning Timer
    //! @param none
    void startWheelReposTimer();

    //! @brief Stop Wheel Repositioning Timer
    //! @param none
    void stopWheelReposTimer();

    //! @brief Handle Key Event
    //! @param aKey Key Code
    //! @param aModifiers Key Modifiers
    virtual void handleKeyEvent(const int& aKey, const Qt::KeyboardModifiers& aModifiers);

protected: // From QFrame

    //! @brief Resize Event
    //! @param aEvent Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);

    //! @brief Timer Event
    //! @param aEvent Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

    //! @brief Key Press Event
    //! @param aEvent Key Event
    virtual void keyPressEvent(QKeyEvent* aEvent);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

    //! @brief Mouse Press Event
    //! @param aEvent Mouse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);

    //! @brief Mouse Release Event
    //! @param aEvent Mouse Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    //! @brief Wheel Event
    //! @param aEvent Key Event
    virtual void wheelEvent(QWheelEvent* aEvent);

    //! @brief Focus In Event
    //! @param aEvent Focus Event
    virtual void focusInEvent(QFocusEvent* aEvent);

protected: // Data

    //! List Model
    ListModel*              model;
    //! Delegate
    ItemDelegate*           delegate;
    //! Delegate Cache
    CacheType               cache;
    //! Orientation
    ListBoxOrientation      orientation;
    //! Cache Overhead Percentage According To Visible Item Count
    int                     cacheOverhead;
    //! Cache Count Max
    int                     cacheCountMax;
    //! Item Spacing
    int                     spacing;
    //! Delegat eSize
    int                     delegateSize;
    //! Full Item Size
    int                     fullItemSize;
    //! Visible Item Count
    int                     visibleItemCount;
    //! Visible Full Item Count
    int                     fullVisibleItemCount;
    //! Visible Item Count Max
    int                     visibleItemCountMax;
    //! First Cached Item Index
    int                     firstCachedItem;
    //! Last Cached Item Index
    int                     lastCachedItem;
    //! First Visible Item Index
    int                     firstVisibleItem;
    //! Last Visible Item Index
    int                     lastVisibleItem;
    //! Original Scroll Pos
    int                     orgScrollPos;
    //! Scroll Pos
    int                     scrollPos;
    //! Min Scroll Pos
    int                     minScrollPos;
    //! Max Scroll Pos
    int                     maxScrollPos;
    //! Previous/Original Scroll Pos
    int                     prevScrollPos;
    //! Previous Index
    int                     prevIndex;
    //! Current Item Index
    int                     currentIndex;
    //! Easing
    bool                    easing;
    //! Ease Speed
    qreal                   easeSpeed;
    //! Added Index
    int                     addedIndex;
    //! Updated Index
    int                     updatedIndex;
    //! Removed Index
    int                     removedIndex;
    //! Ease Anim
    QPropertyAnimation*     easeAnim;
    //! Reposition Anim
    QPropertyAnimation*     reposAnim;
    //! SnapTo Anim
    QPropertyAnimation*     snapToAnim;
    //! GoTo Anim
    QPropertyAnimation*     gotoAnim;
    //! Drag Filter Timer ID
    int                     dragFilterID;
    //! Snap To Item
    bool                    snapToItem;
    //! Pressed
    bool                    pressed;
    //! Key Pressed
    bool                    keyPressed;
    //! Key Repeat
    bool                    keyRepeat;
    //! Last Key
    int                     lastKey;
    //! Items Moved
    bool                    itemsMoved;
    //! Wheel Repos Timer ID
    int                     wheelReposID;
};

#endif // LISTBOX_H
