#include <QFileInfo>
#include <QKeyEvent>
#include <QDir>
#include <QDesktopWidget>
#include <QDebug>

#include <QDialogButtonBox>
#include <mcwinterface.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "infodialog.h"
#include "preferencesdialog.h"
#include "createdirdialog.h"
#include "createlinkdialog.h"
#include "deletefiledialog.h"
#include "deleteprogressdialog.h"
#include "transferfiledialog.h"
#include "transferprogressdialog.h"
#include "helpwindow.h"
#include "viewerwindow.h"
#include "searchdialog.h"
#include "selectfilesdialog.h"
#include "remotefileutilclient.h"
#include "infodialog.h"
#include "settingscontroller.h"
#include "utility.h"
#include "defaultsettings.h"
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
    , settings(SettingsController::getInstance())
    , leftPanel(NULL)
    , rightPanel(NULL)
    , focusedPanel(NULL)
    , viewMenuActionGroup(new QActionGroup(NULL))
    , modifierKeys(Qt::NoModifier)
    , aboutDialog(NULL)
    , preferencesDialog(NULL)
    , createDirDialog(NULL)
    , createLinkDialog(NULL)
    , deleteFileDialog(NULL)
    , transferFileDialog(NULL)
    , helpWindow(NULL)
    , searchFileDialog(NULL)
    , selectFilesDialog(NULL)
    , viewSearchResult(false)

{
    qDebug() << "MainWindow::MainWindow";

    // Setup UI
    ui->setupUi(this);

    // Add Actions To Action Group
    viewMenuActionGroup->addAction(ui->actionSort_by_Name);
    viewMenuActionGroup->addAction(ui->actionSort_by_Extension);
    viewMenuActionGroup->addAction(ui->actionSort_by_Type);
    viewMenuActionGroup->addAction(ui->actionSort_by_Size);
    viewMenuActionGroup->addAction(ui->actionSort_by_Date);
    viewMenuActionGroup->addAction(ui->actionSort_by_Owner);
    viewMenuActionGroup->addAction(ui->actionSort_by_Permissions);
    // Set Exclusive
    viewMenuActionGroup->setExclusive(true);

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

    connect(leftPanel, SIGNAL(launchCreateLink()), this, SLOT(launchCreateLink()));
    connect(rightPanel, SIGNAL(launchCreateLink()), this, SLOT(launchCreateLink()));

    connect(leftPanel, SIGNAL(launchSearch()), this, SLOT(launchSearch()));
    connect(rightPanel, SIGNAL(launchSearch()), this, SLOT(launchSearch()));

    connect(leftPanel, SIGNAL(launchDelete()), this, SLOT(launchDelete()));
    connect(rightPanel, SIGNAL(launchDelete()), this, SLOT(launchDelete()));

    connect(leftPanel, SIGNAL(showPreferences()), this, SLOT(showPreferences()));
    connect(rightPanel, SIGNAL(showPreferences()), this, SLOT(showPreferences()));

    connect(leftPanel, SIGNAL(launchFileSelect()), this, SLOT(launchFileSelect()));
    connect(rightPanel, SIGNAL(launchFileSelect()), this, SLOT(launchFileSelect()));

    connect(leftPanel, SIGNAL(launchFileDeselect()), this, SLOT(launchFileDeselect()));
    connect(rightPanel, SIGNAL(launchFileDeselect()), this, SLOT(launchFileDeselect()));

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI(const bool& aReload, const int& aFocusedPanel)
{
    qDebug() << "MainWindow::restoreUI - aReload: " << aReload;

    // Check Left Panel
    if (leftPanel) {
        // Restore Left Panel UI
        leftPanel->restoreUI(aReload);
    }

    // Check Right Panel
    if (rightPanel) {
        // Restore Right Panel UI
        rightPanel->restoreUI(aReload);
    }

    // Check FocusedPanel
    if (aFocusedPanel == 0) {
        // Set Focus
        leftPanel->setPanelFocus(true);
    } else {
        // Set Focus
        rightPanel->setPanelFocus(true);
    }

    // Update Function Keys
    updateFunctionKeys();

    // Update Menu
    updateMenu();
}

//==============================================================================
// Load Settings
//==============================================================================
void MainWindow::loadSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    qDebug() << "MainWindow::loadSettings";

    // Get Window Width
    int windowWidth = settings->value(SETTINGS_KEY_MAIN_WIDTH, DEFAULT_MAIN_WINDOW_WIDTH).toInt();
    // Get Window height
    int windowHeight = settings->value(SETTINGS_KEY_MAIN_HEIGHT, DEFAULT_MAIN_WINDOW_HEIGHT).toInt();

    // Get Desktop Widget
    QDesktopWidget desktop;

    // Get First Screen
    QWidget* screen = desktop.screen();

    // Calculate X Pos
    int posX = screen ? (screen->width() - windowWidth) / 2 : 0;
    // Calculate Y Pos
    int posY = screen ? (screen->height() - windowHeight) / 2 : 0;

    // Set Geometry
    setGeometry(posX, posY, windowWidth, windowHeight);
/*
    // Get Last Focused Panel
    QString lastFocusedPanel = settings.value(SETTINGS_KEY_MAIN_ACTIVEPANEL).toString();

    // Check Last Focused Panel
    if (lastFocusedPanel == DEFAULT_PANEL_NAME_LEFT) {
        // Set Focused Panel
        focusedPanel = leftPanel;
    } else {
        // Set Focused Panel
        focusedPanel = rightPanel;
    }
*/
    // ...

}

