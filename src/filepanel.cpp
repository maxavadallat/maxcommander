#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QImageReader>
#include <QDebug>

#include <mcwinterface.h>

#include "mainwindow.h"
#include "busyindicator.h"
#include "filepanel.h"
#include "filelistmodel.h"
#include "filelistimageprovider.h"
#include "confirmdialog.h"
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
    , modifierKeys(Qt::NoModifier)
    , currentIndex(-1)
    , visualItemsCount(-1)
    , lastDirName("")
    , lastIndex(-1)
    , showHidden(true)
    , extVisible(true)
    , typeVisible(false)
    , sizeVisible(true)
    , dateVisible(true)
    , ownerVisible(false)
    , permsVisible(false)
    , attrsVisible(false)
    , extWidth(40)
    , typeWidth(120)
    , sizeWidth(60)
    , dateWidth(130)
    , ownerWidth(60)
    , permsWidth(100)
    , attrsWidth(60)
    , dirWatcherTimerID(-1)
    , dwDirChanged(false)
    , dwFileChanged(false)
    , ownKeyPress(false)

{
    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void FilePanel::init()
{
    // Update Supported Image Formats
    updateSupportedImageFormats();

    // ...

    // Create File List Model
    fileListModel = new FileListModel();

    // Check File List Model
    if (fileListModel) {
        // Connect Signals
        connect(fileListModel, SIGNAL(busyChanged(bool)), this, SLOT(fileModelBusyChanged(bool)));
        connect(fileListModel, SIGNAL(dirFetchFinished()), this, SLOT(fileModelDirFetchFinished()));
        connect(fileListModel, SIGNAL(dirCreated(QString)), this, SLOT(fileModelDirCreated(QString)));
        connect(fileListModel, SIGNAL(error(QString,int)), this, SLOT(fileModelError(QString,int)));
    }

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

    // Register Busy Indicator
    qmlRegisterType<BusyIndicator>("MyCustomComponents", 1, 0, "BusyIndicator");

    // Set Resize Mode
    ui->fileListWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Set Source
    ui->fileListWidget->setSource(QUrl("qrc:/qml/FileList.qml"));


    // Connect Signals
    connect(ui->fileListWidget, SIGNAL(focusChanged(bool)), this, SLOT(setPanelFocus(bool)));

    // Connect Signals - Dir Watcher
    connect(&dirWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
    connect(&dirWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));

    // ...

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
// Get Extension Visible
//==============================================================================
bool FilePanel::getExtVisible()
{
    return extVisible;
}

//==============================================================================
// Set Extension Visible
//==============================================================================
void FilePanel::setExtVisible(const bool& aExtVisible)
{
    // Check Ext Visible
    if (extVisible != aExtVisible) {
        // Set Ext Visible
        extVisible = aExtVisible;
        // Emit Ext Visible Changed Signal
        emit extVisibleChanged(extVisible);
    }
}

//==============================================================================
// Get Type Visible
//==============================================================================
bool FilePanel::getTypeVisible()
{
    return typeVisible;
}

//==============================================================================
// Set Type Visible
//==============================================================================
void FilePanel::setTypeVisible(const bool& aTypeVisible)
{
    // Check Type Visible
    if (typeVisible != aTypeVisible) {
        // Set Type Visible
        typeVisible = aTypeVisible;
        // Emit Type Visible Changed Signal
        emit typeVisibleChanged(typeVisible);
    }
}

//==============================================================================
// Get Size Visible
//==============================================================================
bool FilePanel::getSizeVisible()
{
    return sizeVisible;
}

//==============================================================================
// Set Size Visible
//==============================================================================
void FilePanel::setSizeVisible(const bool& aSizeVisible)
{
    // Check Size Visible
    if (sizeVisible != aSizeVisible) {
        // Set Size Visible
        sizeVisible = aSizeVisible;
        // Emit Size Visible Changed Signal
        emit sizeVisibleChanged(sizeVisible);
    }
}

//==============================================================================
// Get Date Visible
//==============================================================================
bool FilePanel::getDateVisible()
{
    return dateVisible;
}

//==============================================================================
// Set Date Visible
//==============================================================================
void FilePanel::setDateVisible(const bool& aDateVisible)
{
    // Check Date Visible
    if (dateVisible != aDateVisible) {
        //qDebug() << "FilePanel::setDateVisible - aDateVisible: " << aDateVisible;

        // Set Date Visible
        dateVisible = aDateVisible;
        // Emit Date Visible Changed Signal
        emit dateVisibleChanged(dateVisible);
    }
}

//==============================================================================
// Get Owner Visible
//==============================================================================
bool FilePanel::getOwnerVisible()
{
    return ownerVisible;
}

//==============================================================================
// Set Owner Visible
//==============================================================================
void FilePanel::setOwnerVisible(const bool& aOwnerVisible)
{
    // Check Owner Visible
    if (ownerVisible != aOwnerVisible) {
        // Set Owner Visible
        ownerVisible = aOwnerVisible;
        // Emit Owner Visible Changed Signal
        emit ownerVisibleChanged(ownerVisible);
    }
}

//==============================================================================
// Get Permissions Visible
//==============================================================================
bool FilePanel::getPermsVisible()
{
    return permsVisible;
}

//==============================================================================
// Set Permissions Visible
//==============================================================================
void FilePanel::setPermsVisible(const bool& aPermsVisible)
{
    // Check Permissions Visible
    if (permsVisible != aPermsVisible) {
        // Set Permissions Visible
        permsVisible = aPermsVisible;
        // Emit Permissions Visible Changed Signal
        emit permsVisibleChanged(permsVisible);
    }
}

//==============================================================================
// Get Attributes Visible
//==============================================================================
bool FilePanel::getAttrsVisible()
{
    return attrsVisible;
}

//==============================================================================
// Set Attributes Visible
//==============================================================================
void FilePanel::setAttrsVisible(const bool& aAttrsVisible)
{
    // Check Attributes Visible
    if (attrsVisible != aAttrsVisible) {
        // Set Attributes Visible
        attrsVisible = aAttrsVisible;
        // Emit Attributes Visible Changed Signal
        emit attrsVisibleChanged(attrsVisible);
    }
}

//==============================================================================
// Get Extensions Column Width
//==============================================================================
int FilePanel::getExtWidth()
{
    return extWidth;
}

//==============================================================================
// Set Extensions Column Width
//==============================================================================
void FilePanel::setExtWidth(const int& aWidth)
{
    // Check Extensions Width
    if (extWidth != aWidth) {
        // Set Width
        extWidth = aWidth;
        // Emit Width Changed Signal
        emit extWidthChanged(extWidth);
    }
}

//==============================================================================
// Get Type Column Width
//==============================================================================
int FilePanel::getTypeWidth()
{
    return typeWidth;
}

//==============================================================================
// Set Type Column Width
//==============================================================================
void FilePanel::setTypeWidth(const int& aWidth)
{
    // Check Type Width
    if (typeWidth != aWidth) {
        // Set Width
        typeWidth = aWidth;
        // Emit Width Changed Signal
        emit typeWidthChanged(typeWidth);
    }
}

//==============================================================================
// Get Size Column Width
//==============================================================================
int FilePanel::getSizeWidth()
{
    return sizeWidth;
}

//==============================================================================
// Set Size Column Width
//==============================================================================
void FilePanel::setSizeWidth(const int& aWidth)
{
    // Check Size Width
    if (sizeWidth != aWidth) {
        // Set Width
        sizeWidth = aWidth;
        // Emit Width Changed Signal
        emit sizeWidthChanged(sizeWidth);
    }
}

//==============================================================================
// Get Date Column Width
//==============================================================================
int FilePanel::getDateWidth()
{
    return dateWidth;
}

//==============================================================================
// Set Date Column Width
//==============================================================================
void FilePanel::setDateWidth(const int& aWidth)
{
    // Check Date Width
    if (dateWidth != aWidth) {
        // Set Width
        dateWidth = aWidth;
        // Emit Width Changed Signal
        emit dateWidthChanged(dateWidth);
    }
}

//==============================================================================
// Get Owner Column Width
//==============================================================================
int FilePanel::getOwnerWidth()
{
    return ownerWidth;
}

//==============================================================================
// Set Owner Column Width
//==============================================================================
void FilePanel::setOwnerWidth(const int& aWidth)
{
    // Check Owner Width
    if (ownerWidth != aWidth) {
        // Set Width
        ownerWidth = aWidth;
        // Emit Width Changed Signal
        emit ownerWidthChanged(ownerWidth);
    }
}

//==============================================================================
// Get Permissions Column Width
//==============================================================================
int FilePanel::getPermsWidth()
{
    return permsWidth;
}

//==============================================================================
// Set Permissions Column Width
//==============================================================================
void FilePanel::setPermsWidth(const int& aWidth)
{
    // Check Permissions Width
    if (permsWidth != aWidth) {
        // Set Width
        permsWidth = aWidth;
        // Emit Width Changed Signal
        emit permsWidthChanged(permsWidth);
    }
}

//==============================================================================
// Get Attributes Column Width
//==============================================================================
int FilePanel::getAttrsWidth()
{
    return attrsWidth;
}

//==============================================================================
// Set Attributes Column Width
//==============================================================================
void FilePanel::setAttrsWidth(const int& aWidth)
{
    // Check Attributes Width
    if (attrsWidth != aWidth) {
        // Set Width
        attrsWidth = aWidth;
        // Emit Width Changed Signal
        emit attrsWidthChanged(attrsWidth);
    }
}

//==============================================================================
// Get Sorting
//==============================================================================
int FilePanel::getSorting()
{
    return sorting;
}

//==============================================================================
// Set Sorting
//==============================================================================
void FilePanel::setSorting(const int& aSorting)
{
    // Check Sorting
    if (sorting != aSorting) {
        qDebug() << "FilePanel::setSorting - aSorting: " << aSorting;
        // Set Sorting
        sorting = aSorting;
        // Reset Reverse Order
        reverseOrder = false;

        // Check Model
        if (fileListModel) {
            // Set Sorting Method
            fileListModel->setSorting(sorting);
            // Set Reverse Order
            fileListModel->setReverse(reverseOrder);
        }

        // Reload
        reload(0);

        // Emit Sorting Changed Signal
        emit sortingChanged(sorting);
    }
}

//==============================================================================
// Get Reversed Order
//==============================================================================
bool FilePanel::getReverseOrder()
{
    return reverseOrder;
}

//==============================================================================
// Set Reversed Order
//==============================================================================
void FilePanel::setReverseOrder(const bool& aReverse)
{
    // Check Reverse Order
    if (reverseOrder != aReverse) {
        qDebug() << "FilePanel::setReverseOrder - aReverse: " << aReverse;
        // Set Reverse Order
        reverseOrder = aReverse;

        // Check Model
        if (fileListModel) {
            // Set Reverse Order
            fileListModel->setReverse(reverseOrder);
        }

        // Reload
        reload(0);

        // Emit Reverse Order Changed Signal
        emit reverseOrderChanged(reverseOrder);
    }
}

//==============================================================================
// Get Current File Info
//==============================================================================
QFileInfo FilePanel::getCurrFileInfo()
{
    // Check File List Model
    if (fileListModel) {
        // Get Current File Info
        return fileListModel->getFileInfo(currentIndex);
    }

    return QFileInfo();
}

//==============================================================================
// Busy
//==============================================================================
bool FilePanel::busy()
{
    // Check File List Model
    if (fileListModel) {
        // Get File List Model Busy State
        return fileListModel->getBusy();
    }

    return false;
}

//==============================================================================
// Get File Index By File Name
//==============================================================================
int FilePanel::getFileIndex(const QString& aFileName)
{
    // Check File List Model
    if (fileListModel) {
        // Get Index
        return fileListModel->getFileIndex(aFileName);
    }

    return -1;
}

//==============================================================================
// Create Dir
//==============================================================================
void FilePanel::createDir(const QString& aDirPath)
{
    // Check File List Model
    if (fileListModel) {
        // Create Dir
        fileListModel->createDir(aDirPath);
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
// Select All Files
//==============================================================================
void FilePanel::selectAllFiles()
{
    // Check File List Model
    if (fileListModel) {
        // Select All Items
        fileListModel->selectAll();
    }
}

//==============================================================================
// Deselect All Files
//==============================================================================
void FilePanel::deselectAllFiles()
{
    // Check File List Model
    if (fileListModel) {
        // Deselect All Items
        fileListModel->deselectAll();
    }
}

//==============================================================================
// Toggle Current File Selection
//==============================================================================
void FilePanel::toggleCurrentFileSelection()
{
    // Check File List Model
    if (fileListModel) {
        // Set Item Selection
        fileListModel->setSelected(currentIndex, !fileListModel->getSelected(currentIndex));
    }
}

//==============================================================================
// Get Selected Files
//==============================================================================
QStringList FilePanel::getSelectedFiles()
{
    // Check File List Model
    if (fileListModel) {
        // Get Selected
        QStringList result = fileListModel->getAllSelected();

        // Check Result
        if (result.count() <= 0) {
            // Get Current File Name
            QString fileName = fileListModel->getFileInfo(currentIndex).fileName();
            // Check Current File Name
            if (fileName != "." && fileName != "..") {
                // Add Current File
                result << fileName;
            }
        }

        return result;
    }

    return QStringList();
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

        qDebug() << "FilePanel::goUp - panelName: " << panelName << " - parentDir: " << parentDir << " - lastDirName: " << lastDirName;

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

    // Set Ext Visible
    setExtVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_EXT, true).toBool());
    // Set Type Visible
    setTypeVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_TYPE, false).toBool());
    // Set Size Visible
    setSizeVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_SIZE, true).toBool());
    // Set Date Visible
    setDateVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_DATE, true).toBool());
    // Set Owner Visible
    setOwnerVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_OWNER, false).toBool());
    // Set Permissions Visible
    setPermsVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_PERMS, false).toBool());
    // Set Attributes Visible
    setAttrsVisible(settings.value(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_ATTR, false).toBool());

    // Set Extensions Column Width
    setExtWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_EXT, extWidth).toInt());
    // Set Extensions Column Width
    setTypeWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_TYPE, typeWidth).toInt());
    // Set Extensions Column Width
    setSizeWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_SIZE, sizeWidth).toInt());
    // Set Extensions Column Width
    setDateWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_DATE, dateWidth).toInt());
    // Set Extensions Column Width
    setOwnerWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_OWNER, ownerWidth).toInt());
    // Set Extensions Column Width
    setPermsWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_PERMS, permsWidth).toInt());
    // Set Extensions Column Width
    setAttrsWidth(settings.value(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_ATTR, attrsWidth).toInt());

    // Set Sorting
    //setSorting(settings.value(panelName + SETTINGS_KEY_PANEL_SORTTYPE, DEFAULT_SORT_NAME).toInt());
    sorting = settings.value(panelName + SETTINGS_KEY_PANEL_SORTTYPE, DEFAULT_SORT_NAME).toInt();
    // Emit Sorting Changed Signal
    emit sortingChanged(sorting);
    // Set Reverse Order
    //setSorting(settings.value(panelName + SETTINGS_KEY_PANEL_REVERSE, false).toBool());
    reverseOrder = settings.value(panelName + SETTINGS_KEY_PANEL_REVERSE, false).toBool();
    // Emit Reverse Order Changed signal
    emit reverseOrderChanged(reverseOrder);

    // Check File List Model
    if (fileListModel) {
        // Set Sorting
        fileListModel->setSorting(sorting);
        // Set Reverse Order
        fileListModel->setReverse(reverseOrder);
    }

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

    // Set Value - Extension Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_EXT, extVisible);
    // Set Value - Type Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_TYPE, typeVisible);
    // Set Value - Size Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_SIZE, sizeVisible);
    // Set Value - Date Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_DATE, dateVisible);
    // Set Value - Owner Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_OWNER, ownerVisible);
    // Set Value - Permissions Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_PERMS, permsVisible);
    // Set Value - Attributes Visible
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_ENABLED_ATTR, attrsVisible);

    // Set Value - Extensions Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_EXT, extWidth);
    // Set Value - Type Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_TYPE, typeWidth);
    // Set Value - Size Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_SIZE, sizeWidth);
    // Set Value - Date Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_DATE, dateWidth);
    // Set Value - Owner Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_OWNER, ownerWidth);
    // Set Value - Permissions Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_PERMS, permsWidth);
    // Set Value - Attributes Width
    settings.setValue(panelName + SETTINGS_KEY_PANEL_COL_WIDTH_ATTR, attrsWidth);

    // Set Value - Sorting
    settings.setValue(panelName + SETTINGS_KEY_PANEL_SORTTYPE, sorting);
    // Set Value - Reverse Order
    settings.setValue(panelName + SETTINGS_KEY_PANEL_REVERSE, reverseOrder);


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
    QString availableSpace = QString(DEFAULT_AVAILABLE_SPACE_FORMAT_STRING).arg(fileListModel ? fileListModel->getFileCount() : 0)
                                                                           .arg((getFreeSpace(currentDir) >> 10))
                                                                           .arg((getTotalSpace(currentDir) >> 10));

    // Set Text
    ui->availableSpaceLabel->setText(availableSpace);
}

