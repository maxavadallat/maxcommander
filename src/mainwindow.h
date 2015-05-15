#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class FilePanel;
class RemoteFileUtilClient;
class AboutDialog;
class ConfirmDialog;
class PreferencesDialog;
class CreateDirDialog;
class DeleteFileDialog;
class TransferFileDialog;
class ViewerWindow;
class HelpWindow;
class DeleteProgressDialog;
class TransferProgressDialog;
class SearchDialog;


//==============================================================================
// Main Window Class
//==============================================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // Get Main Window Instance
    static MainWindow* getInstance();

    // Show Window
    void showWindow();

    // Release Instance
    void release();

public slots:

    // Show About Slot
    void showAbout();
    // Show Help Slot
    void showHelp();

    // Launch Terminal Slot
    void launchTerminal(const QString& aDirPath);

    // Launch Viewer
    void launchViewer(const bool& aEditMode = false, const bool& aNewFile = false);

    // Launch File Copy Slot
    void launchFileCopy();
    // Launch File Move/Rename Slot
    void launchFileMove();

    // Launch Create Dir Slot
    void launchCreateDir();

    // Launch Search Slot
    void launchSearch();

    // Launch Create Link Slot
    void launchCreateLink();

    // Launch Delete Slot
    void launchDelete();

    // Show Preferences Slot
    void showPreferences();

    // Quit Application Slot
    void quitApp();

    // Shut Down
    void shutDown();


protected:

    // Constructor
    explicit MainWindow(QWidget* aParent = NULL);

    // Init
    void init();

    // Restore UI
    void restoreUI(const bool& aReload = true, const int& aFocusedPanel = 0);

    // Load Settings
    void loadSettings();

    // Save Settings
    void saveSettings();

    // Destructor
    virtual ~MainWindow();

protected slots:

    // Modifier Keys Changed Slot
    void modifierKeysChanged(const int& aModifiers);

    // Update Function Keys Slot
    void updateFunctionKeys();

    // Update Menu Slot
    void updateMenu();

    // Toggle Hidden Files Slot
    void toggleHiddenFile();

    // Viewer Window Closed Slot
    void viewerWindowClosed(ViewerWindow* aViewer);
    // Delete Progress Window Closed Slot
    void deleteProgressClosed(DeleteProgressDialog* aDeleteProgressDialog);
    // Transfer Progress Window Closed Slot
    void transferProgressClosed(TransferProgressDialog* aTransferProgressDialog);

    // Focused Panel Changed Slot
    void focusedPanelChanged(FilePanel* aFocusedPanel);

    // Launch Transfer Slot
    void launchTransfer(const QString& aOperation);

    // Settings Has Changed Slot
    void settingsHasChanged();

protected slots: // Actions & Button

    // Help Button Clicked Slot
    void on_helpButton_clicked();
    // Help Button Clicked Slot
    void on_terminalButton_clicked();
    // Help Button Clicked Slot
    void on_viewButton_clicked();
    // Help Button Clicked Slot
    void on_editButton_clicked();
    // Help Button Clicked Slot
    void on_copyButton_clicked();
    // Help Button Clicked Slot
    void on_moveButton_clicked();
    // Help Button Clicked Slot
    void on_makeDirButton_clicked();
    // Help Button Clicked Slot
    void on_delButton_clicked();
    // Help Button Clicked Slot
    void on_optionsButton_clicked();
    // Help Button Clicked Slot
    void on_exitButton_clicked();

    // Action About Triggered Slot
    void on_actionAbout_triggered();
    // Action About Triggered Slot
    void on_actionPreferences_triggered();
    // Action About Triggered Slot
    void on_actionQuit_triggered();
    // Action About Triggered Slot
    void on_actionHelp_triggered();
    // Action About Triggered Slot
    void on_actionOptions_triggered();
    // Action About Triggered Slot
    void on_actionCompare_Files_triggered();
    // Action About Triggered Slot
    void on_actionSelect_all_triggered();
    // Action About Triggered Slot
    void on_actionSelect_None_triggered();
    // Action About Triggered Slot
    void on_actionTerminal_triggered();

    // Action About Triggered Slot
    void on_actionSort_by_Name_triggered();
    // Action About Triggered Slot
    void on_actionSort_by_Extension_triggered();
    // Action About Triggered Slot
    void on_actionSort_by_Type_triggered();
    // Action About Triggered Slot
    void on_actionSort_by_Size_triggered();
    // Action About Triggered Slot
    void on_actionSort_by_Date_triggered();
    // Action About Triggered Slot
    void on_actionSort_by_Owner_triggered();
    // Action Sort By Permission Triggered Slot
    void on_actionSort_by_Permissions_triggered();

    // Action About Triggered Slot
    void on_actionShow_Hide_Hiden_triggered();
    // Action About Triggered Slot
    void on_actionNew_Directory_triggered();
    // Action About Triggered Slot
    void on_actionNew_File_triggered();
    // Action About Triggered Slot
    void on_actionDelete_File_triggered();
    // Action About Triggered Slot
    void on_actionCreate_Link_triggered();

    // Action Reload Triggered Slot
    void on_actionReload_triggered();

    // Action View Triggered Slot
    void on_actionView_triggered();
    // Action Edit Triggered Slot
    void on_actionEdit_triggered();

    // Action Copy File Triggered Slot
    void on_actionCopy_triggered();
    // Action Move File Triggered Slot
    void on_actionMove_triggered();

    // Action Exit Triggered Slot
    void on_actionExit_triggered();

private:

    // UI
    Ui::MainWindow*                 ui;
    // Action Group
    QActionGroup*                   viewMenuActionGroup;

    // Left Drive Buttons - Windows

    // Right Drive Buttons - Windows

    // Left Panel
    FilePanel*                      leftPanel;

    // Right Panel
    FilePanel*                      rightPanel;

    // Focused Panel
    FilePanel*                      focusedPanel;

    // Modifier Keys Pressed
    int                             modifierKeys;

    // Dir List Counter
    int                             dirListCounter;

    // About Dialog
    AboutDialog*                    aboutDialog;
    // Preferences Dialog
    PreferencesDialog*              preferencesDialog;
    // Create Dir Dialog
    CreateDirDialog*                createDirDialog;
    // Delete File Dialog
    DeleteFileDialog*               deleteFileDialog;
    // Transfer File Dialog
    TransferFileDialog*             transferFileDialog;
    // Help Window
    HelpWindow*                     helpWindow;
    // Search File Dialog
    SearchDialog*                   searchFileDialog;

    // Transfer Progress Dialogs
    QList<TransferProgressDialog*>  transferProgressDialogs;
    // Delete Progress Dialogs
    QList<DeleteProgressDialog*>    deleteProgressDialogs;
    // Viewer Windows
    QList<ViewerWindow*>            viewerWindows;

};

#endif // MAINWINDOW_H