//==============================================================================
// Save Settings
//==============================================================================
void MainWindow::saveSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    qDebug() << "MainWindow::saveSettings";

    // Set Value - Width
    settings->setValue(SETTINGS_KEY_MAIN_WIDTH, geometry().width());
    // Set Value - Height
    settings->setValue(SETTINGS_KEY_MAIN_HEIGHT, geometry().height());

    // Set Active Panel
    settings->setValue(SETTINGS_KEY_MAIN_ACTIVEPANEL, focusedPanel ? focusedPanel->getPanelName() : "");

    // ...
}

//==============================================================================
// Show Window
//==============================================================================
void MainWindow::showWindow()
{
    qDebug() << "MainWindow::showWindow";

    // Load Settings
    loadSettings();

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
    helpWindow->loadContent();

    // Show Help Window
    helpWindow->showWindow();
}

//==============================================================================
// Launch Terminal
//==============================================================================
void MainWindow::launchTerminal(const QString& aDirPath)
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Get Terminal App
    QString terminalApp = settings ? settings->value(SETTINGS_KEY_APPS_TERMINAL, DEFAULT_SETTINGS_TERMINAL_PATH_MAC_OSX).toString() : "";

    // Check Terminal App
    if (!terminalApp.isEmpty()) {

        qDebug() << "MainWindow::launchTerminal - aDirPath: " << aDirPath;


#if defined(Q_OS_MACX)

        // Set Command Line
        QString cmdLine = QString(DEFAULT_EXEC_APP_SYSTEM_COMMAND_WITH_PARAM_MAC_OSX).arg(terminalApp).arg(aDirPath);

        // Execute
        system(cmdLine.toLocal8Bit().data());

#elif defined(Q_OS_UNIX)


#else defined(Q_OS_WIN)


#endif // Q_OS_WIN


    }

    // ...
}

//==============================================================================
// Launch Viewer
//==============================================================================
void MainWindow::launchViewer(const bool& aEditMode, const bool& aNewFile)
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Reset Modifier Keys
    focusedPanel->resetModifierKeys();

    // Get File info
    QFileInfo fileInfo = focusedPanel->getCurrFileInfo();

    // Check File Type
    if (!aNewFile && (fileInfo.isDir() || fileInfo.isBundle() || fileInfo.isSymLink())) {
        qDebug() << "MainWindow::launchViewer - aEditMode: " << aEditMode << " - NO SELECTED FILE!";

        // Launch Dir Quick View Maybe...

        // ...

        return;
    }

    qDebug() << "MainWindow::launchViewer - aEditMode: " << aEditMode << " - aNewFile: " << aNewFile;


    // Check Settings For Using External Viewer

    // Reset Search Result View
    viewSearchResult = false;

    // Launch Viewer
    launchViewer(focusedPanel->getCurrFileInfo().absoluteFilePath(), focusedPanel, aEditMode, aNewFile);
}

