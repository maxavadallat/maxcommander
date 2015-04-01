#include <QSettings>
#include <QFileInfo>
#include <QKeyEvent>
#include <QDir>
#include <QDebug>

#include <QDialogButtonBox>
#include <mcwinterface.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "preferencesdialog.h"
#include "createdirdialog.h"
#include "deletefiledialog.h"
#include "deleteprogressdialog.h"
#include "transferfiledialog.h"
#include "transferprogressdialog.h"
#include "helpwindow.h"
#include "viewerwindow.h"
#include "remotefileutilclient.h"
#include "utility.h"
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
    , leftPanel(NULL)
    , rightPanel(NULL)
    , focusedPanel(NULL)
    , modifierKeys(Qt::NoModifier)
    , aboutDialog(NULL)
    , preferencesDialog(NULL)
    , createDirDialog(NULL)
    , deleteFileDialog(NULL)
    , transferFileDialog(NULL)
    , helpWindow(NULL)

{
    qDebug() << "MainWindow::MainWindow";

    // Setup UI
    ui->setupUi(this);

    // Set Left Panel
    leftPanel  = ui->leftPanel;
    rightPanel = ui->rightPanel;

    // Init
    init();

    // Restore UI
    //restoreUI();
}

//==============================================================================
// Init
//==============================================================================
void MainWindow::init()
{
    qDebug() << "MainWindow::init";

    // Set Panel Name
    leftPanel->setPanelName(DEFAULT_PANEL_NAME_LEFT);
    // Set Panel Name
    rightPanel->setPanelName(DEFAULT_PANEL_NAME_RIGHT);

    // Connect Signals
    connect(leftPanel, SIGNAL(exitKeyReleased()), this, SLOT(quitApp()));
    connect(rightPanel, SIGNAL(exitKeyReleased()), this, SLOT(quitApp()));

    connect(leftPanel, SIGNAL(modifierKeysChanged(int)), this, SLOT(modifierKeysChanged(int)));
    connect(rightPanel, SIGNAL(modifierKeysChanged(int)), this, SLOT(modifierKeysChanged(int)));

    connect(leftPanel, SIGNAL(focusedPanelChanged(FilePanel*)), this, SLOT(focusedPanelChanged(FilePanel*)));
    connect(rightPanel, SIGNAL(focusedPanelChanged(FilePanel*)), this, SLOT(focusedPanelChanged(FilePanel*)));

    // ...

    connect(leftPanel, SIGNAL(launchTerminal(QString)), this, SLOT(launchTerminal(QString)));
    connect(rightPanel, SIGNAL(launchTerminal(QString)), this, SLOT(launchTerminal(QString)));

    connect(leftPanel, SIGNAL(launchViewer(bool)), this, SLOT(launchViewer(bool)));
    connect(rightPanel, SIGNAL(launchViewer(bool)), this, SLOT(launchViewer(bool)));

    connect(leftPanel, SIGNAL(launchFileCopy()), this, SLOT(launchFileCopy()));
    connect(rightPanel, SIGNAL(launchFileCopy()), this, SLOT(launchFileCopy()));

    connect(leftPanel, SIGNAL(launchFileMove()), this, SLOT(launchFileMove()));
    connect(rightPanel, SIGNAL(launchFileMove()), this, SLOT(launchFileMove()));

    connect(leftPanel, SIGNAL(launchCreateDir()), this, SLOT(launchCreateDir()));
    connect(rightPanel, SIGNAL(launchCreateDir()), this, SLOT(launchCreateDir()));

    connect(leftPanel, SIGNAL(launchDelete()), this, SLOT(launchDelete()));
    connect(rightPanel, SIGNAL(launchDelete()), this, SLOT(launchDelete()));

    connect(leftPanel, SIGNAL(showPreferences()), this, SLOT(showPreferences()));
    connect(rightPanel, SIGNAL(showPreferences()), this, SLOT(showPreferences()));

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI()
{
    qDebug() << "MainWindow::restoreUI";

    // ...

    // Restore Left Panel UI
    leftPanel->restoreUI();
    // Restore Right Panel UI
    rightPanel->restoreUI();

    // Set Focus
    leftPanel->setPanelFocus(true);

    // Update Function Keys
    updateFunctionKeys();
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

    // Check About Dialog
    if (!aboutDialog) {
        // Create About Dialog
        aboutDialog = new AboutDialog();
    }

    // Exec About Dialog
    aboutDialog->exec();
}

//==============================================================================
// Show Preferences
//==============================================================================
void MainWindow::showPreferences()
{
    qDebug() << "MainWindow::showPreferences";

    // Check Preferences Dialog
    if (!preferencesDialog) {
        // Create Preferences Dialog
        preferencesDialog = new PreferencesDialog();
    }

    // Show Dialog
    preferencesDialog->execDialog();
}

//==============================================================================
// Show Help
//==============================================================================
void MainWindow::showHelp()
{
    qDebug() << "MainWindow::showHelp";

    // Check Help Window
    if (!helpWindow) {
        // Ceate Help Window
        helpWindow = new HelpWindow();
    }

    // Load Content
    //helpWindow->loadContent();

    // Show Help Window
    helpWindow->showWindow();
}

//==============================================================================
// Launch Delete
//==============================================================================
void MainWindow::launchDelete()
{
    // Check Focused Panel
    if (focusedPanel) {
        qDebug() << "MainWindow::launchDelete";

        // Check Delete File Dialog
        if (!deleteFileDialog) {
            // Create Delete File Dialog
            deleteFileDialog = new DeleteFileDialog();
        }

        // Setup Delete File Dialog

        // ...

        // Launch Dialog
        if (deleteFileDialog->exec()) {

            // Create Delete Progress Dialog

            // Add To Delete Progress Dialog List

            // Show

            // Build Queue

            // Process Queueu

        }
    }
}

//==============================================================================
// Launch File Copy
//==============================================================================
void MainWindow::launchFileCopy()
{
    qDebug() << "MainWindow::launchFileCopy";

    // Check Transfer File Dialog
    if (!transferFileDialog) {
        // Create Transfer File Dialog
        transferFileDialog = new TransferFileDialog();
    }

    // Setup Delete File Dialog - Copy

    // ...

    // Launch Dialog
    if (transferFileDialog->exec()) {

        // Create Transfer Progress Dialog

        // Add To Transfer Progress Dialog List

        // Show

        // Build Queue

        // Process Queueu
    }
}

//==============================================================================
// Launch File Move/REname
//==============================================================================
void MainWindow::launchFileMove()
{
    qDebug() << "MainWindow::launchFileMove";

    // Check Transfer File Dialog
    if (!transferFileDialog) {
        // Create Transfer File Dialog
        transferFileDialog = new TransferFileDialog();
    }

    // Setup Delete File Dialog - Move

    // ...

    // Launch Dialog
    if (transferFileDialog->exec()) {

        // Create Transfer Progress Dialog

        // Add To Transfer Progress Dialog List

        // Show

        // Build Queue

        // Process Queueu
    }
}

//==============================================================================
// Launch Create Dir
//==============================================================================
void MainWindow::launchCreateDir()
{
    // Check Focused Panel
    if (focusedPanel) {
        qDebug() << "MainWindow::launchCreateDir";

        // Check CreateDirDialog
        if (!createDirDialog) {
            // Create Create Dir Dialog
            createDirDialog = new CreateDirDialog();
        }

        // Init Dir Path
        QString dirPath = focusedPanel->getCurrentDir();
        // Chekc Dir Path
        if (!dirPath.endsWith("/")) {
            // Adjust Dir Path
            dirPath += "/";
        }

        // Setup Dialog
        createDirDialog->setDirPath(dirPath);

        // ...

        // Launch Dialog
        if (createDirDialog->exec()) {
            // Get Dir Path
            dirPath = createDirDialog->getDirectory();

            // Create Directory
            focusedPanel->createDir(dirPath);
        }
    }
}

//==============================================================================
// Launch Viewer
//==============================================================================
void MainWindow::launchViewer(const bool& aEditMode)
{
    // Check Focused Panel
    if (focusedPanel) {
        qDebug() << "MainWindow::launchViewer - aEditMode: " << aEditMode;

        // Create New Viewer Window
        ViewerWindow* newViewer = new ViewerWindow();

        // Load File
        newViewer->loadFile(focusedPanel->getCurrFileInfo().absoluteFilePath());

        // Setup Viewer Window

        // Set Edit Mode
        newViewer->setEditModeEnabled(aEditMode);

        // ...

        // Connect Signal
        connect(newViewer, SIGNAL(viewerClosed(ViewerWindow*)), this, SLOT(viewerWindowClosed(ViewerWindow*)));

        // Add To Viewer List
        viewerWindows << newViewer;

        // Show Window
        newViewer->showWindow();
    }
}

//==============================================================================
// Launch Terminal
//==============================================================================
void MainWindow::launchTerminal(const QString& aDirPath)
{
    // Check Focused Panel
    if (focusedPanel) {
        qDebug() << "MainWindow::launchTerminal - aDirPath: " << aDirPath;

        // ...
    }
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

    // Abort & Clear Transfer Progress Dialogs
    while (transferProgressDialogs.count() > 0) {
        // Take Last Dialog
        TransferProgressDialog* lastDialog = transferProgressDialogs.takeLast();
        // Abort
        lastDialog->abort();
        // Delete Last Dialog
        delete lastDialog;
    }

    // Abort & Clear Delete Progress Dialogs
    while (deleteProgressDialogs.count() > 0) {
        // Take Last Dialog
        DeleteProgressDialog* lastDialog = deleteProgressDialogs.takeLast();
        // Abort
        lastDialog->abort();
        // Delete Last Dialog
        delete lastDialog;
    }

    // Close Viewer Windows
    while (viewerWindows.count() > 0) {
        // Take Last
        ViewerWindow* lastWindow = viewerWindows.takeLast();
        try {
            // Delete Last Window
            delete lastWindow;
        } catch (...) {
            qCritical() << "#### MainWindow::shutDown - ERROR DELETING VIEWER WINDOW!";
        }
    }
}

//==============================================================================
// Modifier Keys Changed Slot
//==============================================================================
void MainWindow::modifierKeysChanged(const int& aModifiers)
{
    qDebug() << "MainWindow::modifierKeysChanged - aModifiers: " << aModifiers;

    // Check Modifier Keys
    if (modifierKeys != aModifiers) {
        // Set Modifier Keys
        modifierKeys = aModifiers;

        // Update Function Keys
        updateFunctionKeys();
    }
}

//==============================================================================
// Update Function Keys
//==============================================================================
void MainWindow::updateFunctionKeys()
{
    //qDebug() << "MainWindow::updateFunctionKeys";

    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - SHIFT";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr("Rename"));
        ui->makeDirButton->setText(tr(""));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::ControlModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - CONTROL";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr("Sort by Name"));
        ui->editButton->setText(tr("Sort by Ext"));
        ui->copyButton->setText(tr("Sort by Size"));
        ui->moveButton->setText(tr("Sort by Date"));
        ui->makeDirButton->setText(tr(""));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::AltModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - ALT";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr(""));
        ui->makeDirButton->setText(tr("Search"));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::MetaModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - META";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr(""));
        ui->makeDirButton->setText(tr(""));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else {
        //qDebug() << "MainWindow::updateFunctionKeys";

        // Set Button Text
        ui->helpButton->setText(tr("Help"));
        ui->terminalButton->setText(tr("Terminal"));
        ui->viewButton->setText(tr("View"));
        ui->editButton->setText(tr("Edit"));
        ui->copyButton->setText(tr("Copy"));
        ui->moveButton->setText(tr("Move"));
        ui->makeDirButton->setText(tr("MakeDir"));
        ui->delButton->setText(tr("Delete"));
        ui->optionsButton->setText(tr("Options"));
        ui->exitButton->setText(tr("Exit"));
    }
}

//==============================================================================
// Update Menu
//==============================================================================
void MainWindow::updateMenu()
{
    qDebug() << "MainWindow::updateMenu";

}

//==============================================================================
// Toggle Hidden Files
//==============================================================================
void MainWindow::toggleHiddenFile()
{
    qDebug() << "MainWindow::toggleHiddenFile";

    // Get Show Hidden
    bool showHidden = focusedPanel ? focusedPanel->getShowHidden() : true;

    // Check Left Panel
    if (leftPanel) {
        // Set Show Hidden
        leftPanel->setShowHidden(!showHidden);
    }

    // Check Right Panel
    if (rightPanel) {
        // Set Show Hidden
        rightPanel->setShowHidden(!showHidden);
    }

}

//==============================================================================
// Viewer Window Closed Slot
//==============================================================================
void MainWindow::viewerWindowClosed(ViewerWindow* aViewer)
{
    // Check Viewer
    if (aViewer) {
        qDebug() << "MainWindow::viewerWindowClosed - aViewer: " << aViewer;

        // Get Viewer Windows Count
        int vwCount = viewerWindows.count();
        // Go Thru Viewer Windows
        for (int i=0; i<vwCount; ++i) {
            // Get Viewer Window
            ViewerWindow* window = viewerWindows[i];
            // Check Window
            if (window == aViewer) {
                // Remove From Window List
                viewerWindows.removeAt(i);

                try {
                    // Delete Viewer Window
                    delete window;
                } catch (...) {
                    qCritical() << "#### MainWindow::viewerWindowClosed - ERROR DELETING WIEVER WINDOW!!";
                }

                return;
            }
        }
    }
}

//==============================================================================
// Delete Progress Window Closed Slot
//==============================================================================
void MainWindow::deleteProgressClosed(DeleteProgressDialog* aDeleteProgressDialog)
{
    // Check Progress Dialog
    if (aDeleteProgressDialog) {

    }
}

//==============================================================================
// Transfer Progress Window Closed Slot
//==============================================================================
void MainWindow::transferProgressClosed(TransferProgressDialog* aTransferProgressDialog)
{
    // Check Progress Dialog
    if (aTransferProgressDialog) {

    }
}

//==============================================================================
// Focused Panel Changed Slot
//==============================================================================
void MainWindow::focusedPanelChanged(FilePanel* aFocusedPanel)
{
    //qDebug() << "MainWindow::focusedPanelChanged";

    // Set Focused Panel
    focusedPanel = aFocusedPanel;
}

//==============================================================================
// Help Button Clicked Slot
//==============================================================================
void MainWindow::on_helpButton_clicked()
{
    //qDebug() << "MainWindow::on_helpButton_clicked";

    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Help Action
        ui->actionHelp->trigger();
    }
}

