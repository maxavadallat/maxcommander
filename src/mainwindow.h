#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// INCLUDES

#include <QDebug>
#include <QMainWindow>
#include <QDialogButtonBox>

#include "constants.h"

// FORWARD DECLARATIONS

namespace Ui {
class MainWindow;
}

class Settings;
class CustomPanel;
class CustomFileList;
class CustomHeader;
class CreateDirDialog;
class InfoDialog;
class ConfirmationDialog;
class SearchDialog;
class CopyDialog;
class CopyProgressDialog;
class DeleteProgressDialog;
class ViewerWindow;
class HelpDialog;
class TreeWindow;
class AboutDialog;

// DECLARATIONS

//==============================================================================
//! @class MainWindow Application Main Window Class
//==============================================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //! @brief Get Main Window Instance - Static Constructor
    //! @param none
    static MainWindow* getInstance();

    //! @brief Release Instance
    //! @param none
    void release();

    //! @brief Launch Help
    //! @param none
    void launchHelp();

    //! @brief Show tree
    //! @param none
    void showTree();

    //! @brief View File
    //! @param none
    void viewFile();

    //! @brief Edit File
    //! @param none
    void editFile();

    //! @brief Copy Files
    //! @param none
    void copyFiles();

    //! @brief Move Files
    //! @param none
    void moveFiles();

    //! @brief Launch Rename Current File
    //! @param none
    void launchRenameCurrentFile();

    //! @brief Launch Create Dir
    //! @param none
    void launchCreateDir();

    //! @brief Launch Delete Files
    //! @param none
    void launchDelete();

    //! @brief Launch Copy
    //! @param none
    void launchCopy();

    //! @brief Launch Move
    //! @param none
    void launchMove();

    //! @brief Launch Search
    //! @param none
    void launchSearch();

    //! @brief Launch Options
    //! @param none
    void launchOptions();

    //! @brief Exit Application
    //! @param none
    void exitApp();

    //! @brief Switch To Panel
    //! @param aIndex Panel Index
    void switchToPanel(const int& aIndex);

    //! @brief Load Settings
    //! @param none
    void loadSettings();

    //! @brief Save Settings
    //! @param none
    void saveSettings();

    //! @brief Show Info Dialog
    //! @param aTitle Dialog Title Text
    //! @param aInfoMsg Dialog Info Text
    //! @param aModal Dialog Modal State
    void showInfo(const QString& aTitle, const QString& aInfoMsg, const bool& aModal = false);

    //! @brief Show Confirmation Dialog
    //! @param aTitle Dialog Title Text
    //! @param aConfirmMsg Dialog Confirmation Text
    //! @param aButtons Dialog Buttons To Use
    //! @return Confirmation Index
    int showConfirmation(const QString& aTitle, const QString& aConfirmMsg, const QDialogButtonBox::StandardButtons& aButtons, const QDialogButtonBox::StandardButton& aDefault = QDialogButtonBox::NoButton);

    //! @brief Show Status Message
    //! @param aMessage Message
    //! @param aTimeOut Message Timeout
    void showStatusMessage(const QString& aMessage, const int& aTimeOut = DEFAULT_STATUS_MESSAGE_TIMEOUT);

    //! @brief Clear Status Message
    //! @param none
    void clearStatusMessage();

