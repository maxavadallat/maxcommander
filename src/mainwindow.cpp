
// INCLUDES

#include <QApplication>
#include <QEventLoop>

#include "constants.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "custompanel.h"
#include "ui_custompanel.h"
#include "customfilelist.h"
#include "ui_customfilelist.h"
#include "customheader.h"
#include "settingswindow.h"
#include "settings.h"
#include "createdirdialog.h"
#include "infodialog.h"
#include "confirmationdialog.h"
#include "searchdialog.h"
#include "copydialog.h"
#include "copyprogressdialog.h"
#include "deleteprogressdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
MainWindow::MainWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainWindow)
    , shiftKeyPressed(false)
    , altKeyPressed(false)
    , controlKeyPressed(false)
    , metaKeyPressed(false)
    , cPanelIndex(-1)
    , cPanelName("")
    , settings(NULL)
    , createDirDialog(NULL)
    , infoDialog(NULL)
    , confirmDialog(NULL)
    , searchDialog(NULL)
    , copyDialog(NULL)
{
    qDebug() << "Creating MainWindow...";

    // Setup UI
    ui->setupUi(this);

    // Load Settings
    loadSettings();

    connect(ui->mainPanel1->ui->fileList, SIGNAL(fileListKeyPressed(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyPressed(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel1->ui->fileList, SIGNAL(fileListKeyReleased(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyReleased(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel1->ui->fileList, SIGNAL(listBoxFocusChanged(QString,bool)), this, SLOT(panelFocusChanged(QString,bool)));

    connect(ui->mainPanel2->ui->fileList, SIGNAL(fileListKeyPressed(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyPressed(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel2->ui->fileList, SIGNAL(fileListKeyReleased(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyReleased(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel2->ui->fileList, SIGNAL(listBoxFocusChanged(QString,bool)), this, SLOT(panelFocusChanged(QString,bool)));

    // ...

    qDebug() << "Creating MainWindow...done";
}

//==============================================================================
// Launch Help
//==============================================================================
void MainWindow::launchHelp()
{
    qDebug() << "MainWindow::launchHelp";

}

//==============================================================================
// Show tree
//==============================================================================
void MainWindow::showTree()
{
    qDebug() << "MainWindow::showTree";

}

//==============================================================================
// View File
//==============================================================================
void MainWindow::viewFile()
{
    qDebug() << "MainWindow::viewFile";

}

//==============================================================================
// Edit File
//==============================================================================
void MainWindow::editFile()
{
    qDebug() << "MainWindow::editFile";

}

//==============================================================================
// Copy Files
//==============================================================================
void MainWindow::copyFiles()
{
    qDebug() << "MainWindow::copyFiles";

}

//==============================================================================
// Move Files
//==============================================================================
void MainWindow::moveFiles()
{
    qDebug() << "MainWindow::moveFiles";

}

//==============================================================================
// Rename Current File
//==============================================================================
void MainWindow::renameCurrentFile()
{
    qDebug() << "MainWindow::renameCurrentFile";

}

//==============================================================================
// Create Dir
//==============================================================================
void MainWindow::launchCreateDir()
{
    qDebug() << "MainWindow::launchCreateDir";

    // Check Create Dir Diralog
    if (!createDirDialog) {
        // Create Create Dir Dialog
        createDirDialog = new CreateDirDialog();
    }

    // Check Create Dir Dialog
    if (createDirDialog) {
        // Exec Dialog
        if (createDirDialog->exec()) {
            qDebug() << "MainWindow::createDir - cPanelName: " << cPanelName << " - newDir: " << createDirDialog->getDirText();

            // ...
        }
    }
}

//==============================================================================
// Delete Files
//==============================================================================
void MainWindow::launchDelete()
{
    // Check UI
    if (!ui || !ui->mainPanel1)
        return;

    // Check UI
    if (!ui || !ui->mainPanel2)
        return;

    qDebug() << "MainWindow::launchDelete";

    // Init Selected Files Count
    int sfCount = 0;
    // Init Current File Name
    QString currFileName = QString("");

    // Get Current Panel Index
    if (cPanelIndex == 0) {
        // Get Selected Files Count
        sfCount = ui->mainPanel1->getSelectedCount();
        // Check Panel UI
        if (ui->mainPanel1->ui && ui->mainPanel1->ui->fileList) {
            // Get Current File Name
            currFileName = ui->mainPanel1->ui->fileList->getItemData(ui->mainPanel1->ui->fileList->getCurrentIndex())->getFileInfo().fileName();
        }
    } else if (cPanelIndex == 1) {
        // Get Selected Files Count
        sfCount = ui->mainPanel2->getSelectedCount();
        // Check Panel UI
        if (ui->mainPanel2->ui && ui->mainPanel2->ui->fileList) {
            // Get Current File Name
            currFileName = ui->mainPanel2->ui->fileList->getItemData(ui->mainPanel2->ui->fileList->getCurrentIndex())->getFileInfo().fileName();
        }
    }

    // Check Current File Name
    if (currFileName == QString("..") || currFileName == QString(".")) {
        return;
    }

    // Init Confirm Title
    QString confirmTitle = (sfCount > 1) ? QString(DEFAULT_DELETE_DIALOG_TITLE_TEXT_FILES) : QString(DEFAULT_DELETE_DIALOG_TITLE_TEXT_FILE);
    // Init Confirm Text
    QString confirmText = (sfCount > 1) ? QString(DEFAULT_DELETE_DIALOG_TEXT_FILES).arg(sfCount) : QString(DEFAULT_DELETE_DIALOG_TEXT_FILE).arg(currFileName);

    // Show Confirmation
    int result = showConfirmation(confirmTitle, confirmText, QDialogButtonBox::Yes | QDialogButtonBox::No);

    // Check Result
    if (result == QDialog::Accepted) {

        // Build Delete Queue

        // Exec Delete Queue

    }
}

//==============================================================================
// Launch Copy
//==============================================================================
void MainWindow::launchCopy()
{
    qDebug() << "MainWindow::launchCopy";

}

//==============================================================================
// Launch Move
//==============================================================================
void MainWindow::launchMove()
{
    qDebug() << "MainWindow::launchMove";

}

//==============================================================================
// Launch Search
//==============================================================================
void MainWindow::launchSearch()
{
    qDebug() << "MainWindow::launchSearch";

    // Check Search Dialog
    if (!searchDialog) {
        // Create Search Dialog
        searchDialog = new SearchDialog();
    }

    // Check Search Dialog
    if (searchDialog) {
        // Launch Search Dialog
        if (searchDialog->exec()) {

            // Start Search

        }

        // Reset Alt Key Pressed
        altKeyPressed = false;
        // Configure Function Keys
        configureFunctionKeys();
    }
}

//==============================================================================
// Launch Options
//==============================================================================
void MainWindow::launchOptions()
{
    qDebug() << "MainWindow::launchOptions";
    // Init Event Loop
    QEventLoop eventLoop;
    // Init Settigns Window
    SettingsWindow sw(&eventLoop);
    // Show Settings Window
    sw.show();
    // Execute Event Loop
    eventLoop.exec(QEventLoop::DialogExec);
}

//==============================================================================
// Exit Application
//==============================================================================
void MainWindow::exitApp()
{
    qDebug() << "MainWindow::exitApp";
    // Exit Application
    QApplication::exit(0);
}

//==============================================================================
// Switch To Panel
//==============================================================================
void MainWindow::switchToPanel(const int& aIndex)
{
    // Check Index
    if (cPanelIndex != aIndex && aIndex >= 0 && aIndex < DEFAULT_FILELIST_PANEL_COUNT) {
        // Set Current Panel Index
        cPanelIndex = aIndex;
        // Check UI
        if (!ui || !ui->mainPanel1 || !ui->mainPanel2)
            return;

        // Check Current Panel Index
        if (cPanelIndex == 0) {
            // Set Panel Active
            ui->mainPanel1->setActive(true);
            // Set Other Panel Inactive
            ui->mainPanel2->setActive(false);
        } else if (cPanelIndex == 1){
            // Set Other Panel Inactive
            ui->mainPanel2->setActive(true);
            // Set Panel Active
            ui->mainPanel1->setActive(false);
        }
    }
}

//==============================================================================
// Show Info Dialog
//==============================================================================
void MainWindow::showInfo(const QString& aTitle, const QString& aInfoMsg, const bool& aModal)
{
    // Check Title And Info Message
    if (!aTitle.isEmpty() && !aInfoMsg.isEmpty()) {

        // Check Info Dialog
        if (!infoDialog) {
            // Create Info Dialog
            infoDialog = new InfoDialog();
        }

        // Check Info Dialog
        if (infoDialog) {
            // Set Title
            infoDialog->setTitle(aTitle);
            // Set Info Message
            infoDialog->setInfoMsg(aInfoMsg);
            // Check Modal State
            if (aModal) {
                // Exec
                infoDialog->exec();
            } else {
                // Center
                infoDialog->move((width() - infoDialog->width()) / 2, (height() - infoDialog->height()) / 2);
                // Show
                infoDialog->show();
            }
        }
    }
}

//==============================================================================
// Show Confirmation Dialog
//==============================================================================
int MainWindow::showConfirmation(const QString& aTitle, const QString& aConfirmMsg, const QDialogButtonBox::StandardButtons& aButtons)
{
    // Check Title, Message And Buttons
    if (!aTitle.isEmpty() && !aConfirmMsg.isEmpty() && aButtons) {
        // Check Confirmation Dialog
        if (!confirmDialog) {
            // Create Confirmation Dialog
            confirmDialog = new ConfirmationDialog();
        }

        // Check Confirmation Dialog
        if (confirmDialog) {
            // Set Confirm Dialog Title
            confirmDialog->setTitle(aTitle);
            // Set Confirm Dialog Text
            confirmDialog->setConfirmMsg(aConfirmMsg);
            // Set Buttons
            confirmDialog->setButtons(aButtons);
            // Exec Confirm Dialog
            return confirmDialog->exec();
        }
    }

    return 0;
}

//==============================================================================
// Show Create Dir Dialog
//==============================================================================
bool MainWindow::showCreateDirDialog()
{
    // ...

    return false;
}

//==============================================================================
// Configure Function Keys
//==============================================================================
void MainWindow::configureFunctionKeys()
{
    // Check UI
    if (!ui)
        return;

    // Check Shift Key Pressed
    if (shiftKeyPressed) {
        // Set Button Texts
        ui->f1Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F1_SHIFT);
        ui->f2Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F2_SHIFT);
        ui->f3Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F3_SHIFT);
        ui->f4Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F4_SHIFT);
        ui->f5Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F5_SHIFT);
        ui->f6Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F6_SHIFT);
        ui->f7Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F7_SHIFT);
        ui->f8Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F8_SHIFT);
        ui->f9Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F9_SHIFT);
        ui->f10Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F10_SHIFT);
    // Check If Alt Key Pressed
    } else if (altKeyPressed) {
        // Set Button Texts
        ui->f1Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F1_ALT);
        ui->f2Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F2_ALT);
        ui->f3Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F3_ALT);
        ui->f4Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F4_ALT);
        ui->f5Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F5_ALT);
        ui->f6Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F6_ALT);
        ui->f7Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F7_ALT);
        ui->f8Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F8_ALT);
        ui->f9Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F9_ALT);
        ui->f10Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F10_ALT);
    }
