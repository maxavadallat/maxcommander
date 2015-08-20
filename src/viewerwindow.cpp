#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDesktopWidget>
#include <QApplication>
#include <QQuickWidget>
#include <QMimeDatabase>
#include <QMimeType>
#include <QQmlContext>
#include <QQmlEngine>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QTextCursor>
#include <QDebug>

#include <mcwinterface.h>

#include "viewerwindow.h"
#include "ui_viewerwindow.h"
#include "confirmdialog.h"
#include "remotefileutilclient.h"
#include "audiotagimageprovider.h"
#include "confirmdialog.h"
#include "findtextdialog.h"
#include "settingscontroller.h"
#include "constants.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
ViewerWindow::ViewerWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::ViewerWindow)
    , settings(SettingsController::getInstance())
    , fileName("")
    , editMode(false)
    , dirty(false)
    , fileModified(false)
    , mime("")
    , fileIsNew(false)
    , imageBrowser(NULL)
    , findDialog(NULL)
    , searchTerm("")
    , searchPos(-1)
    , ownKeyPress(false)
    , findDialogKeyPress(false)
{
    qDebug() << "ViewerWindow::ViewerWindow";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void ViewerWindow::init()
{
    qDebug() << "ViewerWindow::init";

    // Set Read Only Mode
    ui->textEdit->setReadOnly(!editMode);

    // Connect Signal
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));

    // Quick Widget Set Visible
    ui->quickWidget->setVisible(true);
    // Text Edit Set Visible
    ui->textEdit->setVisible(false);

    // Set Context Properties
    QQmlContext* ctx = ui->quickWidget->rootContext();
    // Set Context Properties - Viewer Content
    ctx->setContextProperty(DEFAULT_IMAGE_VIEWER_CONTENT, fileName);

    // Get Engine
    QQmlEngine* engine = ui->quickWidget->engine();
    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_AUDIO_TAG_PROVIDER_ID), new AudioTagImageProvider());

    // Set Resize Mode
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Set Focus Policy
    setFocusPolicy(Qt::StrongFocus);

    // ...
}

//==============================================================================
// Get Content Source
//==============================================================================
QString ViewerWindow::getContentSource()
{
    return fileName;
}

//==============================================================================
// Get Is New File
//==============================================================================
bool ViewerWindow::isNewFile()
{
    return fileIsNew;
}

//==============================================================================
// Get Is File Modified
//==============================================================================
bool ViewerWindow::isFileModified()
{
    return fileModified;
}

//==============================================================================
// Set Search Term
//==============================================================================
void ViewerWindow::setSearchTerm(const QString& aTerm)
{
    // Check Search Term
    if (searchTerm != aTerm) {
        //qDebug() << "ViewerWindow::setSearchTerm - aTerm: " << aTerm;

        // Set Search Term
        searchTerm = aTerm;

        // ...
    }
}

//==============================================================================
// New File
//==============================================================================
bool ViewerWindow::newFile(const QString& aDirPath)
{
    qDebug() << "ViewerWindow::newFile - aDirPath: " << aDirPath;

    // Quick Widget Set Visible
    ui->quickWidget->setVisible(false);
    // Text Edit Set Visible
    ui->textEdit->setVisible(true);
    // Set Wrap Mode
    ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
    // Set Menu Item Checked
    ui->actionWord_Wrap->setChecked(false);


    // Reset File Name
    fileName = "";
    // Get Current Dir
    currentDir = aDirPath;

    // Set File Is New
    fileIsNew = true;

    // ...

    return true;
}

