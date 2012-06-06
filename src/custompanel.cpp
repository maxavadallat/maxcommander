
// INCLUDES

#include <QDebug>
#include <QFontMetrics>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>

#include "constants.h"
#include "custompanel.h"
#include "ui_custompanel.h"
#include "customfilelist.h"
#include "headerpopup.h"
#include "settings.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
CustomPanel::CustomPanel(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::CustomPanel)
    , active(false)
    , settings(NULL)
    , execBundles(true)
    , headerPopup(NULL)
{
    qDebug() << "Creating CustomPanel...";

    // Get Settings Instance
    settings = Settings::getInstance();

    // Setup UI
    ui->setupUi(this);
    // Set File List Header
    ui->fileList->setFileListHeader(ui->header);
    // Set Header Sorting
    ui->header->setSorting(0, false, false);

    // Create Header Popup
    headerPopup = new HeaderPopup(NULL);

    // Connect Signals
    connect(ui->header, SIGNAL(headerActivated()), this, SLOT(headerActivated()));
    connect(ui->header, SIGNAL(orderChanged(int,bool,bool)), this, SLOT(headerOrderChanged(int,bool,bool)));
    connect(ui->header, SIGNAL(headerOptions(QPoint)), this, SLOT(headerOptionsLaunch(QPoint)));

    connect(ui->fileList, SIGNAL(listBoxFocusChanged(QString,bool)), this, SLOT(fileListFocusChanged(QString,bool)));
    connect(ui->fileList, SIGNAL(currentDirChanged(QString)), this, SLOT(currentDirChanged(QString)));
    connect(ui->fileList, SIGNAL(currentDirReadFinished()), this, SLOT(currentDirReadFinished()));
    connect(ui->fileList, SIGNAL(fileSelected(FileItemData*)), this, SLOT(fileListFileSelected(FileItemData*)));

    connect(headerPopup, SIGNAL(closeButtonClicked()), this, SLOT(headerPopupCloseButtonClicked()));

#if defined (Q_OS_WIN)

    // ...

#elif defined (Q_OS_MAC)

    // Remove Drive Button Widget
    ui->currDirPanelLayout->removeWidget(ui->driveButton);
    // Hide Drive Button
    ui->driveButton->setVisible(false);

#elif defined (Q_OS_LINUX)

    // Remove Drive Button Widget
    ui->currDirPanelLayout->removeWidget(ui->driveButton);
    // Hide Drive Button
    ui->driveButton->setVisible(false);

#endif // Q_OS

    // Remove Selection Label Widget
    //ui->currDirPanelLayout->removeWidget(ui->selectionLabel);
    // Hide Selection Status Label
    //ui->selectionLabel->setVisible(false);

    // Hide Selection Panel
    ui->selectionPanel->setVisible(false);

    // Set Focus Policy
    setFocusPolicy(Qt::NoFocus);

    qDebug() << "Creating CustomPanel...done";
}

//==============================================================================
// Set Panel Name
//==============================================================================
void CustomPanel::setPanelName(const QString& aPanelName)
{
    qDebug() << "CustomPanel::setPanelName - aPanelName: " << aPanelName;
    // Set Panel Name
    panelName = aPanelName;

    // Check UI & Custom File List
    if (ui && ui->fileList) {
        // Set Panel Name
        ui->fileList->setPanelName(aPanelName);
    }
}

//==============================================================================
// Set Active
//==============================================================================
void CustomPanel::setActive(const bool& aActive)
{
    // Check Active State
    if (active != aActive) {
        qDebug() << "CustomPanel::setActive - panelName: " << panelName << " - aActive: " << aActive;

        // Set Active
        active = aActive;

        // Check File List
        if (ui && ui->fileList) {
            // Set Active
            ui->fileList->setActive(active);
        }

        // Check Header
        if (ui && ui->header) {
            // Set Active
            ui->header->setActive(active);
        }
    }
}

//==============================================================================
// Get Active State
//==============================================================================
bool CustomPanel::isActive()
{
    return active;
}

//==============================================================================
// Set Show Hidden Files State
//==============================================================================
void CustomPanel::setShowHiddenFiles(const bool& aShowHidden)
{
    // Check Show Hidden Files
    if (showHidden != aShowHidden) {
        // Set Show Hidden
        showHidden = aShowHidden;

        // Check UI
        if (ui && ui->fileList) {
            // Set Show Hidden
            ui->fileList->setShowHiddenFiles(showHidden);
        }

        // Check Settings
        if (settings) {
            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_GENERAL));
            // Set Show Hidden Files
            settings->setValue(SETTIGNS_KEY_SHOW_HIDDEN_FILES, showHidden);
        }
    }
}