#if defined(Q_OS_MAC)
    // Check If Control Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {
        // Set Button Texts
        ui->f1Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F1_CONTROL);
        ui->f2Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F2_CONTROL);
        ui->f3Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F3_CONTROL);
        ui->f4Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F4_CONTROL);
        ui->f5Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F5_CONTROL);
        ui->f6Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F6_CONTROL);
        ui->f7Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F7_CONTROL);
        ui->f8Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F8_CONTROL);
        ui->f9Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F9_CONTROL);
        ui->f10Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F10_CONTROL);
    } else {
        // Set Button Texts
        ui->f1Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F1);
        ui->f2Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F2);
        ui->f3Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F3);
        ui->f4Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F4);
        ui->f5Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F5);
        ui->f6Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F6);
        ui->f7Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F7);
        ui->f8Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F8);
        ui->f9Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F9);
        ui->f10Button->setText(DEFAULT_FUNCTION_KEY_TEXT_F10);
    }
}

//==============================================================================
// Load Settings
//==============================================================================
void MainWindow::loadSettings()
{
    // Check UI
    if (ui && ui->mainPanel1 && ui->mainPanel2) {
        qDebug() << "MainWindow::loadSettings";

        // Check Settings
        if (!settings) {
            // Get Settings Instance
            settings = Settings::getInstance();
        }

        // Set Panels Name For Settings
        ui->mainPanel1->setPanelName(QString(DEFAULT_FILELIST_PANEL_NAME1));
        ui->mainPanel2->setPanelName(QString(DEFAULT_FILELIST_PANEL_NAME2));

        // Load Panel Settings
        ui->mainPanel1->loadSettings();
        ui->mainPanel2->loadSettings();

        // Check Settings
        if (settings) {
            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_GENERAL));
            // Get Last Active Panel Index
            //cPanelIndex = settings->getValue(SETTINGS_KEY_ACTIVE_PANEL, 0).toInt();
            // Switch To Panel
            switchToPanel(settings->getValue(SETTINGS_KEY_ACTIVE_PANEL, 0).toInt());
        }
    }
}