//==============================================================================
// Terminal Button Clicked Slot
//==============================================================================
void MainWindow::on_terminalButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Terminal Action
        ui->actionTerminal->trigger();
    }
}

//==============================================================================
// View Button Clicked Slot
//==============================================================================
void MainWindow::on_viewButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger View Action
        ui->actionView->trigger();
    }
}

//==============================================================================
// Edit Button Clicked Slot
//==============================================================================
void MainWindow::on_editButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Edit Action
        ui->actionEdit->trigger();
    }
}

//==============================================================================
// Copy Button Clicked Slot
//==============================================================================
void MainWindow::on_copyButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Copy Action
        ui->actionCopy->trigger();
    }
}

//==============================================================================
// Move Button Clicked Slot
//==============================================================================
void MainWindow::on_moveButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Move Action
        ui->actionMove->trigger();
    }
}

//==============================================================================
// Make Dir Button Clicked Slot
//==============================================================================
void MainWindow::on_makeDirButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Make Dir Action
        ui->actionNew_Directory->trigger();
    }
}

//==============================================================================
// Delete Button Clicked Slot
//==============================================================================
void MainWindow::on_delButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Delete Action
        ui->actionDelete_File->trigger();
    }
}

//==============================================================================
// Options Button Clicked Slot
//==============================================================================
void MainWindow::on_optionsButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Options Action
        ui->actionPreferences->trigger();
    }
}

