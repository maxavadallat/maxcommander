
// INCLUDES

#include <QApplication>
#include <QEventLoop>
#include <QProcess>

#if defined(Q_OS_WIN)

#include <Shellapi.h>

#endif // Q_OS_WIN

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
#include "viewerwindow.h"
#include "filerenamer.h"
#include "helpdialog.h"
#include "treewindow.h"
#include "aboutdialog.h"
#include "mainqueuedialog.h"
#include "fileutilsclient.h"
#include "adminpassdialog.h"


// Main Window Singleton
static MainWindow* mainWindow = NULL;


// IMPLEMENTATION

//==============================================================================
// Get Main Window Instance
//==============================================================================
MainWindow* MainWindow::getInstance()
{
    // Check Main Window
    if (!mainWindow) {
        // Create Main Window
        mainWindow = new MainWindow();
    } else {
        // Increase Ref Count
        mainWindow->refCount++;
    }

    return mainWindow;
}

//==============================================================================
// Release Instance
//==============================================================================
void MainWindow::release()
{
    // Decrease Ref Count
    refCount--;

    // Check Ref Count
    if (refCount <= 0) {
        // Delete Main Window
        delete this;
    }
}

//==============================================================================
// Constructor
//==============================================================================
MainWindow::MainWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , refCount(1)
    , ui(new Ui::MainWindow)
    , shiftKeyPressed(false)
    , altKeyPressed(false)
    , controlKeyPressed(false)
    , metaKeyPressed(false)
    , adminMode(false)
    , cPanelIndex(-1)
    , cPanelName("")
    , settings(NULL)
    , viewerWindow(NULL)
    , createDirDialog(NULL)
    , infoDialog(NULL)
    , confirmDialog(NULL)
    , errorDialog(NULL)
    , searchDialog(NULL)
    , copyDialog(NULL)
    , helpDialog(NULL)
    , treeWindow(NULL)
    , mainQueueDialog(NULL)
    , adminPassDialog(NULL)
    , fileUtilsClient(NULL)
{
    qDebug() << "Creating MainWindow...";

    // Setup UI
    ui->setupUi(this);

    // Load Settings
    loadSettings();

    // Set Main Toolbar Visibility
    //ui->mainToolbar->setVisible();

    connect(ui->mainPanel1->ui->fileList, SIGNAL(fileListKeyPressed(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyPressed(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel1->ui->fileList, SIGNAL(fileListKeyReleased(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyReleased(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel1->ui->fileList, SIGNAL(listBoxFocusChanged(QString,bool)), this, SLOT(panelFocusChanged(QString,bool)));

    connect(ui->mainPanel2->ui->fileList, SIGNAL(fileListKeyPressed(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyPressed(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel2->ui->fileList, SIGNAL(fileListKeyReleased(QString,int,Qt::KeyboardModifiers)), this, SLOT(panelKeyReleased(QString,int,Qt::KeyboardModifiers)));
    connect(ui->mainPanel2->ui->fileList, SIGNAL(listBoxFocusChanged(QString,bool)), this, SLOT(panelFocusChanged(QString,bool)));

    // ...

    // Show Status Message
    showStatusMessage(tr("Ready."));

    qDebug() << "Creating MainWindow...done";
}

//==============================================================================
// Launch Help
//==============================================================================
void MainWindow::launchHelp()
{
    qDebug() << "MainWindow::launchHelp";

    // Check Help Dialog
    if (!helpDialog) {
        // Create Help Dialog
        helpDialog = new HelpDialog();
    }

    // Check Help Dialog
    if (helpDialog) {
        // Show Help Dialog
        helpDialog->show();
    }
}

//==============================================================================
// Show tree
//==============================================================================
void MainWindow::showTree()
{
    qDebug() << "MainWindow::showTree";

    // Check Tree Window
    if (!treeWindow) {
        // Create Tree Window
        treeWindow = new TreeWindow();
    }

    // Check Tree Window
    if (treeWindow) {
        // Init Dir Path
        QString dirPath = QString("");
        // Init File Info
        QFileInfo fileInfo;

        // Check Current Panel
        if (cPanelIndex == 0) {
            // Get File Info
            fileInfo = ui->mainPanel1->getFileInfo(ui->mainPanel1->getCurrentIndex());
        } else {
            // Get File Info
            fileInfo = ui->mainPanel2->getFileInfo(ui->mainPanel2->getCurrentIndex());
        }

        // Check File Info
        if (fileInfo.isDir()) {
            // Set Dir Path
            dirPath = fileInfo.absoluteFilePath();
        } else {
            // Check Current Panel
            if (cPanelIndex == 0) {
                // Set Dir Path
                dirPath = ui->mainPanel1->ui->fileList->getCurrentDir();
            } else {
                // Set Dir Path
                dirPath = ui->mainPanel2->ui->fileList->getCurrentDir();
            }
        }

        // Check Dir Path
        if (!dirPath.isEmpty()) {
            // Set Root
            treeWindow->setRoot(dirPath);
            // Show Tree Window
            treeWindow->show();
        }
    }
}

//==============================================================================
// View File
//==============================================================================
void MainWindow::viewFile()
{
    // Check UI
    if (!ui || !ui->mainPanel1 || !ui->mainPanel2) {
        return;
    }

    // Check Viewer Window
    if (!viewerWindow) {
        // Create Viewer Window
        viewerWindow = new ViewerWindow();
    }

    // Init File Path
    QString filePath = QString("");
    // Check Current Panel
    if (cPanelIndex == 0) {
        // Get File Path
        filePath = ui->mainPanel1->getFileInfo(ui->mainPanel1->getCurrentIndex()).absoluteFilePath();
    } else {
        // Get File Path
        filePath = ui->mainPanel2->getFileInfo(ui->mainPanel2->getCurrentIndex()).absoluteFilePath();
    }

    // Check File Path & Load File
    if (!filePath.isEmpty() && viewerWindow && viewerWindow->loadFile(filePath)) {

        qDebug() << "MainWindow::viewFile";

    }
}

//==============================================================================
// Edit File
//==============================================================================
void MainWindow::editFile()
{
    // Check UI
    if (!ui || !ui->mainPanel1 || !ui->mainPanel2) {
        return;
    }

    // Check Editor Window

    // Create Editor Window


    // Init File Path
    QString filePath = QString("");
    // Init File Info
    QFileInfo fileInfo;
    // Check Current Panel
    if (cPanelIndex == 0) {
        // Get File Info
        fileInfo = ui->mainPanel1->getFileInfo(ui->mainPanel1->getCurrentIndex());
    } else {
        // Get File Info
        fileInfo = ui->mainPanel2->getFileInfo(ui->mainPanel2->getCurrentIndex());
    }

    // Check File Info
    if (!fileInfo.isDir()) {
        // Set File Path
        filePath = fileInfo.absoluteFilePath();
    }

    // Check File Path
    if (!filePath.isEmpty()) {

        qDebug() << "MainWindow::editFile";

    }
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
// Launch Rename Current File
//==============================================================================
void MainWindow::launchRenameCurrentFile()
{
    // Check UI
    if (!ui || !ui->mainPanel1)
        return;

    // Check UI
    if (!ui || !ui->mainPanel2)
        return;

    // Init Current File List Item Delegate
    FileListDelegate* itemDelegate = NULL;
    // Init Renamer Parent
    FileListBox* renamerParent = NULL;
    // Init Current Dir
    QString currentDir = QString("");

    // Check Current Panel
    if (cPanelIndex == 0) {
        // Check Panel UI
        if (ui->mainPanel1->ui && ui->mainPanel1->ui->fileList && ui->mainPanel1->ui->fileList->listbox()) {
            // Set Renamer Parent
            renamerParent = (FileListBox*)ui->mainPanel1->ui->fileList->listbox();
            // Get Current File List Item Delegate
            itemDelegate = (FileListDelegate*)renamerParent->getItem(ui->mainPanel1->ui->fileList->getCurrentIndex());
            // Get Current Dir
            currentDir = ui->mainPanel1->ui->fileList->getCurrentDir();
        }
    } else if (cPanelIndex == 1) {
        // Check Panel UI
        if (ui->mainPanel2->ui && ui->mainPanel2->ui->fileList && ui->mainPanel2->ui->fileList->listbox()) {
            // Set Renamer Parent
            renamerParent = (FileListBox*)ui->mainPanel2->ui->fileList->listbox();
            // Get Current File List Item Delegate
            itemDelegate = (FileListDelegate*)renamerParent->getItem(ui->mainPanel2->ui->fileList->getCurrentIndex());
            // Get Current Dir
            currentDir = ui->mainPanel2->ui->fileList->getCurrentDir();
        }
    }

    // Check Current File List Item Delegate
    if (itemDelegate && itemDelegate->getFileInfo().fileName() != QString("..") ) {
        // Init Source Name
        QString sourceName = itemDelegate->getFileInfo().fileName();

        qDebug() << "MainWindow::launchRenameCurrentFile - src: " << sourceName;

        //qDebug() << "MainWindow::launchRenameCurrentFile - renamerParent: " << renamerParent->mapToGlobal(renamerParent->pos());

        // Init Event Loop
        QEventLoop eventLoop;
        // Init File Renamer Window
        FileRenamer fr(&eventLoop, renamerParent);
        // Set Source/Initial File Name
        fr.setFileName(itemDelegate->getFileInfo().fileName());
        // Show File Renamer Window
        fr.showRenamer(itemDelegate);
        // Execute Event Loop
        eventLoop.exec(QEventLoop::DialogExec);

        // Check If Accepted
        if (fr.getAccepted()) {
            // Get Target Name
            QString targetName = fr.getFileName();

            qDebug() << "MainWindow::launchRenameCurrentFile - trg: " << targetName;

            // Check Current Dir
            if (!currentDir.endsWith(QChar('/')) && !currentDir.endsWith(QChar('\\'))) {
                // Add Separator
                currentDir += QDir::separator();
            }

            // Init Source File Path
            QString sourceFilePath = currentDir + sourceName;
            // Init Target File Path
            QString targetFilePath = currentDir + targetName;
            // Init Rename Options
            int renameOptions = 0;
            // Init Rename Abort Signal
            bool renameAbort = false;

            // Rename File
            FileUtils::renameFile(sourceFilePath, targetFilePath, renameOptions, renameAbort);
        }
    }
}

//==============================================================================
// Create Dir
//==============================================================================
void MainWindow::launchCreateDir()
{
    // Check UI
    if (!ui || !ui->mainPanel1)
        return;

    // Check UI
    if (!ui || !ui->mainPanel2)
        return;

    // Check Create Dir Diralog
    if (!createDirDialog) {
        // Create Create Dir Dialog
        createDirDialog = new CreateDirDialog();
    }

    // Check Create Dir Dialog
    if (createDirDialog) {
        qDebug() << "MainWindow::launchCreateDir";

        // Exec Dialog
        if (createDirDialog->exec()) {
            qDebug() << "MainWindow::createDir - cPanelName: " << cPanelName << " - newDir: " << createDirDialog->getDirText();

            // Init Current File List
            CustomFilelist* currentFileList = NULL;
            // Init Current Dir
            QString currentDir = QString("");

            // Get Current Panel Index
            if (cPanelIndex == 0) {
                // Check Panel UI
                if (ui->mainPanel1->ui && ui->mainPanel1->ui->fileList) {
                    // Get Current File List
                    currentFileList = ui->mainPanel1->ui->fileList;
                }
            } else if (cPanelIndex == 1) {
                // Check Panel UI
                if (ui->mainPanel2->ui && ui->mainPanel2->ui->fileList) {
                    // Get Current File List
                    currentFileList = ui->mainPanel2->ui->fileList;
                }
            }

            // Get Current Dir
            currentDir = currentFileList->getCurrentDir();

            // Check Current Dir Path
            if (!currentDir.endsWith(QString("/")) && !currentDir.endsWith(QString("\\"))) {
                // Add Slash
                currentDir += QDir::separator();
            }

            // Set Prev Dir Name To Go To After File List Reload
            currentFileList->prevDirName = createDirDialog->getDirText();

            // Get Dir Name Temp
            QString dirNameTemp = currentDir + createDirDialog->getDirText();

            // Init Options
            int createDirOptions = 0;

            // Init Abort Signal
            bool abortSig = false;

            // Create Dir
            if (FileUtils::createDir(dirNameTemp, createDirOptions, abortSig, this)) {

                // ...

                // Reload Panel

                // Go To New Dir

            } else {
                // Reset Previous File Name
                currentFileList->prevDirName = QString("");
            }
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

    // Check Selected Items Count & Current File Name
    if (sfCount < 1 && (currFileName == QString("..") || currFileName == QString("."))) {
        return;
    }

    qDebug() << "MainWindow::launchDelete";

    // Init Confirm Title
    QString confirmTitle = (sfCount > 1) ? QString(DEFAULT_DELETE_DIALOG_TITLE_TEXT_FILES) : QString(DEFAULT_DELETE_DIALOG_TITLE_TEXT_FILE);
    // Init Confirm Text
    QString confirmText = (sfCount > 1) ? QString(DEFAULT_DELETE_DIALOG_TEXT_FILES).arg(sfCount) : QString(DEFAULT_DELETE_DIALOG_TEXT_FILE).arg(currFileName);

    // Check Confirmation Dialog
    if (!confirmDialog) {
        // Create Confirmation Dialog
        confirmDialog = new ConfirmationDialog();
    }

    // Check Confirmation Dialog
    if (confirmDialog) {
        // Set Title
        confirmDialog->setTitle(confirmTitle);
        // Set Confirm Text
        confirmDialog->setConfirmMsg(confirmText);
        // Add Button
        confirmDialog->addButton(QString(DEFAULT_BUTTON_TEXT_QUEUE), QDialogButtonBox::ActionRole, DEFAULT_DIALOG_RESULT_QUEUE);

        // Exec Dialog
        if (confirmDialog->exec()) {
            // Init Operation Queue Handler
            FileOpQueueViewAPI* opQueueHandler = NULL;

            // Check Dialog Result
            if (confirmDialog->result() == DEFAULT_DIALOG_RESULT_OK) {
                qDebug() << "MainWindow::launchDelete - DELETE";
                // Create New Delete Progress Dialog
                DeleteProgressDialog* newDialog = new DeleteProgressDialog();
                // Check New Delete Progress Dialog
                if (newDialog) {
                    // Add Dialog To Dialogs List
                    dialogs << newDialog;
                }
                // Set Operation Queue Handler
                opQueueHandler = newDialog;
            // Check Dialog Result
            } else if (confirmDialog->result() == DEFAULT_DIALOG_RESULT_QUEUE) {
                qDebug() << "MainWindow::launchDelete - QUEUE";
                // Check Main Queue Dialog
                if (!mainQueueDialog) {
                    // Create Main Queue Dialog
                    mainQueueDialog = new MainQueueDialog();
                }
                // Set Operation Queue Handler
                opQueueHandler = mainQueueDialog;
            }

            // Check Operation Queue handler
            if (opQueueHandler) {
                // Show Copy Progress Dialog
                opQueueHandler->show();
                // Set Modal
                opQueueHandler->setModal(true);
                // Add Items To Delete Queue
                addItemsToQueue(OPERATION_ID_DELETE, opQueueHandler, cPanelIndex, sfCount, currFileName);
                // Reset Modal
                opQueueHandler->setModal(false);
                // Exec/Processs Copy Queue
                opQueueHandler->processQueue();
            }
        }
    }
}

//==============================================================================
// Launch Copy
//==============================================================================
void MainWindow::launchCopy()
{
    // Check UI
    if (!ui || !ui->mainPanel1)
        return;

    // Check UI
    if (!ui || !ui->mainPanel2)
        return;

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
    if ((currFileName == QString("..") || currFileName == QString(".")) && sfCount<1) {
        return;
    }

    qDebug() << "MainWindow::launchCopy";

    // Check Copy Dialog
    if (!copyDialog) {
        // Create Copy Dialog
        copyDialog = new CopyDialog();
    }

    // Check Copy Dialog
    if (copyDialog) {
        // Check Selected File Count
        if (sfCount > 0) {
            // Set Title
            copyDialog->setTitle(QString(DEFAULT_COPY_DIALOG_TITLE_TEXT_FILES));
            // Set Copy Msg
            copyDialog->setCopyMsg(QString(DEFAULT_COPY_DIALOG_TEXT_TEMPLATE_FILES).arg(sfCount));
        } else {
            // Set Title
            copyDialog->setTitle(QString(DEFAULT_COPY_DIALOG_TITLE_TEXT_FILE));
            // Set Copy Msg
            copyDialog->setCopyMsg(QString(DEFAULT_COPY_DIALOG_TEXT_TEMPLATE_FILE).arg(currFileName));
        }

        // Init Target Path
        QString targetPath = QString("");

        // Check Current Panel Index
        if (cPanelIndex == 0) {
            // Set Target Path
            targetPath = ui->mainPanel2->ui->fileList->getCurrentDir();
        } else {
            // Set Target Path
            targetPath = ui->mainPanel1->ui->fileList->getCurrentDir();
        }

        // Check Target Path
        if (!targetPath.endsWith(QString("/")) && !targetPath.endsWith(QString("\\"))) {
            // Add Slash
            targetPath += QDir::separator();
        }

        // Check Selected File Count
        if (sfCount > 0) {
            // Add WildCards To Target path
            targetPath += QString("*.*");
        } else {
            // Add Current File Name To Target Path
            targetPath += currFileName;
        }

        // Set Initial Target Path
        copyDialog->setTarget(targetPath);

        // Exec Copy Dialog
        if (copyDialog->exec()) {
            // Init Operation Queue Handler
            FileOpQueueViewAPI* opQueueHandler = NULL;

            // Check Dialog Result
            if (copyDialog->result() == DEFAULT_DIALOG_RESULT_QUEUE) {
                // Check Main Queue Dialog
                if (!mainQueueDialog) {
                    // Create Main Queue Dialog
                    mainQueueDialog = new MainQueueDialog();
                }
                // Set Operation Queue Handler
                opQueueHandler = mainQueueDialog;
            } else if (copyDialog->result() == DEFAULT_DIALOG_RESULT_OK) {
                // Create New Copy Progress Dialog
                CopyProgressDialog* newDialog = new CopyProgressDialog();
                // Check New Copy Progress Dialog
                if (newDialog) {
                    // Set Window Title
                    newDialog->setWindowTitle(QString(DEFAULT_COPY_PROGRESS_DIALOG_TITLE));
                    // Add Dialog To Dialogs List
                    dialogs << newDialog;
                }
                // Set Operation Queue Handler
                opQueueHandler = newDialog;
            }

            // Check Opearion Queue Handler
            if (opQueueHandler) {
                // Show Copy Progress Dialog
                opQueueHandler->show();
                // Set Modal
                opQueueHandler->setModal(true);

                // Add Items To Copy Queue
                addItemsToQueue(OPERATION_ID_COPY, opQueueHandler, cPanelIndex, sfCount, currFileName, copyDialog->getTarget());

                // Reset Modal
                opQueueHandler->setModal(false);
                // Exec/Processs Copy Queue
                opQueueHandler->processQueue();
            }
        }
    }
}

//==============================================================================
// Launch Move
//==============================================================================
void MainWindow::launchMove()
{
    // Check UI
    if (!ui || !ui->mainPanel1)
        return;

    // Check UI
    if (!ui || !ui->mainPanel2)
        return;

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
    if ((currFileName == QString("..") || currFileName == QString(".")) && sfCount<1) {
        return;
    }

    qDebug() << "MainWindow::launchMove";

    // Check Copy Dialog
    if (!copyDialog) {
        // Create Copy Dialog
        copyDialog = new CopyDialog();
    }

    // Check Copy Dialog
    if (copyDialog) {

        // Check Selected File Count
        if (sfCount > 1) {
            // Set Title
            copyDialog->setTitle(QString(DEFAULT_MOVE_DIALOG_TITLE_TEXT_FILES));
            // Set Copy Msg
            copyDialog->setCopyMsg(QString(DEFAULT_MOVE_DIALOG_TEXT_TEMPLATE_FILES).arg(sfCount));
        } else {
            // Set Title
            copyDialog->setTitle(QString(DEFAULT_MOVE_DIALOG_TITLE_TEXT_FILE));
            // Set Copy Msg
            copyDialog->setCopyMsg(QString(DEFAULT_MOVE_DIALOG_TEXT_TEMPLATE_FILE).arg(currFileName));
        }

        // Init Target Path
        QString targetPath = QString("");

        // Check Current Panel Index
        if (cPanelIndex == 0) {
            // Set Target Path
            targetPath = ui->mainPanel2->ui->fileList->getCurrentDir();
        } else {
            // Set Target Path
            targetPath = ui->mainPanel1->ui->fileList->getCurrentDir();
        }

        // Check Target Path
        if (!targetPath.endsWith(QString("/")) && !targetPath.endsWith(QString("\\"))) {
            // Add Slash
            targetPath += QString("/");
        }

        // Set Initial Target Path
        copyDialog->setTarget(targetPath);

        // Exec Copy Dialog
        if (copyDialog->exec()) {
            // Init Operation Queue Handler
            FileOpQueueViewAPI* opQueueHandler = NULL;

            // Check Dialog Result
            if (copyDialog->result() == DEFAULT_DIALOG_RESULT_QUEUE) {
                // Check Main Queue Dialog
                if (!mainQueueDialog) {
                    // Create Main Queue Dialog
                    mainQueueDialog = new MainQueueDialog();
                }
                // Set Operation Queue Handler
                opQueueHandler = mainQueueDialog;

            // Check Dialog Result
            } else if (copyDialog->result() == DEFAULT_DIALOG_RESULT_OK) {
                // Create New Copy Progress Dialog
                CopyProgressDialog* newDialog = new CopyProgressDialog();
                // Check New Copy Progress Dialog
                if (newDialog) {
                    // Set Window Title
                    newDialog->setWindowTitle(QString(DEFAULT_MOVE_PROGRESS_DIALOG_TITLE));
                    // Add Dialog To Dialogs List
                    dialogs << newDialog;
                }
                // Set Operation Queue Handler
                opQueueHandler = newDialog;
            }

            // Check Opearion Queue Handler
            if (opQueueHandler) {
                // Show Copy Progress Dialog
                opQueueHandler->show();
                // Set Modal
                opQueueHandler->setModal(true);
                // Add Items To Copy Queue
                addItemsToQueue(OPERATION_ID_MOVE, opQueueHandler, cPanelIndex, sfCount, currFileName, copyDialog->getTarget());
                // Reset Modal
                opQueueHandler->setModal(false);
                // Exec/Processs Copy Queue
                opQueueHandler->processQueue();
            }
        }
    }
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
    // Save Settings First
    saveSettings();
    // Init Event Loop
    QEventLoop eventLoop;
    // Init Settigns Window
    SettingsWindow sw(&eventLoop);
    // Show Settings Window
    sw.show();
    // Execute Event Loop
    eventLoop.exec(QEventLoop::DialogExec);
    // Load Settings
    loadSettings();
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
            // Connect Signal
            connect(infoDialog, SIGNAL(infoDialogFinished(QDialog*)), this, SLOT(infoDialogFinished(QDialog*)));
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
int MainWindow::showConfirmation(const QString& aTitle,
                                 const QString& aConfirmMsg,
                                 const QDialogButtonBox::StandardButtons& aButtons,
                                 const QDialogButtonBox::StandardButton& aDefault)
{
    // Check Title, Message And Buttons
    if (!aTitle.isEmpty() && !aConfirmMsg.isEmpty()) {

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
            // Check Buttons
            if (aButtons) {
                // Set Buttons
                confirmDialog->setStandardButtons(aButtons, aDefault);
            }

            // Exec Confirm Dialog
            int result = confirmDialog->exec();

            return result;
        }
    }

    return 0;
}

//==============================================================================
// Show Confirmation Dialog Slot
//==============================================================================
void MainWindow::showConfirmation(ConfirmDialogProvider* aDialogProvider, const int& aType, QEventLoop* aEventLoop)
{
    // Check Dialog Provider
    if (aDialogProvider) {
        // Launch Confirmation
        int result = aDialogProvider->launchConfirm(aType);
        // Exit Confirmation
        aDialogProvider->exitConfirm(aEventLoop, result);
    }
}

//==============================================================================
// Show Error Dialog Slot
//==============================================================================
void MainWindow::showError(ErrorDialogProvider* aDialogProvider, const int& aErrorCode, QEventLoop* aEventLoop)
{
    // Check Dialog Provider
    if (aDialogProvider) {
        // Launch Error Dialog
        int result = aDialogProvider->launchError(aErrorCode);
        // Exit Error
        aDialogProvider->exitError(aEventLoop, result);
    }
}

//==============================================================================
// Info Dialog Finished Slot
//==============================================================================
void MainWindow::infoDialogFinished(QDialog* aDialog)
{
    qDebug() << "MainWindow::infoDialogFinished - aDialog: " << aDialog;

    // Check Dialog
    if (aDialog) {
        // Remove From Dialogs
        dialogs.removeOne(aDialog);

        // Delete Dialog
        delete aDialog;
        aDialog = NULL;
    }

    // Reset Info Dialog
    infoDialog = NULL;
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
            // Switch To Panel
            switchToPanel(settings->getValue(SETTINGS_KEY_ACTIVE_PANEL, 0).toInt());
            // Init On Off Setting Value
            bool onOff = false;
            // Get Setting
            onOff = settings->getValue(SETTINGS_KEY_SHOW_TOOLBAR, true).toBool();
            // Set Main Toolbar Visibility
            ui->mainToolbar->setVisible(onOff);
            // Set Menu Item Checked
            ui->actionShowToolbar->setChecked(onOff);

            onOff = settings->getValue(SETTINGS_KEY_SHOW_BUTTONBAR, true).toBool();
            // Set Main Button Bar Visibility
            ui->mainButtonbar->setVisible(onOff);
            // Set Menu Item Checked
            ui->actionShowButtonBar->setChecked(onOff);

            onOff = settings->getValue(SETTINGS_KEY_SHOW_STATUSBAR, true).toBool();
            // Set Status Bar Visibility
            ui->statusBar->setVisible(onOff);
            // Set Menu Item Checked
            ui->actionShowStatusBar->setChecked(onOff);
        }

        // Reload
        ui->mainPanel1->ui->fileList->reload();
        ui->mainPanel2->ui->fileList->reload();
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
            settings->setValue(QString(SETTINGS_KEY_ACTIVE_PANEL), cPanelIndex);
            // Save Show Tool Bar
            settings->setValue(QString(SETTINGS_KEY_SHOW_TOOLBAR), ui->actionShowToolbar->isChecked());
            // Save Show Button Bar
            settings->setValue(QString(SETTINGS_KEY_SHOW_BUTTONBAR), ui->actionShowButtonBar->isChecked());
            // Save Show Status Bar
            settings->setValue(QString(SETTINGS_KEY_SHOW_STATUSBAR), ui->actionShowStatusBar->isChecked());
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
    //qDebug() << "MainWindow::panelFocusChanged - aPanelName: " << aPanelName << " - aFocused: " << aFocused;

    // Check Current Panel Name
    if (cPanelName != aPanelName && aFocused) {
        // Set Current Panel Name
        cPanelName = aPanelName;

        // Check Current Panel Name
        if (cPanelName == QString(DEFAULT_FILELIST_PANEL_NAME1)) {
            // Set Current Panel Index
            cPanelIndex = 0;
        } else if (cPanelName == QString(DEFAULT_FILELIST_PANEL_NAME2)) {
            // Set Current Panel Index
            cPanelIndex = 1;
        }
    }

    // Check Last Focused
    if (!aFocused) {
        // Reset Shift Key Pressed
        shiftKeyPressed = false;
        // Reset Alt Key Pressed
        altKeyPressed = false;
        // Reset Control Key Pressed
        controlKeyPressed = false;
        // Reset Meta Key Pressed
        metaKeyPressed = false;

        // Configure Function Keys
        configureFunctionKeys();
    }
}

//==============================================================================
// Panel Key Pressed Slot
//==============================================================================
void MainWindow::panelKeyPressed(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    // Check Current Panel Name
    if (cPanelName == aPanelName) {
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

        case Qt::Key_Escape: {
            // Check If Shift Key Pressed
            if (shiftKeyPressed) {
                // Show Minimized
                //showMinimized();
                // Set Window State
                setWindowState(Qt::WindowMinimized);
            }
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
        launchRenameCurrentFile();
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
// Show Status Message
//==============================================================================
void MainWindow::showStatusMessage(const QString& aMessage, const int& aTimeOut)
{
    // Check UI
    if (ui && ui->statusBar) {
        // Show Status Message
        ui->statusBar->showMessage(aMessage, aTimeOut);
    }
}

//==============================================================================
// Clear Status Message
//==============================================================================
void MainWindow::clearStatusMessage()
{
    // Check UI
    if (ui && ui->statusBar) {
        // Clear Status Message
        ui->statusBar->clearMessage();
    }
}

//==============================================================================
// Launch Admin Password Query Dialog
//==============================================================================
QString MainWindow::launchAdminPassQuery()
{
    // Init Result
    QString result = QString("");

    // Create Admin Pass Query Dialog
    AdminPassDialog* adminPassDialog = new AdminPassDialog();

    // Get Admin Password
    if (adminPassDialog && adminPassDialog->exec()) {
        // Set Result
        result = adminPassDialog->getPassword();
    }

    // Delete Admin Pass Dialog
    delete adminPassDialog;
    adminPassDialog = NULL;

    return result;
}

//==============================================================================
// On Action New Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_triggered()
{
}

//==============================================================================
// On Action Quit Triggered Slot
//==============================================================================
void MainWindow::on_actionQuit_triggered()
{
    // Exit Application
    exitApp();
}

//==============================================================================
// On Action Select All Triggered Slot
//==============================================================================
void MainWindow::on_actionSelectAll_triggered()
{
}

//==============================================================================
// On Action Reload Panel Triggered Slot
//==============================================================================
void MainWindow::on_actionReloadPanel_triggered()
{
}

//==============================================================================
// On Action Swap Panels Triggered Slot
//==============================================================================
void MainWindow::on_actionSwapPanels_triggered()
{
}

//==============================================================================
// On Action Show Hidden Files Triggered Slot
//==============================================================================
void MainWindow::on_actionShowHidden_triggered()
{
}

//==============================================================================
// On Action About Triggered Slot
//==============================================================================
void MainWindow::on_actionAbout_triggered()
{
    // Init About Dialog
    AboutDialog ad;
    // Exec Dialog
    ad.exec();
}

//==============================================================================
// On Action Minimize Triggered Slot
//==============================================================================
void MainWindow::on_actionMinimize_triggered()
{
    // Show Minimized
    //showMinimized();
    // Set Window State
    setWindowState(Qt::WindowMinimized);
}

//==============================================================================
// On Action Restore Triggered Slot
//==============================================================================
void MainWindow::on_actionRestore_triggered()
{
    // Restore Window
    showNormal();
}

//==============================================================================
// On Action Show Tool Bar Triggered Slot
//==============================================================================
void MainWindow::on_actionShowToolbar_triggered()
{
    // Check UI & Tool Bar
    if (ui && ui->actionShowToolbar && ui->mainToolbar) {
        // Set Main Tool Bar Visibility
        ui->mainToolbar->setVisible(ui->actionShowToolbar->isChecked());
    }
}

//==============================================================================
// On Action Show Button Bar Triggered Slot
//==============================================================================
void MainWindow::on_actionShowButtonBar_triggered()
{
    // Check UI & Button Bar
    if (ui && ui->actionShowButtonBar && ui->mainButtonbar) {
        // Set Main Button Bar Visibility
        ui->mainButtonbar->setVisible(ui->actionShowButtonBar->isChecked());
    }
}

//==============================================================================
// On Action Show Status Bar Triggered Slot
//==============================================================================
void MainWindow::on_actionShowStatusBar_triggered()
{
    // Check UI & Status Bar
    if (ui && ui->actionShowStatusBar && ui->statusBar) {
        // Set Main Status Bar Visibility
        ui->statusBar->setVisible(ui->actionShowStatusBar->isChecked());
    }
}

//==============================================================================
// On Action Zoom In Triggered Slot
//==============================================================================
void MainWindow::on_actionZoomIn_triggered()
{

}

//==============================================================================
// On Action Zoom Out Triggered Slot
//==============================================================================
void MainWindow::on_actionZoomOut_triggered()
{

}

//==============================================================================
// On Action Zoom to Normal Triggered Slot
//==============================================================================
void MainWindow::on_actionZoomToNormal_triggered()
{

}

//==============================================================================
// On Action Help Triggered Slot
//==============================================================================
void MainWindow::on_actionHelp_triggered()
{
    // Launch help
    launchHelp();
}

//==============================================================================
// On Action Check Updates Triggered Slot
//==============================================================================
void MainWindow::on_actionCheckUpdates_triggered()
{

}

//==============================================================================
// On Action Copy Triggered Slot
//==============================================================================
void MainWindow::on_actionCopy_triggered()
{

}

//==============================================================================
// On Action Cut Triggered Slot
//==============================================================================
void MainWindow::on_actionCut_triggered()
{

}

//==============================================================================
// On Action Paste Triggered Slot
//==============================================================================
void MainWindow::on_actionPaste_triggered()
{

}

//==============================================================================
// On Action Compare Triggered Slot
//==============================================================================
void MainWindow::on_actionCompare_triggered()
{

}

//==============================================================================
// On Action Pack Triggered Slot
//==============================================================================
void MainWindow::on_actionPack_triggered()
{

}

//==============================================================================
// On Action Unpack Triggered Slot
//==============================================================================
void MainWindow::on_actionUnpack_triggered()
{

}

//==============================================================================
// Clear Progress Dialogs
//==============================================================================
void MainWindow::clearDialogs()
{
    qDebug() << "MainWindow::clearDialogs";

    // Get Dialogs Count
    int dCount = dialogs.count();

    // Go Thru Dialogs
    for (int i=dCount-1; i>=0; --i) {
        // Get Dialog
        QDialog* dialog = dialogs[i];
        // Check Dialog
        if (dialog) {
            // Delete Dialog
            delete dialog;
            dialog = NULL;
        }
    }

    // Clear Dialogs
    dialogs.clear();
}

//==============================================================================
// Build And Add Queue
//==============================================================================
void MainWindow::addItemsToQueue(const int& aOperation,
                                 FileOpQueueViewAPI* aQueueHandler,
                                 const int& aSourcePanelIndex,
                                 const int& aSelCount,
                                 const QString& aCurrFileName,
                                 const QString& aTargetFileName)
{
    // Check Queue Handler
    if (aQueueHandler &&
        ui &&
        ui->mainPanel1 &&
        ui->mainPanel1->ui &&
        ui->mainPanel1->ui->fileList &&
        ui->mainPanel2 &&
        ui->mainPanel2->ui &&
        ui->mainPanel2->ui->fileList) {
        // Init Mutex
        QMutex mutex;
        // Lock Mutex
        mutex.lock();

        // Switch Operation
        switch (aOperation) {
            case OPERATION_ID_MAKEDIR:
                qDebug() << "MainWindow::buildQueue - MAKEDIR";
            break;

            case OPERATION_ID_COPY:
                qDebug() << "MainWindow::buildQueue - COPY";
            break;

            case OPERATION_ID_MOVE:
                qDebug() << "MainWindow::buildQueue - MOVE";
            break;

            case OPERATION_ID_RENAME:
                qDebug() << "MainWindow::buildQueue - RENAME";
            break;

            case OPERATION_ID_DELETE:
                qDebug() << "MainWindow::buildQueue - DELETE";
            break;

            default:
            break;
        }

        // Get Source File List
        CustomFilelist* srcFileList = (aSourcePanelIndex == 0) ? ui->mainPanel1->ui->fileList : ui->mainPanel2->ui->fileList;
        // Get Target File List
        CustomFilelist* trgFileList = (aSourcePanelIndex == 0) ? ui->mainPanel2->ui->fileList : ui->mainPanel1->ui->fileList;

        // Init Source Dir Name
        QString sourceDirName = srcFileList->getCurrentDir();
        // Init Target Dir Name
        QString targetDirName = trgFileList->getCurrentDir();

        // Check Selected Count
        if (aSelCount > 0) {
            // Get Source File List Count
            int sflCount = srcFileList->getItemCount();
            // Go Thru Selected Items
            for (int i=0; i<sflCount; ++i) {
                // Get Item Data
                FileItemData* itemData = srcFileList->getItemData(i);
                // Get File Name
                QString fileName = itemData ? itemData->getFileInfo().fileName() : QString("");
                // Check Item Data
                if (itemData && itemData->isSelected() && !fileName.isEmpty() && fileName != QString("..")) {
                    // Create New Operation Entry
                    FileOperationEntry* newOpEntry = FileUtils::createFileOperationEntry(aQueueHandler, aOperation, sourceDirName, fileName, targetDirName, aTargetFileName);
                    // Add To Queue Handler
                    aQueueHandler->addOperationEntry(newOpEntry);
                    // Reset Selected Flag
                    itemData->setSelected(false);
                }
            }
        } else {
            // Create New Operation Entry
            FileOperationEntry* newOpEntry = FileUtils::createFileOperationEntry(aQueueHandler, aOperation, sourceDirName, aCurrFileName, targetDirName, aTargetFileName);
            // Add To Queue Handler
            aQueueHandler->addOperationEntry(newOpEntry);
        }

        // Unlock Mutex
        mutex.unlock();
    }
}

//==============================================================================
// Create Dir Error Callback
//==============================================================================
int MainWindow::createDirError(const QString& aDirPath, const int& aErrorCode)
{
    qDebug() << "MainWindow::createDirError - aDirPath: " << aDirPath << " - aErrorCode: " << aErrorCode;

    // Check Error Dialog
    if (!errorDialog) {
        // Create Error Dialog
        errorDialog = new ConfirmationDialog();
        // Set Error Dialog Title
        errorDialog->setTitle(QString(DEFUALT_DIALOG_TITLE_DIRCREATE_ERROR));
        // Clear Buttons
        errorDialog->clearButtons();
    }

    // Get Dir Name
    QString dirName = FileUtils::getDirName(aDirPath);

#if defined (Q_OS_WIN)


#elif defined (Q_OS_UNIX)

    switch (aErrorCode) {
        case EEXIST: {
            // Check Error Dialog
            if (errorDialog) {
                // Add Buttons
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_RETRY), QDialogButtonBox::ActionRole, (int)FOORTRetry, true);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNORE), QDialogButtonBox::ActionRole, (int)FOORTIgnore);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);
                // Set Error Message
                errorDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_DIRCREATE_ERROR_ALREADYEXISTS).arg(dirName));
            }
        } break;

        case EACCES: {
            // Check Error Dialog
            if (errorDialog) {
                // Add Buttons
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_RETRY), QDialogButtonBox::ActionRole, (int)FOORTRetry, true);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ASADMIN), QDialogButtonBox::ActionRole, (int)FOORTAsRoot);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_IGNORE), QDialogButtonBox::ActionRole, (int)FOORTIgnore);
                errorDialog->addButton(QString(DEFAULT_BUTTON_TEXT_ABORT), QDialogButtonBox::ActionRole, (int)FOORTAbort);
                // Set Error Message
                errorDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_DIRCREATE_ERROR_ACCESSDENIED).arg(dirName));
            }
        } break;

        // ...

        default: {
            // Check Error Dialog
            if (errorDialog) {
                // Set Error Message
                errorDialog->setConfirmMsg(QString(DEFAULT_DIALOG_TEXT_DEFAULT_ERROR).arg(aErrorCode));
            }
        } break;
    }