//==============================================================================
// Launch Viewer
//==============================================================================
void MainWindow::launchViewer(const QString& aFileName, FilePanel* aFilePanel, const bool& aEditMode, const bool& aNewFile)
{
    // Create New Viewer Window
    ViewerWindow* newViewer = new ViewerWindow();

    // Set Edit Mode
    newViewer->setEditModeEnabled(aEditMode);

    // Check New File
    if (aNewFile) {
        // New File
        if (!newViewer->newFile(aFilePanel->getCurrentDir())) {
            // Show Info - Can't Save
            InfoDialog warningDialog(tr(DEFAULT_WARNING_TEXT_CANT_CREATE_NEW_FILE), EIDTWarning);
            // Exec Dialog
            warningDialog.exec();

            // Delete New Viewer
            delete newViewer;

            return;
        }
    } else {
        // Load File
        if (!newViewer->loadFile(aFileName, aFilePanel->getPanelName())) {

            // Show Info - Unsupported Format
            InfoDialog warningDialog(tr(DEFAULT_WARNING_TEXT_UNSUPPORTED_FILE_FORMAT), EIDTWarning);
            // Exec Dialog
            warningDialog.exec();

            // Delete New Viewer
            delete newViewer;

            return;
        }
    }

    // Connect Signal
    connect(newViewer, SIGNAL(viewerClosed(ViewerWindow*)), this, SLOT(viewerWindowClosed(ViewerWindow*)), Qt::QueuedConnection);

    // Add To Viewer List
    viewerWindows << newViewer;

    // Show Window
    newViewer->showWindow();
}

//==============================================================================
// Launch File Copy
//==============================================================================
void MainWindow::launchFileCopy()
{
    //qDebug() << "MainWindow::launchFileCopy";

    // Launch Transfer
    launchTransfer(DEFAULT_OPERATION_COPY_FILE);
}

//==============================================================================
// Launch File Move/REname
//==============================================================================
void MainWindow::launchFileMove()
{
    //qDebug() << "MainWindow::launchFileMove";

    // Launch Transfer
    launchTransfer(DEFAULT_OPERATION_MOVE_FILE);
}

