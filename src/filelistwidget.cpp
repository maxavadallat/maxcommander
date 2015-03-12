#include <QDebug>

#include "filelistwidget.h"

//==============================================================================
// Constuctor
//==============================================================================
FileListWidget::FileListWidget(QWidget* aParent)
    : QQuickWidget(aParent)
    , focused(false)
{
}

//==============================================================================
// Focus In Event
//==============================================================================
void FileListWidget::focusInEvent(QFocusEvent* aEvent)
{
    QQuickWidget::focusInEvent(aEvent);

    // Check If Focused
    if (!focused) {
        // Set Focused
        focused = true;
        // Emit focus Changed Signal
        emit focusChanged(focused);
    }
}

//==============================================================================
// Focus Out Event
//==============================================================================
void FileListWidget::focusOutEvent(QFocusEvent* aEvent)
{
    QQuickWidget::focusOutEvent(aEvent);

    // Check If Focused
    if (focused) {
        // Set Focused
        focused = false;
        // Emit focus Changed Signal
        emit focusChanged(focused);
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileListWidget::~FileListWidget()
{

}

