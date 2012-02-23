#ifndef HEADERPOPUP_H
#define HEADERPOPUP_H

// INCLUDES

#include <QFrame>
#include <QEventLoop>
#include <QKeyEvent>


// FORWARD DECLARATIONS

namespace Ui {
class HeaderPopup;
}


// DECLARATIONS

//==============================================================================
//! @class HeaderPopup Header Popup Menu/Dialog Class
//==============================================================================
class HeaderPopup : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit HeaderPopup(QWidget* aParent = NULL);

    //! @brief Popup
    //! @param aPosition Position
    void popup(const QPoint& aPosition);

    //! @brief Set Item Selected
    //! @param aIndex Item Index
    //! @param aSelected Item Selected
    void setItemSelected(const int& aIndex, const bool& aSelected);

    //! @brief Get Item Selected
    //! @param aIndex Item Index
    //! @return Item Selected
    bool getItemSelected(const int& aIndex);

    //! @brief Destructor
    //! @param none
    virtual ~HeaderPopup();

signals:

    //! @brief Close Button Clicked Signal
    //! @param none
    void closeButtonClicked();

protected slots:

    //! @brief Close Button Clicked Slot
    //! @param none
    void on_closeButton_clicked();

protected: // From QFrame

    //! @brief Key Press Event
    //! @param aEvent Key Event
    virtual void keyPressEvent(QKeyEvent* aEvent);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

protected:

    //! UI
    Ui::HeaderPopup*    ui;
    //! Event Loop
    QEventLoop          eventLoop;
    //! Key Pressed
    bool                keyPressed;
};

#endif // HEADERPOPUP_H
