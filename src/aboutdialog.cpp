
// INCLUDES

#include <QPainter>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "constants.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
AboutDialog::AboutDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::AboutDialog)
{
    // Set Window Flags
    setWindowFlags(Qt::Popup);
    // Set Window Attributes
    setAttribute(Qt::WA_NoSystemBackground);

    // Setup UI
    ui->setupUi(this);

    // Set Up Background Color
    bgColor = QColor::fromRgba(DEFAULT_DIALOG_BACKGROUND_COLOR);
}

//==============================================================================
// Paint Eventr
//==============================================================================
void AboutDialog::paintEvent(QPaintEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Init Painter
        QPainter painter(this);
        // Save Painter
        painter.save();

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);

        // Set Pen & Brush
        painter.setPen(QPen(Qt::black, DEFAULT_DIALOG_BORDER_WIDTH));
        painter.setBrush(bgColor);

        int halfPenWidth = DEFAULT_DIALOG_BORDER_WIDTH / 2;

        // Draw Background
        painter.drawRoundedRect(rect().adjusted(halfPenWidth, halfPenWidth, -halfPenWidth, -halfPenWidth), DEFAULT_DIALOG_BACKGROUND_RADIUS, DEFAULT_DIALOG_BACKGROUND_RADIUS);

        // Restore Painter
        painter.restore();
    }
}

//==============================================================================
// Destructor
//==============================================================================
AboutDialog::~AboutDialog()
{
    // Delete UI
    delete ui;
}