//==============================================================================
// Exit Button Clicked Slot
//==============================================================================
void MainWindow::on_exitButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Action Exit
        ui->actionExit->trigger();
    }
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
    // Show Help
    showHelp();
}

//==============================================================================
// Action Options Triggered Slot
//==============================================================================
void MainWindow::on_actionOptions_triggered()
{
    // Show Preferences
    showPreferences();
}

//==============================================================================
// Action Compare Files Triggered Slot
//==============================================================================
void MainWindow::on_actionCompare_Files_triggered()
{
    // Compare Files
}

//==============================================================================
// Action Select All Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_all_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Select All Files In Focused Panel
        focusedPanel->selectAllFiles();
    }
}

//==============================================================================
// Action Select None Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_None_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Deselect All Files In Focused Panel
        focusedPanel->deselectAllFiles();
    }
}

//==============================================================================
// Action Terminal Triggered Slot
//==============================================================================
void MainWindow::on_actionTerminal_triggered()
{
    // Launch Terminal
}

//==============================================================================
// Action Sort By Name Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Name_triggered()
{
    // Sort Focused Panel Items By Name
}

//==============================================================================
// Action Sort By Extension Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Extension_triggered()
{
    // Sort Focused Panel Items By Extension

}

//==============================================================================
// Action Sort By Type Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Type_triggered()
{
    // Sort Focused Panel Items By Type

}