//==============================================================================
// Load File
//==============================================================================
bool ViewerWindow::loadFile(const QString& aFileName, const QString& aPanelName)
{
    // Init Mime Database
    QMimeDatabase mimeDatabase;

    // Get Mime Tpye
    mime = mimeDatabase.mimeTypeForFile(aFileName).name();

    // Reset File Is New
    fileIsNew = false;

    qDebug() << "ViewerWindow::loadFile - aFileName: " << aFileName << " - mime: " << mime;

    // Set File Name
    fileName = aFileName;
    // Emit Content Source Changed
    emit contentSourceChanged(fileName);

    // Set Context Properties
    QQmlContext* ctx = ui->quickWidget->rootContext();
    // Set Context Properties - Viewer Content
    ctx->setContextProperty(DEFAULT_IMAGE_VIEWER_CONTENT, fileName);

    // Edit Mode & Check Mime Type - Load All Files As Text in Edit Mode
    if (isSupportedTextMime(editMode, mime)) {
        // Quick Widget Set Visible
        ui->quickWidget->setVisible(false);
        // Text Edit Set Visible
        ui->textEdit->setVisible(true);

        // Init File
        QFile file(aFileName);

        // Open File
        if (file.open(QIODevice::ReadOnly)) {
            // Init Text Stream
            QTextStream textStream(&file);

            // Check Mime Again
            if (mime.startsWith(QString(DEFAULT_MIME_PREFIX_APP) + QString(DEFAULT_MIME_XML)) ||
                mime.startsWith(QString(DEFAULT_MIME_PREFIX_APP) + QString(DEFAULT_MIME_SHELLSCRIPT))) {
                // Load From File
                ui->textEdit->setPlainText(textStream.readAll());
            } else {
                // Load From File
                ui->textEdit->setText(textStream.readAll());
            }

            // Reset Dirty Flag
            dirty = false;

            // Close File
            file.close();

            // Update Window Title
            updateWindowTitle();
        }

    } else if (!editMode && mime.startsWith(DEFAULT_MIME_PREFIX_IMAGE)) {

        // Text Edit Set Visible
        ui->textEdit->setVisible(false);
        // Quick Widget Set Visible
        ui->quickWidget->setVisible(true);

        // Set Source
        ui->quickWidget->setSource(QUrl("qrc:/qml/ImageViewer.qml"));

        // Check Image Browser
        if (!imageBrowser) {
            // Create Image Browser
            imageBrowser = new ImageBrowser(fileName, aPanelName);

            // Connect Signals
            connect(imageBrowser, SIGNAL(currentIndexChanged(int)), this, SLOT(imageBrowserCurrentIndexChanged(int)));
            connect(imageBrowser, SIGNAL(currentFileChanged(QString)), this, SLOT(imageBrowserCurrentFileChanged(QString)));
            connect(imageBrowser, SIGNAL(currentFileChanged(QString)), this, SIGNAL(currentImageFileChanged(QString)));
            connect(imageBrowser, SIGNAL(selectCurrent()), this, SLOT(handleImageBrowserSelection()));

            // Set Context Properties
            QQmlContext* ctx = ui->quickWidget->rootContext();
            // Set Context Properties - Image Browser
            ctx->setContextProperty(DEFAULT_IMAGE_BROWSER, imageBrowser);
        }

        // Set Context Property
        //ctx->setContextProperty(DEFAULT_IMAGE_BROWSER, imageBrowser);

    } else if (!editMode && mime.startsWith(DEFAULT_MIME_PREFIX_AUDIO)) {

        // Text Edit Set Visible
        ui->textEdit->setVisible(false);
        // Quick Widget Set Visible
        ui->quickWidget->setVisible(true);

        // Set Source
        ui->quickWidget->setSource(QUrl("qrc:/qml/VideoViewer.qml"));

    } else if (!editMode && mime.startsWith(DEFAULT_MIME_PREFIX_VIDEO)) {

        // Text Edit Set Visible
        ui->textEdit->setVisible(false);
        // Quick Widget Set Visible
        ui->quickWidget->setVisible(true);

        // Set Source
        ui->quickWidget->setSource(QUrl("qrc:/qml/VideoViewer.qml"));

    } else {

        // Unsupported Format

        // Reset File Name
        fileName = "";

        return false;
    }

    // ...

    return true;
}

