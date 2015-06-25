
#include <QDebug>

#include "filelistwidget.h"
#include "constants.h"

//==============================================================================
// Constuctor
//==============================================================================
FileListWidget::FileListWidget(QWidget* aParent)
    : QQuickWidget(aParent)
    , focused(false)
    , dragTimerID(-1)
    , dragOn(false)
    , insidePress(false)
    , dragPosX(0)
    , dragPosY(0)
{
    //qDebug() << "FileListWidget::FileListWidget";

    // ...
}

//==============================================================================
// Start Drag Timer
//==============================================================================
void FileListWidget::startDragTimer()
{
    // Check Timer ID
    if (dragTimerID == -1) {
        //qDebug() << "FileListWidget::startDragTimer";
        // Start Timer
        dragTimerID = startTimer(DEFAULT_ONE_SEC * 0.7);
    }
}

//==============================================================================
// Stop Drag Timer
//==============================================================================
void FileListWidget::stopDragTimer()
{
    // Check Timer ID
    if (dragTimerID != -1) {
        //qDebug() << "FileListWidget::stopDragTimer";
        // Kill Timer
        killTimer(dragTimerID);
        // Reset Timer ID
        dragTimerID = -1;
    }
}

//==============================================================================
// Focus In Event
//==============================================================================
void FileListWidget::focusInEvent(QFocusEvent* aEvent)
{
    QQuickWidget::focusInEvent(aEvent);

    // Check If Focused
    if (!focused) {
        //qDebug() << "FileListWidget::focusInEvent";
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
        //qDebug() << "FileListWidget::focusOutEvent";
        // Set Focused
        focused = false;
        // Emit focus Changed Signal
        emit focusChanged(focused);
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void FileListWidget::mousePressEvent(QMouseEvent* aEvent)
{
    QQuickWidget::mousePressEvent(aEvent);

    // Check Event
    if (aEvent) {
        //qDebug() << "FileListWidget::mousePressEvent";

        // Set Inside Press
        insidePress = true;

        // Check Buttons
        if (aEvent->button() == Qt::LeftButton) {
            // Set Drag Pos X
            dragPosX = aEvent->pos().x();
            // Set Drag Pos Y
            dragPosY = aEvent->pos().y();

            // Start Drag Timer
            startDragTimer();

        }

        // ...
    }
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void FileListWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
    QQuickWidget::mouseMoveEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Stop Drag Timer
        stopDragTimer();
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void FileListWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
    QQuickWidget::mouseReleaseEvent(aEvent);

    // Check Event
    if (aEvent) {
        //qDebug() << "FileListWidget::mouseReleaseEvent";

        // Stop Drag Timer
        stopDragTimer();

        // Check Inside Press
        if (insidePress) {
            // Reset Inside Press
            insidePress = false;

            // ...

            // Check Drag On
            if (dragOn) {
                // Reset Drag
                dragOn = false;

                //qDebug() << "FileListWidget::mouseReleaseEvent - dragOn: " << dragOn;

                // Emit Drag Dropped Signal
                //emit dragDropped();
            }

        } else {

            // ...

        }
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void FileListWidget::timerEvent(QTimerEvent* aEvent)
{
    QQuickWidget::timerEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Check Event ID
        if (aEvent->timerId() == dragTimerID) {
            // Stop Timer
            stopDragTimer();

            // Set Drag On
            dragOn = true;

            qDebug() << "FileListWidget::timerEvent - dragOn: " << dragOn;

            // Emit Drag Started Signal
            emit dragStarted(dragPosX, dragPosY);
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileListWidget::~FileListWidget()
{
    // ...

    //qDebug() << "FileListWidget::~FileListWidget";
}

