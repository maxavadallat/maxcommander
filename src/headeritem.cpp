
// INCLUDES

#include <QPainter>

#include "headeritem.h"
#include "ui_headeritem.h"
#include "customheader.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
HeaderItem::HeaderItem(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::HeaderItem)
    , parentHeader(NULL)
    , inUse(true)
    , index(-1)
    , active(false)
    , selected(false)
    , resized(false)
{
    // Setup UI
    ui->setupUi(this);

    // Connect Signals
    connect(ui->resizeArea, SIGNAL(dragStarted(int)), this, SLOT(resizeAreaDragStarted(int)));
    connect(ui->resizeArea, SIGNAL(dragged(int)), this, SLOT(resizeAreaDragged(int)));
    connect(ui->resizeArea, SIGNAL(dragFinished()), this, SLOT(resizeAreaDragFinished()));
    connect(ui->resizeArea, SIGNAL(doubleClicked()), this, SLOT(resizeAreaDoubleClicked()));

    connect(ui->title, SIGNAL(titleSelected(bool, bool)), this, SLOT(titleAreaSelected(bool, bool)));
    connect(ui->title, SIGNAL(orderChanged(bool, bool)), this, SLOT(titleAreaOrderChanged(bool, bool)));
    connect(ui->title, SIGNAL(rightButtonReleased(QPoint)), this, SLOT(titleAreaRightClicked(QPoint)));

    // ...
}

//==============================================================================
// Set Index
//==============================================================================
void HeaderItem::setIndex(const int& aIndex)
{
    // Set Index
    index = aIndex;
}

//==============================================================================
// Set Parent Header
//==============================================================================
void HeaderItem::setParentHeader(CustomHeader* aParentHeader)
{
    // Set Parent Header
    parentHeader = aParentHeader;
}

//==============================================================================
// Get Index
//==============================================================================
int HeaderItem::getIndex()
{
    return index;
}

//==============================================================================
// Set In Use State
//==============================================================================
void HeaderItem::setInUse(const bool& aInuse)
{
    // Check In Use
    if (inUse != aInuse) {
        // Set In Use
        inUse = aInuse;
        // Set Visibility
        setVisible(inUse);
/*
        // Check In Use
        if (inUse) {
            // Set Width
            setWidth(minimumWidth());
        } else {
            // Emit Header Item Size Changed
            emit sizeChanged(index, 0, -width());
        }
*/
    }
}

//==============================================================================
// Get In Use State
//==============================================================================
bool HeaderItem::getInUse()
{
    return inUse;
}

//==============================================================================
// Set Title
//==============================================================================
void HeaderItem::setTitle(const QString& aTitle)
{
    // Check Title Area
    if (ui->title) {
        // Set Title
        ui->title->setTitle(aTitle);
    }
}

//==============================================================================
// Get Title
//==============================================================================
QString HeaderItem::getTitle()
{
    // Check Title Area
    if (ui->title) {
        return ui->title->getTitle();
    }

    return QString();
}

//==============================================================================
// Set Title Alignment
//==============================================================================
void HeaderItem::setTitleAlignment(const Qt::Alignment& aAlignment)
{
    // Check Title Area
    if (ui->title) {
        // Set Title Alignment
        ui->title->setTitleAlignment(aAlignment);
    }
}

//==============================================================================
// Set Selected
//==============================================================================
void HeaderItem::setSelected(const bool& aSelected, const bool& aRefresh)
{
    // Check Selected
    if (selected != aSelected) {
        // Set Selected
        selected = aSelected;
        // Check title Area
        if (ui->title) {
            // Set Selected
            ui->title->setSelected(selected, false, aRefresh);
        }
        // Check Refresh
        if (aRefresh) {
            // Update
            update();
        }
    }
}

//==============================================================================
// Get Selected
//==============================================================================
bool HeaderItem::getSelected()
{
    return selected;
}

//==============================================================================
// Set Reversed
//==============================================================================
void HeaderItem::setReversed(const bool& aReversed, const bool& aRefresh)
{
    // Check Title Area
    if (ui->title && ui->title->getReversed() != aReversed) {
        // Set Reversed
        ui->title->setReversed(aReversed, false, aRefresh);
    }
}

//==============================================================================
// Get Reversed
//==============================================================================
bool HeaderItem::getReversed()
{
    // Check Title Area
    if (ui->title) {
        return ui->title->getReversed();
    }

    return false;
}