//==============================================================================
// Launch Transfer
//==============================================================================
void MainWindow::launchTransfer(const QString& aOperation)
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Check Transfer File Dialog
    if (!transferFileDialog) {
        // Create Transfer File Dialog
        transferFileDialog = new TransferFileDialog();
    }

    // Check Operation
    if (aOperation == DEFAULT_OPERATION_COPY_FILE) {
        // Set Title
        transferFileDialog->setWindowTitle(tr(DEFAULT_TITLE_COPY_FILES));
    } else if (aOperation == DEFAULT_OPERATION_MOVE_FILE) {
        // Set Title
        transferFileDialog->setWindowTitle(tr(DEFAULT_TITLE_MOVE_FILES));
    }

    // Init Selected Files
    QStringList selectedFiles = focusedPanel->getSelectedFiles();

    // Get Source Panel
    FilePanel* sourcePanel = (focusedPanel == leftPanel) ? leftPanel : rightPanel;
    // Get Target Panel
    FilePanel* targetPanel = (focusedPanel == leftPanel) ? rightPanel : leftPanel;

    // Init Transfer Source Dir
    QString transferSourceDir = sourcePanel->getCurrentDir();
    // Init Transfer Target Dir
    QString transferTargetDir = targetPanel->getCurrentDir();

    // Check Transfer Source
    if (!transferSourceDir.endsWith("/")) {
        // Adjust Transfer Source
        transferSourceDir += "/";
    }

    // Check Transfer Target
    if (!transferTargetDir.endsWith("/")) {
        // Adjust Transfer Target
        transferTargetDir += "/";
    }

    // Check Selected Files Count
    if (selectedFiles.count() > 1) {

        // Set Transfer Dialog Source File Text
        transferFileDialog->setSourceFileText(transferSourceDir + "*", focusedPanel->getSearchResultsMode());
        // Set Transfer Dialog Target File Text
        transferFileDialog->setTargetFileText(transferTargetDir + "*", focusedPanel->getSearchResultsMode());

    } else if (selectedFiles.count() == 1) {

        // Set Transfer Dialog Source File Text
        //transferFileDialog->setSourceFileText(transferSourceDir + selectedFiles[0], focusedPanel->getSearchResultsMode());
        transferFileDialog->setSourceFileText(selectedFiles[0], focusedPanel->getSearchResultsMode());

        // Set Transfer Dialog Target File Text
        //transferFileDialog->setTargetFileText(transferTargetDir + selectedFiles[0], focusedPanel->getSearchResultsMode());
        transferFileDialog->setTargetFileText(transferTargetDir + QFileInfo(selectedFiles[0]).fileName(), focusedPanel->getSearchResultsMode());

    } else {

        qDebug() << "MainWindow::launchTransfer - aOperation: " << aOperation << " - NO SELECTED FILE!";

        // Nothing Selected

        // ...

        return;
    }

    qDebug() << "MainWindow::launchTransfer - aOperation: " << aOperation << " - count: " << selectedFiles.count();

    // ...

    // Launch Dialog
    if (transferFileDialog->exec()) {
        // Create Transfer Progress Dialog
        TransferProgressDialog* newTransferProgressDialog = new TransferProgressDialog(aOperation);
        // Connect Closed Signal
        connect(newTransferProgressDialog, SIGNAL(dialogClosed(TransferProgressDialog*)), this, SLOT(transferProgressClosed(TransferProgressDialog*)), Qt::QueuedConnection);

        // Init Copy Options
        int copyOptions = transferFileDialog->getCopyHidden() ? DEFAULT_COPY_OPTIONS_COPY_HIDDEN : 0;

        // Check If Source Changed
        if (transferFileDialog->getSourceChanged() || transferFileDialog->getTargetChanged()) {

            // Clear Selected Files
            selectedFiles.clear();

            // Get Transfer Source
            QString sourceText = isPathRelative(transferFileDialog->getSourceFileText()) ? transferSourceDir + transferFileDialog->getSourceFileText() : transferFileDialog->getSourceFileText();
            // Get Transfer Target
            QString targetText = isPathRelative(transferFileDialog->getTargetFileText()) ? transferSourceDir + transferFileDialog->getTargetFileText() : transferFileDialog->getTargetFileText();

            // Get Source Info
            QFileInfo sourceInfo(sourceText);

            // Check If Source File Exists
            if (sourceInfo.exists()) {
                // Launch Progress Dialog
                newTransferProgressDialog->launch(sourceText, targetText, copyOptions);

            } else {

                // Get Target Info
                //QFileInfo targetInfo(targetText);

                // Split Source Elements
                QStringList sourceElements = splitPath(sourceText);
                // Split Target Elements
                QStringList targetElements = splitPath(targetText);

                // Update Transfer Source Dir
                transferSourceDir = sourceElements[0];
                // Update Transfer Target Dir
                transferTargetDir = targetElements[0];

                // Init Source Pattern
                QString sourcePattern = sourceElements[1];
                // Init Target Pattern
                QString targetPattern = targetElements[1];

                // Update Source Info
                sourceInfo = QFileInfo(transferSourceDir);

                // Check If Transfer Source Dir Exists
                if (sourceInfo.isDir()) {

                    // Add To Transfer Progress Dialog List
                    transferProgressDialogs << newTransferProgressDialog;

                    // Launch Progress Dialog
                    newTransferProgressDialog->launch(transferSourceDir, transferTargetDir, sourcePattern, targetPattern, copyOptions);

                } else {

                    // Init Info Dialog
                    InfoDialog infoDialog(tr(DEFAULT_WARNING_TEXT_INVALID_SOURCE), EIDTWarning);

                    // Exec Info Dialog
                    infoDialog.exec();

                    // Delete New Progress Dialog
                    delete newTransferProgressDialog;
                    // REset Pointer
                    newTransferProgressDialog = NULL;
                }
            }

        } else {

            // Add To Transfer Progress Dialog List
            transferProgressDialogs << newTransferProgressDialog;

            // Check Selected Files Count
            if (selectedFiles.count() == 1) {
                // Launch Progress Dialog
                newTransferProgressDialog->launch(transferFileDialog->getSourceFileText(), transferFileDialog->getTargetFileText(), copyOptions);
            } else {
                // Launch Progress Dialog
                newTransferProgressDialog->launch(transferSourceDir, transferTargetDir, selectedFiles, copyOptions);
            }
        }

        // Clear Selected Files
        focusedPanel->deselectAllFiles();
    }
}

//==============================================================================
// Launch Create Dir
//==============================================================================
void MainWindow::launchCreateDir()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

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