//==============================================================================
// Update Supported Image Formates
//==============================================================================
void FilePanel::updateSupportedImageFormats()
{
    // Get Supported Image Formats Bye Array
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    // Get Count
    int flCount = formats.count();
    // Go Thru Formats
    for (int i=0; i<flCount; ++i) {
        // Add Format String
        supportedImageFormats << QString(formats[i]);
    }
}

//==============================================================================
// File List Model Busy Changed Slot
//==============================================================================
void FilePanel::fileModelBusyChanged(const bool& aBusy)
{
    // Check Busy
    if (aBusy) {
        // Stop Dir Watcher
        stopDirWatcher();
    } else {
        // Start Dir Watcher
        startDirWatcher();
    }

    // Emit Busy Changed Signal
    emit busyChanged(aBusy);
}

//==============================================================================
// File Model Fetch Ready
//==============================================================================
void FilePanel::fileModelDirFetchFinished()
{
    qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName << " - lastDirName: " << lastDirName;

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
// File Model Dir Created Slot
//==============================================================================
void FilePanel::fileModelDirCreated(const QString& aDirPath)
{
    // Get Path
    QString path = getDirPath(aDirPath);

    // Check Path
    if (path == currentDir) {
        qDebug() << "FilePanel::fileModelDirCreated - panelName: " << panelName << " - aDirPath: " << aDirPath;

        // Get Dir Name
        QString dirName = getDirName(aDirPath);

        // Insert Dir Name
        if (fileListModel) {
            // Insert Dir Name
            fileListModel->insertItem(dirName);
            // Set Current Index
            setCurrentIndex(fileListModel->findIndex(dirName));
        }
    }
}

//==============================================================================
// File Model Error
//==============================================================================
void FilePanel::fileModelError(const QString& aPath, const int& aError)
{
    // Check File List Model
    if (fileListModel) {
        qDebug() << "FilePanel::fileModelError - panelName: " << panelName << " - op: " << fileListModel->lastOperation() << " - aPath: " << aPath << " - aError: " << aError;

        // Check Last Operation - List Dir
        if (fileListModel->lastOperation() == DEFAULT_OPERATION_LIST_DIR) {


        // Check Last Operation - Make Dir
        } else if (fileListModel->lastOperation() == DEFAULT_OPERATION_MAKE_DIR) {
            // Directory Exists
            ConfirmDialog confirmDialog;
            // Clear Buttons
            //confirmDialog.clearButtons();
            // Configure Buttons
            confirmDialog.configureButtons(QDialogButtonBox::Abort);

            // Switch Error
            switch (aError) {
                case DEFAULT_ERROR_EXISTS: {
                    // Set Text
                    confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS));
                    // Add Button
                    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_RETRY), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_RETRY);
                    // Set Path
                    confirmDialog.setPath(aPath);
                    // Exec Dialog
                    if (confirmDialog.exec()) {
                        // Send User Response/Confirm
                        fileListModel->sendUserResponse(confirmDialog.getActionIndex(), confirmDialog.getPath());
                    } else {
                        // Send User Response/Confirm
                        fileListModel->sendUserResponse(DEFAULT_CONFIRM_ABORT, confirmDialog.getPath());
                    }

                } break;

                case DEFAULT_ERROR_ACCESS:
                    // No Access

                break;

                case DEFAULT_ERROR_GENERAL:
                    // General

                break;

                default:
                break;
            }
        }
    }
}

