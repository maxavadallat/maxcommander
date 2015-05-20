#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QImageReader>
#include <QTimer>
#include <QDebug>

#include <mcwinterface.h>

#include "mainwindow.h"
#include "busyindicator.h"
#include "filepanel.h"
#include "ui_filepanel.h"
#include "filelistmodel.h"
#include "filelistimageprovider.h"
#include "remotefileutilclient.h"
#include "confirmdialog.h"
#include "transferprogressmodel.h"
#include "utility.h"
#include "constants.h"
#include "defaultsettings.h"


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
    , lastFileName("")
    , lastIndex(-1)
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

    , sorting(DEFAULT_SORT_EXT)
    , reverseOrder(false)

    , showHiddenFiles(true)
    , showDirsFirst(true)
    , caseSensitiveSort(false)

    , textColor(DEFAULT_SETTINGS_TEXT_COLOR)
    , textBGColor(DEFAULT_SETTINGS_TEXT_BG_COLOR)
    , currentColor(DEFAULT_SETTINGS_CURRENT_COLOR)
    , currentBGColor(DEFAULT_SETTINGS_CURRENT_BG_COLOR)
    , selectedColor(DEFAULT_SETTINGS_SELECTED_COLOR)
    , selectedBGColor(DEFAULT_SETTINGS_SELECTED_BG_COLOR)
    , currentSelectedColor(DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR)
    , currentSelectedBGColor(DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR)
    , hiddenColor(DEFAULT_SETTINGS_HIDDEN_COLOR)
    , hiddenBGColor(DEFAULT_SETTINGS_HIDDEN_BG_COLOR)
    , linkColor(DEFAULT_SETTINGS_LINK_COLOR)
    , linkBGColor(DEFAULT_SETTINGS_LINK_BG_COLOR)
    , fontName(DEFAULT_SETTINGS_FONT_NAME)
    , fontSize(DEFAULT_SETTINGS_FONT_SIZE)
    , fontBold(DEFAULT_SETTINGS_FONT_BOLD)
    , fontItalic(DEFAULT_SETTINGS_FONT_ITALIC)

    , thumbWidth(DEFAULT_SETTINGS_THUMB_WIDTH)
    , thumbHeight(DEFAULT_SETTINGS_THUMB_HEIGHT)

    , dirWatcherTimerID(-1)
    , dwDirChanged(false)
    , dwFileChanged(false)
    , ownKeyPress(false)
    , fileRenameActive(false)
    , fileRenamer(NULL)
    , fileRenamerUpdate(false)
    , fileTransferUpdate(false)
    , fileDeleteUpdate(false)
    , dirScanner(NULL)
    , loading(false)
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
        connect(fileListModel, SIGNAL(fileRenamed(QString,QString)), this, SLOT(fileModelFileRenamed(QString,QString)));
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
void FilePanel::setCurrentDir(const QString& aCurrentDir, const QString& aLastFileName)
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

        // Check LAst File Name
        if (!aLastFileName.isEmpty()) {
            // Set Last File Name
            lastFileName = aLastFileName;
        }

        // Check Model
        if (fileListModel) {
            // Set Loading
            setLoading(true);
            // Set Current Dir
            fileListModel->setCurrentDir(currentDir);
        }

        // ...

        // Set Text
        ui->currDirLabel->setText(currentDir);

        // Emit Current dir Changed Signal
        emit currentDirChanged(currentDir);

    } else {
        // Check Last File Name
        if (!lastFileName.isEmpty()) {
            // Get Index
            int lastFileIndex = getFileIndex(aLastFileName);

            // Set Current Index
            setCurrentIndex(lastFileIndex);
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
// Get Show Hidden
//==============================================================================
bool FilePanel::getShowHidden()
{
    return showHiddenFiles;
}

//==============================================================================
// Set Show Hidden
//==============================================================================
void FilePanel::setShowHidden(const bool& aHidden)
{
    // Check Show Hidden
    if (showHiddenFiles != aHidden) {
        qDebug() << "FilePanel::setShowHidden - panelName: " << panelName << " - aHidden: " << aHidden;
        // Set Show Hidden
        showHiddenFiles = aHidden;

        // Check File Model
        if (fileListModel) {
            // Set Show Hidden Files
            fileListModel->setShowHiddenFiles(showHiddenFiles);
        }

        // Init Settings
        QSettings settings;
        // Set Value
        settings.setValue(SETTINGS_KEY_SHOW_HIDDEN_FILES, showHiddenFiles);
        // Sync
        settings.sync();

        // Reload
        reload(0);

        // Emit Show Hidden Changed Signal
        emit showHiddenChanged(showHiddenFiles);
    }
}

//==============================================================================
// Get Show Directories First
//==============================================================================
bool FilePanel::getShowDirsFirst()
{
    return showDirsFirst;
}

//==============================================================================
// Set Show Directories First
//==============================================================================
void FilePanel::setShowDirsFirst(const bool& aShow)
{
    // Check Show Dirs First
    if (showDirsFirst != aShow) {
        // Set Show Dirs First
        showDirsFirst = aShow;
        // Emit Show Dirs First Changed Signal
        emit showDirsFirstChanged(showDirsFirst);
    }
}

//==============================================================================
// Get Case Sensitive Sorting
//==============================================================================
bool FilePanel::getCaseSensitiveSort()
{
    return caseSensitiveSort;
}

//==============================================================================
// Set Case Sensitive Sorting
//==============================================================================
void FilePanel::setCaseSensitiveSort(const bool& aCaseSensitive)
{
    // Check Case Sensitive Sorting
    if (caseSensitiveSort != aCaseSensitive) {
        // Set Case Sensitive Sorting
        caseSensitiveSort = aCaseSensitive;
        // Emit Case Sensitive Sorting hanged Signal
        emit caseSensitiveSortChanged(caseSensitiveSort);
    }
}

//==============================================================================
// Get Normal Text Color
//==============================================================================
QString FilePanel::getTextColor()
{
    return textColor;
}

//==============================================================================
// Set Normal Text Color
//==============================================================================
void FilePanel::setTextColor(const QString& aColor)
{
    // Check Text Color
    if (textColor != aColor) {
        // Set Text Color
        textColor = aColor;
        // Emit Text Color Changed Signal
        emit textColorChanged(textColor);
    }
}

//==============================================================================
// Get Normal Text BG Color
//==============================================================================
QString FilePanel::gettextBGColor()
{
    return textBGColor;
}

//==============================================================================
// Set Normal Text BG Color
//==============================================================================
void FilePanel::setTextBGColor(const QString& aColor)
{
    // Check Text BG Color
    if (textBGColor != aColor) {
        // Set Text BG Color
        textBGColor = aColor;
        // Emit Text BG Color Changed Signal
        emit textBGColorChanged(textBGColor);
    }
}

//==============================================================================
// Get Current Text Color
//==============================================================================
QString FilePanel::getCurrentColor()
{
    return currentColor;
}

//==============================================================================
// Set Current Text Color
//==============================================================================
void FilePanel::setCurrentColor(const QString& aColor)
{
    // Check Current Color
    if (currentColor != aColor) {
        // Set Current Color
        currentColor = aColor;
        // Emit Current color Changed Signal
        emit currentColorChanged(currentColor);
    }
}

//==============================================================================
// Get Current Text BG Color
//==============================================================================
QString FilePanel::getCurrentBGColor()
{
    return currentBGColor;
}

//==============================================================================
// Set Current Text BG Color
//==============================================================================
void FilePanel::setCurrentBGColor(const QString& aColor)
{
    // Check Current BG Color
    if (currentBGColor != aColor) {
        // Set Current BG Color
        currentBGColor = aColor;
        // Emit Current BG Color Changed Signal
        emit currentBGColorChanged(currentBGColor);
    }
}

//==============================================================================
// Get Selected Text Color
//==============================================================================
QString FilePanel::getSelectedColor()
{
    return selectedColor;
}

//==============================================================================
// Set Selected Text Color
//==============================================================================
void FilePanel::setSelectedColor(const QString& aColor)
{
    // Check Selected Color
    if (selectedColor != aColor) {
        // Set Selected Color
        selectedColor = aColor;
        // Emit Selected Color Changed Signal
        emit selectedColorChanged(selectedColor);
    }
}

//==============================================================================
// Get Selected Text BG Color
//==============================================================================
QString FilePanel::getSelectedBGColor()
{
    return selectedBGColor;
}

//==============================================================================
// Set Selected Text BG Color
//==============================================================================
void FilePanel::setSelectedBGColor(const QString& aColor)
{
    // Check Selected BG Color
    if (selectedBGColor != aColor) {
        // Set Selected BG Color
        selectedBGColor = aColor;
        // Emit Selected BG Color Changed Signal
        emit selectedBGColorChanged(selectedBGColor);
    }
}

//==============================================================================
// Get Current Selected Text Color
//==============================================================================
QString FilePanel::getCurrentSelectedColor()
{
    return currentSelectedColor;
}

//==============================================================================
// Set Current Selected Text Color
//==============================================================================
void FilePanel::setCurrentSelectedColor(const QString& aColor)
{
    // Check Current Selected Color
    if (currentSelectedColor != aColor) {
        // Set Current Selected Color
        currentSelectedColor = aColor;
        // Emit Current Selected Color Changed Signal
        emit currentSelectedColorChanged(currentSelectedColor);
    }
}

//==============================================================================
// Get Current Selected Text BG Color
//==============================================================================
QString FilePanel::getCurrentSelectedBGColor()
{
    return currentSelectedBGColor;
}

//==============================================================================
// Set Current Selected Text BG Color
//==============================================================================
void FilePanel::setCurrentSelectedBGColor(const QString& aColor)
{
    // Check Current Selected BG Color
    if (currentSelectedBGColor != aColor) {
        // Set Current Selected BG Color
        currentSelectedBGColor = aColor;
        // Emit Currnt Selected BG Color Changed Signal
        emit currentSelectedBGColorChanged(currentSelectedBGColor);
    }
}

//==============================================================================
// Get Hidden Text Color
//==============================================================================
QString FilePanel::getHiddenColor()
{
    return hiddenColor;
}

//==============================================================================
// Set Hidden Text Color
//==============================================================================
void FilePanel::setHiddenColor(const QString& aColor)
{
    // Check Hidden Color
    if (hiddenColor != aColor) {
        // Set Hidden Color
        hiddenColor = aColor;
        // Emit Hidden Color Changed Signal
        emit hiddenColorChanged(hiddenColor);
    }
}

//==============================================================================
// Get Hidden Text BG Color
//==============================================================================
QString FilePanel::getHiddenBGColor()
{
    return hiddenBGColor;
}

//==============================================================================
// Set Hidden Text BG Color
//==============================================================================
void FilePanel::setHiddenBGColor(const QString& aColor)
{
    // Check Hidden BG Color
    if (hiddenBGColor != aColor) {
        // Set Hidden BG Color
        hiddenBGColor = aColor;
        // Emit hidden BG Color Changed Signal
        emit hiddenBGColorChanged(hiddenBGColor);
    }
}

//==============================================================================
// Get Link Text Color
//==============================================================================
QString FilePanel::getLinkColor()
{
    return linkColor;
}

//==============================================================================
// Set Link Text Color
//==============================================================================
void FilePanel::setLinkColor(const QString& aColor)
{
    // Check Link Color
    if (linkColor != aColor) {
        // Set Link Color
        linkColor = aColor;
        // Emit Link Color Changed Signal
        emit linkColorChanged(linkColor);
    }
}

//==============================================================================
// Get Link Text BG Color
//==============================================================================
QString FilePanel::getLinkBGColor()
{
    return linkBGColor;
}

//==============================================================================
// Set Link Text BG Color
//==============================================================================
void FilePanel::setLinkBGColor(const QString& aColor)
{
    // Check Link BG Color
    if (linkBGColor != aColor) {
        // Set Link BG Color
        linkBGColor = aColor;
        // Emit hidden BG Color Changed Signal
        emit linkBGColorChanged(linkBGColor);
    }
}

//==============================================================================
// Get Font Name
//==============================================================================
QString FilePanel::getFontName()
{
    return fontName;
}

//==============================================================================
// Set Font Name
//==============================================================================
void FilePanel::setFontName(const QString& aFontName)
{
    // Check Font Name
    if (fontName != aFontName) {
        // Set Fonr name
        fontName = aFontName;
        // Emit Font name Changed Signal
        fontNameChanged(fontName);
    }
}

//==============================================================================
// Get Font Size
//==============================================================================
int FilePanel::getFontSize()
{
    return fontSize;
}

//==============================================================================
// Set Font Size
//==============================================================================
void FilePanel::setFontSize(const int& aSize)
{
    // Check Font Size
    if (fontSize != aSize) {
        // Set Font Size
        fontSize = aSize;
        // Emit Font Size Changed
        emit fontSizeChanged(fontSize);
    }
}

//==============================================================================
// Get Font Bold
//==============================================================================
bool FilePanel::getFontBold()
{
    return fontBold;
}

//==============================================================================
// Set Font Bold
//==============================================================================
void FilePanel::setFontBold(const bool& aBold)
{
    // Check Font Bold
    if (fontBold != aBold) {
        // Set Font Bold
        fontBold = aBold;
        // Emit Font Bold Changed
        emit fontBoldChanged(fontBold);
    }
}

//==============================================================================
// Get Font Italic
//==============================================================================
bool FilePanel::getFontItalic()
{
    return fontItalic;
}

//==============================================================================
// Set Font Italic
//==============================================================================
void FilePanel::setFontItalic(const bool& aItalic)
{
    // Check Font Italic
    if (fontItalic != aItalic) {
        // Set Font Italic
        fontItalic = aItalic;
        // Emit Font Italic Changed Signal
        emit fontItalicChanged(fontItalic);
    }
}

//==============================================================================
// Get Thumb Width
//==============================================================================
int FilePanel::getThumbWidth()
{
    return thumbWidth;
}

//==============================================================================
// Set Thumb Width
//==============================================================================
void FilePanel::setThumbWidth(const int& aWidth)
{
    // Check Thumb Width
    if (thumbWidth != aWidth) {
        // Set Thumb Width
        thumbWidth = aWidth;
        // Emit Thumb Width Changed
        emit thumbWidthChanged(thumbWidth);
    }
}

//==============================================================================
// Get Thumb Height
//==============================================================================
int FilePanel::getThumbHeight()
{
    return thumbHeight;
}

//==============================================================================
// Set Thumb height
//==============================================================================
void FilePanel::setThumbHeight(const int& aHeight)
{
    // Check Thumb Height
    if (thumbHeight != aHeight) {
        // Set Thumb Height
        thumbHeight = aHeight;
        // Emit Thumb Height Changed Signal
        emit thumbHeightChanged(thumbHeight);
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
// Get Loading
//==============================================================================
bool FilePanel::getloading()
{
    return loading;
}

//==============================================================================
// Set Loading
//==============================================================================
void FilePanel::setLoading(const bool& aLoading)
{
    // Check Loading
    if (loading != aLoading) {
        //qDebug() << "FilePanel::setLoading - aLoading: " << aLoading;

        // Set Loading
        loading = aLoading;
        // Emit Loading Changed Signal
        emit loadingChanged(loading);
    }
}

//==============================================================================
// Get File Index By File Name
//==============================================================================
int FilePanel::getFileIndex(const QString& aFileName)
{
    // Check File List Model
    if (fileListModel) {
        // Get Index
        return fileListModel->findIndex(aFileName);
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
// Rename File
//==============================================================================
void FilePanel::renameFile(const QString& aSource, const QString& aTarget)
{
    // Check Source & Target
    if (aSource == aTarget || aTarget.isEmpty()) {
        return;
    }

    // Check File Renamer
    if (!fileRenamer) {
        // Create File Renamer
        fileRenamer = new FileRenamer();

        // Connect Signal
        connect(fileRenamer, SIGNAL(finished(QString,QString)), this, SLOT(renamerFinished(QString,QString)));
    }

    // Check File Renamer
    if (!fileRenamer) {
        qDebug() << "FilePanel::renameFile - aSource: " << aSource << " - aTarget: " << aTarget << " - NO FILE RENAMER!!";
        return;
    }

    qDebug() << "FilePanel::renameFile - aSource: " << aSource << " - aTarget: " << aTarget;

    // Init Local Dir Name
    QString localDir(currentDir);
    // Check Local Dir
    if (!localDir.endsWith("/")) {
        // Adjust Local Dir
        localDir += "/";
    }

    // Rename File
    fileRenamer->renameFile(localDir + aSource, localDir + aTarget);
}

//==============================================================================
// Scan Dir
//==============================================================================
void FilePanel::scanDir(const QString& aDirPath)
{
    // Check Dir Scanner
    if (!dirScanner) {
        // Create Dir Scanner
        dirScanner = new DirScanner();
        // Connect Signal
        connect(dirScanner, SIGNAL(scanSizeChanged(QString,quint64)), this, SLOT(scanSizeChanged(QString,quint64)));
    }

    qDebug() << "FilePanel::scanDir - aDirPath: " << aDirPath;

    // Scan Dir
    dirScanner->scanDir(aDirPath);
}

//==============================================================================
// File Rename Active
//==============================================================================
bool FilePanel::getFileRenameActive()
{
    return fileRenameActive;
}

//==============================================================================
// Set File Rename Active
//==============================================================================
void FilePanel::setFileRenameActive(const bool& aFileRenameActive)
{
    // Check File Rename Active
    if (fileRenameActive != aFileRenameActive) {
        // Set File Rename Active
        fileRenameActive = aFileRenameActive;

        // ...

        // Emit File Rename Active Changed Signal
        emit fileRenameActiveChanged(fileRenameActive);
    }
}

//==============================================================================
// Get Modifier Keys
//==============================================================================
int FilePanel::getModifierKeys()
{
    return modifierKeys;
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
    // Get Bounded Index
    int boundedIndex = fileListModel ? qBound(0, aCurrentIndex, fileListModel->rowCount()-1) : 0;

    // Check Current Index
    if (currentIndex != boundedIndex) {
        //qDebug() << "FilePanel::setCurrentIndex - aCurrentIndex: " << aCurrentIndex;

        // Set Current Index
        currentIndex = boundedIndex;

        // ...

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
        //qDebug() << "FilePanel::setVisualItemsCount - aVisualCount: " << aVisualCount;

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
    qDebug() << "FilePanel::reload - panelName: " << panelName << " - aIndex: " << aIndex;

    // Get Last Index
    lastIndex = aIndex;

    // Reset Current Index
    currentIndex = -1;

    // Reset DW Dir Changed
    dwDirChanged = false;
    // Reset DW File Changed
    dwFileChanged = false;

    // Check File List Model
    if (fileListModel) {
        // Set Loading
        setLoading(true);
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
        qDebug() << "FilePanel::selectAllFiles - panelName: " << panelName;
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
        qDebug() << "FilePanel::deselectAllFiles - panelName: " << panelName;
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
        qDebug() << "FilePanel::toggleCurrentFileSelection - panelName: " << panelName;
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
    qDebug() << "FilePanel::gotoDrive - panelName: " << panelName << " - aDriveIndex: " << aDriveIndex;

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
// Handle Item Selection
//==============================================================================
void FilePanel::handleItemSelection()
{
    // Check File List Model
    if (!fileListModel) {
        qWarning() << "FilePanel::handleItemSelection - panelName: " << panelName << " - NO FILE LIST MODEL!!";
        return;
    }

    // Get File Info
    QFileInfo fileInfo = fileListModel->getFileInfo(currentIndex);

    // Check If Dir
    if (fileInfo.isDir() && !fileInfo.isBundle() /*|| fileInfo.isBundle()*/) {
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
        qDebug() << "FilePanel::handleItemSelection - panelName: " << panelName << " - fileName: " << fileInfo.fileName();

        // Handle Exec Files

        // Check If File is Executeable
        if (fileInfo.isExecutable()) {

#if defined(Q_OS_MACX)

            // Execute
            system(QString(DEFAULT_EXEC_APP_SYSTEM_COMMAND_MAC_OSX).arg(fileInfo.absoluteFilePath()).toLocal8Bit().data());

#elif defined(Q_OS_UNIX)


#else defined(Q_OS_WIN)


#endif // Q_OS_WIN

        } else {

#if defined(Q_OS_MACX)

            // Open
            system(QString(DEFAULT_OPEN_FILE_SYSTEM_COMMAND_MAC_OSX).arg(fileInfo.absoluteFilePath()).toLocal8Bit().data());

#elif defined(Q_OS_UNIX)


#else defined(Q_OS_WIN)


#endif // Q_OS_WIN

        }

        // ...

    }
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

    // Get Current File Info
    QFileInfo fileInfo = fileListModel->getFileInfo(currentIndex);

    qDebug() << "FilePanel::handleItemSelect - panelName: " << panelName << " - fileName: " << fileInfo.fileName();

    // Check If Dir
    if (fileInfo.isDir() || fileInfo.isBundle()) {
        // Check File Name
        if (fileInfo.fileName() != QString("..")) {
            // Scan Dir
            scanDir(fileInfo.absoluteFilePath());
        }
    } else {
        // Toggle Selection
        fileListModel->setSelected(currentIndex, !fileListModel->getSelected(currentIndex));
    }
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

        // Reset Current Index
        setCurrentIndex(-1);
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void FilePanel::restoreUI(const bool& aReload)
{
    qDebug() << "FilePanel::restoreUI - panelName: " << panelName << " - aReload: " << aReload;

    // Init Settings
    QSettings settings;

    // Get Saved Current Dir
    QString savedDir = settings.value(panelName + SETTINGS_KEY_PANEL_DIR, QDir::homePath()).toString();

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
    sorting = settings.value(panelName + SETTINGS_KEY_PANEL_SORTTYPE, DEFAULT_SORT_NAME).toInt();
    // Emit Sorting Changed Signal
    emit sortingChanged(sorting);
    // Set Reverse Order
    reverseOrder = settings.value(panelName + SETTINGS_KEY_PANEL_REVERSE, false).toBool();
    // Emit Reverse Order Changed signal
    emit reverseOrderChanged(reverseOrder);

    // Get Show Hidden Files
    showHiddenFiles = settings.value(SETTINGS_KEY_SHOW_HIDDEN_FILES, DEFAULT_SETTINGS_SHOW_HIDDEN_FILES).toBool();
    // Get Show Dirs First
    showDirsFirst = settings.value(SETTINGS_KEY_DIRFIRST, DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST).toBool();
    // Get Case Sensitive Sorting
    caseSensitiveSort = settings.value(SETTINGS_KEY_CASE_SENSITIVE, DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING).toBool();

    // Check File List Model
    if (fileListModel) {
        // Set Sorting
        fileListModel->setSorting(sorting);
        // Set Reverse Order
        fileListModel->setReverse(reverseOrder);
        // Set Dirs First
        fileListModel->setShowDirsFirst(showDirsFirst);
        // Set Show Hidden Files
        fileListModel->setShowHiddenFiles(showHiddenFiles);
        // Set Case Sensitive Sorting
        fileListModel->setCaseSensitiveSorting(caseSensitiveSort);
    }

    // Set Normal Text Color
    setTextColor(settings.value(SETTINGS_KEY_PANEL_COLOR_TEXT, DEFAULT_SETTINGS_TEXT_COLOR).toString());
    // Set Normal Text BG Color
    setTextBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_TEXT_BG, DEFAULT_SETTINGS_TEXT_BG_COLOR).toString());
    // Set Current Text Color
    setCurrentColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT, DEFAULT_SETTINGS_CURRENT_COLOR).toString());
    // Set Current Text BG Color
    setCurrentBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_BG, DEFAULT_SETTINGS_CURRENT_BG_COLOR).toString());
    // Set Selected Text Color
    setSelectedColor(settings.value(SETTINGS_KEY_PANEL_COLOR_SELECTED, DEFAULT_SETTINGS_SELECTED_COLOR).toString());
    // Set Selected Text BG Color
    setSelectedBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_SELECTED_BG, DEFAULT_SETTINGS_SELECTED_BG_COLOR).toString());
    // Set Current Selected Text Color
    setCurrentSelectedColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED, DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR).toString());
    // Set Current Selected Text BG Color
    setCurrentSelectedBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG, DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR).toString());
    // Set Hidden Text Color
    setHiddenColor(settings.value(SETTINGS_KEY_PANEL_COLOR_HIDDEN, DEFAULT_SETTINGS_HIDDEN_COLOR).toString());
    // Set Hidden Text BG Color
    setHiddenBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG, DEFAULT_SETTINGS_HIDDEN_BG_COLOR).toString());
    // Set Link Text Color
    setLinkColor(settings.value(SETTINGS_KEY_PANEL_COLOR_LINK, DEFAULT_SETTINGS_LINK_COLOR).toString());
    // Set Link Text BG Color
    setLinkBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_LINK_BG, DEFAULT_SETTINGS_LINK_BG_COLOR).toString());

    // Set Font Name
    setFontName(settings.value(SETTINGS_KEY_PANEL_FONT_NAME, DEFAULT_SETTINGS_FONT_NAME).toString());
    // Set Font Size
    setFontSize(settings.value(SETTINGS_KEY_PANEL_FONT_SIZE, DEFAULT_SETTINGS_FONT_SIZE).toInt());
    // Set Font Bold
    setFontBold(settings.value(SETTINGS_KEY_PANEL_FONT_BOLD, DEFAULT_SETTINGS_FONT_BOLD).toBool());
    // Set Font Italic
    setFontItalic(settings.value(SETTINGS_KEY_PANEL_FONT_ITALIC, DEFAULT_SETTINGS_FONT_ITALIC).toBool());

    // Set Thumb Width
    setThumbWidth(settings.value(SETTINGS_KEY_THUMBS_WIDTH, DEFAULT_SETTINGS_THUMB_WIDTH).toInt());
    // Set Thumb height
    setThumbHeight(settings.value(SETTINGS_KEY_THUMBS_HEIGHT, DEFAULT_SETTINGS_THUMB_HEIGHT).toInt());


    // ...

    // Check Reload
    if (aReload) {
        // Set Current Dir
        setCurrentDir(savedDir);
    }
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
    settings.setValue(SETTINGS_KEY_SHOW_HIDDEN_FILES, showHiddenFiles);

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
    // Check Last Dir Name
    if (!lastDirName.isEmpty()) {
        qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName << " - lastDirName: " << lastDirName;

        // Find Index
        int lastDirIndex = fileListModel ? fileListModel->findIndex(lastDirName) : 0;
        // Reset Last Dir Name
        lastDirName = "";
        // Set Current Index
        setCurrentIndex(lastDirIndex);
        // Set Loading
        setLoading(false);

    } else if (!lastFileName.isEmpty()) {
        qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName << " - lastFileName: " << lastFileName;

        // Find Index
        int lastFileIndex = fileListModel ? fileListModel->findIndex(lastFileName) : 0;
        // Reset Last File Name
        lastFileName = "";
        // Set Current Index
        setCurrentIndex(lastFileIndex);

        // Check Last Index
        if (lastIndex == 0) {
            // Set Loading
            setLoading(false);
        }


    } else if (lastIndex != -1) {
        qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName << " - lastIndex: " << lastIndex;

        // Set Current Index
        setCurrentIndex(lastIndex);

        // Check Last Index
        if (lastIndex == 0) {
            // Set Loading
            setLoading(false);
        }

        // Reset LAst Index
        lastIndex = -1;

    } else {
        qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName << " - currentDir: " << currentDir;

        // Set Current Index
        setCurrentIndex(0);

        // Set Loading
        setLoading(false);

        // ...
    }

    // Update Available Space Label
    updateAvailableSpaceLabel();

    // Check If Dir Watcher Has Changes
    if (dwDirChanged || dwFileChanged) {
        qDebug() << "FilePanel::fileModelDirFetchFinished - panelName: " << panelName << " - CHANGED!!";

        // Reload
        //reload();
    }

    // Emit Current Index Changed Signal For Sync
    emit currentIndexChanged(currentIndex);

    // ...
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
// File List Model File Renames Slot
//==============================================================================
void FilePanel::fileModelFileRenamed(const QString& aSource, const QString& aTarget)
{
    // Get Path
    QString path = getDirPath(aTarget);

    // Check Path
    if (path == currentDir) {
        qDebug() << "FilePanel::fileModelFileRenamed - panelName: " << panelName << " - aSource: " << aSource << " - aTarget: " << aTarget;

        // ...
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
// File Model Need Confirm Found Slot
//==============================================================================
void FilePanel::fileModelNeedConfirm(const int& aCode, const QString& aPath, const QString& aSource, const QString& aTarget)
{
    Q_UNUSED(aPath);

    // Check File List Model
    if (fileListModel) {
        qDebug() << "FilePanel::fileModelNeedConfirm - panelName: " << panelName << " - op: " << fileListModel->lastOperation() << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aCode: " << aCode;

        // ...
    }
}

//==============================================================================
// Start Dir Watcher
//==============================================================================
void FilePanel::startDirWatcher()
{
    // Check Dir Watcher Timer ID
    if (dirWatcherTimerID == -1) {
        //qDebug() << "FilePanel::startDirWatcher";
        // Start Timer
        dirWatcherTimerID = startTimer(DEFAULT_ONE_SEC * 5);

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
        //qDebug() << "FilePanel::stopDirWatcher";
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
        // Check File Renamer Update
        if (fileRenamerUpdate) {
            // Reset file Renamer Update
            fileRenamerUpdate = false;

            return;
        }

        qDebug() << "#### FilePanel::directoryChanged - aDirPath: " << aDirPath;

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

        qDebug() << "#### FilePanel::fileChanged - aFilePath: " << aFilePath;

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

    // ...

}

//==============================================================================
// Rename Finished Slot
//==============================================================================
void FilePanel::renamerFinished(const QString& aSource, const QString& aTarget)
{
    // Init Target File Info
    QFileInfo targetInfo(aTarget);

    // Check Target Info Path
    if (targetInfo.absolutePath() == currentDir) {
        qDebug() << "FilePanel::renamerFinished - aSource: " << aSource << " - aTarget: " << aTarget;

        // Check File List Model
        if (fileListModel) {
            // Set File  Renamer Update
            fileRenamerUpdate = true;

            // Set Last File Name
            lastFileName = targetInfo.fileName();

            // Reload
            reload();
        }
    }
}

//==============================================================================
// Scan Size Changed Slot
//==============================================================================
void FilePanel::scanSizeChanged(const QString& aDirPath, const quint64& aSize)
{
    // Check File List Model
    if (fileListModel) {
        // Init Dir Info
        QFileInfo dirInfo(aDirPath);
        // Find Index
        int dirIndex = fileListModel->findIndex(dirInfo.fileName());

        // Check Index
        if (dirIndex >= 0 && dirIndex < fileListModel->rowCount()) {
            //qDebug() << "FilePanel::scanSizeChanged - aDirPath: " << aDirPath << " - aSize: " << aSize;

            // Update Dir Size
            fileListModel->updateDirSize(dirIndex, aSize);
        }
    }
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
            return true;

            case Qt::Key_Control:
                //qDebug() << "FilePanel::keyReleaseEvent - key: CONTROL";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::ControlModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;

            case Qt::Key_AltGr:
            case Qt::Key_Alt:
                //qDebug() << "FilePanel::keyReleaseEvent - key: ALT";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::AltModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;

            case Qt::Key_Meta:
                //qDebug() << "FilePanel::keyReleaseEvent - key: META";
                // Update Modifier Keys
                modifierKeys = modifierKeys ^ Qt::MetaModifier;
                // Emit Modifier Keys Changed Signal
                emit modifierKeysChanged(modifierKeys);
            return true;

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

        // Check Focus
        if (!ui->fileListWidget->hasFocus()) {
            // Set Focus
            ui->fileListWidget->setFocus();
        }

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
        // Check If File Renamer Active
        if (fileRenameActive) {
            return;
        }

        // Set Own Key Press
        ownKeyPress = true;

        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Check If Shift Is Pressed -> Tray
            break;

            case Qt::Key_Up:
/*
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Go Prev
                    goPrev();
                }
*/
            break;

            case Qt::Key_Down:
/*
                // Check If Auto Repeat
                if (aEvent->isAutoRepeat()) {
                    // Accept
                    aEvent->accept();
                    // Go Next
                    goNext();
                }
*/
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
                //qDebug() << "FilePanel::keyPressEvent - key: " << aEvent->key();
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

    // Check If File Renamer Active
    if (fileRenameActive) {
        return;
    }

    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Check If Shift Is Pressed -> Tray
            break;

            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Handle Item Exec
                    handleItemSelection();
                }
            break;

            case Qt::Key_Space:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Handle Item Select
                    handleItemSelect();
                }
            break;

            case Qt::Key_Up:
                // Check Modifier - SHIFT
                if (modifierKeys == Qt::ShiftModifier && fileListModel) {
                    // Check Current Index
                    if (currentIndex == 0) {
                        // Set Selected
                        fileListModel->setSelected(currentIndex, !fileListModel->getSelected(currentIndex));
                    }
                } else if (modifierKeys == Qt::NoModifier) {
                    // Go Prev
                    //goPrev();
                }
            break;

            case Qt::Key_Down:
                // Check Modifier - SHIFT
                if (modifierKeys == Qt::ShiftModifier && fileListModel) {
                    // Check Current Index
                    if (currentIndex == fileListModel->rowCount()-1) {
                        // Set Selected
                        fileListModel->setSelected(currentIndex, !fileListModel->getSelected(currentIndex));
                    }
                } else if (modifierKeys == Qt::NoModifier) {
                    // Go Next
                    //goNext();
                }
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
                    // Emit Launch Viewer
                    emit launchViewer(false);
                }
            break;

            case Qt::Key_F4:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch Viewer
                    emit launchViewer(true);
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

                } else if (modifierKeys == Qt::ShiftModifier) {

                    // Check If File Renamer Active
                    if (!fileRenameActive) {
                        // Emit Launch File Rename
                        emit launchFileRename();
                    }
                }
            break;

            case Qt::Key_F7:
                // Check Modifier Keys
                if (modifierKeys == Qt::NoModifier) {
                    // Emit Launch Create Dir
                    emit launchCreateDir();
                } else if (modifierKeys == Qt::AltModifier) {
                    // Emit Launch Search
                    emit launchSearch();
                }
            break;

            case Qt::Key_S:
                // Check Modifier Keys
                if (modifierKeys == Qt::ControlModifier) {
                    // Emit Launch Search
                    emit launchSearch();
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
            // Check File List Model
            if (fileListModel) {
                // Check If Busy
                if (fileListModel->getBusy()) {

                    //qDebug() << "FilePanel::timerEvent - dirWatcherTimerID - BUSY!";

                } else if (dwDirChanged || dwFileChanged){

                    //qDebug() << "FilePanel::timerEvent - dirWatcherTimerID";

                    // Reset Dir Changed
                    dwDirChanged = false;
                    // Reset File Changed
                    dwFileChanged = false;

                    // Reload
                    reload(currentIndex);
                }
            }
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

    // Check File Renamer
    if (fileRenamer) {
        // Delete File Renamer
        delete fileRenamer;
        fileRenamer = NULL;
    }

    // Check Dir Scanner
    if (dirScanner) {
        // Delete Dir Scanner
        delete dirScanner;
        dirScanner = NULL;
    }
}

