//==============================================================================
// Save File
//==============================================================================
void ViewerWindow::saveFile(const bool& aShowFileDialog)
{
    // Check Edit Mode
    if (!editMode) {
        return;
    }

    // Check If Show File Dialog & Dirty
    if (!aShowFileDialog && !dirty) {
        return;
    }

    qDebug() << "ViewerWindow::saveFile";

    // Init Save File Name
    QString saveFileName = "";

    // Check File Namem
    if (!fileName.isEmpty()) {
        // Get Saved File Name
        saveFileName = aShowFileDialog ? QFileDialog::getSaveFileName(this, tr(DEFAULT_TITLE_SAVE_FILE), fileName, "", NULL) : fileName;
    } else {
        // Get Saved File Name
        saveFileName = QFileDialog::getSaveFileName(this, tr(DEFAULT_TITLE_SAVE_NEW_FILE), currentDir);
    }

    // Check Save File Name
    if (!saveFileName.isEmpty()) {
        // Save File As
        saveFileAs(saveFileName);
    }
}

//==============================================================================
// Save File As
//==============================================================================
void ViewerWindow::saveFileAs(const QString& aFileName)
{
    qDebug() << "ViewerWindow::saveFileAs - aFileName: " << aFileName;

    // Init File
    QFile file(aFileName);

    // Open File
    if (file.open(QIODevice::WriteOnly)) {
        // Init Text Stream
        QTextStream textStream(&file);

        // Load From File
        textStream << ui->textEdit->toPlainText();

        // Reset Dirty Flag
        dirty = false;

        // Set File Name
        fileName = aFileName;

        // Close File
        file.close();
    }
}

//==============================================================================
// Show Window
//==============================================================================
void ViewerWindow::showWindow()
{
    qDebug() << "ViewerWindow::showWindow";

    // Restore UI
    restoreUI();

    // Show
    show();

    // Check Search Term
    if (!searchTerm.isEmpty()) {
        // Enable Find Next
        ui->actionFind_Next->setEnabled(true);
    }

    // Set Focus
    setFocus();
}

//==============================================================================
// Close
//==============================================================================
void ViewerWindow::closeWindow()
{
    qDebug() << "ViewerWindow::closeWindow";

    // Save Settings
    saveSettings();

    // Close
    close();
}

