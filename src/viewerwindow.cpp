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
#include <QDebug>

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
// Load File
//==============================================================================
void ViewerWindow::loadFile(const QString& aFileName)
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

    settings.setValue(SETTINGS_KEY_VIEWER_WORDWRAP, ui->textEdit->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere);

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
// Close Event
//==============================================================================
void ViewerWindow::closeEvent(QCloseEvent* aEvent)
{
    // Check Event
    if (aEvent) {

        // Check Dirty Flag
        if (dirty) {
            qDebug() << "ViewerWindow::​closeEvent - IGNORE!";

            // Ask For Save

            // Ignore Event
            aEvent->ignore();

        } else {
            qDebug() << "ViewerWindow::​closeEvent";

            // Accept Event
            aEvent->accept();

            // Emit Viewer Closed Signal
            emit viewerClosed(this);
        }
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

    qDebug() << "ViewerWindow::~ViewerWindow";

}
