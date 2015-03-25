#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>

#include "filelistimageprovider.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
FileListImageProvider::FileListImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
    , iconWidth(DEFAULT_ICON_WIDTH_32)
    , iconHeight(DEFAULT_ICON_HEIGHT_32)
{
    // Get Settings

    // Get Supported Image Formats
    supportedFormats = QImageReader::supportedImageFormats();

    // Move To Thread
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
    if (aID == QString("//dev")) {
        // Update File Name
        fileName = "/etc";
    }

    // Check ID
    if (aID.startsWith("/dev")) {
        // Update File Name
        fileName = "/.file";
    }

#endif // Q_OS_MACX

    // Get File Icon Image
    QImage image = getFileIconImage(fileName, iconWidth, iconHeight);

    // Check Image
    if (!image.isNull()) {
        return image;
    }

    return QImage(":/resources/images/icons/default_file.png");
}

//==============================================================================
// Destructor
//==============================================================================
FileListImageProvider::~FileListImageProvider()
{
}