//==============================================================================
// Save Settings
//==============================================================================
void MainWindow::saveSettings()
{
    // Check UI
    if (ui && ui->mainPanel1 && ui->mainPanel2) {
        qDebug() << "MainWindow::saveSettings";
        // Check Settings
        if (settings) {
            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_GENERAL));
            // Set Last Active Panel Index
            settings->setValue(SETTINGS_KEY_ACTIVE_PANEL, cPanelIndex);
        }
        // Save Panel Settings
        ui->mainPanel1->saveSettings();
        ui->mainPanel2->saveSettings();
    }
}

//==============================================================================
// Panel Focus Changed Slot
//==============================================================================
void MainWindow::panelFocusChanged(const QString& aPanelName, const bool& aFocused)
{
    // Check Focused
    if (aFocused && cPanelName != aPanelName) {
        qDebug() << "MainWindow::panelFocusChanged - aPanelName: " << aPanelName << " - aFocused: " << aFocused;

        // Set Current Panel Name
        cPanelName = aPanelName;
        // Check Current Panel Name
        if (cPanelName == QString(DEFAULT_FILELIST_PANEL_NAME1)) {
            // Set Current Panel Index
            cPanelIndex = 0;
            // Check UI
            if (ui->mainPanel2) {
                // Set Other Panel Inactive
                ui->mainPanel2->setActive(false);
            }
        } else if (cPanelName == QString(DEFAULT_FILELIST_PANEL_NAME2)) {
            // Set Current Panel Index
            cPanelIndex = 1;
            // Check UI
            if (ui->mainPanel1) {
                // Set Other Panel Inactive
                ui->mainPanel1->setActive(false);
            }
        }
    }
}

