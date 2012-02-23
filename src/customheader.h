#ifndef CUSTOMHEADER_H
#define CUSTOMHEADER_H

// INCLUDES

#include <QDebug>
#include <QFrame>
#include <QList>
#include <QResizeEvent>


// FORWARD DECLARATIONS

namespace Ui {
class CustomHeader;
}

class HeaderItem;


// DECLARATIONS

//==============================================================================
//! @class CustomHeader Custom Header Class
//==============================================================================
class CustomHeader : public QFrame
{
    Q_OBJECT

public:
    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit CustomHeader(QWidget* aParent = NULL);

    //! @brief Set Header Active
    //! @param aActive Active State
    void setActive(const bool& aActive);

    //! @brief Set Sorting Order
    //! @param aIndex Header Item/Sorting Index
    //! @param aReversed Reverse Order
    //! @param aRefresh Refresh
    void setSorting(const int& aIndex, const bool& aReversed, const bool& aRefresh = true);

    //! @brief Get Sorting Order
    //! @param none
    //! @return Current Order
    int getSorting();

    //! @brief Get Reversed Direction
    //! @param none
    //! @return Reversed Direction
    bool getReversed();

    //! @brief Get Header Active
    //! @param none
    //! @return Header Active State
    bool getActive();

    //! @brief Append Header Item
    //! @param aItem New Header Item
    void appendItem(HeaderItem* aItem);

    //! @brief Remove Header Item
    //! @param aIndex Item Index
    void removeItem(const int& aIndex);

    //! @brief Get Header Item Count
    //! @param none
    //! @return Heder Item Count
    int getItemCount();

    //! @brief Set Header Item Width
    //! @param aIndex Item Index
    //! @param aWidth Item Width
    void setItemWidth(const int& aIndex, const int& aWidth);

    //! @brief Get Header Item Width
    //! @param aIndex Item Index
    //! @return Header Item Width
    int getItemWidth(const int& aIndex);

    //! @brief Set Header Item Useage
    //! @param aIndex Item Index
    //! @param aInuse Item Useage
    void setItemUseage(const int& aIndex, const bool& aInuse);

    //! @brief Get Header Item Useage
    //! @param aIndex Item Index
    //! @return Header Item Useage
    bool getItemUseage(const int& aIndex);

    //! @brief Destructor
    //! @param none
    virtual ~CustomHeader();

signals:

    //! @brief Header Activated Signal
    //! @param none
    void headerActivated();

    //! @brief Order Changed Signal
    //! @param aIndex Header Item Index
    //! @param aReversed Reverse Order
    void orderChanged(const int& aIndex, const bool& aReversed);

    //! @brief Header Item Size Changed Signal
    //! @param aIndex Header Item Index
    //! @param aWidth Header Item Width
    void itemSizeChanged(const int& aIndex, const int& aWidth);

    //! @brief Header Item Pos Changed Signal
    //! @param aIndex Header Item Index
    //! @param aPos Header Item Pos X
    void itemPosChanged(const int& aIndex, const int& aPos);

    //! @brief Header Options Signal
    //! @param aPos Mouse Position
    void headerOptions(const QPoint& aPos);

protected slots:

    //! @brief Header Item Selection Changed Slot
    //! @param aIndex Index
    //! @param aSelected New Selection
    //! @param aActivation Activation
    void headerSelectionChanged(const int& aIndex, const bool& aSelected, const bool& aActivation);

    //! @brief Header Item Order Changed Slot
    //! @param aIndex Index
    //! @param aReversed New Order
    //! @param aActivation Activation
    void headerOrderChanged(const int& aIndex, const bool& aReversed, const bool& aActivation);

    //! @brief Header Item Size Changed Slot
    //! @param aIndex Index
    //! @param aWidth Header Item Width
    //! @param aDelta Header Item Width Delta
    void headerItemSizeChanged(const int& aIndex, const int& aWidth, const int& aDelta);

    //! @brief Header Item Pos Changed Slot
    //! @param aIndex Index
    //! @param aDelta Header Item Pos Delta
    void headerItemPosChanged(const int& aIndex, const int& aDelta);

    //! @brief Header Item Resize Started Slot
    //! @param aIndex Index
    void headerItemResizeStarted(const int& aIndex);

    //! @brief Header Item Resize Finished Slot
    //! @param aIndex Index
    void headerItemResizeFinished(const int& aIndex);

    //! @brief Header Item Resize Area Double CLicked Slot
    //! @param aIndex Index
    void headerItemResizeAreaDoubleClicked(const int& aIndex);

    //! @brief Header Item Right Clicked Slot
    //! @param aIndex Item Index
    void headerItemRightClicked(const int& aIndex, const QPoint& aPos);

protected:
    friend class HeaderItem;
    friend class CustomPanel;

    //! @brief Get Item Size
    //! @param aIndex Header Item Index
    //! @return Item Size
    int getItemSize(const int& aIndex);

    //! @brief Get Item Minimum Size
    //! @param aIndex Header Item Index
    //! @return Item Minimum Size
    int getItemMinSize(const int& aIndex);

    //! @brief Get Item Maximum Size
    //! @param aIndex Header Item Index
    //! @param useMinWidth Use Items Minimum Width
    //! @return Item Maximum Size
    int getItemMaxSize(const int& aIndex, const bool& useMinWidth = true);

    //! @brief Append Header Item To Item List
    //! @param aItem New Header Item
    void appendItemToList(HeaderItem* aItem);

    //! @brief Remove Header Item From Item List
    //! @param aIndex Item Index
    void removeItemFromList(const int& aIndex);

    //! @brief Can Header Item Shrink
    //! @param aIndex Item Index
    //! @return true If Header Item Can Shrink
    bool canItemShrink(const int& aIndex);

    //! @brief Can Header Item Grow
    //! @param aIndex Item Index
    //! @return true If Header Item Can Grow
    bool canItemGrow(const int& aIndex);

    //! @brief Adjust Items Size And Position
    //! @param aIndex Item Index
    //! @param aDelta Size Change Delta
    void adjustItemsSizeAndPos(const int& aIndex, const int& aDelta);

    //! @brief Get Item Right
    //! @param aIndex Item Index
    //! @return Item Right
    int getItemRight(const int& aIndex);

    //! @brief Get Item Max Pos
    //! @param aIndex Item Index
    //! @return Item Max Pos
    int getItemMaxPos(const int& aIndex);

    //! @brief Update Minimum Size
    //! @param none
    void updateMinSize();

    //! @brief Get Next In Use Item Index
    //! @param aIndex Item Index
    //! @return Next In Use Item Index
    int getNextItemIndex(const int& aIndex);

    //! @brief Get Item
    //! @param aIndex Item Index
    //! @return Item Or Null
    HeaderItem* getItem(const int& aIndex);

protected: // From QFrame

    //! @brief Resize Event
    //! @param aEvent Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);

protected:

    //! UI
    Ui::CustomHeader*   ui;
    //! Active State
    bool                active;
    //! Header Item List
    QList<HeaderItem*>  itemList;
    //! Current Header Item Index
    int                 index;
    //! Previous Selected Header Item Index
    int                 prevSelIndex;
    //! Reverse Order
    bool                reverse;
    //! Next Item Max Size
    int                 nextItemMaxSize;
};

#endif // CUSTOMHEADER_H
