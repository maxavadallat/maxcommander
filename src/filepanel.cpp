#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QImageReader>
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
    , currentDir("")
    , panelName("")
    , panelHasFocus(false)
    , fileListModel(NULL)
    , fileListImageProvider(NULL)
    , modifierKeys(Qt::NoModifier)
    , currentIndex(-1)
    , visualItemsCount(-1)
    , lastDirName("")
    , lastIndex(-1)
    , showHidden(true)
{
    // Setup UI
    ui->setupUi(this);

    // Get Supported Image Formats Bye Array
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    // Get Count
    int flCount = formats.count();
    // Go Thru Formats
    for (int i=0; i<flCount; ++i) {
        // Add Format String
        supportedImageFormats << QString(formats[i]);
    }

    // ...

    // Create File List Model
    fileListModel = new FileListModel();

    // Connect Signals
    connect(fileListModel, SIGNAL(dirFetchFinished()), this, SLOT(fileModelDirFetchFinished()));

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
    //updateAvailableSpaceLabel();
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

        // Check If Exists
        if (!newDirInfo.exists()) {

            // ...

            return;
        }

        // Check If Is Dir
        if (!(newDirInfo.isDir() || newDirInfo.isBundle())) {

            // ...

            return;
        }

        // Check Access
        if (!(newDirInfo.permissions() & QFileDevice::ReadUser)) {

            // ...

            return;
        }

        // Set Current Dir
        currentDir = aCurrentDir;

        // Check Model
        if (fileListModel) {
            // Set Current Dir
            fileListModel->setCurrentDir(currentDir);
        }

        // ...

        // Set Text
        ui->currDirLabel->setText(currentDir);

        // Update Available Space Label
        //updateAvailableSpaceLabel();

        // Emit Current dir Changed Signal
        emit currentDirChanged(currentDir);
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
// Set Panel Name
//==============================================================================
void FilePanel::setPanelName(const QString& aPanelName)
{
    // Check Panel Name
    if (panelName != aPanelName) {
        // Set Panel Name
        panelName = aPanelName;

        // ...
    }
}

//==============================================================================
// Get Panel Focus
//==============================================================================
bool FilePanel::getPanelFocus()
{
    return panelHasFocus;
}

//==============================================================================
// Get Current Index
//==============================================================================
int FilePanel::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Get Visual Items Count
//==============================================================================
int FilePanel::getvisualItemsCount()
{
    return visualItemsCount;
}

//==============================================================================
// Get Show Hidden
//==============================================================================
bool FilePanel::getShowHidden()
{
    return showHidden;
}

//==============================================================================
// Set Show Hidden
//==============================================================================
void FilePanel::setShowHidden(const bool& aHidden)
{
    // Check Show Hidden
    if (showHidden != aHidden) {
        qDebug() << "FilePanel::setShowHidden - panelName: " << panelName << " - aHidden: " << aHidden;
        // Set Show Hidden
        showHidden = aHidden;

        // Init Settings
        QSettings settings;
        // Set Value
        settings.setValue(SETTINGS_KEY_SHOW_HIDDEN_FILES, showHidden);
        // Sync
        settings.sync();

        // Reload
        reload(0);

        // Emit Show Hidden Changed Signal
        emit showHiddenChanged(showHidden);
    }
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

            // Emit Focused Panle Has Changed
            emit focusedPanelChanged(this);

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
// Set Current Index
//==============================================================================
void FilePanel::setCurrentIndex(const int& aCurrentIndex)
{
    // Check Current Index
    if (currentIndex != aCurrentIndex) {
        // Set Current Index
        currentIndex = qBound(0, aCurrentIndex, fileListModel ? fileListModel->rowCount()-1 : 0);

        // Emit Current Index Changed Signal
        emit currentIndexChanged(currentIndex);
    }
}

//==============================================================================
// Set Visual Items Count
//==============================================================================
void FilePanel::setVisualItemsCount(const int& aVisualCount)
{
    // Check Current Index
    if (visualItemsCount != aVisualCount) {
        // Set Current Index
        visualItemsCount = aVisualCount;

        // Emit Visual Items Count Changed Signal
        emit visualItemsCountChanged(visualItemsCount);
    }
}

//==============================================================================
// Get Supported Image Formats
//==============================================================================
QStringList FilePanel::getSupportedImageFormats()
{
    return supportedImageFormats;
}

//==============================================================================
// Reload
//==============================================================================
void FilePanel::reload(const int& aIndex)
{
    // Get Last Index
    lastIndex = aIndex;

    // Check File List Model
    if (fileListModel) {
        //qDebug() << "FilePanel::reload - panelName: " << panelName;
        // Reload
        fileListModel->reload();
    }
}

//==============================================================================
// Go To Home Directory
//==============================================================================
void FilePanel::gotoHome()
{
    //qDebug() << "FilePanel::gotoHome - panelName: " << panelName;

    // Set Current Dir
    setCurrentDir(QDir::homePath());
    // Set Current Index
    setCurrentIndex(0);
}

//==============================================================================
// Go To Root
//==============================================================================
void FilePanel::gotoRoot()
{
    //qDebug() << "FilePanel::gotoRoot - panelName: " << panelName;

    // Set Current Dir
    setCurrentDir("/");
    // Set Current Index
    setCurrentIndex(0);
}

//==============================================================================
// Go To Drive
//==============================================================================
void FilePanel::gotoDrive(const int& aDriveIndex)
{
    qDebug() << "FilePanel::goUp - panelName: " << panelName << " - aDriveIndex: " << aDriveIndex;

    // ...
}

//==============================================================================
// Go Up To Parent Dir
//==============================================================================
void FilePanel::goUp()
{
    // Get Parent Dir
    QString parentDir = getParentDir(currentDir);

    // Check Parent Dir
    if (!parentDir.isEmpty()) {
        // Get Last Dir Name to Jump
        lastDirName = getDirName(currentDir);

        //qDebug() << "FilePanel::goUp - panelName: " << panelName << " - parentDir: " << parentDir;

        // Set Current Dir
        setCurrentDir(parentDir);
    }
}

//==============================================================================
// Go To Next Item
//==============================================================================
void FilePanel::goNext()
{
    //qDebug() << "FilePanel::goNext - panelName: " << panelName;

    // Set Current Index
    setCurrentIndex(currentIndex + 1);
}

//==============================================================================
// Go To Prev Item
//==============================================================================
void FilePanel::goPrev()
{
    //qDebug() << "FilePanel::goPrev - panelName: " << panelName;

    // Set Current Index
    setCurrentIndex(currentIndex - 1);
}

//==============================================================================
// Go To Page Up
//==============================================================================
void FilePanel::pageUp()
{
    //qDebug() << "FilePanel::pageUp - panelName: " << panelName;

    // Set Current Index
    setCurrentIndex(currentIndex - visualItemsCount);
}

//==============================================================================
// Go To Page Down
//==============================================================================
void FilePanel::pageDown()
{
    //qDebug() << "FilePanel::pageDown - panelName: " << panelName;

    // Set Current Index
    setCurrentIndex(currentIndex + visualItemsCount);
}

//==============================================================================
// Go To Home
//==============================================================================
void FilePanel::goFirst()
{
    //qDebug() << "FilePanel::goFirst - panelName: " << panelName;

    // Set Current Index
    setCurrentIndex(0);
}

//==============================================================================
// Go To End
//==============================================================================
void FilePanel::goLast()
{
    //qDebug() << "FilePanel::goLast - panelName: " << panelName;

    // Set Current Index
    setCurrentIndex(fileListModel ? fileListModel->rowCount()-1 : 0);
}

//==============================================================================
// Handle Item Select
//==============================================================================
void FilePanel::handleItemSelect()
{
    // Check File List Model
    if (!fileListModel) {
        qWarning() << "FilePanel::handleItemSelect - panelName: " << panelName << " - NO FILE LIST MODEL!!";
        return;
    }

    //qDebug() << "FilePanel::handleItemSelect - panelName: " << panelName;

    // Get File Info
    QFileInfo fileInfo = fileListModel->getFileInfo(currentIndex);

    // Check If Dir
    if (fileInfo.isDir() || fileInfo.isBundle()) {
        // Check File Name
        if (fileInfo.fileName() == QString("..")) {
            // Go Up
            goUp();
        } else {
            // Set Current Dir
            setCurrentDir(fileInfo.absoluteFilePath());
            // Set Current Index
            setCurrentIndex(0);
        }
    } else {

        // Handle Files

    }

    // ...
}

//==============================================================================
// Rename
//==============================================================================
void FilePanel::renameCurrent()
{
    qDebug() << "FilePanel::renameCurrent - panelName: " << panelName;

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void FilePanel::clear()
{
    //qDebug() << "FilePanel::clear - panelName: " << panelName;

    // Check File List Model
    if (fileListModel) {
        // Clear
        fileListModel->clear();
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void FilePanel::restoreUI()
{
    qDebug() << "FilePanel::restoreUI - panelName: " << panelName;

    // Init Settings
    QSettings settings;

    // Get Saved Current Dir
    QString savedDir = settings.value(panelName + SETTINGS_KEY_PANEL_DIR, QDir::homePath()).toString();

    // Get Show Hidden
    showHidden = settings.value(SETTINGS_KEY_SHOW_HIDDEN_FILES, true).toBool();

    // Set Current Dir
    setCurrentDir(savedDir);

    // ...

}

//==============================================================================
// Save Settings
//==============================================================================
void FilePanel::saveSettings()
{
    qDebug() << "FilePanel::saveSettings - panelName: " << panelName;

    // Init Settings
    QSettings settings;

    // Set Value - Current Dir
    settings.setValue(panelName + SETTINGS_KEY_PANEL_DIR, currentDir);
    // Set Value - Show Hidden
    settings.setValue(SETTINGS_KEY_SHOW_HIDDEN_FILES, showHidden);

    // ...

    // Sync
    settings.sync();
}

//==============================================================================
// Update Available Space Label
//==============================================================================
void FilePanel::updateAvailableSpaceLabel()
{
    // Format Available Space Text
    QString availableSpace = QString(DEFAULT_AVAILABLE_SPACE_FORMAT_STRING).arg(fileListModel ? fileListModel->rowCount() : 0)
                                                                           .arg((getFreeSpace(currentDir) >> 10))
                                                                           .arg((getTotalSpace(currentDir) >> 10));

    // Set Text
    ui->availableSpaceLabel->setText(availableSpace);
}

//==============================================================================
// File Model Fetch Ready
//==============================================================================
void FilePanel::fileModelDirFetchFinished()
{
    qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName;

    // Check Last Dir Name
    if (!lastDirName.isEmpty()) {
        // Find Index
        int lastDirIndex = fileListModel ? fileListModel->findIndex(lastDirName) : 0;
        // Reset Last Dir Name
        lastDirName = "";
        // Set Current Index
        setCurrentIndex(lastDirIndex);
    } else if (lastIndex != -1) {
        // Set Current Index
        setCurrentIndex(lastIndex);
        // Reset LAst Index
        lastIndex = -1;
    }

    // Update Available Space Label
    updateAvailableSpaceLabel();
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
        qDebug() << "FilePanel::focusInEvent - panel: " << panelName;

        // Set Style Sheet
        setStyleSheet(DEFAULT_PANEL_FOCUSED_STYLE_SHEET);
        // Set Style Sheet
        ui->currDirLabel->setStyleSheet(DEFAULT_PANEL_TRASPARENT_STYLE_SHEET);
        // Set Style Sheet
        ui->availableSpaceLabel->setStyleSheet(DEFAULT_PANEL_TRASPARENT_STYLE_SHEET);

        // Emit Focused Panel Changed Signal
        emit focusedPanelChanged(this);
    }
}

//==============================================================================
// Focus Out Event
//==============================================================================
void FilePanel::focusOutEvent(QFocusEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        qDebug() << "FilePanel::focusOutEvent - panel: " << panelName;

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
            case Qt::Key_R:

            break;

            case Qt::Key_Escape:

            break;

            case Qt::Key_Tab:
                //qDebug() << "FilePanel::keyPressEvent - key: TAB";

            break;

            case Qt::Key_Return:
            case Qt::Key_Enter:

            break;

            case Qt::Key_Space:


            break;

            case Qt::Key_Up:

            break;

            case Qt::Key_Down:

            break;

            case Qt::Key_Backspace:
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
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {

                }
            break;

            case Qt::Key_End:
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {

                }
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
            case Qt::Key_R:
/*
                // Check If Control Pressed
                if (modifierKeys & Qt::ControlModifier) {
                    // Check File List Model
                    if (fileListModel) {
                        // Reload
                        fileListModel->reload();
                    }
                }
*/
            break;

            case Qt::Key_Escape:

            break;

            case Qt::Key_Tab:

            break;

            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Handle Item Selection
                handleItemSelect();
            break;

            case Qt::Key_Space:

            break;

            case Qt::Key_Up:
            break;

            case Qt::Key_Down:
            break;

            case Qt::Key_Backspace:
            case Qt::Key_Left:
                // Go Up
                goUp();
            break;

            case Qt::Key_Right:
                // Check If Is Dir
                if (fileListModel) {
                    // Get File Info
                    QFileInfo fileInfo = fileListModel->getFileInfo(currentIndex);
                    // Check File Info
                    if ((fileInfo.isDir() || fileInfo.isBundle()) && fileInfo.fileName() != QString("..")) {
                        // Set Current Dir
                        setCurrentDir(fileInfo.absoluteFilePath());
                        // Set Current Index
                        setCurrentIndex(0);
                    }
                }
            break;

            case Qt::Key_PageUp:
                // Page Up
                pageUp();
            break;

            case Qt::Key_PageDown:
                // Page Down
                pageDown();
            break;

            case Qt::Key_Home:
                // Go First
                goFirst();
            break;

            case Qt::Key_End:
                // Go Last
                goLast();
            break;

            case Qt::Key_F6:
                // Rename
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