//==============================================================================
// Launch Search Slot
//==============================================================================
void MainWindow::launchSearch()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    qDebug() << "MainWindow::launchSearch";

    // Reset Modifier Keys
    focusedPanel->resetModifierKeys();

    // Check Search Dialog
    if (!searchFileDialog) {
        // Create Search File Dialog
        searchFileDialog = new SearchDialog();

        // Connect Signals
        connect(searchFileDialog, SIGNAL(searchResultSelected(QString)), this, SLOT(searchResultSelected(QString)));
        connect(searchFileDialog, SIGNAL(searchResultView(QString,bool)), this, SLOT(searchResultView(QString,bool)));
    }

    // Check If Dialog Shown
    if (!searchFileDialog->isDialogShown()) {
        qDebug() << "MainWindow::launchSearch";

        // Init Dir Path
        QString dirPath = focusedPanel->getCurrentDir();
        // Show Dialog
        searchFileDialog->showDialog(dirPath, focusedPanel);
    }
}

//==============================================================================
// Launch Create Link
//==============================================================================
void MainWindow::launchCreateLink()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    qDebug() << "MainWindow::launchCreateLink";

    // Reset Modifier Keys
    focusedPanel->resetModifierKeys();

    // Check Create Link Dialog
    if (!createLinkDialog) {
        // Create Dialog
        createLinkDialog = new CreateLinkDialog();
    }

    // Launch Dialog
    if (createLinkDialog->launchDialog(focusedPanel->getCurrentDir())) {

        // Init Link Path
        QString linkPath = focusedPanel->getCurrentDir();

        // Check Link Path
        if (!linkPath.endsWith("/")) {
            // Adjust Link Path
            linkPath += "/";
        }

        // Add Link Name
        linkPath += createLinkDialog->getLinkName();

        // Create Link
        focusedPanel->createLink(linkPath, createLinkDialog->getLinkTarget());
    }

    // ...

}

//==============================================================================
// Launch Delete
//==============================================================================
void MainWindow::launchDelete()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Get Number Of Transfer Progress Dialogs
    int numTransferProgressDialogs = transferProgressDialogs.count();
    // Get number Of Delete Progress Dialogs
    int numDeleteProgressDialogs = deleteProgressDialogs.count();

    // Check Number Of Progress Dialogs
    if ((numTransferProgressDialogs + numDeleteProgressDialogs) >= MAX_NUMBER_OF_FILE_THREADS) {
        qWarning() << "MainWindow::launchDelete - MAX NUMBER OF THREADS REACHED!!";

        return;
    }

    // Check Delete File Dialog
    if (!deleteFileDialog) {
        // Create Delete File Dialog
        deleteFileDialog = new DeleteFileDialog();
    }

    // Get Selected File List
    QStringList selectedFileList = focusedPanel->getSelectedFiles();

    // Check Selected File List
    if (selectedFileList.count() == 0) {
        qDebug() << "MainWindow::launchDelete - NO SELECTED FILE!";

        return;
    }

    qDebug() << "MainWindow::launchDelete";

    // Get Focused Panel Current Dir
    QString currDir = focusedPanel->getCurrentDir();

    // Check Current Dir
    if (!currDir.endsWith("/")) {
        // Adjust Current Dir
        currDir += "/";
    }

    // Check Selected File List
    if (selectedFileList.count() == 1) {
        // Setup Delete File Dialog
        deleteFileDialog->setFileName(currDir + selectedFileList[0]);
    } else {
        // Setup Delete File Dialog
        deleteFileDialog->setFileName(currDir);
    }

    // ...

    // Launch Dialog
    if (deleteFileDialog->exec()) {

        // Create Delete Progress Dialog
        DeleteProgressDialog* newDialog = new DeleteProgressDialog();

        // Connect Signal
        connect(newDialog, SIGNAL(dialogClosed(DeleteProgressDialog*)), this, SLOT(deleteProgressClosed(DeleteProgressDialog*)), Qt::QueuedConnection);

        // Add To Delete Progress Dialog List
        deleteProgressDialogs << newDialog;

        // Launch
        newDialog->launch(focusedPanel->getCurrentDir(), focusedPanel->getSelectedFiles());

        // Clear Selected Files
        focusedPanel->deselectAllFiles();
    }
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

        // Connect Signals
        connect(preferencesDialog, SIGNAL(settingsHasChanged()), this, SLOT(settingsHasChanged()));
    }

    // Show Dialog
    preferencesDialog->execDialog();
}

