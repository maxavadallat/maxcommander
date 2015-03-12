#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class FilePanel;

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

    // Show About
    void showAbout();

    // Show Preferences
    void showPreferences();

    // Quit Application
    void quitApp();

    // Shut Down
    void shutDown();

signals:


protected slots:


protected:

    // Constructor
    explicit MainWindow(QWidget* aParent = NULL);

    // Init
    void init();

    // Restore UI
    void restoreUI();

    // Save Settings
    void saveSettings();

    // Destructor
    virtual ~MainWindow();

protected: // From QMainWindow

    // Function Keys Mode
    enum EFNKeyModeType {
        EFNKMNormal = 0,
        EFNKMControl,
        EFNKMAlt,
        EFNKMShift
    };

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

private slots:

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
    // Action Exit Triggered Slot
    void on_actionExit_triggered();

private:

    // UI
    Ui::MainWindow*         ui;

    // Left Drive Buttons - Windows

    // Right Drive Buttons - Windows

    // Left Panel
    FilePanel*              leftPanel;

    // Right Panel
    FilePanel*              rightPanel;

    // Command Buttons

};

#endif // MAINWINDOW_H