//==============================================================================
// Set Edit Mode Enabled
//==============================================================================
void ViewerWindow::setEditModeEnabled(const bool& aEnabled)
{
    // Check Edit Mode
    if (editMode != aEnabled) {
        qDebug() << "ViewerWindow::setEditModeEnabled - aEnabled: " << aEnabled;
        // Set Edit Mode
        editMode = aEnabled;

        // Set Read Only Mode
        ui->textEdit->setReadOnly(!editMode);

        // Check Edit Mode
        if (editMode) {
            // Set Wrap Mode
            ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
        }
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void ViewerWindow::restoreUI()
{
    // Get Editor Width
    int viewerWidth = settings->value(SETTINGS_KEY_VIEWER_WIDTH).toInt();
    int viewerHeight = settings->value(SETTINGS_KEY_VIEWER_HEIGHT).toInt();
    // Get Desktop Widget
    QDesktopWidget* desktop = QApplication::desktop();

    qDebug() << "ViewerWindow::restoreUI - availableGeometry: " << desktop->availableGeometry();

    // Calculate Editor Position
    int viewerPosX = (desktop->availableGeometry().size().width() - viewerWidth) / 2;
    int viewerPosY = (desktop->availableGeometry().size().height() - viewerHeight) / 2;

    // Set Geometry
    setGeometry(viewerPosX, viewerPosY, viewerWidth, viewerHeight);

    // Check Edit Mode
    if (editMode) {
        // Set Word Wrap Mode
        ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
        // Show Message
        ui->statusbar->showMessage(tr(DEFAULT_TEXT_WORD_WRAP_OFF), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
        // Set Menu Item Checked
        ui->actionWord_Wrap->setChecked(false);
    } else {
        // Get Wrap Mode
        bool wrapMode = settings->value(SETTINGS_KEY_VIEWER_WORDWRAP, false).toBool();
        // Set Menu Item Checked
        ui->actionWord_Wrap->setChecked(wrapMode);

        // Check Mime - Text Files
        if (mime.startsWith(DEFAULT_MIME_PREFIX_TEXT)) {
            // Check Wrap Mode
            if (wrapMode) {
                // Set Word Wrap Mode
                ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
                // Show Message
                ui->statusbar->showMessage(tr(DEFAULT_TEXT_WORD_WRAP_ON), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
            } else {
                // Set Word Wrap Mode
                ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
                // Show Message
                ui->statusbar->showMessage(tr(DEFAULT_TEXT_WORD_WRAP_OFF), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
            }
        } else {

            // ...

        }
    }

    // Update Window Title
    updateWindowTitle();

    // Update Menu Bar
    updateMenuBar();

    // Init Loaded Font
    QFont loadedFont(settings->value(SETTINGS_KEY_VIEWER_FONTNAME).toString(), settings->value(SETTINGS_KEY_VIEWER_FONTSIZE).toInt());
    // Set Bold
    loadedFont.setBold(settings->value(SETTINGS_KEY_VIEWER_FONTBOLD).toBool());
    // Set Italic
    loadedFont.setItalic(settings->value(SETTINGS_KEY_VIEWER_FONTITALIC).toBool());

    // Set Font
    ui->textEdit->setFont(loadedFont);

    // Set Find Next Enabled
    ui->actionFind_Next->setEnabled(false);

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void ViewerWindow::saveSettings()
{
    qDebug() << "ViewerWindow::saveSettings";

    // Set Value
    settings->setValue(SETTINGS_KEY_VIEWER_WIDTH, rect().width());
    settings->setValue(SETTINGS_KEY_VIEWER_HEIGHT, rect().height());

    // Check Text Edit Visibility
    if (ui->textEdit->isVisible()) {
        // Set Value - Word Wrap
        settings->setValue(SETTINGS_KEY_VIEWER_WORDWRAP, ui->textEdit->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere);
    }

    // Save Font Family
    settings->setValue(SETTINGS_KEY_VIEWER_FONTNAME, ui->textEdit->font().family());
    settings->setValue(SETTINGS_KEY_VIEWER_FONTSIZE, ui->textEdit->font().pointSize());
    settings->setValue(SETTINGS_KEY_VIEWER_FONTBOLD, ui->textEdit->font().bold());
    settings->setValue(SETTINGS_KEY_VIEWER_FONTITALIC, ui->textEdit->font().italic());

    // ...
}

//==============================================================================
// Toggle Wrap Mode
//==============================================================================
void ViewerWindow::toggleWrapMode()
{
    // Check Wrap Mode
    if (ui->textEdit->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere) {
        //qDebug() << "ViewerWindow::toggleWrapMode - wrap OFF";
        // Set Wrap Mode
        ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
        // Set Status Text
        ui->statusbar->showMessage(tr(DEFAULT_TEXT_WORD_WRAP_OFF), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
        // Set Menu Item Checked
        ui->actionWord_Wrap->setChecked(false);
    } else {
        //qDebug() << "ViewerWindow::toggleWrapMode - wrap ON";
        // Set Wrap Mode
        ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        // Set Status Text
        ui->statusbar->showMessage(tr(DEFAULT_TEXT_WORD_WRAP_ON), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
        // Set Menu Item Checked
        ui->actionWord_Wrap->setChecked(true);
    }
}

//==============================================================================
// Is Mime Supported
//==============================================================================
bool ViewerWindow::isSupportedTextMime(const bool& aEditMode, const QString& aMime)
{
    // Check Edit Mode
    if (aEditMode) {

    }

    // Check Contains
    if (aMime.contains(DEFAULT_MIME_TEXT))
        return true;

    // Check Contains
    if (aMime.contains(DEFAULT_MIME_XML))
        return true;

    // Check Contains
    if (aMime.contains(DEFAULT_MIME_SHELLSCRIPT))
        return true;

    // Check Contains
    if (aMime.contains(DEFAULT_MIME_JAVASCRIPT))
        return true;

    if (aMime.contains(DEFAULT_MIME_SUBRIP))
        return true;

    return false;
}

//==============================================================================
// Update Window Title
//==============================================================================
void ViewerWindow::updateWindowTitle()
{
    // Check File Name
    if (editMode && (fileName.isEmpty() || fileName.isNull())) {
        // Set Window Title
        setWindowTitle(tr(DEFAULT_TITLE_EDITOR_NEW_FILE));
    } else {
        // Set Window Title
        setWindowTitle(editMode ? tr(DEFAULT_TITLE_EDITOR) + fileName : tr(DEFAULT_TITLE_VIEWER) + fileName);
    }
}

//==============================================================================
// Update Menu Bar
//==============================================================================
void ViewerWindow::updateMenuBar()
{
    // Check Edit Mode
    if (editMode) {
        // Set Actions Enabled
        ui->actionSave->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
        ui->actionWord_Wrap->setEnabled(false);
    } else {
        // Set Actions Enabled
        ui->actionSave->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
        ui->actionWord_Wrap->setEnabled(true);
    }
}

//==============================================================================
// Handle Image Browser Selection
//==============================================================================
void ViewerWindow::handleImageBrowserSelection()
{
    // Check Image Browser
    if (imageBrowser) {
        // Emit Image Selected
        emit imageSelected(imageBrowser->getCurrentDir() + imageBrowser->getCurrentFile(), imageBrowser->getPanelName());
    }

    // Close Window
    closeWindow();
}

//==============================================================================
// Text Changed Slot
//==============================================================================
void ViewerWindow::textChanged()
{
    //qDebug() << "ViewerWindow::textChanged";

    // Mark Dirty
    dirty = true;

    // Set File Modified
    fileModified = true;
}

//==============================================================================
// Current Index Changed Slot
//==============================================================================
void ViewerWindow::imageBrowserCurrentIndexChanged(const int& aCurrentIndex)
{
    Q_UNUSED(aCurrentIndex);

    //qDebug() << "ViewerWindow::imageBrowserCurrentIndexChanged - aCurrentIndex: " << aCurrentIndex;

    // ...
}

//==============================================================================
// Current File Changed Slot
//==============================================================================
void ViewerWindow::imageBrowserCurrentFileChanged(const QString& aCurrentFile)
{
    //qDebug() << "ViewerWindow::imageBrowserCurrentFileChanged - aCurrentFile: " << aCurrentFile;

    // Set File Name
    fileName = aCurrentFile;

    // Set Context Properties
    QQmlContext* ctx = ui->quickWidget->rootContext();
    // Set Context Properties - Viewer Content
    ctx->setContextProperty(DEFAULT_IMAGE_VIEWER_CONTENT, fileName);

    // Set Window Title
    setWindowTitle(tr(DEFAULT_TITLE_VIEWER) + fileName);
}

//==============================================================================
// Search Text
//==============================================================================
void ViewerWindow::searchText(const QString& aSearchTerm)
{
    // Check Text Edit
    if (ui->textEdit->isVisible()) {
        //qDebug() << "ViewerWindow::searchText - aSearchTerm: " << aSearchTerm;

        // Set Search Term
        setSearchTerm(aSearchTerm);

        // Find Next
        findNext();
    }
}

//==============================================================================
// Find Next Search Term Occurence
//==============================================================================
void ViewerWindow::findNext()
{
    //qDebug() << "ViewerWindow::findNext";

    // Init Find Flags
    QTextDocument::FindFlags findFlags = 0;

    // Check Find Dialog
    if (findDialog) {
        // Check Case Sensitive Search
        if (findDialog->getCaseSensitiveSearch()) {
            // Adjust Options
            findFlags |= QTextDocument::FindCaseSensitively;
        }

        // Check Whole Word
        if (findDialog->getWholeWordSearch()) {
            // Adjust Options
            findFlags |= QTextDocument::FindWholeWords;
        }

        // Check Backward Search
        if (findDialog->getBackwardSearch()) {
            // Adjust Options
            findFlags |= QTextDocument::FindBackward;
        }
    }

    // Find Occurence
    if (ui->textEdit->find(searchTerm, findFlags)) {
        // Check Find Dialog
        if (findDialog) {
            // Enable Find Next Button
            findDialog->enableFindNextButton(true);
        }

        // Set Find Next Action Enabled
        ui->actionFind_Next->setEnabled(true);

    } else {
        // Check Find Dialog
        if (findDialog) {
            // Check Wrap Around
            if (findDialog->getWrapAroundWhenFinished()) {

                // Get Text Cursor
                QTextCursor textCursor = ui->textEdit->textCursor();
                // Move Position
                textCursor.movePosition(QTextCursor::Start);
                // Set Updated Text Cursor
                ui->textEdit->setTextCursor(textCursor);
            }

            // Enable Find Next Button
            findDialog->enableFindNextButton(false);
        }

        // Set Find Next Action Enabled
        ui->actionFind_Next->setEnabled(false);
    }
}

//==============================================================================
// find Dialog Escape Pressed Slot
//==============================================================================
void ViewerWindow::findDialogEscapePressed()
{
    //qDebug() << "ViewerWindow::findDialogEscapePressed";

    // Set Find Dialog Key Press
    findDialogKeyPress = true;
}

//==============================================================================
// Close Event
//==============================================================================
void ViewerWindow::closeEvent(QCloseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        qDebug() << "ViewerWindow::​closeEvent - dirty: " << dirty;

        // Check Dirty Flag
        if (dirty) {
            // Init Confirm Dialog
            ConfirmDialog confirmDialog;

            // Set Dialog Title
            confirmDialog.setConfirmTitle(tr(DEFAULT_TITLE_FILE_MODIFIED));

            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_CONFIRM_TEXT_SAVE_CHANGES));

            // Configure Standard Buttons
            confirmDialog.configureButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);

            // ...

            // Exec Confirm Dialog
            if (confirmDialog.exec()) {
                // Save File
                saveFile(false);
            }
        }

        // Accept Event
        aEvent->accept();

        // Emit Viewer Closed Signal
        emit viewerClosed(this);
    }
}

//==============================================================================
// Key Press Event
//==============================================================================
void ViewerWindow::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {

        // Set Own Key Pressed
        ownKeyPress = true;

    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void ViewerWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check If Key Pressed
    if (!ownKeyPress) {
        return;
    }

    // Reset Own Key Press
    ownKeyPress = false;

    // Check Find Dialog Key Press
    if (findDialogKeyPress) {
        // Reset Find Dialog Key Press
        findDialogKeyPress = false;

        return;
    }

    // Check Event
    if (aEvent) {
        // Switch Event Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Close Window
                closeWindow();
            break;

            case Qt::Key_Home:
                // Check Image Browser
                if (imageBrowser) {
                    // Go To First
                    imageBrowser->gotoFirst();
                }
            break;

            case Qt::Key_Left:
                // Check Image Browser
                if (imageBrowser) {
                    // Go To Prev
                    imageBrowser->gotoPrev();
                }
            break;

            case Qt::Key_Right:
                // Check Image Browser
                if (imageBrowser) {
                    // Go To Next
                    imageBrowser->gotoNext();
                }
            break;

            case Qt::Key_End:
                // Check Image Browser
                if (imageBrowser) {
                    // Go To Last
                    imageBrowser->gotoLast();
                }
            break;

            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Handle Selection
                handleImageBrowserSelection();
            break;

            case Qt::Key_S: {

                Qt::KeyboardModifiers modifiers = aEvent->modifiers();

                // Check Modifiers
                if (modifiers & Qt::ControlModifier) {
                    // Save File
                    saveFile();
                }
            } break;

            default:
            break;
        }
    }
}

//==============================================================================
// On Action Save Triggered Slot
//==============================================================================
void ViewerWindow::on_actionSave_triggered()
{
    // Save File
    saveFile(false);
}

//==============================================================================
// On Action Save As Triggered Slot
//==============================================================================
void ViewerWindow::on_actionSave_As_triggered()
{
    // Set File Is New Manually
    fileIsNew = true;

    // Save File
    saveFile();
}

//==============================================================================
// On Action Close Triggered Slot
//==============================================================================
void ViewerWindow::on_actionClose_triggered()
{
    // Close
    close();
}

//==============================================================================
// On Word Wrap Action Triggered Slot
//==============================================================================
void ViewerWindow::on_actionWord_Wrap_triggered()
{
    // Toggle Wrap Mode
    toggleWrapMode();
}

//==============================================================================
// On Font Action Triggered Slot
//==============================================================================
void ViewerWindow::on_actionFont_triggered()
{
    // Init Font Selected
    bool fontSelected = false;

    // Launch Font Dialog
    QFont selectedFont = QFontDialog::getFont(&fontSelected, ui->textEdit->font(), NULL, tr(DEFAULT_TITLE_SELECT_FONT));

    // Check If Font Selected
    if (fontSelected) {
        // Sert Font
        ui->textEdit->setFont(selectedFont);
    }
}

//==============================================================================
// On Action Find Triggered Slot
//==============================================================================
void ViewerWindow::on_actionFind_triggered()
{
    // Check Text Editor Visibility
    if (ui->textEdit->isVisible()) {
        // Check Find Dialog
        if (!findDialog) {
            // Create Find Dialog
            findDialog = new FindTextDialog();

            // Connect Signals
            connect(findDialog, SIGNAL(findText(QString)), this, SLOT(searchText(QString)));
            connect(findDialog, SIGNAL(findNext()), this, SLOT(findNext()));
            connect(findDialog, SIGNAL(escapePresed()), this, SLOT(findDialogEscapePressed()));
        }

        // Launch Find Text Dialog
        findDialog->exec();
    }
}

//==============================================================================
// On Action Find Next Triggered Slot
//==============================================================================
void ViewerWindow::on_actionFind_Next_triggered()
{
    // Fnd Next
    findNext();
}

//==============================================================================
// Destructor
//==============================================================================
ViewerWindow::~ViewerWindow()
{
    // Save Settings
    saveSettings();

    // Delete Quick Widget
    delete ui->quickWidget;

    // Delete UI
    delete ui;

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        // Reset Settins
        settings = NULL;
    }

    // Check Image Browser
    if (imageBrowser) {
        // Delete Image Browser
        delete imageBrowser;
        imageBrowser = NULL;
    }

    // Check Find Dialog
    if (findDialog) {
        // Delete Dialog
        delete findDialog;
        findDialog = NULL;
    }

    qDebug() << "ViewerWindow::~ViewerWindow";
}

























//==============================================================================
// Constructor
//==============================================================================
ImageBrowser::ImageBrowser(const QString& aFileName, const QString& aPanelName, QObject* aParent)
    : QObject(aParent)
    , fileUtil(NULL)
    , panelName(aPanelName)
    , currentIndex(-1)
    , currentFile(aFileName)
    , currentDir("")
{
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void ImageBrowser::init()
{
    // Init Current File Info
    QFileInfo fileInfo(currentFile);
    // Check File Info
    if (fileInfo.exists()) {
        // Set Current Dir
        currentDir = fileInfo.absolutePath();

        // Check Current Dir
        if (!currentDir.endsWith("/")) {
            // Update Current Dir
            currentDir += "/";
        }

        // Check File Util
        if (!fileUtil) {
            // Create File Util
            fileUtil = new RemoteFileUtilClient();

            // Connect Signals
            connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
            connect(fileUtil, SIGNAL(clientStatusChanged(uint, int)), this, SLOT(clientStatusChanged(uint, int)));
            connect(fileUtil, SIGNAL(dirListItemFound(uint,QString,QString)), this, SLOT(dirListItemFound(uint,QString,QString)));
            connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
            connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
            connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        }

        // Connect To Server
        fileUtil->connectToFileServer();
    }

    // ...
}

//==============================================================================
// Find Index By File Name
//==============================================================================
int ImageBrowser::findIndex(const QString& aFileName)
{
    // Get Image Files Count
    int ifCount = imageFiles.count();

    // Go Thru Image Files
    for (int i=0; i<ifCount; ++i) {
        // Check File Name
        if (imageFiles[i] == aFileName) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Set Current Index
//==============================================================================
void ImageBrowser::setCurrentIndex(const int& aIndex)
{
    // Get Bounded Index
    int boundedIndex = qBound(0, aIndex, imageFiles.count()-1);

    // Check Current Index
    if (currentIndex != boundedIndex) {
        //qDebug() << "ImageBrowser::setCurrentIndex - boundedIndex: " << boundedIndex;
        // Set Current Index
        currentIndex = boundedIndex;

        // Emit Current Index Changed Signal
        emit currentIndexChanged(currentIndex);

        // Check Image Files
        if (imageFiles.count() > 0) {
            // Set Current File
            currentFile = imageFiles[currentIndex];

            // Emit Current File Changed Signal
            emit currentFileChanged(currentDir + currentFile);
        }
    }
}

//==============================================================================
// Get Current File
//==============================================================================
QString ImageBrowser::getCurrentFile()
{
    return currentFile;
}

//==============================================================================
// Get Current Index
//==============================================================================
int ImageBrowser::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Get Panel Name
//==============================================================================
QString ImageBrowser::getPanelName()
{
    return panelName;
}

//==============================================================================
// Get Current Dir
//==============================================================================
QString ImageBrowser::getCurrentDir()
{
    return currentDir;
}

//==============================================================================
// Go To First
//==============================================================================
void ImageBrowser::gotoFirst()
{
    // Set Current Index
    setCurrentIndex(0);
}

//==============================================================================
// Go To Prev
//==============================================================================
void ImageBrowser::gotoPrev()
{
    // Set Current Index
    setCurrentIndex(currentIndex - 1);
}

//==============================================================================
// Go To Next
//==============================================================================
void ImageBrowser::gotoNext()
{
    // Set Current Index
    setCurrentIndex(currentIndex + 1);
}

//==============================================================================
// Go To Last
//==============================================================================
void ImageBrowser::gotoLast()
{
    // Set Current Index
    setCurrentIndex(imageFiles.count() - 1);
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void ImageBrowser::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    Q_UNUSED(aID);

    // Check If Connected
    if (aConnected && fileUtil) {
        // Init Settings
        QSettings settings;
        // Get Sorting
        int sorting = settings.value(panelName + SETTINGS_KEY_PANEL_SORTTYPE, DEFAULT_SORT_NAME).toInt();

        // Fetch Dir Items
        fileUtil->getDirList(currentDir, QDir::Files, sorting);
    }
}

//==============================================================================
//==============================================================================
// Client Status Changed Slot
void ImageBrowser::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    Q_UNUSED(aID);

    qDebug() << "ImageBrowser::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void ImageBrowser::fileOpFinished(const unsigned int& aID,
                    const QString& aOp,
                    const QString& aPath,
                    const QString& aSource,
                    const QString& aTarget)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "ImageBrowser::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // Check Path
    if (QDir(currentDir) == QDir(aPath)) {
        // Init File Info
        QFileInfo fileInfo(currentFile);
        // Get Image Index
        currentIndex = findIndex(fileInfo.fileName());
    }
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void ImageBrowser::fileOpAborted(const unsigned int& aID,
                   const QString& aOp,
                   const QString& aPath,
                   const QString& aSource,
                   const QString& aTarget)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "ImageBrowser::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void ImageBrowser::fileOpError(const unsigned int& aID,
                 const QString& aOp,
                 const QString& aPath,
                 const QString& aSource,
                 const QString& aTarget,
                 const int& aError)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "ImageBrowser::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

    // ...
}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void ImageBrowser::dirListItemFound(const unsigned int& aID,
                                    const QString& aPath,
                                    const QString& aFileName)
{
    Q_UNUSED(aID);

    // Check Path
    if (currentDir == aPath) {
        // Init Mime Database
        QMimeDatabase mimeDatabase;
        // Get Mime Tpye
        QString mimeType = mimeDatabase.mimeTypeForFile(aFileName).name();
        // Check Mime
        if (mimeType.startsWith(DEFAULT_MIME_PREFIX_IMAGE)) {
            // Add To Image List
            imageFiles << aFileName;
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
ImageBrowser::~ImageBrowser()
{
    // Clear Images
    imageFiles.clear();

    // Check File Util
    if (fileUtil) {
        // Close
        fileUtil->close();

        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }
}









