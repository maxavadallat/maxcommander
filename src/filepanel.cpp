#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QDebug>

#include "mainwindow.h"
#include "filepanel.h"
#include "filelistmodel.h"
#include "filelistimageprovider.h"
#include "ui_filepanel.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
FilePanel::FilePanel(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::FilePanel)
    , currentDir(QDir::homePath())
    , panelName("")
    , panelHasFocus(false)
    , fileListModel(NULL)
    , fileListImageProvider(NULL)
    , modifierKeys(Qt::NoModifier)
{
    // Setup UI
    ui->setupUi(this);

    // ...

    // Create File List Model
    fileListModel = new FileListModel();

    // Set Context Properties
    QQmlContext* ctx = ui->fileListWidget->rootContext();

    // Set Context Properties - Main Controller
    ctx->setContextProperty(DEFAULT_MAIN_CONTROLLER_NAME, this);
    // Set Context Properties - File List Model
    ctx->setContextProperty(DEFAULT_FILE_LIST_MODEL_NAME, fileListModel);

    // Get Engine
    QQmlEngine* engine = ui->fileListWidget->engine();

    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_FILE_ICON_PROVIDER_ID), new FileListImageProvider());

    // Set Resize Mode
    ui->fileListWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Set Source
    ui->fileListWidget->setSource(QUrl("qrc:/qml/FileList.qml"));


    // Connect Signals
    connect(ui->fileListWidget, SIGNAL(focusChanged(bool)), this, SLOT(setPanelFocus(bool)));

    // Update Available Space Label
    updateAvailableSpaceLabel();

}

//==============================================================================
// Get Current Dir
//==============================================================================
QString FilePanel::getCurrentDir()
{
    return currentDir;
}

//==============================================================================
// Set Current Dir
//==============================================================================
void FilePanel::setCurrentDir(const QString& aCurrentDir)
{
    // Check Current Dir
    if (currentDir != aCurrentDir) {
        // Init New Dir Info
        QFileInfo newDirInfo(aCurrentDir);
        // Check New Dir Info
        if (newDirInfo.exists() && (newDirInfo.isDir() || newDirInfo.isBundle())) {
            // Set Current Dir
            currentDir = aCurrentDir;


            // ...

            // Emit Current dir Changed Signal
            emit currentDirChanged(currentDir);
        }
    }
}

//==============================================================================
// Get Panel Name
//==============================================================================
QString FilePanel::getPanelName()
{
    return panelName;
}

//==============================================================================
// Get Panel Focus
//==============================================================================
bool FilePanel::getPanelFocus()
{
    return panelHasFocus;
}

//==============================================================================
// Set Panel Has Focus
//==============================================================================
void FilePanel::setPanelFocus(const bool& aFocus)
{
    // Check Panel Has Focus
    if (panelHasFocus != aFocus) {
        // Set Panel Has Focus
        panelHasFocus = aFocus;

        // Check Focus
        if (panelHasFocus) {
            // Set Style Sheet
            setStyleSheet(DEFAULT_PANEL_FOCUSED_STYLE_SHEET);
            // Set Style Sheet
            ui->currDirLabel->setStyleSheet(DEFAULT_PANEL_TRASPARENT_STYLE_SHEET);
            // Set Style Sheet
            ui->availableSpaceLabel->setStyleSheet(DEFAULT_PANEL_TRASPARENT_STYLE_SHEET);
        } else {
            // Set Style Sheet
            setStyleSheet("");
            // Set Style Sheet
            ui->currDirLabel->setStyleSheet("");
            // Set Style Sheet
            ui->availableSpaceLabel->setStyleSheet("");
        }

        // Emit Panel Focus Changed Signal
        emit panelFocusChanged(panelHasFocus);
    }
}

//==============================================================================
// Go To Home Directory
//==============================================================================
void FilePanel::gotoHome()
{

}

//==============================================================================
// Go To Root
//==============================================================================
void FilePanel::gotoRoot()
{

}

