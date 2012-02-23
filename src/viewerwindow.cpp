
// INCLUDES

#include <QFileInfo>
#include <QUrl>
#include <QDebug>

#include "constants.h"
#include "viewerwindow.h"
#include "ui_viewerwindow.h"
#include "settings.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ViewerWindow::ViewerWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::ViewerWindow)
    , fileName("")
    , settings(NULL)
    , wordWrap(false)
    , searchTerm("")
    , caseSensitive(false)
    , wholeWord(false)
    , searchDirection(true)
    , searchWrapAround(true)
{
    // Setup UI
    ui->setupUi(this);

    // Get Settings
    settings = Settings::getInstance();

    // Load Settings
    loadSettings();

    qDebug() << "Creating ViewerWindow...done";
}

//==============================================================================
// Load File
//==============================================================================
bool ViewerWindow::loadFile(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);
    // Check File Path
    if (fileInfo.exists() && !fileInfo.isDir() && fileInfo.size() <= DEFAULT_VIEWER_MAX_FILE_SIZE && fileName != aFilePath && ui && ui->viewer) {
        // Set File Name
        fileName = aFilePath;
/*
        // Check Document
        if (ui->viewer->document()) {

            // Set Document
            ui->viewer->document()->setHtml(QUrl::fromLocalFile(fileName).toString());

        } else {
            qDebug() << "ViewerWindow::loadFile - NO VIEWER DOCUMENT";

            return false;
        }
*/
        // Init Text File
        QFile textFile(fileName);
        // Open File
        if (textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Init Text Stream
            QTextStream inStream(&textFile);
            // Set Plain Text
            ui->viewer->setPlainText(inStream.readAll());
            // Close File
            textFile.close();
            // Show
            show();
            // Return True
            return true;
        }

    } else {
        qDebug() << "ViewerWindow::loadFile - The Shit Hit The Fan...";
    }

    return false;
}

//==============================================================================
// Close
//==============================================================================
void ViewerWindow::closeViewer()
{
    // Check UI
    if (ui && ui->viewer)  {
        // Clear
        ui->viewer->clear();
    }

    // Reset File Name
    fileName = QString("");

    // Close
    close();
}

//==============================================================================
// Print File
//==============================================================================
void ViewerWindow::printFile()
{
    // ...
}

//==============================================================================
// Set Word Wrap
//==============================================================================
void ViewerWindow::setWordWrap(const bool& aWordWrap)
{
    // Check Word Wrap
    if (wordWrap != aWordWrap) {
        // Set Word Wrap
        wordWrap = aWordWrap;

        // Check UI
        if (ui && ui->viewer) {
            // Set Word Wrap Mode
            ui->viewer->setWordWrapMode(wordWrap ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
        }

        // Check Settngs
        if (settings) {
            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_VIEWER));
            // Set Value
            settings->setValue(QString(SETTINGS_KEY_VIEWER_WORD_WRAP), wordWrap);
        }
    }
}

//==============================================================================
// Find Text
//==============================================================================
void ViewerWindow::findText(const QString& aSearchTerm, const bool& aCaseSensitive, const bool& aWholeWord, const bool& aDirection, const bool& aWrapAround)
{
    // Check Search Term
    if (!aSearchTerm.isEmpty()) {
        // Set Search Term
        searchTerm = aSearchTerm;
        // Set Case Sensitve
        caseSensitive = aCaseSensitive;
        // Set Whole Word
        wholeWord = aWholeWord;
        // Set Direction
        searchDirection = aDirection;
        // Set Wrap Around
        searchWrapAround = aWrapAround;

        // Check Settings
        if (settings) {
            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_VIEWER));
            // Set Case Sensitive Value
            settings->setValue(QString(SETTINGS_KEY_VIEWER_CASE_SENSITIVE), caseSensitive);
            // Set Whole Word Value
            settings->setValue(QString(SETTINGS_KEY_VIEWER_WHOLE_WORD), wholeWord);
        }

        // Find Text

        // Select

        // Scroll To

    }
}

