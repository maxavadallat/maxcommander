#ifndef HEADERITEM_H
#define HEADERITEM_H

// INCLUDES

#include <QDebug>
#include <QFrame>


// FORWARD DECLARATIONS

namespace Ui {
class HeaderItem;
}

class CustomHeader;


// DECLARATIONS

//==============================================================================
//! @class HeaderItem Header Item Class
//==============================================================================
class HeaderItem : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit HeaderItem(QWidget* aParent = NULL);

    //! @brief Set Index
    //! @param aIndex New Index
    void setIndex(const int& aIndex);

    //! @brief Set Parent Header
    //! @param aParentHeader Parent Custom Header Widget
    void setParentHeader(CustomHeader* aParentHeader);

    //! @brief Get Index
    //! @param none
    //! @return Header Item Index
    int getIndex();

    //! @brief Set In Use State
    //! @param aInuse In use State
    void setInUse(const bool& aInuse);

    //! @brief Get In Use State
    //! @param none
    //! @return true If HEader Item Is In Use/Visible
    bool getInUse();

    //! @brief Set Title
    //! @param aTitle New Title
    void setTitle(const QString& aTitle);

    //! @brief Get Title Text
    //! @param none
    //! @return Title Text
    QString getTitle();

    //! @brief Set Title Alignment
    //! @param aAlignment New Title Alignment
    void setTitleAlignment(const Qt::Alignment& aAlignment);

    //! @brief Set Selected state
    //! @param aSelected Selected State
    //! @param aRefresh Refresh
    void setSelected(const bool& aSelected, const bool& aRefresh = true);

    //! @brief Get Selected State
    //! @param none
    //! @return Selected State
    bool getSelected();

    //! @brief Set Reversed State
    //! @param aReversed Reversed State
    //! @param aRefresh Refresh
    void setReversed(const bool& aReversed, const bool& aRefresh = true);

    //! @brief Get Reversed Order State
    //! @param none
    //! @return Reversed Order State
    bool getReversed();

    //! @brief Set Active State
    //! @param aActive Active State
    //! @param aRefresh Refresh
    void setActive(const bool& aActive, const bool& aRefresh = true);

    //! @brief Get Active State
    //! @param none
    //! @return Active State
    bool getActive();

    //! @brief Set Width
    //! @param aWidth New Width
    void setWidth(const int& aWidth);

    //! @brief Get Width
    //! @param none
    //! @return Width
    int getWidth();

    //! @brief Set Resize Area Size
    //! @param aSize New Resize Area Size
    void setResizeAreaSize(const int& aSize);

    //! @brief Get Resize Area Size
    //! @param none
    //! @return Resize Area Size
    int getResizeAreaSize();

    //! @brief Set Resize Area Visibility
    //! @param aVisible Resize Area Visibility
    void setResizeAreaVisible(const bool& aVisible);

    //! @brief Destructor
    //! @param none
    virtual ~HeaderItem();

signals:

    //! @brief Selection Changed Signal
    //! @param aIndex Index
    //! @param aSelected New Selection
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void selectionChanged(const int& aIndex, const bool& aSelected, const bool& aActivation, const bool& aRefresh);

    //! @brief Order Changed Signal
    //! @param aIndex Index
    //! @param aReversed New Order
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void orderChanged(const int& aIndex, const bool& aReversed, const bool& aActivation, const bool& aRefresh);

    //! @brief Size Changed Signal
    //! @param aIndex Header Item Index
    //! @param aWidth New Width
    //! @param aDelta Width Delta
    void sizeChanged(const int& aIndex, const int& aWidth, const int& aDelta);

    //! @brief Position Changed Signal
    //! @param aIndex Header Item Index
    //! @param aDelta Width Delta
    void posChanged(const int& aIndex, const int& aDelta);

    //! @brief Resizing Started Signal
    //! @param aIndex Header Item Index
    void resizeStarted(const int& aIndex);

    //! @brief Resizing Finished Signal
    //! @param aIndex Header Item Index
    void resizeFinished(const int& aIndex);

    //! @brief Resizing Area Double Clicked Signal
    //! @param aIndex Header Item Index
    void resizeDoubleClicked(const int& aIndex);

    //! @brief Right Button Clicked
    //! @param aIndex Header Item Index
    //! @param aPos Mouse Pos
    void rightClicked(const int& aIndex, const QPoint& aPos);

protected slots:

    //! @brief Resize Area Drag Started Slot
    //! @param aPressPos Press Pos
    void resizeAreaDragStarted(const int& aPressPos);

    //! @brief Resize Area Dragged
    //! @param aDelta Drag/Position Delta
    void resizeAreaDragged(const int& aDelta);

    //! @brief Resize Area Drag Finished Slot
    //! @param none
    void resizeAreaDragFinished();

    //! @brief Resize Area Double Clicked Slot
    //! @param none
    void resizeAreaDoubleClicked();

    //! @brief Title Selected Slot
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void titleAreaSelected(const bool& aActivation, const bool& aRefresh);

    //! @brief Order Changed Slot
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void titleAreaOrderChanged(const bool& aActivation, const bool& aRefresh);

    //! @brief Title Area Right Clicked Slot
    //! @param aPos Mouse Position
    void titleAreaRightClicked(const QPoint& aPos);

protected: // From QFrame

    //! @brief Paint Event
    //! @param aEvent Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

    //! @brief Resize Event
    //! @param aEvent Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);

    //! @brief Move Event
    //! @param aEvent Move Event
    virtual void moveEvent(QMoveEvent* aEvent);

protected:

    //! UI
    Ui::HeaderItem*     ui;
    //! Custom Header Parent
    CustomHeader*       parentHeader;
    //! In Use
    bool                inUse;
    //! Index
    int                 index;
    //! Active
    bool                active;
    //! Selected
    bool                selected;
    //! Original Size
    QSize               originalSize;
    //! Resized
    bool                resized;
};

#endif // HEADERITEM_H