//==============================================================================
// Start Dir Watcher
//==============================================================================
void FilePanel::startDirWatcher()
{
    // Check Dir Watcher Timer ID
    if (dirWatcherTimerID == -1) {
        qDebug() << "FilePanel::startDirWatcher";
        // Start Timer
        dirWatcherTimerID = startTimer(DEFAULT_ONE_SEC);

        // Start Dir Watcher
        dirWatcher.addPath(currentDir);
    }
}

//==============================================================================
// Stop Dir Watcher
//==============================================================================
void FilePanel::stopDirWatcher()
{
    // Check Dir Watcher Timer ID
    if (dirWatcherTimerID != -1) {
        qDebug() << "FilePanel::stopDirWatcher";
        // Kill Timer
        killTimer(dirWatcherTimerID);
        // Reset Dir Watcher Timer ID
        dirWatcherTimerID = -1;

        // Get Dir Watcher Directories
        QStringList dwDirs = dirWatcher.directories();
        // Get Dir Wathcer Dirs Count
        int dwdCount = dwDirs.count();

        // Go Thru Dri Watcher Dirs
        for (int i=0; i<dwdCount; ++i) {
            // Stop Dir Watcher/ Remove Dir
            dirWatcher.removePath(dwDirs[i]);
        }
    }
}

