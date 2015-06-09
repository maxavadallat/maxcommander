#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>

namespace Ui {
class ViewerWindow;
}

class ConfirmDialog;
class RemoteFileUtilClient;
class ImageBrowser;


//==============================================================================
// Internal File Viewer Window Class
//==============================================================================
class ViewerWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(QString contentSource READ getContentSource NOTIFY contentSourceChanged)

public:
    // Constructor
    explicit ViewerWindow(QWidget* aParent = NULL);

    // Get Content Source
    QString getContentSource();

    // Get Is New File
    bool isNewFile();
    // Get Is File Modified
    bool isFileModified();

    // Destructor
    virtual ~ViewerWindow();

public slots:

    // New File
    bool newFile(const QString& aDirPath);
    // Load File
    bool loadFile(const QString& aFileName, const QString& aPanelName);
    // Save File
    void saveFile(const bool& aShowFileDialog = true);
    // Save File As
    void saveFileAs(const QString& aFileName);

    // Show Window
    void showWindow();
    // Close
    void closeWindow();

    // Set Edit Mode Enabled
    void setEditModeEnabled(const bool& aEnabled);

signals:

    // Viewer Closed Signal
    void viewerClosed(ViewerWindow* aViewer);

    // Content Source changed Signal
    void contentSourceChanged(const QString& aContentSource);

protected slots:

    // Init
    void init();
    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();

    // Toggle Wrap Mode
    void toggleWrapMode();

    // Is Mime Supported As Text
    bool isSupportedTextMime(const bool& aEditMode, const QString& aMime);

    // Update Window Title
    void updateWindowTitle();

    // Update Menu Bar
    void updateMenuBar();

protected slots: // For QTextEdit

    // Text Changed Slot
    void textChanged();

protected slots: // For Image Browser

    // Current Index Changed Slot
    void imageBrowserCurrentIndexChanged(const int& aCurrentIndex);
    // Current File Changed Slot
    void imageBrowserCurrentFileChanged(const QString& aCurrentFile);

protected: // From QWidget

    // Close Event
    virtual void closeEvent(QCloseEvent* aEvent);
    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

protected slots: // Menu Actions

    // On Action Save Triggered Slot
    void on_actionSave_triggered();
    // On Action Save As Triggered Slot
    void on_actionSave_As_triggered();
    // On Action Close Triggered Slot
    void on_actionClose_triggered();

private:

    // UI
    Ui::ViewerWindow*       ui;
    // File Name
    QString                 fileName;
    // Current Dir
    QString                 currentDir;
    // Edit Mode
    bool                    editMode;
    // Dirty
    bool                    dirty;
    // File Modified
    bool                    fileModified;
    // Mime
    QString                 mime;
    // New File
    bool                    fileIsNew;

    // Image Browser
    ImageBrowser*           imageBrowser;
};

















//==============================================================================
// Image Browser Class
//==============================================================================
class ImageBrowser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentFile READ getCurrentFile NOTIFY currentFileChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex NOTIFY currentIndexChanged)

public:

    // Constructor
    explicit ImageBrowser(const QString& aFileName, const QString& aPanelName, QObject* aParent = NULL);

    // Get Current File
    QString getCurrentFile();
    // Get Current Index
    int getCurrentIndex();

    // Go To First
    void gotoFirst();
    // Go To Prev
    void gotoPrev();
    // Go To Next
    void gotoNext();
    // Go To Last
    void gotoLast();

    // Destructor
    virtual ~ImageBrowser();

signals:

    // Current Index Changed Signal
    void currentIndexChanged(const int& aCurrentIndex);
    // Current File Changed Signal
    void currentFileChanged(const QString& aCurrentFile);

protected slots:

    // Init
    void init();

    // Find Index By File Name
    int findIndex(const QString& aFileName);

    // Set Current Index
    void setCurrentIndex(const int& aIndex);

protected slots: // From Remote File Util

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Slot
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // Dir List Item Found Slot
    void dirListItemFound(const unsigned int& aID,
                          const QString& aPath,
                          const QString& aFileName);

protected:

    // File Util
    RemoteFileUtilClient*   fileUtil;
    // Panel Name
    QString                 panelName;
    // Image Files
    QStringList             imageFiles;
    // Current Index
    int                     currentIndex;
    // Current File
    QString                 currentFile;
    // Current Dir
    QString                 currentDir;
};


#endif // VIEWERWINDOW_H

