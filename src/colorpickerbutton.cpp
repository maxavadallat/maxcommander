
// INCLUDES

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
    , color(QColor(0, 0, 0))
    , pressed(false)
{
    ui->setupUi(this);
}

//==============================================================================
// Set Color
//==============================================================================
void ColorPickerButton::setColor(const QColor& aColor)
{
    // Check UI
    if (ui && ui->colorArea && (color != aColor || ui->colorArea->styleSheet().isEmpty())) {
        // Set Color
        color = aColor;
        // Set Color Area Style Sheet
        ui->colorArea->setStyleSheet(QString(STYLE_SHEET_BACKGROUND_COLOR_TEMPLATE).arg(color.red())
                                                                                   .arg(color.green())
                                                                                   .arg(color.blue()));
    }
}

//==============================================================================
// Get Color
//==============================================================================
QColor ColorPickerButton::getColor()
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