//==============================================================================
// Constructor
//==============================================================================
FileRenamer::FileRenamer(QWidget* aParent)
    : QObject(aParent)
    , fileUtil(NULL)
    , queueIndex(-1)
{
    qDebug() << "FileRenamer::FileRenamer";

    // ...
}

//==============================================================================
// Rename File
//==============================================================================
void FileRenamer::renameFile(const QString& aSource, const QString& aTarget)
{
    qDebug() << "FileRenamer::renameFile - aSource: " << aSource << " - aTarget: " << aTarget;

    // Add Item
    addItem(aSource, aTarget);

    // Check File Util
    if (!fileUtil) {
        // Create File Util
        fileUtil = new RemoteFileUtilClient();

        // Connect Signals
        connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
    }

    // Check File Util
    if (fileUtil) {
        // Check If Connected
        if (!fileUtil->isConnected()) {
            // Connect
            fileUtil->connectToFileServer();
        } else {
            // Check Queue Index
            if (queueIndex == -1) {
                // Reset Queue Index
                queueIndex = 0;
                // Clear Options
                fileUtil->clearFileTransferOptions();
                // Process Queue
                QTimer::singleShot(1, this, SLOT(processQueue()));
            }
        }
    }
}

//==============================================================================
// Add Item
//==============================================================================
void FileRenamer::addItem(const QString& aSource, const QString& aTarget)
{
    qDebug() << "FileRenamer::addItem - aSource: " << aSource << " - aTarget: " << aTarget;

    // Create New Item
    TransferProgressModelItem* newItem = new TransferProgressModelItem(DEFAULT_OPERATION_MOVE_FILE, aSource, aTarget);

    // Add To Queue
    renameQueue << newItem;
}

