#include <QImage>
#include <QDebug>

#include "filelistimageprovider.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
FileListImageProvider::FileListImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
{

}



//==============================================================================
// Request Image
//==============================================================================
QImage FileListImageProvider::requestImage(const QString& aID, QSize* aSize, const QSize& aRequestedSize)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSize);
    Q_UNUSED(aRequestedSize);

    //qDebug() << "FileListImageProvider::requestImage - aID: " << aID;

    // Init Default Image
    QImage image(":/resources/images/icons/default_file.png");

    // Check Image
    if (!image.isNull()) {
        return image;
    }

    return QImage();
}

//==============================================================================
// Destructor
//==============================================================================
FileListImageProvider::~FileListImageProvider()
{

}

