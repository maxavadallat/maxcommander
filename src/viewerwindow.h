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

    // Destructor
    virtual ~ViewerWindow();

public slots:

    // Load File
    void loadFile(const QString& aFileName);
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

protected slots: // For QTextEdit

    // Text Changed Slot
    void textChanged();

protected: // From QWidget

    // Close Event
    virtual void closeEvent(QCloseEvent* aEvent);
    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

private:
    // UI
    Ui::ViewerWindow*       ui;
    // File Name
    QString                 fileName;
    // Edit Mode
    bool                    editMode;
    // Dirty
    bool                    dirty;
    // Mime
    QString                 mime;
};

#endif // VIEWERWINDOW_H

