
// INCLUDES

#include <QPainter>

#include "headerresizearea.h"
#include "ui_headerresizearea.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
HeaderResizeArea::HeaderResizeArea(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::HeaderResizeArea)
    , pressed(false)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Resize Area Size
//==============================================================================
void HeaderResizeArea::setSize(const int& aWidth)
{
    // Set Width
    resize(aWidth, size().height());
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void HeaderResizeArea::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed
        if (!pressed) {
            // Set Pressed
            pressed = true;
            // Save Mouse Global Pos
            pressPos = aEvent->globalPos().x();

            //qDebug() << "HeaderResizeArea::mousePressEvent - pressPos: " << pressPos;

            // Emit Drag Started Signal
            emit dragStarted(pressPos);
        }
    }
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void HeaderResizeArea::mouseMoveEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed
        if (pressed) {
            // Calculate Delta
            int delta = aEvent->globalPos().x() - pressPos;
            // Get Current Pos
            currPos = aEvent->globalPos().x();
            //qDebug() << "HeaderResizeArea::mouseMoveEvent - delta: " << delta;
            // Emit Dragged Signal
            emit dragged(delta);
        }
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void HeaderResizeArea::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed
        if (pressed) {
            // Set Pressed
            pressed = false;

            //qDebug() << "HeaderResizeArea::mouseReleaseEvent";

            // Emit Drag Finished Signal
            emit dragFinished();
        }
    }
}

//==============================================================================
// Mouse Double Click Event
//==============================================================================
void HeaderResizeArea::mouseDoubleClickEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        qDebug() << "HeaderResizeArea::mouseDoubleClickEvent";
        // Emit Double Clicked Signal
        emit doubleClicked();
    }
}

//==============================================================================
// Paint Event
//==============================================================================
void HeaderResizeArea::paintEvent(QPaintEvent* aEvent)
{
    QFrame::paintEvent(aEvent);
    // Check Event
    if (aEvent) {
        // Get Painter
        QPainter painter(this);
        // Save Painter
        painter.save();
        // Get Current Rect
        QRect currentRect = QRect(geometry().width() / 2, 0, geometry().width() - 1, geometry().height() - 1);
        // Adjust Current Rect
        currentRect.adjust(0, geometry().height() / 4, 0, -geometry().height() / 4);
        // Init Pen
        QPen aPen(Qt::DotLine);
        aPen.setColor(Qt::darkGray);
        // Set Pen & Brush
        painter.setPen(aPen);
        painter.setBrush(Qt::NoBrush);
        // Draw Resize Area Bar
        painter.drawLine(currentRect.topLeft(), currentRect.bottomLeft());
        // Restore Painter
        painter.restore();
    }
}

//==============================================================================
// Destructor
//==============================================================================
HeaderResizeArea::~HeaderResizeArea()
{
    delete ui;
}