//==============================================================================
// Find Next Matching Text
//==============================================================================
void ViewerWindow::findNext()
{
    // Check Search Term
    if (!searchTerm.isEmpty()) {

        // Find Next

        // Select

        // Scroll To

    }
}

//==============================================================================
// Load Settings
//==============================================================================
void ViewerWindow::loadSettings()
{
    // Check Settings
    if (settings) {
        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_VIEWER));
        // Set Value
        wordWrap = settings->getValue(QString(SETTINGS_KEY_VIEWER_WORD_WRAP), false).toBool();
        // Get Case Sensitive Search
        caseSensitive = settings->getValue(QString(SETTINGS_KEY_VIEWER_CASE_SENSITIVE), false).toBool();
        // Get Whole Word Search
        wholeWord = settings->getValue(QString(SETTINGS_KEY_VIEWER_WHOLE_WORD), false).toBool();

        // ...
    }

    // Check UI
    if (ui && ui->viewer) {
        // Set Word Wrap Mode
        ui->viewer->setWordWrapMode(wordWrap ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
    }

    // Check UI
    if (ui && ui->actionWord_Wrap) {
        // Set Checked
        ui->actionWord_Wrap->setChecked(wordWrap);
    }
}

//==============================================================================
// Save Settings
//==============================================================================
void ViewerWindow::saveSettings()
{
    // Check Settings
    if (settings) {
        // Save Settings
        settings->save();
    }
}

//==============================================================================
// Hide Event
//==============================================================================
void ViewerWindow::hideEvent(QHideEvent* aHideEvent)
{
    // Check Event
    if (aHideEvent) {
        qDebug() << "ViewerWindow::hideEvent";

        // Check UI
        if (ui && ui->viewer) {
            // Clear
            ui->viewer->clear();
        }

        // Reset File Name
        fileName = QString("");
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void ViewerWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent)  {
        qDebug() << "ViewerWindow::keyReleaseEvent";
        // Check Key
        if (aEvent->key() == Qt::Key_Escape) {
            // Close Viewer
            closeViewer();
        }
    }
}

//==============================================================================
// Action Word Wrap Triggered Slot
//==============================================================================
void ViewerWindow::on_actionWord_Wrap_triggered()
{
    qDebug() << "ViewerWindow::on_actionWord_Wrap_triggered";

    // Toggle Word Wrap
    setWordWrap(!wordWrap);

    // Check UI
    if (ui && ui->actionWord_Wrap) {
        // Set Checked
        ui->actionWord_Wrap->setChecked(wordWrap);
    }
}

//==============================================================================
// Action Font Triggered Slot
//==============================================================================
void ViewerWindow::on_actionFont_triggered()
{
    qDebug() << "ViewerWindow::on_actionFont_triggered";

}

//==============================================================================
// Action Find Triggered Slot
//==============================================================================
void ViewerWindow::on_actionFind_triggered()
{
    qDebug() << "ViewerWindow::on_actionFind_triggered";

}

//==============================================================================
// Action Find Next Triggered Slot
//==============================================================================
void ViewerWindow::on_actionFind_Next_triggered()
{
    qDebug() << "ViewerWindow::on_actionFind_Next_triggered";

}

//==============================================================================
// Action Open Triggered Slot
//==============================================================================
void ViewerWindow::on_actionOpen_triggered()
{
    qDebug() << "ViewerWindow::on_actionOpen_triggered";

}

//==============================================================================
// Action Print Triggered Slot
//==============================================================================
void ViewerWindow::on_actionPrint_triggered()
{
    qDebug() << "ViewerWindow::on_actionPrint_triggered";

}

//==============================================================================
// Action Close Triggered Slot
//==============================================================================
void ViewerWindow::on_actionClose_triggered()
{
    qDebug() << "ViewerWindow::on_actionClose_triggered";
    // Close Viewer
    closeViewer();
}

//==============================================================================
// Destructor
//==============================================================================
ViewerWindow::~ViewerWindow()
{
    // Delete UI
    delete ui;

    // Check Settings
    if (settings) {
        // Release Settings Instance
        settings->release();
        settings = NULL;
    }
}