//==============================================================================
// Panel Key Pressed Slot
//==============================================================================
void MainWindow::panelKeyPressed(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    // Set Current Panel Name
    cPanelName = aPanelName;
    // Set Shift Key Pressed
    shiftKeyPressed = aModifiers & Qt::ShiftModifier;
    // Set Alt Key Pressed
    altKeyPressed = aModifiers & Qt::AltModifier;
    // Set Control Key Pressed
    controlKeyPressed = aModifiers & Qt::ControlModifier;
    // Set Meta Key Pressed
    metaKeyPressed = aModifiers & Qt::MetaModifier;

    // Configure Function Keys
    configureFunctionKeys();

    // Switch Key
    switch (aKey) {
        default: {
            //qDebug() << "MainWindow::panelKeyPressed - cPanelName: " << cPanelName << " - aKey: " << aKey;
        }
    }
}

//==============================================================================
// Panel Key Released Slot
//==============================================================================
void MainWindow::panelKeyReleased(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    // Set Current Panel Name
    cPanelName = aPanelName;
    // Set Shift Key Pressed
    shiftKeyPressed = aModifiers & Qt::ShiftModifier;
    // Set Alt Key Pressed
    altKeyPressed = aModifiers & Qt::AltModifier;
    // Set Control Key Pressed
    controlKeyPressed = aModifiers & Qt::ControlModifier;
    // Set Meta Key Pressed
    metaKeyPressed = aModifiers & Qt::MetaModifier;

    // Switch Key
    switch (aKey) {
        case Qt::Key_Tab: {
            //qDebug() << "MainWindow::panelKeyReleased - aPanelName: " << aPanelName << " - Tab Switching Panel";

            // Switch To Panel
            //switchToPanel((cPanelIndex == 0) ? 1 : 0);
        } break;

        case Qt::Key_F1:    on_f1Button_clicked();  break;
        case Qt::Key_F2:    on_f2Button_clicked();  break;
        case Qt::Key_F3:    on_f3Button_clicked();  break;
        case Qt::Key_F4:    on_f4Button_clicked();  break;
        case Qt::Key_F5:    on_f5Button_clicked();  break;
        case Qt::Key_F6:    on_f6Button_clicked();  break;
        case Qt::Key_F7:    on_f7Button_clicked();  break;
        case Qt::Key_F8:    on_f8Button_clicked();  break;
        case Qt::Key_F9:    on_f9Button_clicked();  break;
        case Qt::Key_F10:   on_f10Button_clicked(); break;

        case Qt::Key_H: {
#ifdef Q_OS_MAC
            if (metaKeyPressed)
#else // Q_OS_MAC
            if (controlKeyPressed)
#endif // Q_OS_MAC
            {
                // Check UI & Panels
                if (ui && ui->mainPanel1 && ui->mainPanel2) {
                    // Set Show Hidden Files
                    ui->mainPanel1->setShowHiddenFiles(!ui->mainPanel1->getShowHiddenFiles());
                    ui->mainPanel2->setShowHiddenFiles(!ui->mainPanel2->getShowHiddenFiles());
                }
            }
        } break;

        case Qt::Key_Delete: {
            // Launch Delete
            launchDelete();
        } break;

        default: {
            //qDebug() << "MainWindow::panelKeyReleased - cPanelName: " << cPanelName << " - aKey: " << aKey;
        } break;
    }

    // Configure Function Keys
    configureFunctionKeys();
}

