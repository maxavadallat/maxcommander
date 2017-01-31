#include <QDebug>

#include "customdirlabel.h"

//==============================================================================
// Constructor
//==============================================================================
CustomDirLabel::CustomDirLabel(QWidget* aParent, Qt::WindowFlags aFlags)
    : QLabel(aParent, aFlags)
    , mousePressed(false)
{
    //qDebug() << "CustomDirLabel::CustomDirLabel";

    // ...
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void CustomDirLabel::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Set Mouse Pressed
        mousePressed = true;
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void CustomDirLabel::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent && mousePressed) {
        // Reset Mouse Pressed
        mousePressed = false;
        // Check Mouse Buttons
        if (aEvent->button() == Qt::RightButton) {
            // Emit Right Mouse Clicked Signal
            emit rightMouseClicked(aEvent->globalPos());
        }
    }
}

//==============================================================================
// Mouse Double Clicked Event
//==============================================================================
void CustomDirLabel::mouseDoubleClickEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Emit Mouse Double Clicked Event
        emit mouseDoubleClicked();
    }
}

//==============================================================================
// Destructor
//==============================================================================
CustomDirLabel::~CustomDirLabel()
{
    // ...

    //qDebug() << "CustomDirLabel::~CustomDirLabel";
}