//==============================================================================
// Get Show Hidden Files State
//==============================================================================
bool CustomPanel::getShowHiddenFiles()
{
    return showHidden;
}

//==============================================================================
// Get Current Index
//==============================================================================
int CustomPanel::getCurrentIndex()
{
    // Check UI
    if (ui && ui->fileList) {
        // Return Current File Index
        return ui->fileList->getCurrentIndex();
    }

    return -1;
}

//==============================================================================
// Get File Info
//==============================================================================
QFileInfo CustomPanel::getFileInfo(const int& aIndex)
{
    // Check UI
    if (ui && ui->fileList) {
        // Return Current File Index
        return ui->fileList->getItemData(aIndex)->getFileInfo();
    }

    return QFileInfo();
}

//==============================================================================
// Get Selected Files Count
//==============================================================================
int CustomPanel::getSelectedCount()
{
    // Check UI
    if (ui && ui->fileList) {
        // Get Selected Count
        return ui->fileList->getSelectedCount();
    }

    return 0;
}

//==============================================================================
// Load Settings
//==============================================================================
void CustomPanel::loadSettings()
{
    qDebug() << "CustomPanel::loadSettings - panelName: " << panelName;

    // Check Settings
    if (settings) {
        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_GENERAL));

        // Set Exec Bundles
        execBundles = settings->getValue(SETTIGNS_KEY_EXECUTE_BUNDLES, true).toBool();
        // Get Show Hidden Files
        showHidden = settings->getValue(SETTIGNS_KEY_SHOW_HIDDEN_FILES, false).toBool();

        // Check UI
        if (ui && ui->fileList) {
            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_APPEARANCE));
            // Get Normal Item Background Color
            int bgColor = settings->getValue(QString(SETTINGS_KEY_NORMAL_BACKGROUND_COLOR), SETTINGS_VALUE_NORMAL_BACKGROUND_COLOR).toInt();
            // Set Background Color
            ui->fileList->setBackgroundColor(bgColor);

            // Update Delegate Icon Size
            ui->fileList->updateDelegateIconSize(false);

            // Set Spacing
            ui->fileList->setItemSpacing(settings->getValue(QString(SETTINGS_KEY_ITEM_SPACING), SETTINGS_VALUE_DEFAULT_ITEM_SPACING).toInt(), false);

            // Set Group
            settings->setGroup(panelName);

#ifdef Q_OS_WIN

            // Init Default Dir
            QString defaultDir = QString("c:/");

#else // Q_OS_WIN

            // Init Default Dir
            QString defaultDir = QDir::homePath();

