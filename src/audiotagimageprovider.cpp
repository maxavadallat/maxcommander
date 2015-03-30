#include <QDebug>

#include "audiotagimageprovider.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
AudioTagImageProvider::AudioTagImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
    , imageWidth(DEFAULT_AUDIO_TAG_IMAGE_WIDTH)
    , imageHeight(DEFAULT_AUDIO_TAG_IMAGE_HEIGHT)
{
    qDebug() << "AudioTagImageProvider::AudioTagImageProvider";

    // ...
}

//==============================================================================
// Request Image
//==============================================================================
QImage AudioTagImageProvider::requestImage(const QString& aID, QSize* aSize, const QSize& aRequestedSize)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSize);
    Q_UNUSED(aRequestedSize);

    qDebug() << "FileListImageProvider::requestImage - aID: " << aID << " - aRequestedSize: " << aRequestedSize;

    // Init File Name
    QString fileName = aID;


    return QImage(":/resources/images/default-audio-icon.png");
}

//==============================================================================
// Destructor
//==============================================================================
AudioTagImageProvider::~AudioTagImageProvider()
{

    // ...

    qDebug() << "AudioTagImageProvider::~AudioTagImageProvider";
}

