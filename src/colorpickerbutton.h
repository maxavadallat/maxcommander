#ifndef COLORPICKERBUTTON_H
#define COLORPICKERBUTTON_H

// INCLUDES

#include <QFrame>
#include <QMouseEvent>
#include <QColor>


// FORWARD DECLARATIONS

namespace Ui {
class ColorPickerButton;
}


// DECLARATIONS

//==============================================================================
//! @class ColorPickerButton Settings Window Color Picker Button Class
//==============================================================================
class ColorPickerButton : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit ColorPickerButton(QWidget* aParent = NULL);

    //! @brief Set Color
    //! @param aColor New Color
    void setColor(const QColor& aColor);

    //! @brief Get Color
    //! @param none
    //! @return Color
    QColor getColor();

    //! @brief Destructor
    //! @param none
    virtual ~ColorPickerButton();

signals:

    //! @brief Clicked Signal
    //! @param none
    void clicked();

protected: // From QFrame

    //! @brief Mouse Press Event
    //! @param aEvent Mouse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);

    //! @brief Mouse Release Event
    //! @param aEvent Mouse Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

protected: // Data

    //! UI
    Ui::ColorPickerButton*  ui;
    //! Color
    QColor                  color;
    //! Pressed
    bool                    pressed;
};

#endif // COLORPICKERBUTTON_H