#endif // Q_OS_WIN

            // Set Show Hidden Files
            ui->fileList->setShowHiddenFiles(showHidden, false);
            // Set Current Dir
            ui->fileList->setCurrentDir(settings->getValue(QString(SETTINGS_KEY_LASTDIR), defaultDir).toString(), false);
        }

        // Check UI
        if (ui && ui->header) {
            // Set Group
            settings->setGroup(panelName);

            // Init Header Item Width
            int headerItemWidth = 0;

            // Get Header Item Width
            headerItemWidth = settings->getValue(SETTING_KEY_HEADER_WIDTH_EXT, 0).toInt();
            // Check Header Item Width
            if (headerItemWidth == -1) {
                // Set Item Usage
                ui->header->setItemUseage(1, false);
            } else {
                // Set Item Usage
                ui->header->setItemUseage(1, true);
                // Set Header Item Width
                ui->header->setItemWidth(1, headerItemWidth);
            }

            // Get Header Item Width
            headerItemWidth = settings->getValue(SETTING_KEY_HEADER_WIDTH_SIZE, 0).toInt();
            // Check Header Item Width
            if (headerItemWidth == -1) {
                // Set Item Usage
                ui->header->setItemUseage(2, false);
            } else {
                // Set Item Usage
                ui->header->setItemUseage(2, true);
                // Set Header Item Width
                ui->header->setItemWidth(2, headerItemWidth);
            }

            // Get Header Item Width
            headerItemWidth = settings->getValue(SETTING_KEY_HEADER_WIDTH_DATE, 0).toInt();
            // Check Header Item Width
            if (headerItemWidth == -1) {
                // Set Item Usage
                ui->header->setItemUseage(3, false);
            } else {
                // Set Item Usage
                ui->header->setItemUseage(3, true);
                // Set Header Item Width
                ui->header->setItemWidth(3, headerItemWidth);
            }

            // Get Header Item Width
            headerItemWidth = settings->getValue(SETTING_KEY_HEADER_WIDTH_OWNER, 0).toInt();
            // Check Header Item Width
            if (headerItemWidth == -1) {
                // Set Item Usage
                ui->header->setItemUseage(4, false);
            } else {
                // Set Item Usage
                ui->header->setItemUseage(4, true);
                // Set Header Item Width
                ui->header->setItemWidth(4, headerItemWidth);
            }

            // Get Header Item Width
            headerItemWidth = settings->getValue(SETTING_KEY_HEADER_WIDTH_PERMS, 0).toInt();
            // Check Header Item Width
            if (headerItemWidth == -1) {
                // Set Item Usage
                ui->header->setItemUseage(5, false);
            } else {
                // Set Item Usage
                ui->header->setItemUseage(5, true);
                // Set Header Item Width
                ui->header->setItemWidth(5, headerItemWidth);
            }

            // Set Item Usage
            ui->header->setItemUseage(0, true);
            // Set Header Item Width
            ui->header->setItemWidth(0, ui->header->getItemMaxSize(0, false));

            // Set Sorting
            ui->header->setSorting(settings->getValue(QString(SETTINGS_KEY_SORTTYPE), 0).toInt(), settings->getValue(QString(SETTINGS_KEY_REVERSE), false).toBool(), false);
        }

        // Check UI
        if (ui && ui->fileList) {
            // Reload
            //ui->fileList->reload();
        }
    }
}

//==============================================================================
// Save Settings
//==============================================================================
void CustomPanel::saveSettings()
{
    // Check Settings
    if (settings) {
        qDebug() << "CustomPanel::saveSettings - panelName: " << panelName;

        // Check UI
        if (ui && ui->header) {
            // Set Group
            settings->setGroup(panelName);
            // Save Header Item Settings
            settings->setValue(QString(SETTING_KEY_HEADER_WIDTH_NAME), ui->header->getItemWidth(0));
            settings->setValue(QString(SETTING_KEY_HEADER_WIDTH_EXT), ui->header->getItemWidth(1));
            settings->setValue(QString(SETTING_KEY_HEADER_WIDTH_SIZE), ui->header->getItemWidth(2));
            settings->setValue(QString(SETTING_KEY_HEADER_WIDTH_DATE), ui->header->getItemWidth(3));
            settings->setValue(QString(SETTING_KEY_HEADER_WIDTH_PERMS), ui->header->getItemWidth(4));
            settings->setValue(QString(SETTING_KEY_HEADER_WIDTH_OWNER), ui->header->getItemWidth(5));
            // Set Order Type
            settings->setValue(QString(SETTINGS_KEY_SORTTYPE), ui->header->getSorting());
            // Set Reversed Order
            settings->setValue(QString(SETTINGS_KEY_REVERSE), ui->header->getReversed());
        }

        // Check UI
        if (ui && ui->fileList) {
            // Set Group
            settings->setGroup(panelName);
            // Set Last Current Dir
            settings->setValue(QString(SETTINGS_KEY_LASTDIR), ui->fileList->getCurrentDir());

            // Set Group
            settings->setGroup(QString(SETTINGS_GROUP_GENERAL));
            // Set Show Hidden Files
            settings->setValue(QString(SETTIGNS_KEY_SHOW_HIDDEN_FILES), ui->fileList->getShowHiddenFiles());

            // ...
        }
    }
}

//==============================================================================
// Current Dir Lable Text
//==============================================================================
void CustomPanel::updateCurrDirLabelText(const QString& aCurrDir)
{
    // Check Curr Dir
    if (!aCurrDir.isEmpty() && ui && ui->currDirLabel) {
        // Init Font Metrics
        QFontMetrics fontMetrics(ui->currDirLabel->font());
        // Set Text
        ui->currDirLabel->setText(fontMetrics.elidedText(aCurrDir, Qt::ElideMiddle, ui->currDirLabel->width()));
        // Set Tool Tip
        ui->currDirLabel->setToolTip(aCurrDir);
    }
}