//==============================================================================
// Directory Changed Slot
//==============================================================================
void FilePanel::directoryChanged(const QString& aDirPath)
{
    // Check Dir Path
    if (currentDir == aDirPath) {
        qDebug() << "FilePanel::stopDirWatcher - aDirPath: " << aDirPath;
        // Set Dir Changed
        dwDirChanged = true;

        // ...
    }
}

//==============================================================================
// File Changed Slot
//==============================================================================
void FilePanel::fileChanged(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);
    // Check File Path
    if (currentDir == fileInfo.absolutePath()) {
        qDebug() << "FilePanel::fileChanged - aFilePath: " << aFilePath;
        // Set File Changed
        dwFileChanged = true;

        // ...
    }
}

//==============================================================================
// Refresh File List Model
//==============================================================================
void FilePanel::refreshFileListModel(const QString& aFilePath)
{
    qDebug() << "FilePanel::refreshFileListModel - aFilePath: " << aFilePath;

}

//==============================================================================
// Handle Modifier Key Press Event
//==============================================================================
bool FilePanel::handleModifierKeyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
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

                    return true;
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

                    return true;
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

                    return true;
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

                    return true;
                }
            break;

            default:
            break;
        }
    }

    return false;
}

//==============================================================================
// Handle Modifier Key Release Event
//==============================================================================
bool FilePanel::handleModifierKeyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Shift:
                //qDebug() << "FilePanel::keyReleaseEvent - key: SHIFT";
                // Update Modifier Keys
                modifierKeys ^= Qt::ShiftModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;;

            case Qt::Key_Control:
                //qDebug() << "FilePanel::keyReleaseEvent - key: CONTROL";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::ControlModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;;

            case Qt::Key_AltGr:
            case Qt::Key_Alt:
                //qDebug() << "FilePanel::keyReleaseEvent - key: ALT";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::AltModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;;

            case Qt::Key_Meta:
                //qDebug() << "FilePanel::keyReleaseEvent - key: META";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::MetaModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;;

            default:
            break;
        }
    }

    return false;
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
    // Handle Modifier Key Press Event
    if (handleModifierKeyPressEvent(aEvent)) {
        return;
    }

    // Check Event
    if (aEvent) {
        // Set Own Key Press
        ownKeyPress = true;

        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Check If Shift Is Pressed -> Tray
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
    // Handle Modifier Key Release Event
    if (handleModifierKeyReleaseEvent(aEvent)) {
        return;
    }

    // Check Own Keypress
    if (!ownKeyPress) {
        //qDebug() << "FilePanel::keyReleaseEvent - NOT OWN KEY EVENT!!";
        return;
    }

    // Reset Own Key Press
    ownKeyPress = false;

    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Check If Shift Is Pressed -> Tray
            break;

            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Handle Item Selection
                handleItemSelect();
            break;

            case Qt::Key_Space:
                // Scan Dir
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

            case Qt::Key_Plus:
                // Check Modifier Keys
                if (modifierKeys == Qt::AltModifier) {

                }
            break;

            case Qt::Key_Minus:
                // Check Modifier Keys
                if (modifierKeys == Qt::AltModifier) {

                }
            break;

            case Qt::Key_Asterisk:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Check File List Model
                    if (fileListModel) {
                        // Toggle All Selection
                        fileListModel->toggleAllSelection();
                    }
                }
            break;

            case Qt::Key_F1:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Show Help
                    emit showHelp();
                }
            break;

            case Qt::Key_F2:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch Terminal
                    emit launchTerminal(currentDir);
                }
            break;

            case Qt::Key_F3:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Get File info
                    QFileInfo fileInfo = getCurrFileInfo();
                    // Check File Type
                    if (!fileInfo.isDir() && !fileInfo.isBundle() && !fileInfo.isSymLink()) {
                        // Emit Launch Viewer
                        emit launchViewer(false);
                    }
                }
            break;

            case Qt::Key_F4:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Get File info
                    QFileInfo fileInfo = getCurrFileInfo();
                    // Check File Type
                    if (!fileInfo.isDir() && !fileInfo.isBundle() && !fileInfo.isSymLink()) {
                        // Emit Launch Viewer
                        emit launchViewer(true);
                    }
                }
            break;

            case Qt::Key_F5:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch File Copy
                    emit launchFileCopy();
                }
            break;

            case Qt::Key_F6:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch File Move/Rename
                    emit launchFileMove();
                }
            break;

            case Qt::Key_F7:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch Create Dir
                    emit launchCreateDir();
                }
            break;

            case Qt::Key_Delete:
            case Qt::Key_F8:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch Delete
                    emit launchDelete();
                }
            break;

            case Qt::Key_F9:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Show Preferences
                    emit showPreferences();
                }
            break;

            case Qt::Key_F10:
                //qDebug() << "FilePanel::keyReleaseEvent - key: F10";
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Exit Key Released Signal
                    emit exitKeyReleased();
                }
            break;

            default:
                qDebug() << "FilePanel::keyReleaseEvent - key: " << aEvent->key();
            break;
        }
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void FilePanel::timerEvent(QTimerEvent* aEvent)
{
    // Cehck EVent
    if (aEvent) {
        // Check Tiemr ID
        if (aEvent->timerId() == dirWatcherTimerID) {
            // Check If Need Refrsh
            if (fileListModel && !fileListModel->getBusy() && (dwDirChanged || dwFileChanged)) {
                qDebug() << "FilePanel::timerEvent - dirWatcherTimerID";
                // Reset Dir Changed
                dwDirChanged = false;
                // Reset File Changed
                dwFileChanged = false;
                // Reload
                reload(currentIndex);
            }

            qDebug() << ".";
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
}