//==============================================================================
// Launch File Select Slot
//==============================================================================
void MainWindow::launchFileSelect()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    //qDebug() << "MainWindow::launchFileSelect";

    // Reset Modifier Keys
    focusedPanel->resetModifierKeys();

    // Check File Select Dialog
    if (!selectFilesDialog) {
        // Create Dialog
        selectFilesDialog = new SelectFilesDialog();
    }

    // Launch Dialog
    if (selectFilesDialog->launchDialog(EFSMSelect)) {
        // Get Pattern
        QString pattern = selectFilesDialog->getPattern();

        // Check Pattern
        if (pattern.indexOf("*") == -1) {
            // Adjust Pattern
            pattern = "*" + pattern + "*";
        }

        qDebug() << "MainWindow::launchFileSelect - pattern: " << pattern;

        // Select Files
        focusedPanel->selectFiles(pattern);
    }
}

//==============================================================================
// Launch File Deselect Slot
//==============================================================================
void MainWindow::launchFileDeselect()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    //qDebug() << "MainWindow::launchFileDeselect";

    // Reset Modifier Keys
    focusedPanel->resetModifierKeys();

    // Check File Select Dialog
    if (!selectFilesDialog) {
        // Create Dialog
        selectFilesDialog = new SelectFilesDialog();
    }

    // Launch Dialog
    if (selectFilesDialog->launchDialog(EFSMDeselect)) {
        // Get Pattern
        QString pattern = selectFilesDialog->getPattern();

        // Check Pattern
        if (pattern.indexOf("*") == -1) {
            // Adjust Pattern
            pattern = "*" + pattern + "*";
        }

        qDebug() << "MainWindow::launchFileDeselect - pattern: " << pattern;

        // Deselect Files
        focusedPanel->deselectFiles(pattern);
    }
}

//==============================================================================
// Settings Has Changed Slot
//==============================================================================
void MainWindow::settingsHasChanged()
{
    qDebug() << "MainWindow::settingsHasChanged";

    // Restore UI
    restoreUI(false, (focusedPanel == leftPanel) ? 0 : 1);

    // Check Left Panel
    if (leftPanel) {
        // Set Last File Name
        leftPanel->lastFileName = leftPanel->getCurrFileInfo().fileName();
        // Reload
        leftPanel->reload();
    }

    // Check Right Panel
    if (rightPanel) {
        // Set Last File Name
        rightPanel->lastFileName = rightPanel->getCurrFileInfo().fileName();
        // Reload
        rightPanel->reload();
    }
}

//==============================================================================
// Search Result Item Selected Slot
//==============================================================================
void MainWindow::searchResultSelected(const QString& aFilePath)
{
    // Check Search File Dialog
    if (searchFileDialog) {
        qDebug() << "MainWindow::searchResultSelected - aFilePath: " << aFilePath;

        // Close Dialog
        searchFileDialog->hide();

        // Get Search File Dialog focused Panel
        FilePanel* searchFocusedPanel = searchFileDialog->getFocusedPanel();

        // Check Focused Panel
        if (searchFocusedPanel) {
            // Refocus
            searchFocusedPanel->setFocus();
            // Init Selected File Info
            QFileInfo selectedFileInfo(aFilePath);
            // Set Current Dir
            searchFocusedPanel->setCurrentDir(selectedFileInfo.absolutePath(), selectedFileInfo.fileName());
        }
    }
}

