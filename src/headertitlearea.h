#ifndef HEADERTITLEAREA_H
#define HEADERTITLEAREA_H

#include <QDebug>
#include <QFrame>
#include <QPaintEvent>

namespace Ui {
class HeaderTitleArea;
}

//==============================================================================
//! @class HeaderTitleArea Header Item Title Area Class
//==============================================================================
class HeaderTitleArea : public QFrame
{
    Q_OBJECT

public:
    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit HeaderTitleArea(QWidget* aParent = NULL);

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
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void setSelected(const bool& aSelected, const bool& aActivation = false, const bool& aRefresh = true);

    //! @brief Get Selected State
    //! @param none
    //! @return Selected State
    bool getSelected();

    //! @brief Set Reversed State
    //! @param aReversed Reversed State
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void setReversed(const bool& aReversed, const bool& aActivation = false, const bool& aRefresh = true);

    //! @brief Get Reversed Order State
    //! @param none
    //! @return Reversed Order State
    bool getReversed();

    //! @brief Destructor
    //! @param none
    virtual ~HeaderTitleArea();

signals:

    //! @brief Title Selected Signal
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void titleSelected(const bool& aActivation, const bool& aRefresh);

    //! @brief Order Changed Signal
    //! @param aActivation Activation
    //! @param aRefresh Refresh
    void orderChanged(const bool& aActivation, const bool& aRefresh);

    //! @brief Right Button Released
    //! @param aPos Mouse Position
    void rightButtonReleased(const QPoint& aPos);

protected:

    //! @brief Update Elements
    //! @param none
    void updateElements();

protected: // From QFrame

    //! @brief Mouse Press Event
    //! @param aEvent Mouse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);

    //! @brief Mouse Release Event
    //! @param aEvent Mouse Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    //! @brief Mouse Move Event
    //! @param aEvent Mouse Event
    virtual void mouseMoveEvent(QMouseEvent* aEvent);

protected:
    //! UI
    Ui::HeaderTitleArea*    ui;
    //! Title Text
    QString                 title;
    //! Selected
    bool                    selected;
    //! Reverse Order - For Order Indicator
    bool                    reverse;
    //! Pressed
    bool                    pressed;
    //! Right Pressed
    bool                    rightPressed;
};

#endif // HEADERTITLEAREA_H