//==============================================================================
// Insert Item
//==============================================================================
void FileRenamer::insertItem(const int& aIndex, const QString& aSource, const QString& aTarget)
{
    qDebug() << "FileRenamer::insertItem - aIndex: " << aIndex << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Create New Item
    TransferProgressModelItem* newItem = new TransferProgressModelItem(DEFAULT_OPERATION_MOVE_FILE, aSource, aTarget);

    // Check Index
    if (aIndex >= 0 && aIndex < renameQueue.count()) {
        // Insert To Queue
        renameQueue.insert(aIndex, newItem);
    } else {
        // Add To Queue
        renameQueue << newItem;
    }
}

//==============================================================================
// Set Item State
//==============================================================================
void FileRenamer::setItemState(const int& aIndex, const int& aState)
{
    qDebug() << "FileRenamer::setItemState - aIndex: " << aIndex << " - aState: " << aState;

    // Check Index
    if (aIndex >= 0 && aIndex < renameQueue.count()) {
        // Get Item
        TransferProgressModelItem* item = renameQueue[aIndex];
        // Set State
        item->state = (TransferProgressState)aState;
    }
}

//==============================================================================
// Process Queue
//==============================================================================
void FileRenamer::processQueue()
{
    // Check Queue Index
    if (fileUtil && queueIndex >= 0 && queueIndex < renameQueue.count()) {
        qDebug() << "FileRenamer::processQueue - queueIndex: " << queueIndex;

        // Get Source File Name
        QString sourceFile = renameQueue[queueIndex]->source;
        // Get Target File Name
        QString targetFile = renameQueue[queueIndex]->target;

        // Rename File
        fileUtil->moveFile(sourceFile, targetFile);
    } else {

        // Check Queue Index
        if (queueIndex != -1) {
            // Clear Queue
            clearQueue();
            // Reset Queue Index
            queueIndex = -1;
        }

        // ...
    }
}

