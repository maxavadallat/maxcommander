
// INCLUDES

#include <QDebug>

#include "constants.h"
#include "colorpickerbutton.h"
#include "ui_colorpickerbutton.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ColorPickerButton::ColorPickerButton(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::ColorPickerButton)
    , color(-1)
    , pressed(false)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Color
//==============================================================================
void ColorPickerButton::setColor(const int& aColor)
{
    // Check UI
    if (ui && ui->colorArea && color != aColor) {
        // Set Color
        color = aColor;

        // Check Color Value
        if (color != -1) {
/*
            // Set Color Area Style Sheet
            ui->colorArea->setStyleSheet(QString(STYLE_SHEET_BACKGROUND_COLOR_TEMPLATE).arg(QColor(color).red())
                                                                                       .arg(QColor(color).green())
                                                                                       .arg(QColor(color).blue()));
*/
            qDebug() << "ColorPickerButton::setColor - color: " << QColor(color).name();

            // Set Color Area Style Sheet
            ui->colorArea->setStyleSheet(QString(STYLE_SHEET_BACKGROUND_COLOR_TEMPLATE).arg(QColor(color).name()));

        } else {
            // Clear Color Area Style Sheet
            ui->colorArea->setStyleSheet(QString(""));
        }
    }
}
/*
//==============================================================================
// Set Color
//==============================================================================
void ColorPickerButton::setColor(const QColor& aColor)
{
    // Check UI
    if (ui && ui->colorArea && (color != aColor || ui->colorArea->styleSheet().isEmpty())) {
        // Set Color
        color = aColor;

        // Check Color Value
        if (color.value() != -1) {
            // Set Color Area Style Sheet
            ui->colorArea->setStyleSheet(QString(STYLE_SHEET_BACKGROUND_COLOR_TEMPLATE).arg(color.red())
                                                                                       .arg(color.green())
                                                                                       .arg(color.blue()));
        } else {
            // Clear Color Area Style Sheet
            ui->colorArea->setStyleSheet(QString(""));
        }
    }
}
*/
//==============================================================================
// Get Color
//==============================================================================
int ColorPickerButton::getColor()
{
    return color;
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void ColorPickerButton::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed State
        if (!pressed) {
            // Set Pressed State
            pressed = true;

            // Set Frame Shadow
            setFrameShadow(QFrame::Sunken);
        }
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void ColorPickerButton::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed State
        if (pressed) {
            // Reset Pressed State
            pressed = false;

            // Set Frame Shadow
            setFrameShadow(QFrame::Raised);

            // Emit Clicked Signal
            emit clicked();
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
ColorPickerButton::~ColorPickerButton()
{
    // Delete UI
    delete ui;
}