//==============================================================================
// Update Status Panel Text
//==============================================================================
void CustomPanel::updateStatusPanelText()
{
    // Check UI
    if (ui && ui->statusLabel && ui->fileList) {
        // Set Text
        ui->statusLabel->setText(QString(DEFAULT_STATUS_PANEL_TEXT_TEMPLATE)
                                 .arg(ui->fileList->getFilesCount())
                                 .arg(FileUtils::getFreeSpace(ui->fileList->getCurrentDir()) / ONE_KILO)
                                 .arg(FileUtils::getTotalSpace(ui->fileList->getCurrentDir()) / ONE_KILO));
    }
}

//==============================================================================
// Update Selection Panel Text
//==============================================================================
void CustomPanel::updateSelectionPanelText()
{
    // Check UI
    if (ui && ui->selectionLabel && ui->fileList) {
        // Set Text
        ui->selectionLabel->setText(QString(DEFAULT_SELECTION_PANEL_TEXT_TEMPLATE)
                                    .arg(ui->fileList->getSelectedCount())
                                    .arg(ui->fileList->getSelectedSize()));
    }
}

//==============================================================================
// File List Focus Changed
//==============================================================================
void CustomPanel::fileListFocusChanged(const QString& aPanelName, const bool& aFocused)
{
    // Check Active State
    if (active != aFocused && panelName == aPanelName) {
        //qDebug() << "CustomPanel::fileListFocusChanged - panelName: " << panelName << " - aFocused: " << aFocused;

        // Set Active State
        active = aFocused;

        // Check UI & Header
        if (ui && ui->header) {
            // Set Header Active
            ui->header->setActive(active);
        }

        // Update
        update();
    }
}

//==============================================================================
// Header Activated Slot
//==============================================================================
void CustomPanel::headerActivated()
{
    //qDebug() << "CustomPanel::headerActivated";
    // Set Active State
    setActive(true);
}

//==============================================================================
// Header Order Changed Slot
//==============================================================================
void CustomPanel::headerOrderChanged(const int& aIndex, const bool& aReversed, const bool& aRefresh)
{
    //qDebug() << "CustomPanel::headerOrderChanged - aIndex: " << aIndex << " - aReversed: " << aReversed;
    // Set Active State
    setActive(true);
    // Check UI
    if (ui && ui->fileList) {
        // Set Order
        ui->fileList->setSorting((FileSortType)aIndex, aReversed, aRefresh);
    }
}

//==============================================================================
// Header Options Launch Slot
//==============================================================================
void CustomPanel::headerOptionsLaunch(const QPoint& aPos)
{
    //qDebug() << "CustomPanel::headerOptionsLaunch";
    // Check Header Popup
    if (headerPopup && ui && ui->header) {
        // Set Items Selected
        headerPopup->setItemSelected(1, ui->header->getItemUseage(1));
        headerPopup->setItemSelected(2, ui->header->getItemUseage(2));
        headerPopup->setItemSelected(3, ui->header->getItemUseage(3));
        headerPopup->setItemSelected(4, ui->header->getItemUseage(4));
        headerPopup->setItemSelected(5, ui->header->getItemUseage(5));

        // Launch Popup
        headerPopup->popup(aPos);
    }
}

//==============================================================================
// Current Directory Changed Slot
//==============================================================================
void CustomPanel::currentDirChanged(const QString& aCurrDir)
{
    // Update Current Dir Label Text
    updateCurrDirLabelText(aCurrDir);
    // Update Status Panel Text
    updateStatusPanelText();
}

//==============================================================================
// Current Directory Changed Slot
//==============================================================================
void CustomPanel::currentDirReadFinished()
{
    // Check UI
    if (ui && ui->fileList) {
        // Update Current Dir Label Text
        updateCurrDirLabelText(ui->fileList->getCurrentDir());
    }
    // Update Status Panel Text
    updateStatusPanelText();
}