#endif // Q_OS_MAC || Q_OS_UNIX

    // Init result
    int result = (int)FOORTIgnore;

    // Check Error Dialog
    if (errorDialog) {
        // Execute Error Dialog
        result = errorDialog->exec();
    }

    return result;
}

//==============================================================================
// Create Dir Started
//==============================================================================
void MainWindow::createDirStarted(const QString& aDirPath)
{
    qDebug() << "MainWindow::createDirStarted - aDirPath: " << aDirPath;

    // ...
}

//==============================================================================
// Creaet Dir Finished
//==============================================================================
void MainWindow::createDirFinished(const QString& aDirPath, const int& aErrorCode)
{
    qDebug() << "MainWindow::createDirFinished - aDirPath: " << aDirPath << " - aErrorCode: " << aErrorCode;

    // Check Error Code & UI
    if (aErrorCode == 0 &&
        ui &&
        ui->mainPanel1 &&
        ui->mainPanel1->ui &&
        ui->mainPanel1->ui->fileList &&
        ui->mainPanel2 &&
        ui->mainPanel2->ui &&
        ui->mainPanel2->ui->fileList) {

        // Init Current File List
        CustomFilelist* currentFileList = NULL;
        // Init Current Dir
        QString currentDirName = FileUtils::getDirName(aDirPath);

        // Get Current Panel Index
        if (cPanelIndex == 0) {
            // Check Panel UI
            if (ui->mainPanel1->ui && ui->mainPanel1->ui->fileList) {
                // Get Current File List
                currentFileList = ui->mainPanel1->ui->fileList;
            }
        } else if (cPanelIndex == 1) {
            // Check Panel UI
            if (ui->mainPanel2->ui && ui->mainPanel2->ui->fileList) {
                // Get Current File List
                currentFileList = ui->mainPanel2->ui->fileList;
            }
        }

        // Check Current File List
        if (currentFileList) {
            // Set Previous Dir Name Before Reload
            currentFileList->prevDirName = currentDirName;
        }
    }
}