//==============================================================================
// Abort
//==============================================================================
void FileRenamer::abort()
{
    // Check File Util
    if (fileUtil) {
        // Check Status
        if (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTError  ) {
            qDebug() << "FileRenamer::abort";

            // Abort
            fileUtil->abort();
        }
    }
}

//==============================================================================
// Clear Queue
//==============================================================================
void FileRenamer::clearQueue()
{
    // Check Queue
    if (renameQueue.count() <= 0) {
        return;
    }

    qDebug() << "FileRenamer::clearQueue";

    // Go Thru Queue
    while (renameQueue.count() > 0) {
        // Get Queue Item
        TransferProgressModelItem* item = renameQueue.takeLast();
        // Delete Item
        delete item;
        item = NULL;
    }
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void FileRenamer::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "FileRenamer::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected) {
        // Check File Util
        if (fileUtil) {
            // Clear Options
            fileUtil->clearFileTransferOptions();
        }
        // Reset Queue Index
        queueIndex = 0;
        // Process Queue
        QTimer::singleShot(1, this, SLOT(processQueue()));
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void FileRenamer::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    qDebug() << "FileRenamer::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...
}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void FileRenamer::fileOpStarted(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aPath,
                                const QString& aSource,
                                const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void FileRenamer::fileOpSkipped(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aPath,
                                const QString& aSource,
                                const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Check Operation
    if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Increase Current Queue Index
        queueIndex++;
        // Process Queue
        processQueue();
    }

    // ...
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void FileRenamer::fileOpFinished(const unsigned int& aID,
                                 const QString& aOp,
                                 const QString& aPath,
                                 const QString& aSource,
                                 const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Check Operation - QUEUE
    if (aOp == DEFAULT_OPERATION_QUEUE) {

        // ... Do Nothing, Process Queue

    // Check Operation - MOVE
    } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {

        // Emit Finished Signal
        emit finished(aSource, aTarget);

        // Increase Current Queue Index
        queueIndex++;
    }

    // Process Queue
    processQueue();

    // ...
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void FileRenamer::fileOpAborted(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aPath,
                                const QString& aSource,
                                const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void FileRenamer::fileOpError(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aPath,
                              const QString& aSource,
                              const QString& aTarget,
                              const int& aError)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aError: " << aError;



    // Check Operation
    if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Init Confirmation Dialog
        ConfirmDialog confirmDialog;

        // Set Dialog Title
        confirmDialog.setConfirmTitle(tr(DEFAULT_ERROR_TITLE_MOVE_FILE));

        // Switch Error
        switch (aError) {
            default:
            case DEFAULT_ERROR_GENERAL: {
                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_MOVE_FILE));
                // Set Path
                confirmDialog.setPath(aSource);
            } break;

            case DEFAULT_ERROR_CANNOT_DELETE_SOURCE_DIR: {
                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_DELETE_SOURCE));
                // Set Path
                confirmDialog.setPath(aSource);
            } break;

            case DEFAULT_ERROR_CANNOT_DELETE_TARGET_DIR: {
                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_DELETE_TARGET));
                // Set Path
                confirmDialog.setPath(aTarget);
            } break;
        }

        // Configure Standard Buttons
        confirmDialog.configureButtons(QDialogButtonBox::Abort);

        // Add Button
        confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_RETRY), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_RETRY);
        confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIP), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIP);
        confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIPALL);

        // Exec Confirm Dialog
        confirmDialog.exec();

        // Get Action Index
        int actionIndex = confirmDialog.getActionIndex();

        // Check File Util
        if (fileUtil) {
            // Send User Response
            fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
        }
    }
}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void FileRenamer::fileOpNeedConfirm(const unsigned int& aID,
                                    const QString& aOp,
                                    const int& aCode,
                                    const QString& aPath,
                                    const QString& aSource,
                                    const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aCode: " << aCode;

    // Init Confirmation Dialog
    ConfirmDialog confirmDialog;

    // Set Dialog Title
    confirmDialog.setConfirmTitle(tr(DEFAULT_TITLE_CONFIRMATION));

    // Switch Code
    switch (aCode) {
        case DEFAULT_ERROR_TARGET_DIR_EXISTS:
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS_MERGE));
        break;
    }

    // Set Path
    confirmDialog.setPath(aPath);

    // Configure Standard Buttons
    confirmDialog.configureButtons(QDialogButtonBox::Abort);

    // Add Button
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_YES), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_YES);
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_YESTOALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_YESALL);
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_NO), QDialogButtonBox::RejectRole, DEFAULT_CONFIRM_NO);
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_NOTOALL), QDialogButtonBox::RejectRole, DEFAULT_CONFIRM_NOALL);

    // Exec Confirm Dialog
    confirmDialog.exec();

    // Get Action Index
    int actionIndex = confirmDialog.getActionIndex();

    // Check File Util
    if (fileUtil) {
        // Send User Response
        fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
    }
}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void FileRenamer::fileOpQueueItemFound(const unsigned int& aID,
                                       const QString& aOp,
                                       const QString& aPath,
                                       const QString& aSource,
                                       const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileRenamer::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Check Operation
    if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Insert Item
        insertItem(queueIndex, aSource, aTarget);
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileRenamer::~FileRenamer()
{
    // Abort
    abort();

    // Clear Queue
    clearQueue();

    // Check File Util
    if (fileUtil) {
        // Close
        fileUtil->close();
        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    qDebug() << "FileRenamer::~FileRenamer";
}




























//==============================================================================
// Constructor
//==============================================================================
DirScannerQueueItem::DirScannerQueueItem(const QString& aDirPath)
    : dirPath(aDirPath)
    , dirSize(0)
    , state(EDSSIdle)
{
}

//==============================================================================
// Destructor
//==============================================================================
DirScannerQueueItem::~DirScannerQueueItem()
{
}












//==============================================================================
// Constructor
//==============================================================================
DirScanner::DirScanner(QObject* aParent)
    : QObject(aParent)
    , fileUtil(NULL)
    , queueIndex(-1)
{
    qDebug() << "DirScanner::DirScanner";

}

//==============================================================================
// Scan Dir
//==============================================================================
void DirScanner::scanDir(const QString& aDirPath)
{
    qDebug() << "DirScanner::scanDir - aDirPath: " << aDirPath;

    // Add Item
    addItem(aDirPath);

    // Check File Util
    if (!fileUtil) {
        // Create File Util
        fileUtil = new RemoteFileUtilClient();

        connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(dirSizeScanProgress(uint,QString,quint64,quint64,quint64)), this, SLOT(fileOpDirSizeScanProgress(uint,QString,quint64,quint64,quint64)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
    }

    // Check File Util
    if (fileUtil) {
        // Check If Connected
        if (!fileUtil->isConnected()) {
            // Connect
            fileUtil->connectToFileServer();
        } else {
            // Check Queue Index
            if (queueIndex == -1) {
                // Set Queue Index
                queueIndex = 0;
                // Clear Options
                fileUtil->clearFileTransferOptions();
                // Process Queue
                QTimer::singleShot(1, this, SLOT(processQueue()));
            }
        }
    }
}

//==============================================================================
// Abort
//==============================================================================
void DirScanner::abort()
{
    // Check File Util
    if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTWaiting) ) {
        qDebug() << "DirScanner::abort";

        // Abort
        fileUtil->abort();
    }
}

//==============================================================================
// Add Item
//==============================================================================
void DirScanner::addItem(const QString& aDirPath)
{
    // Get Dir Path Index
    int dirPathIndex = findIndex(aDirPath);

    // Check Dir Path Index
    if (dirPathIndex >= 0) {

        // Already Added, skipping...

        return;
    }

    qDebug() << "DirScanner::addItem - aDirPath: " << aDirPath;

    // Create New Item
    DirScannerQueueItem* newItem = new DirScannerQueueItem(aDirPath);

    // Add To Queue
    scanQueue << newItem;
}

//==============================================================================
// Set Item State
//==============================================================================
void DirScanner::setItemState(const int& aIndex, const int& aState)
{
    // Check Index
    if (aIndex >= 0 && aIndex < scanQueue.count()) {
        qDebug() << "DirScanner::setItemState - aIndex: " << aIndex << " - aState: " << aState;
        // Set State
        scanQueue[aIndex]->state = (DirScanProgressState)aState;
    }
}

//==============================================================================
// Find Index
//==============================================================================
int DirScanner::findIndex(const QString& aDirPath)
{
    // Get Queue Count
    int sqiCount = scanQueue.count();

    // Go Thru Queue Items
    for (int i=0; i<sqiCount; ++i) {
        // Get Queue Item
        DirScannerQueueItem* item = scanQueue[i];
        // Check Dir Path
        if (item->dirPath == aDirPath) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Process Queue
//==============================================================================
void DirScanner::processQueue()
{
    // Check Queue Index
    if (fileUtil && queueIndex >= 0 && queueIndex < scanQueue.count()) {
        qDebug() << "FileRenamer::processQueue - queueIndex: " << queueIndex;
        // Get Dir Path
        QString dirPath = scanQueue[queueIndex]->dirPath;
        // Scan Dir
        fileUtil->scanDirSize(dirPath);
    } else {
        // Check Queue Index
        if (queueIndex != -1) {
            // Clear Queue
            clearQueue();
            // Reset Queue Index
            queueIndex = -1;
        }
    }
}

//==============================================================================
// Clear Queue
//==============================================================================
void DirScanner::clearQueue()
{
    qDebug() << "DirScanner::clearQueue";

    // While Queue Is Not Empty
    while (scanQueue.count() > 0) {
        // Get Scan Queue Item
        DirScannerQueueItem* item = scanQueue.takeLast();
        // Delete Item
        delete item;
        item = NULL;
    }
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void DirScanner::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "DirScanner::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected) {
        // Check File Util
        if (fileUtil) {
            // Clear Options
            fileUtil->clearFileTransferOptions();
        }
        // Reset Queue Index
        queueIndex = 0;
        // Process Queue
        QTimer::singleShot(1, this, SLOT(processQueue()));
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void DirScanner::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    qDebug() << "DirScanner::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...
}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void DirScanner::fileOpStarted(const unsigned int& aID,
                   const QString& aOp,
                   const QString& aPath,
                   const QString& aSource,
                   const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DirScanner::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;


    // ...
}

//==============================================================================
// Dir Size Scan Progress Signal
//==============================================================================
void DirScanner::fileOpDirSizeScanProgress(const unsigned int& aID,
                                           const QString& aPath,
                                           const quint64& aNumDirs,
                                           const quint64& aNumFiles,
                                           const quint64& aScannedSize)
{
    Q_UNUSED(aID);
    Q_UNUSED(aNumDirs);
    Q_UNUSED(aNumFiles);

    // Find Index
    int dirIndex = findIndex(aPath);

    // Check Queue Index
    if (dirIndex >= 0 && dirIndex < scanQueue.count()) {
        //qDebug() << "DirScanner::fileOpDirSizeScanProgress - aID: " << aID << " - aPath: " << aPath << " - aNumDirs: " << aNumDirs << " - aNumFiles: " << aNumFiles << " - aScannedSize: " << aScannedSize;

        // Get Item
        DirScannerQueueItem* item = scanQueue[dirIndex];
        // Update Scan Queue Item
        item->dirSize = aScannedSize;

        // Emit Scan Size Changed Signal
        emit scanSizeChanged(item->dirPath, item->dirSize);
    }
}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void DirScanner::fileOpSkipped(const unsigned int& aID,
                   const QString& aOp,
                   const QString& aPath,
                   const QString& aSource,
                   const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DirScanner::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SCAN_DIR) {
        // Inc Queue Index
        queueIndex++;

        // Process Queue
        processQueue();
    }
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void DirScanner::fileOpFinished(const unsigned int& aID,
                    const QString& aOp,
                    const QString& aPath,
                    const QString& aSource,
                    const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DirScanner::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SCAN_DIR) {
        // Inc Queue Index
        queueIndex++;
        // Process Queue
        processQueue();
    }
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void DirScanner::fileOpAborted(const unsigned int& aID,
                   const QString& aOp,
                   const QString& aPath,
                   const QString& aSource,
                   const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DirScanner::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void DirScanner::fileOpError(const unsigned int& aID,
                 const QString& aOp,
                 const QString& aPath,
                 const QString& aSource,
                 const QString& aTarget,
                 const int& aError)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DirScanner::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SCAN_DIR) {
        // Init Confirmation Dialog
        ConfirmDialog confirmDialog;

        // Set Dialog Title
        confirmDialog.setConfirmTitle(tr(DEFAULT_ERROR_TITLE_SCAN_DIR));

        // Switch Error
        switch (aError) {
            default:
            case DEFAULT_ERROR_GENERAL: {
                // Set Error Text
                confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_CANT_SCAN_DIR));
                // Set Path
                confirmDialog.setPath(aSource);
            } break;
        }


        // Configure Standard Buttons
        confirmDialog.configureButtons(QDialogButtonBox::Abort);

        // Add Button
        confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_RETRY), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_RETRY);
        confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIP), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIP);
        confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIPALL);

        // Exec Confirm Dialog
        confirmDialog.exec();

        // Get Action Index
        int actionIndex = confirmDialog.getActionIndex();

        // Check File Util
        if (fileUtil) {
            // Send User Response
            fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
DirScanner::~DirScanner()
{
    // Abort
    abort();
    // Clear Queue
    clearQueue();

    qDebug() << "DirScanner::~DirScanner";
}