//==============================================================================
// F1 Button Clicked Slot
//==============================================================================
void MainWindow::on_f1Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Launch Help
        launchHelp();
    }
}

//==============================================================================
// F2 Button Clicked Slot
//==============================================================================
void MainWindow::on_f2Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Show Tree
        showTree();
    }
}

//==============================================================================
// F3 Button Clicked Slot
//==============================================================================
void MainWindow::on_f3Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // View File
        viewFile();
    }
}

//==============================================================================
// F4 Button Clicked Slot
//==============================================================================
void MainWindow::on_f4Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Edit File
        editFile();
    }
}

//==============================================================================
// F5 Button Clicked Slot
//==============================================================================
void MainWindow::on_f5Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Launch Copy Files
        launchCopy();
    }
}

//==============================================================================
// F6 Button Clicked Slot
//==============================================================================
void MainWindow::on_f6Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {
        // Rename Current File
        renameCurrentFile();
    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Launch Move Files
        launchMove();
    }
}

//==============================================================================
// F7 Button Clicked Slot
//==============================================================================
void MainWindow::on_f7Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {
        // Rename Current File
        renameCurrentFile();
    // Check If Alt Key Pressed
    } else if (altKeyPressed) {
        // Launch Search
        launchSearch();
    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Launch Create Directory
        launchCreateDir();
    }
}

//==============================================================================
// F8 Button Clicked Slot
//==============================================================================
void MainWindow::on_f8Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Launch Delete Files
        launchDelete();
    }
}

//==============================================================================
// F9 Button Clicked Slot
//==============================================================================
void MainWindow::on_f9Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Launch Options
        launchOptions();
    }
}

//==============================================================================
// F10 Button Clicked Slot
//==============================================================================
void MainWindow::on_f10Button_clicked()
{
    // Check If Shift Key Pressed
    if (shiftKeyPressed) {

    // Check If Alt Key Pressed
    } else if (altKeyPressed) {

    }
#if defined(Q_OS_MAC)
    // Check If Meta Key Pressed
    else if (metaKeyPressed)
#else // Q_OS_MAC
    // Check If Control Key Pressed
    else if (controlKeyPressed)
#endif // Q_OS_MAC
    {

    } else {
        // Exit Application
        exitApp();
    }
}

//==============================================================================
// Preferences Action Triggered Slot
//==============================================================================
void MainWindow::on_actionPreferences_triggered()
{
    // Launch Options
    launchOptions();
}

//==============================================================================
// Destructor
//==============================================================================
MainWindow::~MainWindow()
{
    qDebug() << "Deleting MainWindow...";

    // Check Settings
    if (settings) {

        // Save Settings
        saveSettings();

        // Save Settings
        settings->save();

        // Release Instance
        settings->release();
        settings = NULL;
    }

    // Check Confirmation Dialog
    if (confirmDialog) {
        // Delete Confirmaion Dialog
        delete confirmDialog;
        confirmDialog = NULL;
    }

    // Check Info Dialog
    if (infoDialog) {
        // Delete Info Dialog
        delete infoDialog;
        infoDialog = NULL;
    }

    // Check Create Dir Dialog
    if (createDirDialog) {
        // Delete Create Dir Dialog
        delete createDirDialog;
        createDirDialog = NULL;
    }

    // Check Search Dialog
    if (searchDialog) {
        // Delete Search Dialgo
        delete searchDialog;
        searchDialog = NULL;
    }

    // Check Copy Dialog
    if (copyDialog) {
        // Delete Copy Dialog
        delete copyDialog;
        copyDialog = NULL;
    }

    // Delete UI
    delete ui;

    qDebug() << "Deleting MainWindow...done";
}

