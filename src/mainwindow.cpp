#include <QSettings>
#include <QFileInfo>
#include <QKeyEvent>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"


// Singleton Instance
static MainWindow* mainWindowInstance = NULL;

//==============================================================================
// Get Main Window Instance - Static Constructor
//==============================================================================
MainWindow* MainWindow::getInstance()
{
    // Check Singleton Instance
    if (!mainWindowInstance) {
        // Create Singleton Instance
        mainWindowInstance = new MainWindow();
    }

    return mainWindowInstance;
}

//==============================================================================
// Release Instance
//==============================================================================
void MainWindow::release()
{
    // Check Main Window Singleton Instance
    if (mainWindowInstance) {
        // Delete Singleton
        delete mainWindowInstance;
        mainWindowInstance = NULL;
    }
}

//==============================================================================
// Constructor
//==============================================================================
MainWindow::MainWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow::MainWindow";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();

    // Restore UI
    restoreUI();
}

//==============================================================================
// Init
//==============================================================================
void MainWindow::init()
{
    qDebug() << "MainWindow::init";

    // Set Panel Name
    ui->leftPanel->panelName = DEFAULT_PANEL_NAME_LEFT;
    // Set Panel Name
    ui->rightPanel->panelName = DEFAULT_PANEL_NAME_RIGHT;

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI()
{
    qDebug() << "MainWindow::restoreUI";



    // ...


    // Check Last Focused Panel

    // Set Focus
    ui->leftPanel->setPanelFocus(true);


}

//==============================================================================
// Save Settings
//==============================================================================
void MainWindow::saveSettings()
{
    qDebug() << "MainWindow::saveSettings";

}

//==============================================================================
// Show Window
//==============================================================================
void MainWindow::showWindow()
{
    qDebug() << "MainWindow::showWindow";

    // Restore UI
    restoreUI();

    // Show
    show();
}

//==============================================================================
// Show About
//==============================================================================
void MainWindow::showAbout()
{
    qDebug() << "MainWindow::showAbout";

}

//==============================================================================
// Show Preferences
//==============================================================================
void MainWindow::showPreferences()
{
    qDebug() << "MainWindow::showPreferences";

}

//==============================================================================
// Quit Application
//==============================================================================
void MainWindow::quitApp()
{
    qDebug() << "MainWindow::quitApp";

    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Shut Down
//==============================================================================
void MainWindow::shutDown()
{
    qDebug() << "MainWindow::shutDown";

}

//==============================================================================
// Key Press Event
//==============================================================================
void MainWindow::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check If Repeat
        if (!aEvent->isAutoRepeat()) {

            // Switch

        } else {

        }

    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void MainWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {

    }
}

//==============================================================================
// Help Button Clicked Slot
//==============================================================================
void MainWindow::on_helpButton_clicked()
{

}

//==============================================================================
// Terminal Button Clicked Slot
//==============================================================================
void MainWindow::on_terminalButton_clicked()
{

}

//==============================================================================
// View Button Clicked Slot
//==============================================================================
void MainWindow::on_viewButton_clicked()
{

}

//==============================================================================
// Edit Button Clicked Slot
//==============================================================================
void MainWindow::on_editButton_clicked()
{

}

//==============================================================================
// Copy Button Clicked Slot
//==============================================================================
void MainWindow::on_copyButton_clicked()
{

}

//==============================================================================
// Move Button Clicked Slot
//==============================================================================
void MainWindow::on_moveButton_clicked()
{

}

//==============================================================================
// Make Dir Button Clicked Slot
//==============================================================================
void MainWindow::on_makeDirButton_clicked()
{

}

//==============================================================================
// Delete Button Clicked Slot
//==============================================================================
void MainWindow::on_delButton_clicked()
{

}

//==============================================================================
// Options Button Clicked Slot
//==============================================================================
void MainWindow::on_optionsButton_clicked()
{

}

//==============================================================================
// Exit Button Clicked Slot
//==============================================================================
void MainWindow::on_exitButton_clicked()
{
    // Trigger Action Exit
    ui->actionExit->trigger();
}

//==============================================================================
// Action About Triggered Slot
//==============================================================================
void MainWindow::on_actionAbout_triggered()
{
    // Show About
    showAbout();
}

//==============================================================================
// Action Preferences Triggered Slot
//==============================================================================
void MainWindow::on_actionPreferences_triggered()
{
    // Show Preferences
    showPreferences();
}

//==============================================================================
// Action Help Triggered Slot
//==============================================================================
void MainWindow::on_actionHelp_triggered()
{

}

//==============================================================================
// Action Options Triggered Slot
//==============================================================================
void MainWindow::on_actionOptions_triggered()
{

}

//==============================================================================
// Action Compare Files Triggered Slot
//==============================================================================
void MainWindow::on_actionCompare_Files_triggered()
{

}

//==============================================================================
// Action Select All Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_all_triggered()
{

}

//==============================================================================
// Action Select None Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_None_triggered()
{

}

//==============================================================================
// Action Terminal Triggered Slot
//==============================================================================
void MainWindow::on_actionTerminal_triggered()
{

}

//==============================================================================
// Action Sort By Name Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Name_triggered()
{

}

//==============================================================================
// Action Sort By Extension Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Extension_triggered()
{

}

//==============================================================================
// Action Sort By Type Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Type_triggered()
{

}

//==============================================================================
// Action Sort By Size Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Size_triggered()
{

}

//==============================================================================
// Action Sort By Date Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Date_triggered()
{

}

//==============================================================================
// Action Sort By Owner Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Owner_triggered()
{

}

//==============================================================================
// Action Sort By Permission Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Permissions_triggered()
{

}

//==============================================================================
// Action Toggle Hidden Files Triggered Slot
//==============================================================================
void MainWindow::on_actionShow_Hide_Hiden_triggered()
{

}

//==============================================================================
// Action New Directory Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_Directory_triggered()
{

}

//==============================================================================
// Action New File Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_File_triggered()
{

}

//==============================================================================
// Action Delete File Triggered Slot
//==============================================================================
void MainWindow::on_actionDelete_File_triggered()
{

}

//==============================================================================
// Action Create Link Triggered Slot
//==============================================================================
void MainWindow::on_actionCreate_Link_triggered()
{

}

//==============================================================================
// Action Quit Triggered Slot
//==============================================================================
void MainWindow::on_actionQuit_triggered()
{
    // Quit Application
    quitApp();
}

//==============================================================================
// Action Exit Triggered Slot
//==============================================================================
void MainWindow::on_actionExit_triggered()
{
    // Quit App
    quitApp();
}

//==============================================================================
// Destructor
//==============================================================================
MainWindow::~MainWindow()
{
    // Save Settings
    saveSettings();

    // Shut Down
    shutDown();

    // Delete UI
    delete ui;

    qDebug() << "MainWindow::~MainWindow";
}



