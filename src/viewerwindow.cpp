#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "viewerwindow.h"
#include "ui_viewerwindow.h"
#include "confirmdialog.h"
#include "remotefileutilclient.h"

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

    // Connect Signal
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));

    // ...
}

//==============================================================================
// Load File
//==============================================================================
void ViewerWindow::loadFile(const QString& aFileName)
{
    qDebug() << "ViewerWindow::loadFile - aFileName: " << aFileName;

    // Init File
    QFile file(aFileName);

    // Open File
    if (file.open(QIODevice::ReadOnly)) {
        // Init Text Stream
        QTextStream textStream(&file);

        // Load From File
        ui->textEdit->setText(textStream.readAll());

        // Reset Dirty Flag
        dirty = false;

        // Close File
        file.close();
    }

    // ...
}

//==============================================================================
// Save File As
//==============================================================================
void ViewerWindow::saveFileAs(const QString& aFileName)
{
    qDebug() << "ViewerWindow::saveFileAs - aFileName: " << aFileName;

    // Save To File

    // ...
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

    // Set Word Wrap Mode
    ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

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
// Text Changed Slot
//==============================================================================
void ViewerWindow::textChanged()
{
    qDebug() << "#### ViewerWindow::textChanged";

    // Mark Dirty
    dirty = true;
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

            // Ask For Save

            // Ignore Event
            aEvent->ignore();

        } else {
            qDebug() << "ViewerWindow::​closeEvent";

            // Accept Event
            aEvent->accept();

            // Emit Viewer Closed Signal
            emit viewerClosed(this);
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