//==============================================================================
// Get Admin Password Query Provider
//==============================================================================
AdminPassQueryProvider* MainWindow::passQueryProvider()
{
    return this;
}

//==============================================================================
// Get File Utils Client
//==============================================================================
RemoteFileUtils* MainWindow::getUtilsClient(const QString& aAdminPass)
{
    // Check Admin Pass & File utils Client
    if (!aAdminPass.isEmpty() && !fileUtilsClient) {
        qDebug() << "MainWindow::getUtilsClient - Creating File utils Client";

        // Create File Utils Client
        fileUtilsClient = new RemoteFileUtils();
    }

    // Connect To Server
    if (fileUtilsClient && !fileUtilsClient->connectToServer(aAdminPass)) {
        // Delete File Utils Client
        delete fileUtilsClient;
        fileUtilsClient = NULL;
    }

    return fileUtilsClient;
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

    // Check Info Dialog
    if (infoDialog) {
        // Delete Info Dialog
        delete infoDialog;
        infoDialog = NULL;
    }

    // Check Confirmation Dialog
    if (confirmDialog) {
        // Delete Confirmaion Dialog
        delete confirmDialog;
        confirmDialog = NULL;
    }

    // Check Erorr Dialog
    if (errorDialog) {
        // Delete Error Dialog
        delete errorDialog;
        errorDialog = NULL;
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

    // Check Help Dialog
    if (helpDialog) {
        // Help Dialog
        delete helpDialog;
        helpDialog = NULL;
    }

    // Check Tree Window
    if (treeWindow) {
        // Delete Tree Window
        delete treeWindow;
        treeWindow = NULL;
    }

    // Check Main Queue Dialog
    if (mainQueueDialog) {
        // Delete Main Queue Dialog
        delete mainQueueDialog;
        mainQueueDialog = NULL;
    }

    // Check Admin Pass Dialog
    if (adminPassDialog) {
        // Delete Dialog
        delete adminPassDialog;
        adminPassDialog = NULL;
    }

    // Clear Dialogs
    clearDialogs();

    // Check Util Client
    if (fileUtilsClient) {
        // Delete File Util Client
        delete fileUtilsClient;
        fileUtilsClient = NULL;
    }

    // Delete UI
    delete ui;

    qDebug() << "Deleting MainWindow...done";
}


