#ifndef FILELISTIMAGEPROVIDER_H
#define FILELISTIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QFileIconProvider>

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

    // Get Settings Reciever
    SettingsReciever* reciever();

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

    // Thumb Width
    int                 thumbWidth;
    // Thumb Height
    int                 thumbHeight;

    // Grid Mode
    bool                gridMode;

    // Supported Image Formats
    QList<QByteArray>   supportedFormats;

    // Settings Controller
    SettingsController* settings;

    // Use Default Images
    bool                useDefaultIcons;

    // Settings Reciever
    SettingsReciever*   settingsReciever;

    // File Icon Provider
    QFileIconProvider   fileIconProvider;
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

    // Thumb Width Changed Slot
    void thumbWidthChanged(const int& aWidth);
    // Thumb Height Changed Slot
    void thumbHeightChanged(const int& aHeight);

    // Grid Thumb Width Changed Slot
    void gridThumbWidthChanged(const int& aWidth);
    // Grid Thumb Height Changed Slot
    void gridThumbHeightChanged(const int& aHeight);

    // Grid Mode Changed Slot
    void gridModeChanged(const bool& aGridMode);

protected:
    // Parent
    FileListImageProvider* ipParent;
};







#endif // FILELISTIMAGEPROVIDER_H