//==============================================================================
// Set Active State
//==============================================================================
void HeaderItem::setActive(const bool& aActive, const bool& aRefresh)
{
    // Check Active State
    if (active != aActive) {
        // Set Active State
        active = aActive;
        // Check Refresh
        if (aRefresh) {
            // Update
            update();
        }
    }
}

//==============================================================================
// Get Active State
//==============================================================================
bool HeaderItem::getActive()
{
    return active;
}

//==============================================================================
// Set Width
//==============================================================================
void HeaderItem::setWidth(const int& aWidth)
{
    //qDebug() << "HeaderItem::setWidth - index: " << index << " - aWidth: " << aWidth;
    // Set Geometry
    setGeometry(geometry().x(), geometry().y(), aWidth, geometry().height());
}

//==============================================================================
// Get Width
//==============================================================================
int HeaderItem::getWidth()
{
    return size().width();
}

//==============================================================================
// Set Resize Area Size
//==============================================================================
void HeaderItem::setResizeAreaSize(const int& aSize)
{
    // Check Resize Area
    if (ui && ui->resizeArea) {
        // Set Minimum Width
        ui->resizeArea->setMinimumWidth(aSize);
        // Update
        update();
    }
}

//==============================================================================
// Get Resize Area Size
//==============================================================================
int HeaderItem::getResizeAreaSize()
{
    // Check Resize Area
    if (ui && ui->resizeArea) {
        return ui->resizeArea->minimumWidth();
    }

    return 0;
}

//==============================================================================
// Set Resize Area Visibility
//==============================================================================
void HeaderItem::setResizeAreaVisible(const bool& aVisible)
{
    // Check Resize Area
    if (ui && ui->resizeArea) {
        // Set Visibilty
        ui->resizeArea->setVisible(aVisible);
    }
}

//==============================================================================
// Resize Area Drag Started Slot
//==============================================================================
void HeaderItem::resizeAreaDragStarted(const int& aPressPos)
{
    //qDebug() << "HeaderItem::resizeAreaDragStarted - aPressPos: " << aPressPos;

    Q_UNUSED(aPressPos);
    // Save Original Size
    originalSize = size();
    // Reset Resized
    resized = false;
    // Emit Resize Started Signal
    emit resizeStarted(index);
}

//==============================================================================
// Resize Area Dragged
//==============================================================================
void HeaderItem::resizeAreaDragged(const int& aDelta)
{
    if (aDelta == 0) {
        //qDebug() << "HeaderItem::resizeAreaDragged - index: " << index << " - NO DELTA";
        return;
    }
    // Init New Width
    int newWidth;

    // Check Parent Header
    if (parentHeader) {
        // Get Item Max Size
        int itemMaxSize = parentHeader->getItemMaxSize(index);
        // Get Item Min Size
        int itemMinSize = minimumWidth();

        //qDebug() << "HeaderItem::resizeAreaDragged - index: " << index << " - itemMinSize: " << itemMinSize;
        //qDebug() << "HeaderItem::resizeAreaDragged - index: " << index << " - itemMaxSize: " << itemMaxSize;

        // Adjust New Width
        newWidth = qBound(itemMinSize, originalSize.width() + aDelta, itemMaxSize);
    } else {
        // Get New Width
        newWidth = qMax(originalSize.width() + aDelta, minimumWidth());
    }

    //qDebug() << "HeaderItem::resizeAreaDragged - size: " << size() << " - delta: " << aDelta << " - newWidth: " << newWidth;
    //qDebug() << "HeaderItem::resizeAreaDragged - size: " << size() << " - delta: " << aDelta;

    // Set Width
    setWidth(newWidth);

    // Set Resized Flag
    resized = true;
}

//==============================================================================
// Resize Area Drag Finished Slot
//==============================================================================
void HeaderItem::resizeAreaDragFinished()
{
    //qDebug() << "HeaderItem::resizeAreaDragFinished";

    // Check Resized Flag
    if (!resized) {
        // Check Slection & Resized
        if (!selected && !resized) {
            // Check Title
            if (ui->title) {
                // Set Selected
                ui->title->setSelected(true, true);
            }
        }
    }

    // Emit Resize Finished Signal
    emit resizeFinished(index);
}

