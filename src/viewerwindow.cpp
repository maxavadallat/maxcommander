#include <QSettings>
#include <QDebug>

#include "viewerwindow.h"
#include "ui_viewerwindow.h"

//==============================================================================
// Constructor
//==============================================================================
ViewerWindow::ViewerWindow(QWidget* aParent)
    : QWidget(aParent)
    , ui(new Ui::ViewerWindow)
    , fileName("")
    , editMode(false)
    , dirty(false)
{
    qDebug() << "ViewerWindow::ViewerWindow";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void ViewerWindow::init()
{
    qDebug() << "ViewerWindow::init";

    // Set Read Only Mode
    ui->textEdit->setReadOnly(!editMode);

    // ...
}

//==============================================================================
// Load File
//==============================================================================
void ViewerWindow::loadFile(const QString& aFileName)
{
    qDebug() << "ViewerWindow::loadFile - aFileName: " << aFileName;

}

//==============================================================================
// Save File As
//==============================================================================
void ViewerWindow::saveFileAs(const QString& aFileName)
{
    qDebug() << "ViewerWindow::saveFileAs - aFileName: " << aFileName;

}

//==============================================================================
// Show Window
//==============================================================================
void ViewerWindow::showWindow()
{
    qDebug() << "ViewerWindow::showWindow";

    // Restore UI
    restoreUI();

    // Show
    show();
}

//==============================================================================
// Close
//==============================================================================
void ViewerWindow::closeWindow()
{
    qDebug() << "ViewerWindow::closeWindow";

    // Save Settings
    saveSettings();

    // Hide
    hide();
}

//==============================================================================
// Set Edit Mode Enabled
//==============================================================================
void ViewerWindow::setEditModeEnabled(const bool& aEnabled)
{
    // Check Edit Mode
    if (editMode != aEnabled) {
        qDebug() << "ViewerWindow::setEditModeEnabled - aEnabled: " << aEnabled;
        // Set Edit Mode
        editMode = aEnabled;

        // Set Read Only Mode
        ui->textEdit->setReadOnly(!editMode);
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void ViewerWindow::restoreUI()
{
    qDebug() << "ViewerWindow::restoreUI";

    // Init Settings
    QSettings settings;

    // Check If Settings Have Key

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void ViewerWindow::saveSettings()
{
    qDebug() << "ViewerWindow::saveSettings";

    // Init Settings
    QSettings settings;

    // ...
}

//==============================================================================
// Close Event
//==============================================================================
void ViewerWindow::closeEvent(QCloseEvent* aEvent)
{
    // Check Event
    if (aEvent) {

        // Check Dirty Flag
        if (dirty) {
            qDebug() << "ViewerWindow::​closeEvent - IGNORE!";

            // Ignore Event
            aEvent->ignore();


        } else {
            qDebug() << "ViewerWindow::​closeEvent";

            // Accept Event
            aEvent->accept();
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
ViewerWindow::~ViewerWindow()
{
    // Save Settings
    saveSettings();

    // Delete UI
    delete ui;

    qDebug() << "ViewerWindow::~ViewerWindow";
}
