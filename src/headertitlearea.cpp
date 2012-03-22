
// INCLUDES

#include "headertitlearea.h"
#include "ui_headertitlearea.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
HeaderTitleArea::HeaderTitleArea(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::HeaderTitleArea)
    , selected(false)
    , reverse(false)
    , pressed(false)
    , rightPressed(false)
{
    // Set Up UI
    ui->setupUi(this);
    // Update Elements
    updateElements();
}

//==============================================================================
// Set Title
//==============================================================================
void HeaderTitleArea::setTitle(const QString& aTitle)
{
    // Check Title Label
    if (ui->titleLabel) {
        //qDebug() << "HeaderTitleArea::setTitle - aTitle: " << aTitle;
        // Set Text
        ui->titleLabel->setText(aTitle);
    }
}

//==============================================================================
// Get Title
//==============================================================================
QString HeaderTitleArea::getTitle()
{
    // Check Title Label
    if (ui->titleLabel) {
        return ui->titleLabel->text();
    }

    return QString("");
}

//==============================================================================
// Set Title Alignment
//==============================================================================
void HeaderTitleArea::setTitleAlignment(const Qt::Alignment& aAlignment)
{
    // Check Title Label
    if (ui->titleLabel) {
        // Set Title Label Alignment
        ui->titleLabel->setAlignment(aAlignment);
    }
}

//==============================================================================
// Set Selected
//==============================================================================
void HeaderTitleArea::setSelected(const bool& aSelected, const bool& aActivation, const bool& aRefresh)
{
    // Check Selected
    if (selected != aSelected) {
        //qDebug() << "HeaderTitleArea::setSelected - aSelected: " << aSelected;
        // Set Selected
        selected = aSelected;
        // Update Elements
        updateElements();
        // Check Refresh
        if (aRefresh) {
            // Update
            update();
        }
        // Emit Selection Changed Signal
        emit titleSelected(aActivation, aRefresh);
    }
}

//==============================================================================
// Get Selected
//==============================================================================
bool HeaderTitleArea::getSelected()
{
    return selected;
}

//==============================================================================
// Set Reversed
//==============================================================================
void HeaderTitleArea::setReversed(const bool& aReversed, const bool& aActivation, const bool& aRefresh)
{
    // Check Selected State
    if (selected) {
        // Check Reversed State
        if (reverse != aReversed) {
            qDebug() << "HeaderTitleArea::setReversed - aReversed: " << aReversed;
            // Set Reverse State
            reverse = aReversed;
            // Update Elements
            updateElements();
            // Check Update
            if (aRefresh) {
                // Update
                update();
            }
            // Emit Order Changed Signal
            emit orderChanged(aActivation, aRefresh);
        }
    }
}

//==============================================================================
// Get Reversed
//==============================================================================
bool HeaderTitleArea::getReversed()
{
    return reverse;
}

//==============================================================================
// Update Elements
//==============================================================================
void HeaderTitleArea::updateElements()
{
    // Check Selected
    if (selected) {
        // Set Sort Direction Indicator Visibility
        ui->iconLabel->setVisible(true);
        // Check Reversed State
        if (reverse) {
            // Set Sort Direction Indicator
            ui->iconLabel->setPixmap(QString(":downArrow"));
        } else {
            // Set Sort Direction Indicator
            ui->iconLabel->setPixmap(QString(":upArrow"));
        }
    } else {
        // Set Sort Direction Indicator Visibility
        ui->iconLabel->setVisible(false);
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void HeaderTitleArea::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Mouse Button
        if (aEvent->button() == Qt::LeftButton) {
            // Check Pressed
            if (!pressed) {
                // Set Pressed
                pressed = true;

                //qDebug() << "HeaderTitleArea::mousePressEvent - left";

                // ...
            }
        // Check Mose Button
        } else if (aEvent->button() == Qt::RightButton) {
            // Chck Right Pressed
            if (!rightPressed) {
                // Set Right Pressed
                rightPressed = true;

                //qDebug() << "HeaderTitleArea::mousePressEvent - right";

                // ...
            }
        }
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void HeaderTitleArea::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed
        if (pressed) {
            // Set Pressed
            pressed = false;
            //qDebug() << "HeaderTitleArea::mouseReleaseEvent - left";

            // Check Selected State
            if (selected) {
                // Set Reversed State
                setReversed(!reverse, true);
            } else {
                // Set Selected
                setSelected(true, true);
            }
        } else if (rightPressed) {
            // Reset Right Pressed
            rightPressed = false;
            //qDebug() << "HeaderTitleArea::mouseReleaseEvent - right";

            // ...

            // Emit Right Button Released Signal
            emit rightButtonReleased(aEvent->globalPos());
        }
    }
}

//==============================================================================
// Mouse Move Event
//==============================================================================
void HeaderTitleArea::mouseMoveEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Pressed
        if (pressed) {
            //qDebug() << "HeaderTitleArea::mouseMoveEvent";

            // ...
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
HeaderTitleArea::~HeaderTitleArea()
{
    delete ui;
}