//==============================================================================
// Resize Area Double Clicked Slot
//==============================================================================
void HeaderItem::resizeAreaDoubleClicked()
{
    //qDebug() << "HeaderItem::resizeAreaDoubleClicked";

    // Emit Resize Area Double Clicked Signal
    emit resizeDoubleClicked(index);
}

//==============================================================================
// Title Selected Slot
//==============================================================================
void HeaderItem::titleAreaSelected(const bool& aActivation, const bool& aRefresh)
{
    //qDebug() << "HeaderItem::titleAreaSelected";

    // Check Title Area
    if (ui->title) {
        // Set/Update Selected State
        selected = ui->title->getSelected();
    }

    // Emit Item Selected Signal
    emit selectionChanged(index, selected, aActivation, aRefresh);
}

//==============================================================================
// Order Changed Slot
//==============================================================================
void HeaderItem::titleAreaOrderChanged(const bool& aActivation, const bool& aRefresh)
{
    //qDebug() << "HeaderItem::titleAreaOrderChanged";

    // Check Title Area
    if (ui->title) {
        // Emit Order Changed Signal
        emit orderChanged(index, ui->title->getReversed(), aActivation, aRefresh);
    }
}

//==============================================================================
// Title Area Right Clicked Slot
//==============================================================================
void HeaderItem::titleAreaRightClicked(const QPoint& aPos)
{
    // Emit Right Clicked Signal
    emit rightClicked(index, aPos);
}

//==============================================================================
// Paint Event
//==============================================================================
void HeaderItem::paintEvent(QPaintEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Get Painter
        QPainter painter(this);
        // Save Painter
        painter.save();
        // Get Current Rect
        QRect currentRect = QRect(0, 0, geometry().width() + 4, geometry().height());
        // Get the state of the section
        QStyleOptionHeader opt;
        // Init State
        QStyle::State state = QStyle::State_None;
        // Add Enabled
        state |= QStyle::State_Enabled;
        // Add State On
        state |= QStyle::State_On;

        // Check Active State
        if (active) {
            // Add Active
            state |= QStyle::State_Active;
            // Add State Sunken
            state |= QStyle::State_Sunken;
        } else {

        }

        // Check Selected
        if (selected) {

        } else {

        }

        // Add State Has Focus
        //state |= QStyle::State_HasFocus;
        // Add State Focus At Border
        //state |= QStyle::State_FocusAtBorder;
        // Add State Selected
        //state |= QStyle::State_Selected;
        // Set State
        opt.state |= state;
        // Set Rect
        opt.rect = currentRect;
        // Check Index
        if (index == 0) {
            // Set Position
            opt.position = QStyleOptionHeader::Beginning;
        } else {
            // Set Position
            opt.position = QStyleOptionHeader::Middle;
        }
        //opt.position = QStyleOptionHeader::OnlyOneSection;
        //opt.position = QStyleOptionHeader::Middle;
        // Draw Control
        style()->drawControl(QStyle::CE_Header, &opt, &painter, this);
/*
        painter.setBrush(Qt::red);
        painter.drawRect(currentRect);
*/
        // Restore Painter
        painter.restore();
    }
}

//==============================================================================
// Resize Event
//==============================================================================
void HeaderItem::resizeEvent(QResizeEvent* aEvent)
{
    //QFrame::resizeEvent(aEvent);

    // Check Event
    if (aEvent) {
        //qDebug() << "HeaderItem::resizeEvent - index: " << index << " - size: " << size();
        // Get Delta
        int delta = (aEvent->oldSize().width() < 0) ? aEvent->size().width() : aEvent->size().width() - aEvent->oldSize().width();
        // Check Delta
        if (delta) {
            // Emit Size Changed Signal
            emit sizeChanged(index, aEvent->size().width(), delta);
        }
    }
}

//==============================================================================
// Move Event
//==============================================================================
void HeaderItem::moveEvent(QMoveEvent* aEvent)
{
    //QFrame::moveEvent(aEvent);

    // Check Event
    if (aEvent) {
        //qDebug() << "HeaderItem::moveEvent - index: " << index << " - pos: " << pos();
        // Get Delta
        int delta = (aEvent->oldPos().x() < 0) ? aEvent->pos().x() : aEvent->pos().x() - aEvent->oldPos().x();
        // Check Delta
        if (delta) {
            // Emit Pos Changed Signal
            emit posChanged(index, delta);
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
HeaderItem::~HeaderItem()
{
    // Delete UI
    delete ui;
}

