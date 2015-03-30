#ifndef FILELISTIMAGEPROVIDER_H
#define FILELISTIMAGEPROVIDER_H

#include <QQuickImageProvider>

//==============================================================================
// File List Image Provider Class
//==============================================================================
class FileListImageProvider : public QQuickImageProvider
{
public:
    // Constructor
    explicit FileListImageProvider();

    // Destructor
    virtual ~FileListImageProvider();

protected: // From QQuickImageProvider

    // Request Image
    virtual QImage requestImage(const QString& aID, QSize* aSize, const QSize& aRequestedSize);

protected: // Data

    // Icon Width
    int                 iconWidth;
    // Icon Height
    int                 iconHeight;

    // Supported Image Formats
    QList<QByteArray>   supportedFormats;
};

#endif // FILELISTIMAGEPROVIDER_H