protected:

    //! @brief Constructor
    //! @param aParent Parent Widget
    MainWindow(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~MainWindow();

    //! @brief Show Create Dir Dialog
    //! @param aDirPath Initial Directory Path
    //! @return true if Dir Creation Confirmed
    bool showCreateDirDialog();

    //! @brief Configure Function Keys
    //! @param none
    void configureFunctionKeys();

protected slots:

    //! @brief Panel Focus Changed Slot
    //! @param aPanelName Panel Name
    //! @param aFocused Panel Focused State
    void panelFocusChanged(const QString& aPanelName, const bool& aFocused);

    //! @brief Panel Key Pressed Slot
    //! @param aPanelName Panel Name
    //! @param aKey Key Code
    //! @param aModifiers Key Modifiers
    void panelKeyPressed(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief Panel Key Released Slot
    //! @param aPanelName Panel Name
    //! @param aKey Key Code
    //! @param aModifiers Key Modifiers
    void panelKeyReleased(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief F1 Button Clicked Slot
    //! @param none
    void on_f1Button_clicked();

    //! @brief F2 Button Clicked Slot
    //! @param none
    void on_f2Button_clicked();

    //! @brief F3 Button Clicked Slot
    //! @param none
    void on_f3Button_clicked();

    //! @brief F4 Button Clicked Slot
    //! @param none
    void on_f4Button_clicked();

    //! @brief F5 Button Clicked Slot
    //! @param none
    void on_f5Button_clicked();

    //! @brief F6 Button Clicked Slot
    //! @param none
    void on_f6Button_clicked();

    //! @brief F7 Button Clicked Slot
    //! @param none
    void on_f7Button_clicked();

    //! @brief F8 Button Clicked Slot
    //! @param none
    void on_f8Button_clicked();

    //! @brief F9 Button Clicked Slot
    //! @param none
    void on_f9Button_clicked();

    //! @brief F10 Button Clicked Slot
    //! @param none
    void on_f10Button_clicked();

    //! @brief Action Preferences Triggered Slot
    //! @param none
    void on_actionPreferences_triggered();

    //! @brief Action New Triggered Slot
    //! @param none
    void on_actionNew_triggered();

    //! @brief Action Quit Triggered Slot
    //! @param none
    void on_actionQuit_triggered();

    //! @brief Action Select All Triggered Slot
    //! @param none
    void on_actionSelectAll_triggered();

    //! @brief Action Reload Panel Triggered Slot
    //! @param none
    void on_actionReloadPanel_triggered();

    //! @brief Action Swap Panels Triggered Slot
    //! @param none
    void on_actionSwapPanels_triggered();

    //! @brief Action Show Hidden Triggered Slot
    //! @param none
    void on_actionShowHidden_triggered();

    //! @brief Action About Triggered Slot
    //! @param none
    void on_actionAbout_triggered();

    //! @brief Action Miniized Triggered Slot
    //! @param none
    void on_actionMinimize_triggered();

    //! @brief Action Restore Triggered Slot
    //! @param none
    void on_actionRestore_triggered();

    //! @brief Action Show Toolbar Triggered Slot
    //! @param none
    void on_actionShowToolbar_triggered();

    //! @brief Action Show Button Bar Triggered Slot
    //! @param none
    void on_actionShowButtonBar_triggered();

    //! @brief Action Show Status Bar Triggered Slot
    //! @param none
    void on_actionShowStatusBar_triggered();

    //! @brief Action Zoom In Triggered Slot
    //! @param none
    void on_actionZoomIn_triggered();

    //! @brief Action Zoom Out Triggered Slot
    //! @param none
    void on_actionZoomOut_triggered();

    //! @brief Action Zoom To Normal Triggered Slot
    //! @param none
    void on_actionZoomToNormal_triggered();

    //! @brief Action Help Triggered Slot
    //! @param none
    void on_actionHelp_triggered();

    //! @brief Action Check Updates Triggered Slot
    //! @param none
    void on_actionCheckUpdates_triggered();

    //! @brief Action Copy Triggered Slot
    //! @param none
    void on_actionCopy_triggered();

    //! @brief Action Cut Triggered Slot
    //! @param none
    void on_actionCut_triggered();

    //! @brief Action Paste Triggered Slot
    //! @param none
    void on_actionPaste_triggered();

    //! @brief Action Comapre Triggered Slot
    //! @param none
    void on_actionCompare_triggered();

    //! @brief Action Pack Triggered Slot
    //! @param none
    void on_actionPack_triggered();

    //! @brief Action Unpack Triggered Slot
    //! @param none
    void on_actionUnpack_triggered();

protected: // Data

    // Reference Count
    int                 refCount;
    //! UI
    Ui::MainWindow*     ui;
    //! Shift Key Pressed
    bool                shiftKeyPressed;
    //! Alt Key Pressed
    bool                altKeyPressed;
    //! Control Key Pressed
    bool                controlKeyPressed;
    //! Meta Key Pressed
    bool                metaKeyPressed;
    //! Current Panel Index
    int                 cPanelIndex;
    //! Current Panel Name
    QString             cPanelName;
    //! Settings
    Settings*           settings;
    //! Viewer Window
    ViewerWindow*       viewerWindow;
    //! Create Dir Dialog
    CreateDirDialog*    createDirDialog;
    //! Info Dialog
    InfoDialog*         infoDialog;
    //! Confirm Dialog
    ConfirmationDialog* confirmDialog;
    //! Search Dialog
    SearchDialog*       searchDialog;
    //! Copy Dialog
    CopyDialog*         copyDialog;
    //! Help Dialog
    HelpDialog*         helpDialog;
    //! Tree Window
    TreeWindow*         treeWindow;
};

#endif // MAINWINDOW_H
