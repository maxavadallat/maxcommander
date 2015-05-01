
#include <QPainter>
#include <QDebug>

#include "colorpickerbutton.h"

#define DEFAULT_COLOR_RECT_DX   16
#define DEFAULT_COLOR_RECT_DY   10

//==============================================================================
// Constructor
//==============================================================================
ColorPickerButton::ColorPickerButton(QWidget* aParent)
    : QPushButton(aParent)
    , color(qRgba(128, 128, 128, 255))
{
    //qDebug() << "ColorPickerButton::ColorPickerButton";

    // ...
}

//==============================================================================
// Get Color
//==============================================================================
QRgb ColorPickerButton::getColor()
{
    return color;
}

//==============================================================================
// Set Color
//==============================================================================
void ColorPickerButton::setColor(const QRgb& aColor)
{
    // Check Color
    if (color != aColor) {
        //qDebug() << "ColorPickerButton::setColor - aColor: " << qAlpha(aColor) << qRed(aColor) << qGreen(aColor) << qBlue(aColor);
        // Set Color
        color = aColor;
        // Update
        update();
    }
}

//==============================================================================
// Paint Event
//==============================================================================
void ColorPickerButton::paintEvent(QPaintEvent* aEvent)
{
    QPushButton::paintEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Init Painter
        QPainter painter(this);

        //painter.setPen();

        // Set Brush
        painter.setBrush(QColor::fromRgba(color));
        // Draw Rect
        painter.drawRect(rect().adjusted(DEFAULT_COLOR_RECT_DX, DEFAULT_COLOR_RECT_DY, -DEFAULT_COLOR_RECT_DX, -DEFAULT_COLOR_RECT_DY-3));
    }
}

//==============================================================================
// Destructor
//==============================================================================
ColorPickerButton::~ColorPickerButton()
{
    // ...

    //qDebug() << "ColorPickerButton::~ColorPickerButton";
}

