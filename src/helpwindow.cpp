#include <QSettings>
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
    // Set URL
    ui->webView->setUrl(aURL);
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
// Destructor
//==============================================================================
HelpWindow::~HelpWindow()
{
    // Save Settings
    saveSettings();

    // Delete UI
    delete ui;
}