//==============================================================================
// Action Sort By Size Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Size_triggered()
{
    // Sort Focused Panel Items By Size

}

//==============================================================================
// Action Sort By Date Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Date_triggered()
{
    // Sort Focused Panel Items By Date

}

//==============================================================================
// Action Sort By Owner Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Owner_triggered()
{
    // Sort Focused Panel Items By Owner

}

//==============================================================================
// Action Sort By Permission Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Permissions_triggered()
{
    // Sort Focused Panel Items By Permission

}

//==============================================================================
// Action Toggle Hidden Files Triggered Slot
//==============================================================================
void MainWindow::on_actionShow_Hide_Hiden_triggered()
{
    // Toggle Show Hidden
    toggleHiddenFile();
}

//==============================================================================
// Action New Directory Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_Directory_triggered()
{
    // Launch Create Dir
    launchCreateDir();
}

//==============================================================================
// Action New File Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_File_triggered()
{
    //
}

//==============================================================================
// Action View File Triggered Slot
//==============================================================================
void MainWindow::on_actionView_triggered()
{
    // Launch Viewer
    launchViewer(false);
}

//==============================================================================
// Action Edit File Triggered Slot
//==============================================================================
void MainWindow::on_actionEdit_triggered()
{
    // Launch Viewer
    launchViewer(true);
}