//==============================================================================
// Search Result Item View Slot
//==============================================================================
void MainWindow::searchResultView(const QString& aFilePath, const bool& aEdit)
{
    // Check Search File Dialog
    if (searchFileDialog) {
        qDebug() << "MainWindow::searchResultView - aFilePath: " << aFilePath << " - aEdit: " << aEdit;

        // Set Search Result View
        viewSearchResult = true;

        // Launch Viewer
        launchViewer(aFilePath, searchFileDialog->getFocusedPanel(), aEdit, false);
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

    // Save Settings
    saveSettings();

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
    //qDebug() << "MainWindow::modifierKeysChanged - aModifiers: " << aModifiers;

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
        //qDebug() << "MainWindow::updateFunctionKeys - SHIFT";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr("Rename"));
        ui->makeDirButton->setText(tr("MakeLink"));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::ControlModifier) {
        //qDebug() << "MainWindow::updateFunctionKeys - CONTROL";

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
        //qDebug() << "MainWindow::updateFunctionKeys - ALT";

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
        //qDebug() << "MainWindow::updateFunctionKeys - META";

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
    //qDebug() << "MainWindow::updateMenu";

    // Check Focused Panel
    if (focusedPanel) {

        // Switch Sorting
        switch (focusedPanel->getSorting()) {
            default:
            case DEFAULT_SORT_NAME:     ui->actionSort_by_Name->setChecked(true); break;
            case DEFAULT_SORT_EXT:      ui->actionSort_by_Extension->setChecked(true); break;
            case DEFAULT_SORT_TYPE:     ui->actionSort_by_Type->setChecked(true); break;
            case DEFAULT_SORT_SIZE:     ui->actionSort_by_Size->setChecked(true); break;
            case DEFAULT_SORT_DATE:     ui->actionSort_by_Date->setChecked(true); break;
            case DEFAULT_SORT_OWNER:    ui->actionSort_by_Owner->setChecked(true); break;
            case DEFAULT_SORT_PERMS:    ui->actionSort_by_Permissions->setChecked(true); break;
            case DEFAULT_SORT_ATTRS:    break;
        }

        // Set Show Hidden Checked
        ui->actionShow_Hide_Hiden->setChecked(settings->getShowHiddenFiles());
    }
}

//==============================================================================
// Toggle Hidden Files
//==============================================================================
void MainWindow::toggleHiddenFile()
{
    // Check Settings
    if (!settings) {
        return;
    }

    qDebug() << "MainWindow::toggleHiddenFile";

    // Set Show Hidden Files
    settings->setShowHiddenFiles(!settings->getShowHiddenFiles());
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

                // Check If File Was Modified
                if (window->isFileModified()) {
                    // Check Focused Panel
                    if (focusedPanel) {

                        // Get File Path
                        QString editedPath = getDirPath(window->getContentSource());

                        // Chek Focused Panel Current Dir
                        if (focusedPanel->getCurrentDir() == editedPath) {
                            // Get File Name
                            QString editedFileName = getFileName(window->getContentSource());

                            // Check If Is New File
                            if (window->isNewFile()) {
                                // Set Last File Name
                                focusedPanel->lastFileName = editedFileName;
                                // Reload
                                focusedPanel->reload();
                            } else {
                                // Find Index
                                int editedIndex = focusedPanel->getFileIndex(editedFileName);
                                // Set Current Index
                                focusedPanel->setCurrentIndex(editedIndex);
                            }
                        }
                    }
                }

                // Remove From Window List
                viewerWindows.removeAt(i);

                // Delete Viewer Window
                delete window;

                break;
            }
        }
    }

    // Check Focused Panel
    if (focusedPanel) {
        // Reset Modifiers
        focusedPanel->resetModifierKeys();
    }

    // Check Search Result View
    if (viewSearchResult) {
        // Reset Serach Result View
        viewSearchResult = false;

        // Check Search File Dialog
        if (searchFileDialog && searchFileDialog->isVisible()) {
            // Show Normal
            searchFileDialog->showNormal();
            // Raise
            searchFileDialog->raise();
            // Activate Window
            searchFileDialog->activateWindow();
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
        qDebug() << "MainWindow::deleteProgressClosed - aDeleteProgressDialog: " << aDeleteProgressDialog;

        // Get Delete Progress Dialogs Count
        int dpdCount = deleteProgressDialogs.count();

        // Go Thru Dialogs
        for (int i=0; i<dpdCount; ++i) {
            // Get Dialog
            DeleteProgressDialog* dialog = deleteProgressDialogs[i];
            // Check Dialog
            if (dialog == aDeleteProgressDialog) {

                // Check Left Panel
                if (leftPanel /*&& (leftPanel->dwDirChanged || leftPanel->dwFileChanged)*/) {
                    // Check Current Dir
                    if (dialog->dirPath == leftPanel->currentDir) {
                        // Reload
                        leftPanel->reload(leftPanel->currentIndex);
                    }
                }

                // Check Right Panel
                if (rightPanel /*&& (rightPanel->dwDirChanged || rightPanel->dwFileChanged)*/) {
                    // Check Current Dir
                    if (dialog->dirPath == rightPanel->currentDir) {
                        // Reload
                        rightPanel->reload(rightPanel->currentIndex);
                    }
                }

                // Remove Dialog From Delete Progress Dialogs
                deleteProgressDialogs.removeAt(i);

                // Delete Later
                dialog->deleteLater();
            }
        }
    }
}

