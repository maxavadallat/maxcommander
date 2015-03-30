#ifndef AUDIOTAGIMAGEPROVIDER_H
#define AUDIOTAGIMAGEPROVIDER_H

#include <QQuickImageProvider>


//==============================================================================
// Audio Tag Image Provider Class
//==============================================================================
class AudioTagImageProvider : public QQuickImageProvider
{
public:

    // Constructor
    AudioTagImageProvider();

    // Destructor
    virtual ~AudioTagImageProvider();

protected:

    // Request Image
    virtual QImage requestImage(const QString& aID, QSize* aSize, const QSize& aRequestedSize);

protected:

    // Image Width
    int                 imageWidth;
    // Image Height
    int                 imageHeight;

};

#endif // AUDIOTAGIMAGEPROVIDER_H