//==============================================================================
// Go To Drive
//==============================================================================
void FilePanel::gotoDrive(const int& aDriveIndex)
{
    Q_UNUSED(aDriveIndex);

    // ...
}

//==============================================================================
// Go Up To Parent Dir
//==============================================================================
void FilePanel::goUp()
{

}

//==============================================================================
// Go To Next Item
//==============================================================================
void FilePanel::goNext()
{

}

//==============================================================================
// Go To Prev Item
//==============================================================================
void FilePanel::goPrev()
{

}

//==============================================================================
// Go To Page Up
//==============================================================================
void FilePanel::pageUp()
{

}

//==============================================================================
// Go To Page Down
//==============================================================================
void FilePanel::pageDown()
{

}

//==============================================================================
// Go To Home
//==============================================================================
void FilePanel::goFirst()
{

}

//==============================================================================
// Go To End
//==============================================================================
void FilePanel::goLast()
{

}

//==============================================================================
// Clear
//==============================================================================
void FilePanel::clear()
{

}

//==============================================================================
// Restore UI
//==============================================================================
void FilePanel::restoreUI()
{

}

//==============================================================================
// Save Settings
//==============================================================================
void FilePanel::saveSettings()
{

}

//==============================================================================
// Update Available Space Label
//==============================================================================
void FilePanel::updateAvailableSpaceLabel()
{
    // Format Available Space Text
    QString availableSpace = QString(DEFAULT_AVAILABLE_SPACE_FORMAT_STRING).arg((getFreeSpace(currentDir) >> 10)).arg((getTotalSpace(currentDir) >> 10));

    // Set Text
    ui->availableSpaceLabel->setText(availableSpace);
}

//==============================================================================
// Home Button Clicked Slot
//==============================================================================
void FilePanel::on_homeButton_clicked()
{
    // Go To Home
    gotoHome();
}

//==============================================================================
// Root Button Clicked Slot
//==============================================================================
void FilePanel::on_rootButton_clicked()
{
    // Go To Root
    gotoRoot();
}

//==============================================================================
// Focus In Event
//==============================================================================
void FilePanel::focusInEvent(QFocusEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        //qDebug() << "FilePanel::focusInEvent - panel: " << panelName;

        // Set Style Sheet
        setStyleSheet(DEFAULT_PANEL_FOCUSED_STYLE_SHEET);
        // Set Style Sheet
        ui->currDirLabel->setStyleSheet(DEFAULT_PANEL_TRASPARENT_STYLE_SHEET);
        // Set Style Sheet
        ui->availableSpaceLabel->setStyleSheet(DEFAULT_PANEL_TRASPARENT_STYLE_SHEET);
    }
}

//==============================================================================
// Focus Out Event
//==============================================================================
void FilePanel::focusOutEvent(QFocusEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        //qDebug() << "FilePanel::focusOutEvent - panel: " << panelName;

        // Reset Style Sheet
        setStyleSheet("");
        // Set Style Sheet
        ui->currDirLabel->setStyleSheet("");
        // Set Style Sheet
        ui->availableSpaceLabel->setStyleSheet("");
    }
}

