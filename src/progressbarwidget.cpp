#include <QSettings>
#include <QDebug>

#include "progressbarwidget.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
ProgressBarWidget::ProgressBarWidget(QWidget* aParent)
    : QWidget(aParent)
    , value(0)
    , minimum(0)
    , maximum(0)
    , scale(0)
    , sValue(0)
    , sMin(0)
    , sMax(0)
    , progressWidth(0)
{
    // Init Settings
    QSettings settings;
    // Load Color
    color = QColor(string2Rgb(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_BG).toString()));
}

//==============================================================================
// Get Value
//==============================================================================
qint64 ProgressBarWidget::getValue()
{
    return value;
}

//==============================================================================
// Set Value
//==============================================================================
void ProgressBarWidget::setValue(const qint64& aValue)
{
    // Check Value
    if (value != aValue) {
        // Set Value
        value = aValue;
        // Update Scaled Values
        updateScaledValues();
        // Update
        update();
        // Emit Value Changed Signal
        emit valueChanged(value);
    }
}

//==============================================================================
// Get Minimum
//==============================================================================
qint64 ProgressBarWidget::getMinimum()
{
    return minimum;
}

//==============================================================================
// Set Minimum
//==============================================================================
void ProgressBarWidget::setMinimum(const qint64& aMinimum)
{
    // Check Min Value
    if (minimum != aMinimum) {
        // Set Min Value
        minimum = aMinimum;
        // Update Scaled Values
        updateScaledValues();
        // Update
        update();
        // Emit Minimum Changed Signal
        emit minimumChanged(minimum);
    }
}

//==============================================================================
// Get Maximum
//==============================================================================
qint64 ProgressBarWidget::getMaximum()
{
    return maximum;
}

//==============================================================================
// Set Maximum
//==============================================================================
void ProgressBarWidget::setMaximum(const qint64& aMaximum)
{
    // Check Max Value
    if (maximum != aMaximum) {
        // Set Max Value
        maximum = aMaximum;

        // Calculate Scale
        calculateScale();

        // Update Scaled Values
        updateScaledValues();

        // Update
        update();
        // Emit Maximum Changed Signal
        emit maximumChanged(minimum);
    }
}

//==============================================================================
// Calculate Scale
//==============================================================================
void ProgressBarWidget::calculateScale()
{
    // Reset Current Progress Scale
    scale = 0;

    // Loop While Scale is OK
    while ((maximum >> scale) > INT_MAX) {
        // In Current Progress Scale
        scale++;
    }

    //qDebug() << "ProgressBarWidget::calculateScale - scale: " << scale;
}

//==============================================================================
// Update Scaled Values
//==============================================================================
void ProgressBarWidget::updateScaledValues()
{
    // Get Scaled Value
    sValue  = value >> scale;
    // Get Scaled Minimum Value
    sMin    = minimum >> scale;
    // Get Scaled MAximum Value
    sMax    = maximum >> scale;

    // Get Widget Width
    int widgetWidth = rect().width();

    // Calculate Progress Width
    progressWidth = (qint64)widgetWidth * (sValue - sMin) / (sMax - sMin);

    //qDebug() << "ProgressBarWidget::updateScaledValues - progressWidth: " << progressWidth;
}

//==============================================================================
// Paint Event
//==============================================================================
void ProgressBarWidget::paintEvent(QPaintEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Init Painter
        QPainter painter(this);
        // Set Pen
        painter.setPen(Qt::NoPen);
        // Set Brush
        painter.setBrush(color);
        // Draw Rect
        painter.drawRect(0, 0, progressWidth, rect().height());
    }
}

//==============================================================================
// Destructor
//==============================================================================
ProgressBarWidget::~ProgressBarWidget()
{
}

