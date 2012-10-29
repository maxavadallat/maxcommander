#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// INCLUDES

#include <QDebug>
#include <QMainWindow>
#include <QDialogButtonBox>
#include <QEventLoop>

#include "fileutils.h"
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
class MainQueueDialog;
class AdminPassDialog;
class FileUtilsClient;
class AdminPassQueryProvider;


// DECLARATIONS

//==============================================================================
//! @class MainWindow Application Main Window Class
//==============================================================================
class MainWindow : public QMainWindow, public DirCreatorObserver, public AdminPassQueryProvider
{
    Q_OBJECT

public:

    //! @brief Get Main Window Instance - Static Constructor
    //! @param none
    static MainWindow* getInstance();

    //! @brief Release Instance
    //! @param none
    void release();

public slots:

    //! @brief Launch Help Slot
    //! @param none
    void launchHelp();

    //! @brief Show Tree Slot
    //! @param none
    void showTree();

    //! @brief View File Slot
    //! @param none
    void viewFile();

    //! @brief Edit File Slot
    //! @param none
    void editFile();

    //! @brief Copy Files Slot
    //! @param none
    void copyFiles();

    //! @brief Move Files Slot
    //! @param none
    void moveFiles();

    //! @brief Launch Rename Current File Slot
    //! @param none
    void launchRenameCurrentFile();

    //! @brief Launch Create Dir Slot
    //! @param none
    void launchCreateDir();

    //! @brief Launch Delete Files Slot
    //! @param none
    void launchDelete();

    //! @brief Launch Copy Slot
    //! @param none
    void launchCopy();

    //! @brief Launch Move Slot
    //! @param none
    void launchMove();

    //! @brief Launch Search Slot
    //! @param none
    void launchSearch();

    //! @brief Launch Options Slot
    //! @param none
    void launchOptions();

    //! @brief Exit Application Slot
    //! @param none
    void exitApp();

    //! @brief Switch To Panel Slot
    //! @param aIndex Panel Index
    void switchToPanel(const int& aIndex);

    //! @brief Load Settings Slot
    //! @param none
    void loadSettings();

    //! @brief Save Settings Slot
    //! @param none
    void saveSettings();

    //! @brief Show Info Dialog Slot
    //! @param aTitle Dialog Title Text
    //! @param aInfoMsg Dialog Info Text
    //! @param aModal Dialog Modal State
    void showInfo(const QString& aTitle, const QString& aInfoMsg, const bool& aModal = false);

    //! @brief Show Confirmation Dialog Slot
    //! @param aTitle Dialog Title Text
    //! @param aConfirmMsg Dialog Confirmation Text
    //! @param aButtons Dialog Buttons To Use
    //! @return Confirmation Code
    int showConfirmation(const QString& aTitle,
                         const QString& aConfirmMsg,
                         const QDialogButtonBox::StandardButtons& aButtons,
                         const QDialogButtonBox::StandardButton& aDefault = QDialogButtonBox::NoButton);

    //! @brief Show Confirmation Dialog Slot
    //! @param aDialogProvider Confirm Dialog Provider
    //! @param aType Confirmation Type
    //! @param aEventLoop Blocking Event Loop
    void showConfirmation(ConfirmDialogProvider* aDialogProvider, const int& aType, QEventLoop* aEventLoop = NULL);

    //! @brief Show Error Dialog Slot
    //! @param aDialogProvider Error Dialog Provider
    //! @param aErrorCode Error Code
    //! @param aEventLoop Blocking Event Loop
    void showError(ErrorDialogProvider* aDialogProvider, const int& aErrorCode, QEventLoop* aEventLoop = NULL);

    //! @brief Show Status Message Slot
    //! @param aMessage Message
    //! @param aTimeOut Message Timeout
    void showStatusMessage(const QString& aMessage, const int& aTimeOut = DEFAULT_STATUS_MESSAGE_TIMEOUT);

    //! @brief Clear Status Message Slot
    //! @param none
    void clearStatusMessage();

    //! @brief Launch Admin Password Query Dialog
    //! @param none
    //! @return Admin Password
    virtual QString launchAdminPassQuery();

protected:

    //! @brief Constructor
    //! @param aParent Parent Widget
    MainWindow(QWidget* aParent = NULL);

    //! @brief Configure Function Keys
    //! @param none
    void configureFunctionKeys();

    //! @brief Clear Progress Dialogs
    //! @param none
    void clearDialogs();

    //! @brief Build And Add Queue
    //! @param aOperation Operation Index
    //! @param aQueueHandler Operation Queue Handler
    //! @param aSourcePanelIndex Source Panel Index
    //! @param aSelCount File Selection Count
    //! @param aCurrFileName Current File Name In Source Panel
    //! @param aTargetFileName Target File Name In Source Panel
    void addItemsToQueue(const int& aOperation,
                         FileOperationQueueHandler* aQueueHandler,
                         const int& aSourcePanelIndex,
                         const int& aSelCount = 0,
                         const QString& aCurrFileName = QString(""),
                         const QString& aTargetFileName = QString(""));

    //! @brief Create Dir Error Callback
    //! @param aDirPath Directory Path
    //! @param aErrorCode Error Code
    //! @return Creation Error Response
    virtual int createDirError(const QString& aDirPath, const int& aErrorCode);

    //! @brief Create Dir Started
    //! @param aDirPath Directory Path
    virtual void createDirStarted(const QString& aDirPath);

    //! @brief Creaet Dir Finished
    //! @param aDirPath Directory Path
    //! @param aErrorCode Directory Creation Error Code
    virtual void createDirFinished(const QString& aDirPath, const int& aErrorCode);

    //! @brief Get Admin Password Query Provider
    //! @param none
    //! @return Admin Password Query Provider
    virtual AdminPassQueryProvider* passQueryProvider();

    //! @brief Get File Utils Client
    //! @param aAdminPass Admin Password
    //! @return File Utils Client
    virtual FileUtilsClient* getUtilsClient(const QString& aAdminPass);

    //! @brief Destructor
    //! @param none
    virtual ~MainWindow();

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

    //! @brief Info Dialog Finished Slot
    //! @param aDialog Dialog
    void infoDialogFinished(QDialog* aDialog);

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
    //! Admin Mode
    bool                adminMode;
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
    //! Error Dialog
    ConfirmationDialog* errorDialog;

    //! Confirmation Dialog Mutex Lock
    QMutex              confirmMutex;
    //! Search Dialog
    SearchDialog*       searchDialog;
    //! Copy Dialog
    CopyDialog*         copyDialog;
    //! Help Dialog
    HelpDialog*         helpDialog;
    //! Tree Window
    TreeWindow*         treeWindow;
    //! Main Queue Dialog
    MainQueueDialog*    mainQueueDialog;
    //! Admin Pass Query Dialog
    AdminPassDialog*    adminPassDialog;

    //! Progress Dialogs List
    QList<QDialog*>     dialogs;

    //! File Util Client For Remote Worker Process
    FileUtilsClient*    fileUtilsClient;
};

#endif // MAINWINDOW_H