//==============================================================================
// File List File Selected Slot
//==============================================================================
void CustomPanel::fileListFileSelected(FileItemData* aFileItemData)
{
    // Check File Item Data
    if (aFileItemData) {
        // Get File Info
        QFileInfo fileInfo = aFileItemData->getFileInfo();
        // Check File If Link
        if (fileInfo.isSymLink()) {
            qDebug() << "CustomPanel::fileListFileSelected - link";
            // Check Settings
            if (settings) {
                // Get Follow Links
                bool followLinks = settings->getValue(QString(SETTIGNS_KEY_FOLLOW_LINKS), true).toBool();
                // Check Follow Links
                if (followLinks && ui && ui->fileList) {
                    // Set Current Dir
                    ui->fileList->setCurrentDir(fileInfo.symLinkTarget(), true, true);
                }
            }
        // Check File If Dir
        } else if ((fileInfo.isDir() && !fileInfo.isBundle()) || (fileInfo.isBundle() && !execBundles) || (fileInfo.fileName() == QString(".."))) {
            // Check File Name
            if (fileInfo.fileName() == QString("..")) {
                qDebug() << "CustomPanel::fileListFileSelected - cdup";
                // Check UI
                if (ui && ui->fileList) {
                    // Go Up
                    ui->fileList->goUp();
                }
            } else {
                qDebug() << "CustomPanel::fileListFileSelected - dir";
                // Check UI
                if (ui && ui->fileList) {
                    // Reset Perv File Index
                    // Set Current Dir
                    ui->fileList->setCurrentDir(fileInfo.absoluteFilePath(), true, true);
                }
            }
        } else {
            qDebug() << "CustomPanel::fileListFileSelected - execute/open: " << fileInfo.absoluteFilePath();
            // Execute/Open Document
            bool started = QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
            // Check If Started
            if (!started) {
                qDebug() << "CustomPanel::fileListFileSelected - execute/open: " << fileInfo.absoluteFilePath() << " - NOT STARTED";
            }
        }
    }
}

//==============================================================================
// Key Press Event
//==============================================================================
void CustomPanel::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            default: {
                // qDebug() << "CustomPanel::keyPressEvent - key: " << aEvent->key();
            } break;
        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void CustomPanel::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            default: {
                // qDebug() << "CustomPanel::keyReleaseEvent - key: " << aEvent->key();
            } break;
        }
    }
}

//==============================================================================
// Resize Event
//==============================================================================
void CustomPanel::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check UI
        if (ui && ui->currDirLabel && ui->fileList) {
            // Update Current Dir Label Text
            updateCurrDirLabelText(ui->fileList->getCurrentDir());
        }
    }
}

//==============================================================================
// Up Dir Button Clicked Slot
//==============================================================================
void CustomPanel::on_upDirButton_clicked()
{
    //qDebug() << "CustomPanel::on_upDirButton_clicked - panelName: " << panelName;
    // Check UI
    if (ui && ui->fileList) {
        // Go Up
        ui->fileList->goUp();
    }
}

//==============================================================================
// Home Dir Button Clicked Slot
//==============================================================================
void CustomPanel::on_homeButton_clicked()
{
    //qDebug() << "CustomPanel::on_homeButton_clicked - panelName: " << panelName;
    // Check UI
    if (ui && ui->fileList) {
        // Go To Home
        ui->fileList->gotoHome();
    }
}

//==============================================================================
// Root Dir Button Clicked Slot
//==============================================================================
void CustomPanel::on_rootButton_clicked()
{
    //qDebug() << "CustomPanel::on_rootButton_clicked - panelName: " << panelName;
    // Check UI
    if (ui && ui->fileList) {
        // Go To Root
        ui->fileList->gotoRoot();
    }
}

//==============================================================================
// Header Popup Close Button Clicked Slot
//==============================================================================
void CustomPanel::headerPopupCloseButtonClicked()
{
    //qDebug() << "CustomPanel::headerPopupCloseButtonClicked";
    // Check UI
    if (ui && ui->header && headerPopup) {
        // Set Header Item Usage
        ui->header->setItemUseage(1, headerPopup->getItemSelected(1));
        ui->header->setItemUseage(2, headerPopup->getItemSelected(2));
        ui->header->setItemUseage(3, headerPopup->getItemSelected(3));
        ui->header->setItemUseage(4, headerPopup->getItemSelected(4));
        ui->header->setItemUseage(5, headerPopup->getItemSelected(5));
    }
}

//==============================================================================
// Destructor
//==============================================================================
CustomPanel::~CustomPanel()
{
    qDebug() << "Deleting CustomPanel...";

    // Save Settings
    saveSettings();

    // Check Header Popup
    if (headerPopup) {
        // Delete Header Popup
        delete headerPopup;
        headerPopup = NULL;
    }

    // Delete UI
    delete ui;

    // Chck Settings
    if (settings) {
        // Release Settings
        settings->release();
        settings = NULL;
    }

    qDebug() << "Deleting CustomPanel...done";
}