//==============================================================================
// Transfer Progress Window Closed Slot
//==============================================================================
void MainWindow::transferProgressClosed(TransferProgressDialog* aTransferProgressDialog)
{
    // Check Progress Dialog
    if (aTransferProgressDialog) {
        qDebug() << "MainWindow::transferProgressClosed - aTransferProgressDialog: " << aTransferProgressDialog;

        // Get Transfer Progress Dialogs Count
        int tpdCount = transferProgressDialogs.count();

        // Go Thru Dialogs
        for (int i=0; i<tpdCount; ++i) {
            // Get Dialog
            TransferProgressDialog* dialog = transferProgressDialogs[i];

            // Check Dialog
            if (dialog == aTransferProgressDialog) {
                // Check Left Panel
                if (leftPanel /*&& (leftPanel->dwDirChanged || leftPanel->dwFileChanged)*/) {
                    // Check Source Path
                    if (dialog->targetPath == leftPanel->currentDir) {
                        // Set Last File Name
                        leftPanel->lastFileName = QFileInfo(dialog->getLastTarget()).fileName();
                        // Try To Find Lat Item File Name & Reload
                        leftPanel->reload();

                    } else if (dialog->operation == DEFAULT_OPERATION_MOVE_FILE && dialog->sourcePath == leftPanel->currentDir) {
                        // Reload
                        leftPanel->reload(leftPanel->currentIndex);
                    }
                }

                // Check Right Panel
                if (rightPanel /*&& (rightPanel->dwDirChanged || rightPanel->dwFileChanged)*/) {
                    // Check Dialog Target Path
                    if (dialog->targetPath == rightPanel->currentDir) {
                        // Set Last Target File Name
                        rightPanel->lastFileName = QFileInfo(dialog->getLastTarget()).fileName();
                        // Reload
                        rightPanel->reload();

                    } else if (dialog->operation == DEFAULT_OPERATION_MOVE_FILE && dialog->sourcePath == rightPanel->currentDir) {
                        // Reload
                        rightPanel->reload(rightPanel->currentIndex);
                    }
                }

                // Remove Dialog From Transfer Progress Dialogs
                transferProgressDialogs.removeAt(i);

                // Delete Later
                dialog->deleteLater();
            }
        }
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

    // Update Menu
    updateMenu();
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
    // Check Focused Panel
    if (focusedPanel) {
        // Check Sorting

        // Sort Focused Panel Items By Name
        focusedPanel->setSorting(DEFAULT_SORT_NAME);
    }
}

//==============================================================================
// Action Sort By Extension Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Extension_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
    // Sort Focused Panel Items By Extension
    }
}

//==============================================================================
// Action Sort By Type Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Type_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Type
        focusedPanel->setSorting(DEFAULT_SORT_EXT);
    }
}

//==============================================================================
// Action Sort By Size Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Size_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Size
        focusedPanel->setSorting(DEFAULT_SORT_SIZE);
    }
}

//==============================================================================
// Action Sort By Date Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Date_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Date
        focusedPanel->setSorting(DEFAULT_SORT_DATE);
    }
}

//==============================================================================
// Action Sort By Owner Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Owner_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Owner
        focusedPanel->setSorting(DEFAULT_SORT_OWNER);
    }
}

//==============================================================================
// Action Sort By Permission Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Permissions_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Permission
        focusedPanel->setSorting(DEFAULT_SORT_PERMS);
    }
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
    // Launch Editor
    launchViewer(true, true);
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
// Action Copy File Triggered Slot
//==============================================================================
void MainWindow::on_actionCopy_triggered()
{
    // Launch File Copy
    launchFileCopy();
}

//==============================================================================
// Action Move File Triggered Slot
//==============================================================================
void MainWindow::on_actionMove_triggered()
{
    // Launch File Move
    launchFileMove();
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
    // Shut Down
    shutDown();

    // Check View Menu Action Group
    if (viewMenuActionGroup) {
        // Delete View Menu Action Group
        delete viewMenuActionGroup;
        viewMenuActionGroup = NULL;
    }

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        // Reset Settings
        settings = NULL;
    }

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

    // Check Search File Dialog
    if (searchFileDialog) {
        // Delete Search File Dialog
        delete searchFileDialog;
        searchFileDialog = NULL;
    }

    // Check Select Files Dialog
    if (selectFilesDialog) {
        // Delete Dialog
        delete selectFilesDialog;
        selectFilesDialog = NULL;
    }

    qDebug() << "MainWindow::~MainWindow";
}



