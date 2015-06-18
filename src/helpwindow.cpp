#include <QSettings>
#include <QKeyEvent>
#include <QDebug>

#include "helpwindow.h"
#include "ui_helpwindow.h"

//==============================================================================
// Constructor
//==============================================================================
HelpWindow::HelpWindow(QWidget* aParent)
    : QWidget(aParent)
    , ui(new Ui::HelpWindow)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Load Content
//==============================================================================
void HelpWindow::loadContent(const QUrl& aURL)
{
    // Check URL
    if (!aURL.isEmpty()) {
        // Set URL
        ui->webView->load(aURL);
    }
}

//==============================================================================
// Show Window
//==============================================================================
void HelpWindow::showWindow()
{
    // Restore UI
    restoreUI();
    // Show
    show();
}

//==============================================================================
// Close
//==============================================================================
void HelpWindow::closeWindow()
{
    // Save Settings
    saveSettings();
    // Close
    close();
}

//==============================================================================
// Restore UI
//==============================================================================
void HelpWindow::restoreUI()
{
    // Init Settings
    QSettings settings;

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void HelpWindow::saveSettings()
{
    // Init Settings
    QSettings settings;

    // ...
}

//==============================================================================
// Key Press Event
//==============================================================================
void HelpWindow::keyPressEvent(QKeyEvent* aEvent)
{
    QWidget::keyPressEvent(aEvent);

    // ...
}

//==============================================================================
// Key Release Event
//==============================================================================
void HelpWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    QWidget::keyReleaseEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Close
                close();
            break;

            default:
            break;
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
HelpWindow::~HelpWindow()
{
    // Save Settings
    saveSettings();

    // Delete UI
    delete ui;
}
