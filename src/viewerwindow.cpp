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
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

#include <mcwinterface.h>

#include "viewerwindow.h"
#include "ui_viewerwindow.h"
#include "confirmdialog.h"
#include "remotefileutilclient.h"
#include "audiotagimageprovider.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
ViewerWindow::ViewerWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::ViewerWindow)
    , fileName("")
    , editMode(false)
    , dirty(false)
    , mime("")
    , imageBrowser(NULL)
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
    // Set Context Properties - Dummy Model
    ctx->setContextProperty(DEFAULT_IMAGE_VIEWER_CONTENT, fileName);

    // Get Engine
    QQmlEngine* engine = ui->quickWidget->engine();
    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_AUDIO_TAG_PROVIDER_ID), new AudioTagImageProvider());

    // Set Resize Mode
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

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
// New File
//==============================================================================
void ViewerWindow::newFile(const QString& aDirPath)
{
    qDebug() << "ViewerWindow::newFile - aDirPath: " << aDirPath;

    // Quick Widget Set Visible
    ui->quickWidget->setVisible(false);
    // Text Edit Set Visible
    ui->textEdit->setVisible(true);

    // Reset File Name
    fileName = "";
    // Get Current Dir
    currentDir = aDirPath;

    // ...
}

//==============================================================================
// Load File
//==============================================================================
void ViewerWindow::loadFile(const QString& aFileName, const QString& aPanelName)
{
    // Init Mime Database
    QMimeDatabase mimeDatabase;

    // Get Mime Tpye
    mime = mimeDatabase.mimeTypeForFile(aFileName).name();

    qDebug() << "ViewerWindow::loadFile - aFileName: " << aFileName << " - mime: " << mime;

    // Set File Name
    fileName = aFileName;
    // Emit Content Source Changed
    emit contentSourceChanged(fileName);

    // Set Context Properties
    QQmlContext* ctx = ui->quickWidget->rootContext();
    // Set Context Properties - Dummy Model
    ctx->setContextProperty(DEFAULT_IMAGE_VIEWER_CONTENT, fileName);

    // Check Mime Type
    if (mime.startsWith(DEFAULT_MIME_PREFIX_TEXT)) {
        // Configure View

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

            // Load From File
            ui->textEdit->setText(textStream.readAll());

            // Reset Dirty Flag
            dirty = false;

            // Close File
            file.close();
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

    }

    // ...
}

//==============================================================================
// Save File As
//==============================================================================
void ViewerWindow::saveFileAs(const QString& aFileName)
{
    qDebug() << "ViewerWindow::saveFileAs - aFileName: " << aFileName;

    // Save To File

    // ...
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
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void ViewerWindow::restoreUI()
{
    // Init Settings
    QSettings settings;

    // Get Editor Width
    int viewerWidth = settings.value(SETTINGS_KEY_VIEWER_WIDTH).toInt();
    int viewerHeight = settings.value(SETTINGS_KEY_VIEWER_HEIGHT).toInt();
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
        ui->statusbar->showMessage(tr("Wrap Off"), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
    } else {
        // Get Wrap Mode
        bool wrapMode = settings.value(SETTINGS_KEY_VIEWER_WORDWRAP, false).toBool();
        // Check Mime - Text Files
        if (mime.startsWith(DEFAULT_MIME_PREFIX_TEXT)) {
            // Check Wrap Mode
            if (wrapMode) {
                // Set Word Wrap Mode
                ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
                // Show Message
                ui->statusbar->showMessage(tr("Wrap ON"), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
            } else {
                // Set Word Wrap Mode
                ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
                // Show Message
                ui->statusbar->showMessage(tr("Wrap Off"), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
            }
        } else {

            // ...

        }
    }

    // Set Window Title
    setWindowTitle(tr("Viewer - ") + fileName);

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void ViewerWindow::saveSettings()
{
    qDebug() << "ViewerWindow::saveSettings";

    // Init Settings
    QSettings settings;

    // Set Value
    settings.setValue(SETTINGS_KEY_VIEWER_WIDTH, rect().width());
    settings.setValue(SETTINGS_KEY_VIEWER_HEIGHT, rect().height());

    // Check Text Edit Visibility
    if (ui->textEdit->isVisible()) {
        // Set Value - Word Wrap
        settings.setValue(SETTINGS_KEY_VIEWER_WORDWRAP, ui->textEdit->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere);
    }

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
        //ui->statusLabel->setText(tr("Wrap Off"));
        ui->statusbar->showMessage(tr("Wrap Off"), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
    } else {
        //qDebug() << "ViewerWindow::toggleWrapMode - wrap ON";
        // Set Wrap Mode
        ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        // Set Status Text
        //ui->statusLabel->setText(tr("Wrap On"));
        ui->statusbar->showMessage(tr("Wrap On"), DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT);
    }
}

//==============================================================================
// Text Changed Slot
//==============================================================================
void ViewerWindow::textChanged()
{
    //qDebug() << "ViewerWindow::textChanged";

    // Mark Dirty
    dirty = true;
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
    // Set Context Properties - Dummy Model
    ctx->setContextProperty(DEFAULT_IMAGE_VIEWER_CONTENT, fileName);

    // Set Window Title
    setWindowTitle(tr("Viewer - ") + fileName);
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

            // Init Save File Name
            QString saveFileName = "";

            // Check File Name
            if (!fileName.isEmpty()) {
                // Get Saved File Name
                saveFileName = QFileDialog::getSaveFileName(this, tr(DEFAULT_TITLE_SAVE_FILE), fileName, "", NULL, QFileDialog::DontConfirmOverwrite);
            } else {
                // Get Saved File Name
                saveFileName = QFileDialog::getSaveFileName(this, tr(DEFAULT_TITLE_SAVE_NEW_FILE), currentDir);
            }

            // Check Save File Name
            if (!saveFileName.isEmpty()) {
                // Init File
                QFile file(saveFileName);

                // Open File
                if (file.open(QIODevice::WriteOnly)) {
                    // Init Text Stream
                    QTextStream textStream(&file);

                    // Load From File
                    textStream << ui->textEdit->toPlainText();

                    // Reset Dirty Flag
                    dirty = false;

                    // Close File
                    file.close();
                }
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

    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void ViewerWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Event Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Close Window
                closeWindow();
            break;

            case Qt::Key_W:
                // Check Editor Mode
                if (!editMode) {
                    // Toggle Wrap Mode
                    toggleWrapMode();
                }
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

            default:
            break;
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
ViewerWindow::~ViewerWindow()
{
    // Save Settings
    saveSettings();

    // Delete UI
    delete ui;

    // Check Image Browser
    if (imageBrowser) {
        // Delete Image Browser
        delete imageBrowser;
        imageBrowser = NULL;
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









