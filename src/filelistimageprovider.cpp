#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>

#include "filelistimageprovider.h"
#include "settingscontroller.h"
#include "utility.h"
#include "defaultsettings.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
FileListImageProvider::FileListImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
    , iconWidth(DEFAULT_ICON_WIDTH_32)
    , iconHeight(DEFAULT_ICON_HEIGHT_32)
    , thumbWidth(DEFAULT_THUMB_WIDTH_300)
    , thumbHeight(DEFAULT_THUMB_HEIGHT_200)
    , gridMode(false)
    , settings(SettingsController::getInstance())
    , useDefaultIcons(DEAFULT_SETTINGS_USE_DEFAULT_ICONS)
    , settingsReciever(new SettingsReciever(this))
{
    // Get Supported Image Formats
    supportedFormats = QImageReader::supportedImageFormats();

    // Get Use Default Icons
    useDefaultIcons = settings->getUseDefaultIcons();

    // Connect Signals
    QObject::connect(settings, SIGNAL(useDefaultIconsChanged(bool)), settingsReciever, SLOT(useDefaultIconsChanged(bool)));
    QObject::connect(settings, SIGNAL(thumbWidthChanged(int)), settingsReciever, SLOT(thumbWidthChanged(int)));
    QObject::connect(settings, SIGNAL(thumbHeightChanged(int)), settingsReciever, SLOT(thumbHeightChanged(int)));
    QObject::connect(settings, SIGNAL(gridThumbWidthChanged(int)), settingsReciever, SLOT(gridThumbWidthChanged(int)));
    QObject::connect(settings, SIGNAL(gridThumbHeightChanged(int)), settingsReciever, SLOT(gridThumbHeightChanged(int)));



    // ...
}

//==============================================================================
// Get Settings Reciever
//==============================================================================
SettingsReciever* FileListImageProvider::reciever()
{
    return settingsReciever;
}

//==============================================================================
// Request Image
//==============================================================================
QImage FileListImageProvider::requestImage(const QString& aID, QSize* aSize, const QSize& aRequestedSize)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSize);
    Q_UNUSED(aRequestedSize);

    //qDebug() << "FileListImageProvider::requestImage - aID: " << aID << " - aRequestedSize: " << aRequestedSize;

    // Init File Name
    QString fileName = aID;

#if defined(Q_OS_MACX)

    // Check ID - Mac OSX HACK!!! X ((
    if (fileName == DEFAULT_FILE_LIST_DEFAULT_ICON_NAME_DIR || aID == QString("//dev") ) {
        // Update File Name
        fileName = "/etc";
    }

    // Check ID
    if (fileName == DEFAULT_FILE_LIST_DEFAULT_ICON_NAME_FILE || aID.startsWith("/dev")) {
        // Update File Name
        fileName = "/.file";
    }

#endif // Q_OS_MACX

    // Check Use Default Icons
    if (useDefaultIcons) {
        // Init File Info
        QFileInfo fileInfo(fileName);

        // Check If Is Dir
        if (fileInfo.isDir()) {
            return QImage(DEFAULT_FILE_ICON_DIR);
        }

        return QImage(DEFAULT_FILE_ICON_FILE);
    }

    // Get File Icon Image
    QImage image = getFileIconImage(fileName, gridMode ? thumbHeight : iconWidth, gridMode ? thumbHeight : iconHeight);

    // Check Image
    if (!image.isNull()) {
        return image;
    }

    return QImage(DEFAULT_FILE_ICON_FILE);
}

//==============================================================================
// Destructor
//==============================================================================
FileListImageProvider::~FileListImageProvider()
{
    // Check Settings Reciever
    if (settingsReciever) {
        // Delete Settings Reciever
        delete settingsReciever;
        // Reset Settings Reciever
        settingsReciever = NULL;
    }

    // Check Settings
    if (settings) {
        // Release
        settings->release();
        // Reset Settings
        settings = NULL;
    }
}











//==============================================================================
// Constructor
//==============================================================================
SettingsReciever::SettingsReciever(FileListImageProvider* aParent)
    : QObject(NULL)
    , ipParent(aParent)
{
}

//==============================================================================
// Use Default Icons Changed Slot
//==============================================================================
void SettingsReciever::useDefaultIconsChanged(const bool& aUseDefaultIcons)
{
    //qDebug() << "SettingsReciever::useDefaultIconsChanged - aUseDefaultIcons: " << aUseDefaultIcons;

    // Set Use Default Image
    ipParent->useDefaultIcons = aUseDefaultIcons;
}

//==============================================================================
// Thumb Width Changed Slot
//==============================================================================
void SettingsReciever::thumbWidthChanged(const int& aWidth)
{
    // Set Icon Width
    ipParent->iconWidth = aWidth;
}

//==============================================================================
// Thumb Height Changed Slot
//==============================================================================
void SettingsReciever::thumbHeightChanged(const int& aHeight)
{
    // Set Icon Height
    ipParent->iconHeight = aHeight;
}

//==============================================================================
// Grid Thumb Width Changed Slot
//==============================================================================
void SettingsReciever::gridThumbWidthChanged(const int& aWidth)
{
    // Set Thumb Width
    ipParent->thumbWidth = aWidth;
}

//==============================================================================
// Grid Thumb Height Changed Slot
//==============================================================================
void SettingsReciever::gridThumbHeightChanged(const int& aHeight)
{
    // Set Thumb Height
    ipParent->thumbWidth = aHeight;
}

// Grid Mode Changed Slot
void SettingsReciever::gridModeChanged(const bool& aGridMode)
{
    // Set Grid Mode
    ipParent->gridMode = aGridMode;
}

//==============================================================================
// Destructor
//==============================================================================
SettingsReciever::~SettingsReciever()
{
}


