#ifndef HEADERRESIZEAREA_H
#define HEADERRESIZEAREA_H

#include <QDebug>
#include <QFrame>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>


namespace Ui {
class HeaderResizeArea;
}

//==============================================================================
//! @class HeaderResizeArea Header Item Resize Area Class
//==============================================================================
class HeaderResizeArea : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit HeaderResizeArea(QWidget* aParent = NULL);

    //! @brief Set Resize Area Size
    //! @param aWidth Width
    void setSize(const int& aWidth);

    //! @brief Destructor
    //! @param none
    virtual ~HeaderResizeArea();

signals:

    //! @brief Drag Started Signal
    //! @param none
    void dragStarted(const int& aPressPos);

    //! @brief Dragged Signal
    //! @param aDelta Position Change
    void dragged(const int& aDelta);

    //! @brief Drag Finished Signal
    //! @param none
    void dragFinished();

    //! @brief Double Clicked Signal
    //! @param none
    void doubleClicked();

protected:

    //! @brief Mouse Press Event
    //! @param aEvent Mouse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);

    //! @brief Mouse Release Event
    //! @param aEvent Mouse Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    //! @brief Mouse Move Event
    //! @param aEvent Mouse Event
    virtual void mouseMoveEvent(QMouseEvent* aEvent);

    //! @brief Mouse Double Click Event
    //! @param aEvent Mouse Event
    virtual void mouseDoubleClickEvent(QMouseEvent* aEvent);

    //! @brief Paint Event
    //! @param aEvent Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected:
    friend class HeaderItem;

    //! UI
    Ui::HeaderResizeArea*   ui;

    //! Pressed State
    bool                    pressed;

    //! Original Position
    int                     pressPos;

    //! Current Pos
    int                     currPos;
};

#endif // HEADERRESIZEAREA_H