//==============================================================================
// Action Delete File Triggered Slot
//==============================================================================
void MainWindow::on_actionDelete_File_triggered()
{
    // Launch Delete Dir
    launchDelete();
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
// Actrion Reload Triggered Slot
//==============================================================================
void MainWindow::on_actionReload_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Reload Focused Panel
        focusedPanel->reload(focusedPanel->currentIndex);
    }
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

    // Check About Dialog
    if (aboutDialog) {
        // Delete Dialog
        delete aboutDialog;
        aboutDialog = NULL;
    }

    // Check Preferences Dialog
    if (preferencesDialog) {
        // Delete Dialog
        delete preferencesDialog;
        preferencesDialog = NULL;
    }

    // Check Create Dir Dialog
    if (createDirDialog) {
        // Delete Dialog
        delete createDirDialog;
        createDirDialog = NULL;
    }

    // Check Delete File Dialog
    if (deleteFileDialog) {
        // Delete Dialog
        delete deleteFileDialog;
        deleteFileDialog = NULL;
    }

    // Check Transfer File Dialog
    if (transferFileDialog) {
        // Delete Dialog
        delete transferFileDialog;
        transferFileDialog = NULL;
    }

    // Check Help Window
    if (helpWindow) {
        // Delete Dialog
        delete helpWindow;
        helpWindow = NULL;
    }


    qDebug() << "MainWindow::~MainWindow";
}


