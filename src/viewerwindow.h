#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QString>
#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class ViewerWindow;
}

class ConfirmDialog;
class RemoteFileUtilClient;

//==============================================================================
// Internal File Viewer Window Class
//==============================================================================
class ViewerWindow : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit ViewerWindow(QWidget* aParent = NULL);

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

protected slots:

    // Init
    void init();
    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();

protected slots: // For QTextEdit

    // Text Changed Slot
    void textChanged();

protected: // From QWidget

    // Close Event
    virtual void closeEvent(QCloseEvent* aEvent);

private:
    // UI
    Ui::ViewerWindow*       ui;
    // File Name
    QString                 fileName;
    // Edit Mode
    bool                    editMode;
    // Dirty
    bool                    dirty;
};

#endif // VIEWERWINDOW_H