//==============================================================================
// Key Press Event
//==============================================================================
void FilePanel::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Tab:
                //qDebug() << "FilePanel::keyPressEvent - key: TAB";

            break;

            case Qt::Key_Up:
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Go Prev
                    goPrev();
                }
            break;

            case Qt::Key_Down:
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Go Next
                    goNext();
                }
            break;

            case Qt::Key_Left:
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Go Up
                    goUp();
                }
            break;

            case Qt::Key_Right:
            break;

            case Qt::Key_PageUp:
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Page Up
                    pageUp();
                }
            break;

            case Qt::Key_PageDown:
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Page Down
                    pageDown();
                }
            break;

            case Qt::Key_Home:
            break;

            case Qt::Key_End:
            break;

            case Qt::Key_Shift:
                //qDebug() << "FilePanel::keyPressEvent - key: SHIFT";
                // Check If Auto Repeat
                if (!aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Update Modifier Keys
                    modifierKeys |= Qt::ShiftModifier;
                    // Emit Modifier Keys Changed Signal
                    emit modifierKeysChanged(modifierKeys);
                }
            break;

            case Qt::Key_Control:
                //qDebug() << "FilePanel::keyPressEvent - key: CONTROL";
                // Check If Auto Repeat
                if (!aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Update Modifier Keys
                    modifierKeys |= Qt::ControlModifier;
                    // Emit Modifier Keys Changed Signal
                    emit modifierKeysChanged(modifierKeys);
                }
            break;

            case Qt::Key_AltGr:
            case Qt::Key_Alt:
                //qDebug() << "FilePanel::keyPressEvent - key: ALT";
                // Check If Auto Repeat
                if (!aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Update Modifier Keys
                    modifierKeys |= Qt::AltModifier;
                    // Emit Modifier Keys Changed Signal
                    emit modifierKeysChanged(modifierKeys);
                }
            break;

            case Qt::Key_Meta:
                //qDebug() << "FilePanel::keyPressEvent - key: META";
                // Check If Auto Repeat
                if (!aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Update Modifier Keys
                    modifierKeys |= Qt::MetaModifier;
                    // Emit Modifier Keys Changed Signal
                    emit modifierKeysChanged(modifierKeys);
                }
            break;

            case Qt::Key_F10:
                //qDebug() << "FilePanel::keyPressEvent - key: F10";
            break;

            default:
                qDebug() << "FilePanel::keyPressEvent - key: " << aEvent->key();
            break;
        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void FilePanel::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Tab:
                //qDebug() << "FilePanel::keyReleaseEvent - key: TAB";

            break;

            case Qt::Key_Up:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Go Prev
                    goPrev();
                }
            break;

            case Qt::Key_Down:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Go Next
                    goNext();
                }
            break;

            case Qt::Key_Left:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Go Up
                    goUp();
                }
            break;

            case Qt::Key_Right:

                // Check If Dir

            break;

            case Qt::Key_PageUp:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Page Up
                    pageUp();
                }
            break;

            case Qt::Key_PageDown:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Page Down
                    pageDown();
                }
            break;

            case Qt::Key_Home:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Go First
                    goFirst();
                }
            break;

            case Qt::Key_End:
                // Check If Accepted
                if (!aEvent->isAccepted()) {
                    // Accept
                    aEvent->accept();
                    // Go Last
                    goLast();
                }
            break;

            case Qt::Key_Shift:
                //qDebug() << "FilePanel::keyReleaseEvent - key: SHIFT";
                // Update Modifier Keys
                modifierKeys ^= Qt::ShiftModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            break;

            case Qt::Key_Control:
                //qDebug() << "FilePanel::keyReleaseEvent - key: CONTROL";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::ControlModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            break;

            case Qt::Key_AltGr:
            case Qt::Key_Alt:
                //qDebug() << "FilePanel::keyReleaseEvent - key: ALT";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::AltModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            break;

            case Qt::Key_Meta:
                //qDebug() << "FilePanel::keyReleaseEvent - key: META";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::MetaModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            break;

            case Qt::Key_F10:
                //qDebug() << "FilePanel::keyReleaseEvent - key: F10";
                // Emit Exit Key Released Signal
                emit exitKeyReleased();
            break;

            default:
                qDebug() << "FilePanel::keyReleaseEvent - key: " << aEvent->key();
            break;
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
FilePanel::~FilePanel()
{
    // Save Settings
    saveSettings();

    delete ui;

    // Check File List Model
    if (fileListModel) {
        // Delete File List Model
        delete fileListModel;
        fileListModel = NULL;
    }

    // Check File List Image Provider
    if (fileListImageProvider) {
        // Delete File List Image Provider
        delete fileListImageProvider;
        fileListImageProvider = NULL;
    }
}

