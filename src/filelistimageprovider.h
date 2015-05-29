#ifndef FILELISTIMAGEPROVIDER_H
#define FILELISTIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>

class SettingsController;
class SettingsReciever;


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
    friend class SettingsReciever;

    // Icon Width
    int                 iconWidth;
    // Icon Height
    int                 iconHeight;

    // Supported Image Formats
    QList<QByteArray>   supportedFormats;

    // Settings Controller
    SettingsController* settings;

    // Use Default Images
    bool                useDefaultIcons;

    // Settings Reciever
    SettingsReciever*   settingsReciever;
};







//==============================================================================
// Settings Reciever Class
//==============================================================================
class SettingsReciever : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit SettingsReciever(FileListImageProvider* aParent);

    // Destructor
    virtual ~SettingsReciever();

protected slots:

    // Use Default Icons Changed Slot
    void useDefaultIconsChanged(const bool& aUseDefaultIcons);

protected:
    // Parent
    FileListImageProvider* ipParent;
};







#endif // FILELISTIMAGEPROVIDER_H

