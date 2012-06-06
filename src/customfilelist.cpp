
// INCLUDES

#include <QTimer>
#include <QFileIconProvider>
#include <QPainter>

#include "constants.h"
#include "customfilelist.h"
#include "ui_customfilelist.h"
#include "customheader.h"
#include "settings.h"

#include "settingswindow.h"
#include "listpopup.h"
#include "mainwindow.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
FileItemData::FileItemData(QObject* aParent)
    : ListModelItemData(aParent)
    , dirSizeScanner(NULL)
    , dirSize(-1)
{
    // ...
}

//==============================================================================
// Constructor
//==============================================================================
FileItemData::FileItemData(const QFileInfo& aInfo, QObject* aParent)
    : ListModelItemData(aParent)
    , info(aInfo)
    , dirSizeScanner(NULL)
    , dirSize(-1)
{
}

//==============================================================================
// Constructor
//==============================================================================
FileItemData::FileItemData(const QString& aFilePath, QObject* aParent)
    : ListModelItemData(aParent)
    , info(aFilePath)
    , dirSizeScanner(NULL)
    , dirSize(-1)
{
}

//==============================================================================
// Set File Info
//==============================================================================
void FileItemData::setFileInfo(const QFileInfo& aInfo, const bool& aUpdate)
{
    // Set Info
    info = aInfo;

    // Check Update Icon
    if (aUpdate) {
        // Emit Data Updated Signal
        emit dataUpdated();
    }
}

//==============================================================================
// Get File Info
//==============================================================================
QFileInfo FileItemData::getFileInfo()
{
    return info;
}

//==============================================================================
// Start Calclating Directory Size
//==============================================================================
void FileItemData::startCalculatingDirSize()
{
    // Check File Name
    if (info.fileName() == QString("..") || info.fileName() == QString("."))
        return;

    // Check Dir Size Scanner
    if (!dirSizeScanner && info.isDir()) {
        // Create Dir Size Scanner
        dirSizeScanner = new DirSizeScanner();
        // Connect Signals
        connect(dirSizeScanner, SIGNAL(sizeUpdate(qint64)), this, SLOT(dirSizeUpdated(qint64)));
        connect(dirSizeScanner, SIGNAL(opFinished()), this, SLOT(dirSizeScanfinished()));
        connect(dirSizeScanner, SIGNAL(opStopped()), this, SLOT(dirSizeScanStopped()));
    }

    // Check Info
    if (dirSizeScanner && info.isDir()) {
        // Start Dir Size Calculator
        dirSizeScanner->scanDirSize(info.absoluteFilePath());
    }
}

//==============================================================================
// Stop Calclating Directory Size
//==============================================================================
void FileItemData::stopCalculatingDirSize()
{
    // Check Info
    if (dirSizeScanner) {
        // Stop Dir Size Scanner
        dirSizeScanner->stop();
    }
}

//==============================================================================
// Get Dir Size
//==============================================================================
qint64 FileItemData::getDirSize()
{
    return dirSize;
}

//==============================================================================
// Set Selected Flag
//==============================================================================
void FileItemData::setSelected(const bool& aSelected)
{
    // Check File Name
    if (info.fileName() == QString("..") || info.fileName() == QString("."))
        return;

    // Set Selected Flag
    selected = aSelected;
    // Emit Data Updated
    emit dataUpdated();
}

//==============================================================================
// Dir Size Updated Slot
//==============================================================================
void FileItemData::dirSizeUpdated(const qint64& aSize)
{
    // Set Dir Size
    dirSize = aSize;
    // Emit Data Updated Signal
    emit dataUpdated();
}

//==============================================================================
// Directory Size Scan Stopped Slot
//==============================================================================
void FileItemData::dirSizeScanStopped()
{
    // Emit Size Scan Finished Signal
    emit sizeScanFinished(index);

    // Delete Dir Size Scanner
}

//==============================================================================
// Directory Size Scan Finished Slot
//==============================================================================
void FileItemData::dirSizeScanfinished()
{
    // Emit Size Scan Finished Signal
    emit sizeScanFinished(index);

    // Delete Dir Size Scanner
}

//==============================================================================
// Delete Dir Size Scanner
//==============================================================================
void FileItemData::deleteDirSizeScanner()
{
    // Check Dir Size Scanner
    if (dirSizeScanner) {
        // Delete Dir Size Scanner
        delete dirSizeScanner;
        dirSizeScanner = NULL;
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileItemData::~FileItemData()
{
    // Delete Dir Size Scanner
    deleteDirSizeScanner();
}











//==============================================================================
// Constructor
//==============================================================================
FileListIcon::FileListIcon(QWidget* aParent)
    : QFrame(aParent)
    , icon(QImage(0, 0, QImage::Format_ARGB32_Premultiplied))
{
    // ...
}

//==============================================================================
// Set Icon Image
//==============================================================================
void FileListIcon::setIcon(const QImage& aImage, const bool& aUpdate)
{
    // Set Icon
    icon = aImage;

    // Check Update
    if (aUpdate) {
        // Update
        update();
    }
}

//==============================================================================
// Paint Event
//==============================================================================
void FileListIcon::paintEvent(QPaintEvent* aEvent)
{
    // Check Event
    if (aEvent) {

        // Get Painter
        QPainter painter(this);

        // Set Render Hint
        //painter.setRenderHint(QPainter::Antialiasing);
        //painter.setRenderHint(QPainter::HighQualityAntialiasing);
        //painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Check Icon
        if (!icon.isNull()) {
            // Calculate Center Pos
            QPoint pos = QPoint((geometry().width() - icon.width()) / 2, (geometry().height() - icon.height()) / 2);
            // Draw Icon
            painter.drawImage(pos, icon);
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileListIcon::~FileListIcon()
{
    // ...
}





















//==============================================================================
// Constructor
//==============================================================================
FileListDelegate::FileListDelegate(ListModelItemData* aData, QWidget* aParent)
    : ItemDelegate(aData, aParent)
    , prevFileName("")
    , prevIconSize(0)
    , iconSize(DEFAULT_ICON_SIZE_MEDIUM)
    , needsIconUpdate(false)
    , settings(NULL)
    , header(NULL)
    , gridLayout(NULL)
    , nameLayout(NULL)
    , iconLabel(NULL)
    , nameLabel(NULL)
    , extLabel(NULL)
    , sizeLabel(NULL)
    , dateLabel(NULL)
    , ownrLabel(NULL)
    , permLabel(NULL)
{
    // Setup UI
    //setupUI();
}

//==============================================================================
// Setup UI
//==============================================================================
void FileListDelegate::setupUI()
{
    //qDebug() << "FileListDelegate::setupUI";

    // Check Settings
    if (!settings) {
        // Get Settings Instance
        settings = Settings::getInstance();
    }

    // Set Focus Policy
    setFocusPolicy(Qt::NoFocus);

    // Update Icon Size
    updateIconSize();

    // Check Header
    if (header) {
        // Create Icon Label
        createIconLabel();
        // Create Name Label
        createNameLabel();
        // Create Name Layout
        createNameLayout();
        // Create Extension Label
        createExtLabel();
        // Create Size Label
        createSizeLabel();
        // Create Date Label
        createDateLabel();
        // Create Owner Label
        createOwnrLabel();
        // Create Permissions Label
        createPermsLabel();
        // Update Delegate Items Size And Pos
        updateDelegateItemsSizeAndPos();
    } else {
        // Create Icon Label
        createIconLabel();
        // Create Name Label
        createNameLabel();
        // Create Name Layout
        createNameLayout();
        // Create Extension Label
        createExtLabel();
        // Create Size Label
        createSizeLabel();
        // Create Date Label
        createDateLabel();
        // Create Owner Label
        createOwnrLabel();
        // Create Permissions Label
        createPermsLabel();
        // Create Delegate layout
        createDelegateLayout();
    }

    // Update UI Values
    updateUI();
}

//==============================================================================
// Create Delegate Layout
//==============================================================================
void FileListDelegate::createDelegateLayout()
{
    // Check Layout
    if (!gridLayout) {
        // Create Grid Layout
        gridLayout = new QGridLayout();
        // Set Layout
        setLayout(gridLayout);
        // Set Spacing
        gridLayout->setSpacing(0);
        // Set Content Margins
        gridLayout->setContentsMargins(0, 0, 0, 0);
        // Add Name Layout To Grid Layout
        gridLayout->addLayout(nameLayout, 0, 0, 1, 1);
        // Add Layout
        gridLayout->addWidget(extLabel, 0, 1, 1, 1);
        // Add Layout
        gridLayout->addWidget(sizeLabel, 0, 2, 1, 1);
        // Add Layout
        gridLayout->addWidget(dateLabel, 0, 3, 1, 1);
        // Add Layout
        gridLayout->addWidget(ownrLabel, 0, 4, 1, 1);
        // Add Layout
        gridLayout->addWidget(permLabel, 0, 5, 1, 1);
    }
}

//==============================================================================
// Create Name Layout
//==============================================================================
void FileListDelegate::createNameLayout()
{
    // Check Name Layout
    if (!nameLayout) {
        // Create Name Layout
        nameLayout = new QGridLayout();
        // Set Spacing
        nameLayout->setSpacing(0);
        // Set Content Margins
        nameLayout->setContentsMargins(DEFAULT_LISTBOX_ITEM_SPACING, DEFAULT_CONTENT_MARGIN_TOP, 0, 0);
        // Add Widget To Layout
        nameLayout->addWidget(iconLabel, 0, 0, 1, 1);
        // Add Widget To Layout
        nameLayout->addWidget(nameLabel, 0, 1, 1, 1);
    }
}

//==============================================================================
// Create Icon Label
//==============================================================================
void FileListDelegate::createIconLabel()
{
    // Check Icon Label
    if (!iconLabel) {
/*
        // Create Icon Label
        iconLabel = new QLabel(this);
        // Set Text Alignment
        iconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        // Set Scaled Content
        iconLabel->setScaledContents(true);
*/
        // Create new icon Label
        iconLabel = new FileListIcon(this);
        // Set Minimum size
        iconLabel->setMinimumSize(iconSize, iconSize);
        // SET MARGIN
        iconLabel->setContentsMargins(0, DEFAULT_CONTENT_MARGIN_TOP, 0, 0);
        // Init Size Policy
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(iconLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        iconLabel->setSizePolicy(sizePolicy1);
    }

    // Reset Icon
    resetIcon();
}

//==============================================================================
// Create Name Label
//==============================================================================
void FileListDelegate::createNameLabel()
{
    // Check Name Label
    if (!nameLabel) {
        // Create Name Label
        nameLabel = new QLabel(this);
        // Set Minimum size
        nameLabel->setMinimumSize(DEFAULT_FILELIST_ITEM_HEIGHT, iconSize);
        // Set Margin
        //nameLabel->setMargin(DEFAULT_LISTBOX_ITEM_SPACING >> 1);
        nameLabel->setContentsMargins(DEFAULT_LISTBOX_ITEM_SPACING, DEFAULT_CONTENT_MARGIN_TOP, 0, 0);
        // Set Text Alignment
        nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        // Init Size Policy
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(nameLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        nameLabel->setSizePolicy(sizePolicy2);
    }
}

//==============================================================================
// Create Extension Label
//==============================================================================
void FileListDelegate::createExtLabel()
{
    // Check Extension Label
    if (!extLabel) {
        // Create Extension Label
        extLabel = new QLabel(this);
        // Set Minimum size
        extLabel->setMinimumSize(DEFAULT_FILELIST_ITEM_HEIGHT, iconSize);
        // Set Margin
        //extLabel->setMargin(DEFAULT_LISTBOX_ITEM_SPACING >> 1);
        extLabel->setContentsMargins(0, DEFAULT_CONTENT_MARGIN_TOP, DEFAULT_LISTBOX_ITEM_SPACING, 0);
        // Set Text Alignment
        extLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        // Init Size Policy
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(extLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        extLabel->setSizePolicy(sizePolicy3);
    }
}

//==============================================================================
// Create Size Label
//==============================================================================
void FileListDelegate::createSizeLabel()
{
    // Check Size Label
    if (!sizeLabel) {
        // Create Size Label
        sizeLabel = new QLabel(this);
        // Set Minimum size
        sizeLabel->setMinimumSize(0, iconSize);
        // Set Margin
        //sizeLabel->setMargin(DEFAULT_LISTBOX_ITEM_SPACING >> 1);
        sizeLabel->setContentsMargins(0, DEFAULT_CONTENT_MARGIN_TOP, DEFAULT_LISTBOX_ITEM_SPACING, 0);
        // Set Text Alignment
        sizeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        // Init Size Policy
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(sizeLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        sizeLabel->setSizePolicy(sizePolicy4);
    }
}

//==============================================================================
// Create Date Label
//==============================================================================
void FileListDelegate::createDateLabel()
{
    // Check Date Label
    if (!dateLabel) {
        // Create Date Label
        dateLabel = new QLabel(this);
        // Set Minimum size
        dateLabel->setMinimumSize(0, iconSize);
        // Set Margin
        //dateLabel->setMargin(DEFAULT_LISTBOX_ITEM_SPACING >> 1);
        dateLabel->setContentsMargins(0, DEFAULT_CONTENT_MARGIN_TOP, 0, 0);
        // Set Text Alignment
        dateLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        // Init Size Policy
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(dateLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        dateLabel->setSizePolicy(sizePolicy5);
    }
}

//==============================================================================
// Create Owner Label
//==============================================================================
void FileListDelegate::createOwnrLabel()
{
    // Check Owner Label
    if (!ownrLabel) {
        // Create Owner Label
        ownrLabel = new QLabel(this);
        // Set Minimum size
        ownrLabel->setMinimumSize(0, iconSize);
        // Set Margin
        //ownrLabel->setMargin(DEFAULT_LISTBOX_ITEM_SPACING >> 1);
        ownrLabel->setContentsMargins(0, DEFAULT_CONTENT_MARGIN_TOP, 0, 0);
        // Set Text Alignment
        ownrLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        // Init Size Policy
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(ownrLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        ownrLabel->setSizePolicy(sizePolicy6);
    }
}

//==============================================================================
// Create Permissions Label
//==============================================================================
void FileListDelegate::createPermsLabel()
{
    // Check Permissions Label
    if (!permLabel) {
        // Create Permissions Label
        permLabel = new QLabel(this);
        // Set Minimum size
        permLabel->setMinimumSize(0, iconSize);
        // Set Margin
        //permLabel->setMargin(DEFAULT_LISTBOX_ITEM_SPACING >> 1);
        permLabel->setContentsMargins(0, DEFAULT_CONTENT_MARGIN_TOP, 0, 0);
        // Set Text Alignment
        permLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        // Init Size Policy
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(permLabel->sizePolicy().hasHeightForWidth());
        // Set Size Policy
        permLabel->setSizePolicy(sizePolicy7);
    }
}

//==============================================================================
// Set Icon
//==============================================================================
void FileListDelegate::setIcon(const QImage& aImage, const bool& aUpdate)
{
    // Check Pixmap
    if (!aImage.isNull()) {
        // Set Icon
        icon = aImage;
        // Icon Label
        if (iconLabel && aUpdate) {
            // Set Pixmap
            //iconLabel->setPixmap(QPixmap::fromImage(icon));
            iconLabel->setIcon(icon);
        }
        // Reset Needs Icon Update
        needsIconUpdate = false;
    } else {
        qDebug() << "FileListDelegate::setIcon - NULL ICON!";
    }
}

//==============================================================================
// Clear UI
//==============================================================================
void FileListDelegate::clearUI()
{
    // Check Grid Layout
    if (gridLayout) {
        // Remove Name Layout
        gridLayout->removeItem(nameLayout);
        // Remove Widgets
        gridLayout->removeWidget(extLabel);
        gridLayout->removeWidget(sizeLabel);
        gridLayout->removeWidget(dateLabel);
        gridLayout->removeWidget(ownrLabel);
        gridLayout->removeWidget(permLabel);
    }
}

//==============================================================================
// Reset Icon
//==============================================================================
void FileListDelegate::resetIcon(const bool& aUpdate)
{
    // Switch Icon Size - Reset Pixmap
    switch (iconSize) {
        case DEFAULT_ICON_SIZE_SMALL:   icon = QImage(QString(":defaultIcon16x16")); break;
        default:
        case DEFAULT_ICON_SIZE_MEDIUM:  icon = QImage(QString(":defaultIcon32x32")); break;
        case DEFAULT_ICON_SIZE_LARGE:   icon = QImage(QString(":defaultIcon64x64")); break;
    }

    //qDebug() << "FileListDelegate::resetIcon - iconSize: " << iconSize;

    // Check Icon Label
    if (iconLabel) {
        // Set Icon
        iconLabel->setIcon(icon, aUpdate);
    }

    // Set Needs Icon Update
    needsIconUpdate = true;
}

//==============================================================================
// Update UI
//==============================================================================
void FileListDelegate::updateUI()
{
    //qDebug() << "FileListDelegate::updateUI";

    // Update Icon Size
    updateIconSize();

    // Check Data
    if (data) {
        //qDebug() << "FileListDelegate::updateUI";
        // Get File Item Data
        FileItemData* fileItemData = reinterpret_cast<FileItemData*>(data);
        // Parent List Got Focus
        bool parentListGotFocus = reinterpret_cast<FileListBox*>(parentList)->gotFocus;
        // Check Settings
        if (settings) {
            // Set Group
            //settings->setGroup(QString(SETTINGS_GROUP_APPEARANCE));
            // Init Foreground Color Key
            QString fgColorKey = QString(SETTINGS_KEY_NORMAL_TEXT_COLOR);
            // Init Background Color Key
            QString bgColorKey = QString(SETTINGS_KEY_NORMAL_BACKGROUND_COLOR);
            // Check If Selected
            if (data->selected) {
                // Check If Current
                if (data->current && parentListGotFocus) {
                    // Update Foreground Color Key
                    fgColorKey = QString(SETTINGS_KEY_CURRENT_SELECTED_TEXT_COLOR);
                    // Update Background Color Key
                    bgColorKey = QString(SETTINGS_KEY_CURRENT_SELECTED_BACKGROUND_COLOR);
                } else {
                    // Update Foreground Color Key
                    fgColorKey = QString(SETTINGS_KEY_SELECTED_TEXT_COLOR);
                    // Update Background Color Key
                    bgColorKey = QString(SETTINGS_KEY_SELECTED_BACKGROUND_COLOR);
                }
            // Cec kIf Current And Not A Link
            } else if (data->current && parentListGotFocus) {
                // Check If Link
                if (fileItemData->info.isSymLink()) {
                    // Update Foreground Color Key
                    fgColorKey = QString(SETTINGS_KEY_LINK_TEXT_COLOR);
                } else {
                    // Update Foreground Color Key
                    fgColorKey = QString(SETTINGS_KEY_CURRENT_TEXT_COLOR);
                }
                // Update Background Color Key
                bgColorKey = QString(SETTINGS_KEY_CURRENT_BACKGROUND_COLOR);
                // Check If Link
            } else if (fileItemData->info.isSymLink()) {
                // Update Foreground Color Key
                fgColorKey = QString(SETTINGS_KEY_LINK_TEXT_COLOR);
                // Update Background Color Key
                bgColorKey = QString(SETTINGS_KEY_LINK_BACKGROUND_COLOR);
            }

            // Update Style Sheet
            settings->updateWidgetStyleSheet(this, fgColorKey, bgColorKey,
                                             fileItemData->info.fileName() != QString("..") &&
                                             fileItemData->info.isHidden() &&
                                             !fileItemData->info.isSymLink());
        }

        // Check File Name Label
        if (nameLabel) {
            // Check If Link
            if (fileItemData->info.isSymLink()) {
                // Set Name Label Text
                nameLabel->setText(QString("%1 -> %2").arg(fileItemData->info.fileName()).arg(fileItemData->info.symLinkTarget()));
            } else if (fileItemData->info.isBundle() && fileItemData->info.fileName() != QString("..")) {
                // Set Name Label Text
                nameLabel->setText(fileItemData->info.baseName());
            } else if (fileItemData->info.isDir()) {
                // Set Name Label Text
                nameLabel->setText(fileItemData->info.fileName());
            } else {
                // Check Complete Base Name
                if (fileItemData->info.completeBaseName().isEmpty()) {
                    // Set Name Label Text
                    nameLabel->setText(fileItemData->info.fileName());
                } else {
                    // Set Name Label Text
                    nameLabel->setText(fileItemData->info.completeBaseName());
                }
            }
        }

        // Check Extension Label
        if (extLabel) {
            // Check File Info
            if (!fileItemData->info.isDir() &&
                !fileItemData->info.isBundle() &&
                !fileItemData->info.isSymLink() &&
                !fileItemData->info.baseName().isEmpty()) {
                // Set Exension Text
                extLabel->setText(fileItemData->info.suffix());
            } else {
                // Clear Extension Label
                extLabel->setText(QString(""));
            }
        }

        // Check Size Label
        if (sizeLabel) {
            // Get Dir Size
            qint64 dirSize = fileItemData->getDirSize();
            // Check Dir Size
            if (dirSize == -1) {
                // Set Size Label Text
                sizeLabel->setText(FileUtils::formatFileSize(fileItemData->info));
            } else {
                // Set Size Label Text
                sizeLabel->setText(FileUtils::formatSize(dirSize));
            }
            // Check Text
            if (sizeLabel->text() == QString(DEFAULT_FILE_SIZE_DIR) ||
                sizeLabel->text() == QString(DEFAULT_FILE_SIZE_LINK) ||
                sizeLabel->text() == QString(DEFAULT_FILE_SIZE_BUNDLE) ||
                sizeLabel->text() == QString(DEFAULT_FILE_SIZE_APP)) {
                // Set Alignment
                sizeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            } else {
                // Set Alignment
                sizeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            }
        }

        // Check Date Label
        if (dateLabel) {
            // Set Date Label Text
            dateLabel->setText(fileItemData->info.lastModified().toString(QString(DEFAULT_FILELIST_DELEGATE_DATE_FORMAT_STRING)));
        }

        // Check Owner Label
        if (ownrLabel) {
            // Set Owner Label Text
            ownrLabel->setText(fileItemData->info.owner());
        }

        // Check Permissions Label
        if (permLabel) {
            // Set Owner Label Text
            permLabel->setText(FileUtils::formatPermissions(fileItemData->info));
        }
    }

    // Update Delegate Size And Pos
    updateDelegateItemsSizeAndPos();
}

//==============================================================================
// Duplicate
//==============================================================================
ItemDelegate* FileListDelegate::duplicate(ListModelItemData* aData)
{
    // Create New Delegate
    FileListDelegate* newDelegate = new FileListDelegate(aData, parentWidget());
    // Set Delegate Header
    newDelegate->setDelegateHeader(header);
    // Return New Delegate
    return newDelegate;
}

//==============================================================================
// Set Data
//==============================================================================
void FileListDelegate::setData(ListModelItemData* aData, const bool& aUpdate)
{
    // Validate Icon
    validateIcon(reinterpret_cast<FileItemData*>(aData));

    // Check If Needs Icon Update
    if (needsIconUpdate) {
        // Reset Icon
        resetIcon();
    }

    // Call Parent's Set Data
    ItemDelegate::setData(aData, aUpdate);
}

//==============================================================================
// Get File Info
//==============================================================================
QFileInfo FileListDelegate::getFileInfo()
{
    // Check Data
    if (data) {
        // Get File Item Data
        FileItemData* fileItemData = reinterpret_cast<FileItemData*>(data);

        return fileItemData->getFileInfo();
    }

    return QFileInfo();
}

//==============================================================================
// Get Item Size
//==============================================================================
QSize FileListDelegate::getItemSize()
{
    // Check Parent Widget
    if (parentWidget()) {
        // Check Orientation
        if (orientation == LBOVertical) {
            // Init Preferred Size
            //int preferredSize = parentWidget()->width() - (DEFAULT_LISTBOX_ITEM_SPACING * 2);
            int preferredSize = parentWidget()->width() - 2;
            // Return Size
            return QSize(preferredSize, iconSize + 2);
        } else {
            // Init Preferred Size
            int preferredSize = parentWidget()->height() - (DEFAULT_LISTBOX_ITEM_SPACING * 2);
            // Return Size
            return QSize(iconSize + 2, preferredSize);
        }
    }

    return QSize(0, 0);
}

//==============================================================================
// Get Needs Icon Update State
//==============================================================================
bool FileListDelegate::getNeedsIconUpdate()
{
    return needsIconUpdate;
}

//==============================================================================
// Set Icon Size
//==============================================================================
void FileListDelegate::setIconSize(const int& aIconSize)
{
    // Check Icon Size
    if (iconSize != aIconSize) {
        // Set Icn Size
        iconSize = aIconSize;

        // ...
    }
}

//==============================================================================
// Get Icon Size
//==============================================================================
int FileListDelegate::getIconSize()
{
    return iconSize;
}

//==============================================================================
// Update Icon Size
//==============================================================================
void FileListDelegate::updateIconSize()
{
    // Check Settings
    if (settings) {
        // Set Settings Group
        settings->setGroup(SETTINGS_GROUP_APPEARANCE);
        // Get Icon Size Settings
        int isSetting = settings->getValue(SETTINGS_KEY_ICONSIZE).toInt();
        // Switch Icon Size Setting
        switch (isSetting) {
            case 0: iconSize = DEFAULT_ICON_SIZE_SMALL; break;
            default:
            case 1: iconSize = DEFAULT_ICON_SIZE_MEDIUM; break;
            case 2: iconSize = DEFAULT_ICON_SIZE_LARGE; break;
        }

        // Set Height
        setMinimumHeight(iconSize);

        // Validate Icon
        validateIcon(reinterpret_cast<FileItemData*>(data));
    }
}

//==============================================================================
// Validate Icon
//==============================================================================
void FileListDelegate::validateIcon(FileItemData* aFileData)
{
    // Check Data
    if (aFileData) {
        // Get File Data
        //FileItemData* fileData = reinterpret_cast<FileItemData*>(data);
        // Check Prev File Name and Prev Icon Size
        if (prevFileName != aFileData->info.absoluteFilePath() || prevIconSize != iconSize) {
            // Set Prev File Name
            prevFileName = aFileData->info.absoluteFilePath();
            // Set Previous Icon Size
            prevIconSize = iconSize;
            // Set Needs Icon Update
            needsIconUpdate = true;
        } else {
            //qDebug() << "FileListDelegate::validateIcon - fileName: " << aFileData->info.fileName() << " - Same File, Same Size";
        }
    } else {
        qDebug() << "FileListDelegate::validateIcon - NO ITEM DATA!";
    }

}

//==============================================================================
// Set Delegate Header
//==============================================================================
void FileListDelegate::setDelegateHeader(CustomHeader* aHeader)
{
    // Set Header
    header = aHeader;

    // Check Header
    if (header) {
        // Connect Signals
        connect(header, SIGNAL(itemSizeChanged(int,int)), this, SLOT(headerSizeChanged(int,int)));
        //connect(header, SIGNAL(itemPosChanged(int,int)), this, SLOT(headerSizeChanged(int,int)));
    }

    // Clear UI
    clearUI();

    // Setup UI
    setupUI();
}

//==============================================================================
// Update Delegate Items Size And Pos
//==============================================================================
void FileListDelegate::updateDelegateItemsSizeAndPos()
{
    // Check Delegate Header
    if (header) {
        //qDebug() << "FileListDelegate::updateDelegateItemsSizeAndPos - iconSize: " << iconSize;
        // Check Name Layout
        if (nameLayout) {
            // Set Geometry
            nameLayout->setGeometry(QRect(0, 0, header->getItemWidth(0)-2, iconSize));
            // Check Ext Label
            if (extLabel) {
                // Set Geometry
                extLabel->setGeometry(QRect(nameLayout->geometry().right()+1, 0, header->getItemWidth(1), iconSize));
                // Check Ext Label
                if (sizeLabel) {
                    // Set Geometry
                    sizeLabel->setGeometry(QRect(extLabel->geometry().right()+1, 0, header->getItemWidth(2), iconSize));
                    // Check Ext Label
                    if (dateLabel) {
                        // Set Geometry
                        dateLabel->setGeometry(QRect(sizeLabel->geometry().right()+1, 0, header->getItemWidth(3), iconSize));
                        // Check Ext Label
                        if (ownrLabel) {
                            // Set Geometry
                            ownrLabel->setGeometry(QRect(dateLabel->geometry().right()+1, 0, header->getItemWidth(4), iconSize));
                            // Check Ext Label
                            if (permLabel) {
                                // Set Geometry
                                permLabel->setGeometry(QRect(ownrLabel->geometry().right()+1, 0, header->getItemWidth(5), iconSize));
                            }
                        }
                    }
                }
            }
        }
    }
}

//==============================================================================
// Header Item Size Changed Slot
//==============================================================================
void FileListDelegate::headerSizeChanged(const int& aIndex, const int& aWidth)
{
    Q_UNUSED(aIndex);
    Q_UNUSED(aWidth);

    // Update Delegate UI Items Size And Pos
    updateDelegateItemsSizeAndPos();
}

//==============================================================================
// Paint Event
//==============================================================================
void FileListDelegate::paintEvent(QPaintEvent* aEvent)
{
    QFrame::paintEvent(aEvent);

    // Validate Icon
    //validateIcon();

    // Check Event
    if (aEvent) {
        //qDebug() << "FileListDelegate::paintEvent";
/*
        // Get Painter
        QPainter painter(this);

        // Set Render Hint
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Check Icon Label
        if (iconLabel) {
            //qDebug() << "FileListDelegate::paintEvent";

            // Draw Pixmap
            //painter.drawPixmap(0, 0, (*iconLabel->pixmap()));

            painter.setPen(Qt::red);
            painter.setBrush(Qt::yellow);

            painter.drawRoundedRect(iconLabel->geometry(), 4, 4);

        }
*/
/*
        // Set Pen & Brush
        if (hasFocus()) {
            // Set Pen
            painter.setPen(Qt::red);    // TODO: get focused color from settings
        } else {
            // Set Pen
            painter.setPen(Qt::white);  // TODO: get normal color from settings
        }

        // Check If Current Item
        if (data && data->current) {
            // Set Brush
            painter.setBrush(Qt::darkGray);             // TODO: get current bg color from settings
        } else {
            // set Brush
            painter.setBrush(QColor(139, 158, 130));    // TODO: get normal bg color from settings
        }

        // Draw Background
        painter.drawRoundedRect(rect().adjusted(0,0,-1,-1), DEFAULT_FILELIST_ITEM_RADIUS, DEFAULT_FILELIST_ITEM_RADIUS);
*/
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileListDelegate::~FileListDelegate()
{
    // Check Header
    if (header) {
        // Disconnect Signals
        //disconnect(header, SIGNAL(itemSizeChanged(int,int)), this, SLOT(headerSizeChanged(int,int)));
        //dicconnect(header, SIGNAL(itemPosChanged(int,int)), this, SLOT(headerSizeChanged(int,int)));
    }

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        settings = NULL;
    }
}













//==============================================================================
// Constructor
//==============================================================================
FileListBox::FileListBox(QWidget* aParent)
    : ListBox(LBOVertical, aParent)
    , header(NULL)
    , gotFocus(false)
    , iconScanner(NULL)
    , prevFCI(-1)
    , prevCC(-1)
{
    // ...

    qDebug() << "Creating FileListBox...done";
}

//==============================================================================
// Set Items Header
//==============================================================================
void FileListBox::setHeader(CustomHeader* aHeader)
{
    //qDebug() << "FileListBox::setHeader";
    // Set Header
    header = aHeader;
    // Check Delegate
    if (delegate) {
        // Get File List Delegate
        FileListDelegate* fileListDelegate = reinterpret_cast<FileListDelegate*>(delegate);
        // Set Delegate HeEader
        fileListDelegate->setDelegateHeader(header);
    }
    // Rebuild Cache
    rebuildCache();
}

//==============================================================================
// Get Items Header
//==============================================================================
CustomHeader* FileListBox::getHeader()
{
    return header;
}

//==============================================================================
// Set Panel Name
//==============================================================================
void FileListBox::setPanelName(const QString& aPanelName)
{
    //qDebug() << "FileListBox::setPanelName - aPanelName: " << aPanelName;
    // Set Panel Name
    panelName = aPanelName;

    // ...
}

//==============================================================================
// Find/get Item Index By Name
//==============================================================================
int FileListBox::findItemByName(const QString& aFileName)
{
    // Get File List Item Count
    int fiCount = model ? model->data.count() : 0;
    // Go Thru Items
    for (int i=0; i<fiCount; i++) {
        // Get Item Data
        FileItemData* itemData = reinterpret_cast<FileItemData*>(model->data[i]);
        // Check item Data
        if (itemData && itemData->getFileInfo().fileName() == aFileName) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Get Selected Files Size
//==============================================================================
qint64 FileListBox::getSelectedSize()
{
    // Get File List Item Count
    int fiCount = model ? model->data.count() : 0;
    // Init Selected Files Size
    qint64 sfSize = 0;

    // Go Thru Items
    for (int i=0; i<fiCount; i++) {
        // Get Item Data
        FileItemData* itemData = reinterpret_cast<FileItemData*>(model->data[i]);
        // Check item Data
        if (itemData && !itemData->getFileInfo().isDir()) {
            // Inc Selected File Size
            sfSize += itemData->getFileInfo().size();
        } else if (itemData && itemData->getFileInfo().isDir()) {
            // Zero Size
        }
    }

    return sfSize;
}

//==============================================================================
// Get Item Data By Index
//==============================================================================
FileItemData* FileListBox::getItemData(const int& aIndex)
{
    return reinterpret_cast<FileItemData*>(ListBox::getData(aIndex));
}

//==============================================================================
// Set Item Icon Size
//==============================================================================
void FileListBox::setItemIconSize(const int& aIconSize, const bool& aRefresh)
{
    // Check Icon Size
    if (iconSize != aIconSize) {
        //qDebug() << "FileListBox::setItemIconSize - aIconSize: " << aIconSize;
        // Set Icon Size
        iconSize = aIconSize;
        // Check Delegate
        if (delegate) {
            // Get File List Delegate
            FileListDelegate* fileListDelegate = reinterpret_cast<FileListDelegate*>(delegate);
            // Set Icon Size
            fileListDelegate->setIconSize(iconSize);
            // Set Delegate Size
            //setDelegateSize( , aRefresh);
            // Emit Icon Size Changed Signel
            emit itemIconSizeChanged(aRefresh);
        }
    }
}

//==============================================================================
// Get Item Icon Size
//==============================================================================
int FileListBox::getItemIconSize()
{
    return iconSize;
}

//==============================================================================
// Update Delegate Icon Size By Settings
//==============================================================================
void FileListBox::updateDelegateIconSize(const bool& aRefresh)
{
    // Check Delegate
    if (delegate) {
        // Get File List Delegate
        FileListDelegate* fileListDelegate = reinterpret_cast<FileListDelegate*>(delegate);
        // Update Icon Size
        fileListDelegate->updateIconSize();
        // Set Icon Size
        setItemIconSize(fileListDelegate->getIconSize(), aRefresh);

        // Check Orientation
        if (orientation == LBOVertical) {
            // Set Delegate Size
            setDelegateSize(fileListDelegate->getItemSize().height(), aRefresh);
        } else {
            // Set Delegate Size
            setDelegateSize(fileListDelegate->getItemSize().width(), aRefresh);
        }
    }
}

//==============================================================================
// Update Icons
//==============================================================================
void FileListBox::updateIcons()
{
    // Get Cached Item Count
    int ciCount = cache.count();

    // Go Through Cache
    for (int i=0; i<ciCount; i++) {
        // Get Cached Item Delegate
        FileListDelegate* itemDelegate = reinterpret_cast<FileListDelegate*>(cache[i]);
        // Check Delegate
        if (itemDelegate) {
            // Validate Icon;
            itemDelegate->validateIcon(reinterpret_cast<FileItemData*>(itemDelegate->data));
        }
    }
}

//==============================================================================
// Select All Items
//==============================================================================
void FileListBox::selectAll()
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Go Thru Items
    for (int i=0; i<dCount; i++) {
        // Get File Item Data
        FileItemData* fileItemData = reinterpret_cast<FileItemData*>(model->data[i]);
        // Check File Item Data
        if (fileItemData && fileItemData->getFileInfo().fileName() != QString("..")) {
            // Toggle Selected
            model->data[i]->toggleSelected();
        }
    }
}

//==============================================================================
// Get File List Box Item
//==============================================================================
const FileListDelegate* FileListBox::getItem(const int& aIndex) const
{
    // Get Data Count
    int dCount = model ? model->data.count() : 0;
    // Check Index
    if (aIndex >= 0 && aIndex < dCount) {
        // Get Cached Item Count
        int ciCount = cache.count();

        // Check First And Last Cached Items
        if (ciCount > 0 && cache[0]->data && aIndex >= cache[0]->data->index && cache[ciCount-1]->data && aIndex <= cache[ciCount-1]->data->index) {
            // Go Through Cache
            for (int i=0; i<ciCount; i++) {
                // Get Cached Item Delegate
                FileListDelegate* itemDelegate = reinterpret_cast<FileListDelegate*>(cache[i]);
                // Check Delegate
                if (itemDelegate && itemDelegate->data && itemDelegate->data->index == aIndex) {
                    return (const FileListDelegate*)itemDelegate;
                }
            }
        }
    }

    return NULL;
}

//==============================================================================
// Connect Delegate Signals
//==============================================================================
void FileListBox::connectDelegateSignals(ItemDelegate* aItem)
{
    ListBox::connectDelegateSignals(aItem);
    // Check Item
    if (aItem && header) {
        // Get File List Item Delegate
        FileListDelegate* fileItem = reinterpret_cast<FileListDelegate*>(aItem);
        // Connect Signals
        connect(header, SIGNAL(itemSizeChanged(int,int)), fileItem, SLOT(headerSizeChanged(int,int)));
    }
}

//==============================================================================
// Disconnect Delegate Signals
//==============================================================================
void FileListBox::disconnectDelegateSignals(ItemDelegate* aItem)
{
    ListBox::disconnectDelegateSignals(aItem);
    // Check Item
    if (aItem && header) {
        // Get File List Item Delegate
        FileListDelegate* fileItem = reinterpret_cast<FileListDelegate*>(aItem);
        // Connect Signals
        disconnect(header, SIGNAL(itemSizeChanged(int,int)), fileItem, SLOT(headerSizeChanged(int,int)));
    }
}

//==============================================================================
// Updating Cached Items Data
//==============================================================================
void FileListBox::updateCachedItemsData(const bool& aUpdateUI)
{
    ListBox::updateCachedItemsData(aUpdateUI);

    // Check Icon Scanner
    if (!iconScanner) {
        // Create Icon Scanner
        iconScanner = new IconScanner();
    }

    // Check Icon Scanner
    if (iconScanner && (prevFCI != firstCachedItem || prevCC != cache.count())) {
        // Set Previous First Cached Item
        prevFCI = firstCachedItem;
        // Set Previous Cache Count
        prevCC = cache.count();

        //qDebug() << "==================================================================";
        //qDebug() << "FileListBox::updateCachedItemsData - Starting Icon Scanner...";

        // Start Scan Icons
        //iconScanner->scanIcons(&cache, firstVisibleItem, (reinterpret_cast<FileListDelegate*>(delegate))->iconSize);

        //QTimer::singleShot(10, this, SLOT(getItemIcons()));
        //getItemIcons();
        // Start Icon Update Timer
        startIconUpdateTimer();

        //qDebug() << "==================================================================";
    }

}

//==============================================================================
// Clear Delegate Cache
//==============================================================================
void FileListBox::clearCache()
{
    // Check Icon Scanner
    if (iconScanner) {
        // Stop Icon Scanner
        iconScanner->stop();
    }

    ListBox::clearCache();

    // Reset Previous First Cached Item
    prevFCI = -1;
    // Reset Previous Cache Count
    prevCC = -1;
}

//==============================================================================
// Handle Key Event
//==============================================================================
void FileListBox::handleKeyEvent(const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    // Switch Key
    switch (aKey) {

        // ...

        default:
            ListBox::handleKeyEvent(aKey, aModifiers);
        break;
    }
}

//==============================================================================
// Start Icon Update Timer
//==============================================================================
void FileListBox::startIconUpdateTimer()
{
    // Stop Timer First
    stopIconUpdateTimer();

    // Check Timer ID
    if (iconUpdateTimerID == -1) {
        // Start Timer
        iconUpdateTimerID = startTimer(DEFAULT_FILE_LIST_BOX_ICON_UPDATE_TIMEOUT);
    }
}

//==============================================================================
// Stop Icon Update Timer
//==============================================================================
void FileListBox::stopIconUpdateTimer()
{
    // Check Timer ID
    if (iconUpdateTimerID != -1) {
        // Kill Timer
        killTimer(iconUpdateTimerID);
        // Reset Timer ID
        iconUpdateTimerID = -1;
    }
}

//==============================================================================
// Get Item Icons
//==============================================================================
void FileListBox::getItemIcons()
{
    // Scan Icons
    iconScanner->scanIcons(&cache, firstVisibleItem, (reinterpret_cast<FileListDelegate*>(delegate))->iconSize);
}

//==============================================================================
// Focus In Event
//==============================================================================
void FileListBox::focusInEvent(QFocusEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Focus
        if (!gotFocus) {
            //qDebug() << "FileListBox::focusInEvent - panelName: " << panelName;
            // Set Got Focus
            gotFocus = true;
            // Trigger Current Item Update
            triggerItemUpdate(currentIndex);
            // Grab Keyboard
            //grabKeyboard();
            // Emit List Box Focused Signal
            emit listBoxFocusChanged(panelName, gotFocus);
        }
    }
}

//==============================================================================
// Focus Out Event
//==============================================================================
void FileListBox::focusOutEvent(QFocusEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Focus
        if (gotFocus) {
            //qDebug() << "FileListBox::focusOutEvent - panelName: " << panelName;
            // Reset Got Focus
            gotFocus = false;
            // Trigger Current Item Update
            triggerItemUpdate(currentIndex);
            // Release Keyboard
            //releaseKeyboard();
            // Emit List Box Focused Signal
            emit listBoxFocusChanged(panelName, gotFocus);
        }
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void FileListBox::timerEvent(QTimerEvent* aEvent)
{
    ListBox::timerEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Check Timer ID
        if (aEvent->timerId() == iconUpdateTimerID) {
            // Stop Icon Update Timer
            stopIconUpdateTimer();
            // Get Item Icons
            getItemIcons();
        }
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void FileListBox::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent) {
        // Check Pressed State
        if (!pressed) {
            //qDebug() << "mousePressEvent::mouseReleaseEvent";
            // Set Pressed State
            pressed = true;

            // Check If has Focus
            if (!hasFocus())
                // Set Focus
                setFocus();

            // ...
        }
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void FileListBox::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event & Data
    if (aEvent) {
        // Check Pressed
        if (pressed) {
            //qDebug() << "mousePressEvent::mouseReleaseEvent";
            // Reset Pressed State
            pressed = false;

            // Check Button
            if (aEvent->button() == Qt::RightButton) {
                // Emit List Box Item Options Signal
                emit itemOptions(-1, aEvent->globalPos());
            }
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileListBox::~FileListBox()
{
    // Check Icon Scanner
    if (iconScanner) {
        // Delete Icon Scanner
        delete iconScanner;
        iconScanner = NULL;
    }

    qDebug() << "Deleting FileListBox...done";
}






















//==============================================================================
// Constructor
//==============================================================================
IconScanner::IconScanner(QObject* aParent)
    : FileUtilThreadBase(aParent)
    , iconSize(DEFAULT_ICON_SIZE_MEDIUM)
    , delegateCache(NULL)
    , firstVisibleIndex(-1)
{
    qDebug() << "Creating IconScanner...done";
}

//==============================================================================
// Scan Icons
//==============================================================================
void IconScanner::scanIcons(const CacheType* aCache, const int& aFVI, const int& aIconSize)
{
    //qDebug() << "IconScanner::scanIcons - aIconSize: " << aIconSize;
    // Lock
    mutex.lock();

    // Set Cache
    delegateCache = (CacheType*)aCache;

    // Set First Visible Index
    firstVisibleIndex = aFVI;

    // Set Icon Size
    iconSize = aIconSize;

    // Unlock
    mutex.unlock();

    // Check Delegate Cache
    if (delegateCache && delegateCache->count() > 0) {
        // Start New Operation
        startOperation();
    }
}

//==============================================================================
// Update Item Icon
//==============================================================================
void IconScanner::updateItemIcon(const int& aIndex)
{
    DEFAULT_THREAD_ABORT_CHECK;
    // Check Cache
    if (delegateCache) {
        // Get Cahced Item
        FileListDelegate* fileListDelegate = reinterpret_cast<FileListDelegate*>((*delegateCache)[aIndex]);
        DEFAULT_THREAD_ABORT_CHECK;
        // Check If Needs Icon
        if (fileListDelegate && fileListDelegate->getNeedsIconUpdate()) {
            //qDebug() << "IconScanner::updateItemIcon - fileName: " << reinterpret_cast<FileItemData*>(fileListDelegate->getData())->getFileInfo().fileName();
            //qDebug() << "IconScanner::updateItemIcon - aIndex: " << aIndex;
            DEFAULT_THREAD_ABORT_CHECK;
            // Sleep
            msleep(1);
            DEFAULT_THREAD_ABORT_CHECK;
            // Get File Item Data
            FileItemData* fileItemData = reinterpret_cast<FileItemData*>(fileListDelegate->getData());
            DEFAULT_THREAD_ABORT_CHECK;
            // Lock Mutex
            mutex.lock();
            // Set Icon
            fileListDelegate->setIcon(FileUtils::getFileIconImage(fileItemData->getFileInfo().absoluteFilePath(), iconSize, iconSize));
            // Unlock Mutex
            mutex.unlock();
        }
    }
}

//==============================================================================
// Do Operation
//==============================================================================
void IconScanner::doOperation()
{
    DEFAULT_THREAD_ABORT_CHECK;

    // Get Cached Items Count
    int ciCount = delegateCache ? delegateCache->count() : 0;

    //qDebug() << "IconScanner::doOperation - iconSize: " << iconSize << " - ciCount: " << ciCount;

    DEFAULT_THREAD_ABORT_CHECK;

    // Init Start Index
    int startIndex = 0;

    DEFAULT_THREAD_ABORT_CHECK;

    // Go To First Visible Index
    while (startIndex < ciCount && delegateCache->at(startIndex)->getData()->index != firstVisibleIndex) {
        DEFAULT_THREAD_ABORT_CHECK;
        // Inc Start Index
        startIndex++;
    }

    //qDebug() << "IconScanner::doOperation - fvi: " << firstVisibleIndex << " - si: " << startIndex;

    DEFAULT_THREAD_ABORT_CHECK;

    // Go Thru Delegates - Start From First Visible
    for (int i=startIndex; i<ciCount; i++) {
        DEFAULT_THREAD_ABORT_CHECK;
        // Update Item Icon
        updateItemIcon(i);
        DEFAULT_THREAD_ABORT_CHECK;
    }

    DEFAULT_THREAD_ABORT_CHECK;

    // Do The Rest Of The Items
    for (int i=startIndex-1; i>=0; i--) {
        DEFAULT_THREAD_ABORT_CHECK;
        // Update Item Icon
        updateItemIcon(i);
        DEFAULT_THREAD_ABORT_CHECK;
    }

    DEFAULT_THREAD_ABORT_CHECK;

    //qDebug() << "IconScanner::doOperation...done";
}

//==============================================================================
// Destructor
//==============================================================================
IconScanner::~IconScanner()
{
    qDebug() << "Deleting IconScanner...done";
}






















//==============================================================================
// Constructor
//==============================================================================
CustomFilelist::CustomFilelist(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::CustomFilelist)
    , settings(NULL)
    , panelName("")
    , forceRefresh(false)
    , currentDirPath("")
    , prevDirName("")
    , prevFileName("")
    , prevFileIndex(-1)
    , dirReader(NULL)
    , fileUtils(NULL)
    , sortOrder(EFSTName)
    , reverseOrder(false)
    , showHidden(false)
    , needToClear(false)
    , dirItemsSizeScanActive(false)
    , sizeScanCurrent(-1)
    , fileSystemWatcher(NULL)
    , fswTimerID(-1)
    , popup(NULL)
{
    qDebug() << "Creating CustomFilelist...";

    // Setup UI
    ui->setupUi(this);

    // Check UI File List
    if (ui->fileListBox) {
        // Create New File List Delegate
        ui->fileListBox->setDelegate(new FileListDelegate(NULL, ui->fileListBox));
        // Connect Signals
        connect(ui->fileListBox, SIGNAL(listBoxKeyPressed(int,Qt::KeyboardModifiers)), this, SLOT(listBoxKeyPressed(int,Qt::KeyboardModifiers)));
        connect(ui->fileListBox, SIGNAL(listBoxKeyReleased(int,Qt::KeyboardModifiers)), this, SLOT(listBoxKeyReleased(int,Qt::KeyboardModifiers)));
        connect(ui->fileListBox, SIGNAL(itemSelected(int)), this, SLOT(listBoxItemSelected(int)));
        connect(ui->fileListBox, SIGNAL(itemOptions(int,QPoint)), this, SLOT(listBoxItemOptions(int,QPoint)));
        connect(ui->fileListBox, SIGNAL(listBoxFocusChanged(QString,bool)), this, SIGNAL(listBoxFocusChanged(QString,bool)));
        connect(ui->fileListBox, SIGNAL(itemIconSizeChanged(bool)), this, SLOT(listBoxItemIconSizeChanged(bool)));
    }

    // Create File Utils
    fileUtils = new FileUtils();

    // Create Directory Reader
    dirReader = new DirReader();

    // Connect Signals
    connect(dirReader, SIGNAL(entryFound(QString)), this, SLOT(dirReaderEntryFound(QString)));
    connect(dirReader, SIGNAL(opStarted()), this, SLOT(dirReaderStarted()));
    connect(dirReader, SIGNAL(opStopped()), this, SLOT(dirReaderStopped()));
    connect(dirReader, SIGNAL(opFinished()), this, SLOT(dirReaderFinished()));
    connect(dirReader, SIGNAL(finished()), this, SLOT(dirReaderThreadFinished()));

    // Get Settings Instance
    settings = Settings::getInstance();

    // Create File System Watcher
    fileSystemWatcher = new QFileSystemWatcher();

    // Connect Signals
    connect(fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(fsDirectoryChanged(QString)));

    qDebug() << "Creating CustomFilelist...done";
}

//==============================================================================
// Set Panel Name
//==============================================================================
void CustomFilelist::setPanelName(const QString& aPanelName)
{
    // Set Panel Name
    panelName = aPanelName;
    qDebug() << "CustomFilelist::setPanelName - panelName: " << panelName;
    // Check UI
    if (ui && ui->fileListBox) {
        // Set Panel Name
        ui->fileListBox->setPanelName(panelName);
    }
    // Set Force Refresh
    //forceRefresh = true;
}

//==============================================================================
// Set Current Directory
//==============================================================================
void CustomFilelist::setCurrentDir(const QString& aDirectory, const bool& aRefresh, const bool& aResetIndex)
{
    // Check Current Dir
    if (QFile::exists(aDirectory)) {
        // Check Current Dir
        if (currentDirPath != QDir::cleanPath(aDirectory) || forceRefresh) {
            qDebug() << "CustomFilelist::setCurrentDir - panelName: " << panelName << " - aDirectory: " << aDirectory;
            // Set Current Dir Path
            currentDirPath = QDir::cleanPath(aDirectory);
            // Set Current Dir
            currentDir.setCurrent(currentDirPath);
            // Emit Current dir Changed
            emit currentDirChanged(currentDirPath);
            // Check Refresh
            if (aRefresh || forceRefresh) {
                // Reload
                reload(aResetIndex);
            }
            // Reset Force Refresh
            forceRefresh = false;
        } else {
            //qDebug() << "CustomFilelist::setCurrentDir - panelName: " << panelName << " - currentDirPath: " << currentDir.currentPath();
        }
    } else {
        qDebug() << "CustomFilelist::setCurrentDir - DIR DOESN'T EXIST";
    }
}

//==============================================================================
// Get Current Directory
//==============================================================================
QString CustomFilelist::getCurrentDir()
{
    return QDir::cleanPath(currentDirPath);
}

//==============================================================================
// Get Item Count
//==============================================================================
int CustomFilelist::getItemCount()
{
    // Check UI
    if (ui && ui->fileListBox) {
        return ui->fileListBox->count();
    }

    return 0;
}

//==============================================================================
// Get Current Directory's Files Count
//==============================================================================
int CustomFilelist::getFilesCount()
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Get First Item Data
        FileItemData* itemData = ui->fileListBox->getItemData(0);
        // Check Data
        if (itemData && itemData->getFileInfo().fileName() == QString("..")) {
            return ui->fileListBox->count() - 1;
        }
        // Get File List Count
        return ui->fileListBox->count();
    }

    return 0;
}

//==============================================================================
// Get Current Directory's Selected Files Count
//==============================================================================
int CustomFilelist::getSelectedCount()
{
    // Check UI
    if (ui && ui->fileListBox) {
        return ui->fileListBox->selectedCount();
    }

    return 0;
}

//==============================================================================
// Get Item Data By Index
//==============================================================================
FileItemData* CustomFilelist::getItemData(const int& aIndex)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Return Item Data
        return ui->fileListBox->getItemData(aIndex);
    }

    return NULL;
}

//==============================================================================
// Get Current Index
//==============================================================================
int CustomFilelist::getCurrentIndex()
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Return Current Index
        return ui->fileListBox->getCurrentIndex();
    }

    return -1;
}

//==============================================================================
// Get Current Directory's Selected Files Size
//==============================================================================
qint64 CustomFilelist::getSelectedSize()
{
    // Check UI
    if (ui && ui->fileListBox) {
        return ui->fileListBox->getSelectedSize();
    }

    return 0;
}

//==============================================================================
// Get File/Directory Size
//==============================================================================
qint64 CustomFilelist::getFileSize(const QString& aFilePath)
{
    // Check File
    if (QFile::exists(aFilePath)) {
        // Init File Info
        QFileInfo fileInfo(aFilePath);

        // Check If File Is A Dir
        if (fileInfo.isDir()) {
            // Return 0 Size
            return 0;
        }
        // Return File Size
        return fileInfo.size();
    }
    // Return 0 Size
    return 0;
}

//==============================================================================
// Set Sorting
//==============================================================================
void CustomFilelist::setSorting(const FileSortType& aSorting, const bool& aReverse, const bool& aRefresh)
{
    // Stop All Items Size Scan
    stopAllItemsSizeScan();

    // Check New Sorting
    if (sortOrder != aSorting || reverseOrder != aReverse) {
        qDebug() << "CustomFilelist::setSorting - aSorting: " << aSorting << " - aReverse: " << aReverse;
        // Set Sort Order
        sortOrder = aSorting;
        // Set Reverse Order
        reverseOrder = aReverse;
        // Check Refresh
        if (aRefresh) {
            // Reload
            reload();
        }
    }
}

//==============================================================================
// Get Sorting Mode
//==============================================================================
FileSortType CustomFilelist::getSorting()
{
    return sortOrder;
}

//==============================================================================
// Get Sorting Direction
//==============================================================================
bool CustomFilelist::getSortDirection()
{
    return reverseOrder;
}

//==============================================================================
// Set Show Hidden Files
//==============================================================================
void CustomFilelist::setShowHiddenFiles(const bool& aShowHidden, const bool& aRefresh)
{
    // Stop All Items Size Scan
    stopAllItemsSizeScan();

    // Check Show Hidden Files
    if (showHidden != aShowHidden) {
        // Set Show Hidden
        showHidden = aShowHidden;
/*
        // Set Need To Clear Flag
        needToClear = true;
        // Check Dir Reader
        if (dirReader && aRefresh) {
            // Start Direcory Reader
            dirReader->readDir(currentDir.currentPath(), sortOrder, reverseOrder, showHidden, nameFilters);
        }
*/
        // Check Refresh
        if (aRefresh) {
            // Reload
            reload();
        }
    }
}

//==============================================================================
// Get Show Hidden Files Mode
//==============================================================================
bool CustomFilelist::getShowHiddenFiles()
{
    return showHidden;
}

//==============================================================================
// Set Name Filters
//==============================================================================
void CustomFilelist::setNameFilters(const QStringList& aNameFilters, const bool& aRefresh)
{
    // Set Name Filters
    nameFilters = aNameFilters;
/*
    // Set Need To Clear Flag
    needToClear = true;
    // Check Dir Reader
    if (dirReader && aRefresh) {
        // Start Direcory Reader
        dirReader->readDir(currentDir.currentPath(), sortOrder, reverseOrder, showHidden, nameFilters);
    }
*/
    // Check Refresh
    if (aRefresh) {
        // Reload
        reload();
    }
}

//==============================================================================
// Set File List Item Header
//==============================================================================
void CustomFilelist::setFileListHeader(CustomHeader* aHeader)
{
    // Check UI
    if (ui && ui->fileListBox) {
        //qDebug() << "CustomFilelist::setFileListHeader";
        // Set Header
        ui->fileListBox->setHeader(aHeader);
    }
}

//==============================================================================
// Set Active
//==============================================================================
void CustomFilelist::setActive(const bool& aActive)
{
    // Chek UI
    if (ui && ui->fileListBox) {
        //qDebug() << "CustomFilelist::setActive - panelName: " << panelName << " - aActive: " << aActive;
        // Check Active
        if (aActive) {
            // Set Focus
            ui->fileListBox->setFocus();
        } else {
            // Clear Focus
            ui->fileListBox->clearFocus();
        }
    }
}

//==============================================================================
// Set Icon Size
//==============================================================================
void CustomFilelist::setItemIconSize(const int& aIconSize, const bool& aRefresh)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Set Icon Size
        ui->fileListBox->setItemIconSize(aIconSize, aRefresh);
    }
}

//==============================================================================
// Get Icon Size
//==============================================================================
int CustomFilelist::getItemIconSize()
{
    // Check UI
    if (ui && ui->fileListBox) {
        return ui->fileListBox->getItemIconSize();
    }

    return 0;
}

//==============================================================================
// Set List Box Item/Delegate Spacing
//==============================================================================
void CustomFilelist::setItemSpacing(const int& aSpacing, const bool& aRefresh)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Set Spacing
        ui->fileListBox->setSpacing(aSpacing, aRefresh);
    }
}

//==============================================================================
// Get List Box Item/Delegate Spacing
//==============================================================================
int CustomFilelist::getItemSpacing()
{
    // Check UI
    if (ui && ui->fileListBox) {
        return ui->fileListBox->getSpacing();
    }

    return 0;
}

//==============================================================================
// Update Delegate Icon Size
//==============================================================================
void CustomFilelist::updateDelegateIconSize(const bool& aRefresh)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Update Delegate Icon Size
        ui->fileListBox->updateDelegateIconSize(aRefresh);
    }
}

//==============================================================================
// Reload Dir
//==============================================================================
void CustomFilelist::reload(const bool& aResetIndex)
{
    // Stop All Items Size Scan
    stopAllItemsSizeScan();
    // Set Needs Clear
    needToClear = true;

    // Check File system Watcher
    if (fileSystemWatcher) {
        // Add Path
        fileSystemWatcher->removePaths(fileSystemWatcher->files() + fileSystemWatcher->directories());
    }

    // Check UI, List Box And Reset Index
    if (ui && ui->fileListBox && !aResetIndex) {
        // Set Last File Index
        prevFileIndex = ui->fileListBox->getCurrentIndex();
    }

    // Check Dir Reader
    if (dirReader) {
        qDebug() << "CustomFilelist::reload";
        // Start Direcory Reader
        dirReader->readDir(currentDirPath, sortOrder, reverseOrder, showHidden, nameFilters);
    }
}

//==============================================================================
// Reload Dir
//==============================================================================
void CustomFilelist::setBackgroundColor(const int& aBgColor)
{
    // Check UI
    if (ui && ui->fileListBox) {
        //qDebug() << "CustomFilelist::setBackgroundColor - aBgColor: " << aBgColor;
        // Check Color
        if (aBgColor != -1) {
            // Set Style Sheet
            ui->fileListBox->setStyleSheet(QString(STYLE_SHEET_BACKGROUND_COLOR_TEMPLATE).arg(QColor(aBgColor).name()));
        } else {
            // Reset Style Sheet
            ui->fileListBox->setStyleSheet(QString(""));
        }
    }
}

//==============================================================================
// Get File List Box
//==============================================================================
const FileListBox* CustomFilelist::listbox()
{
    // Check UI
    if (ui) {
        return (const FileListBox*)(ui->fileListBox);
    }

    return NULL;
}

//==============================================================================
// Go To User Home Directory
//==============================================================================
void CustomFilelist::gotoHome()
{
    //qDebug() << "CustomFilelist::gotoHome";
    // Set Current Dir
    setCurrentDir(QDir::homePath(), true, true);
}

//==============================================================================
// Go To Root Directory
//==============================================================================
void CustomFilelist::gotoRoot()
{
    //qDebug() << "CustomFilelist::gotoRoot";
    // Set Current Dir
    setCurrentDir(QString("/"), true, true);
}

//==============================================================================
// Go Up to Parent Directory
//==============================================================================
void CustomFilelist::goUp()
{
    //qDebug() << "CustomFilelist::goUp - currentDirPath: " << currentDirPath;
    // Get Prev Dir Name
    prevDirName = FileUtils::getDirName(getCurrentDir());
    // Set Current Dir
    setCurrentDir(FileUtils::getParentDirPath(getCurrentDir()), true);
}

//==============================================================================
// Go Forward
//==============================================================================
void CustomFilelist::goForward()
{
    // Check UI
    if (ui && ui->fileListBox) {
        //qDebug() << "CustomFilelist::goForward - currentDirPath: " << currentDirPath;
        // Get Current Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(ui->fileListBox->getCurrentIndex());
        // Check File Item Data
        if (fileItemData && fileItemData->getFileInfo().isDir() && fileItemData->getFileInfo().fileName() != QString("..")) {
            // Set Current Dir
            setCurrentDir(fileItemData->getFileInfo().absoluteFilePath(), true, true);
        }
    }
}

//==============================================================================
// Go Back
//==============================================================================
void CustomFilelist::goBack()
{
    //qDebug() << "CustomFilelist::goBack - currentDirPath: " << currentDirPath;
    // Go Up - For Now
    goUp();
}


//==============================================================================
// Go To Drive
//==============================================================================
void CustomFilelist::gotoDrive(const int& aIndex)
{
#ifdef Q_OS_WIN

    qDebug() << "CustomFilelist::gotoDrive - aIndex: " << aIndex;

    // ...


#else // Q_OS_WIN

    Q_UNUSED(aIndex);

#endif // Q_OS_WIN
}

//==============================================================================
// Go To Next Item
//==============================================================================
void CustomFilelist::goNextItem(const bool& aSelection)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Go To Next Item
        ui->fileListBox->gotoNextItem(aSelection);
    }
}

//==============================================================================
// Go To Prev Item
//==============================================================================
void CustomFilelist::goPrevItem(const bool& aSelection)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Go To Prev Item
        ui->fileListBox->gotoPrevItem(aSelection);
    }
}

//==============================================================================
// Go To Index
//==============================================================================
void CustomFilelist::gotoIndex(const int& aIndex, const bool& aSelection)
{
    // Check UI
    if (ui && ui->fileListBox) {
        // Go To Index
        ui->fileListBox->gotoIndex(aIndex, aSelection);
    }
}
/*
//==============================================================================
// Make Directory
//==============================================================================
void CustomFilelist::makeDir(const QString& aDirPath)
{
    // Check Name
    if (!aDirPath.isEmpty()) {
        // Check If Dir Exists
        if (!QFile::exists(aDirPath)) {
            qDebug() << "CustomFilelist::makeDir - aDirPath: " << aDirPath;

            // Create Dir
            if (fileUtils->createDir(aDirPath)) {

                // Update Dir list

                // ...

            } else {
                qDebug() << "CustomFilelist::makeDir - ERROR CREATING DIR";
            }
        } else {
            qDebug() << "CustomFilelist::makeDir - DIR ALREADY EXISTS!";
        }
    } else {
        qDebug() << "CustomFilelist::makeDir - EMPTY DIR NAME!";
    }
}

//==============================================================================
// Copy Files
//==============================================================================
void CustomFilelist::copyFiles(const QString& aPath)
{
    // Check File Path
    if (QFile::exists(aPath)) {
        qDebug() << "CustomFilelist::copyFiles - aPath: " << aPath;

        // ...

    } else {
        qDebug() << "CustomFilelist::copyFiles - TARGET PATH DOESN'T EXIST";
    }
}

//==============================================================================
// Move Files
//==============================================================================
void CustomFilelist::moveFiles(const QString& aPath)
{
    // Check File Path
    if (QFile::exists(aPath)) {
        qDebug() << "CustomFilelist::moveFiles - aPath: " << aPath;

        // ...

    } else {
        qDebug() << "CustomFilelist::moveFiles - TARGET PATH DOESN'T EXIST";
    }
}

//==============================================================================
// Delete Files
//==============================================================================
void CustomFilelist::deleteFiles()
{
    qDebug() << "CustomFilelist::deleteFiles";

    // ...
}

//==============================================================================
// Search Files
//==============================================================================
void CustomFilelist::searchFiles(const QString& aSearchTerm, const QString& aContent, const bool& aCase, const bool& aWhole)
{
    // Check Settings
    if (settings) {
        // Set Group
        settings->setGroup(SETTINGS_GROUP_SEARCH);
        // Set Cse Sensitive Value
        settings->setValue(SETTINGS_KEY_SEARCH_CASE_SENSITIVE, aCase);
        // Set Whole Word Value
        settings->setValue(SETTINGS_KEY_SEARCH_WHOLE_WORD, aWhole);

        // Add Search Term

        // Add Content Search Term
    }

    // Check Term And Content
    if (aSearchTerm.isEmpty() && aContent.isEmpty()) {
        qDebug() << "CustomFilelist::dirReaderStarted - EMPTY SEARCH";
        return;
    }

    qDebug() << "CustomFilelist::dirReaderStarted - aSearchTerm: " << aSearchTerm << " - aContent: " << aContent;

    // ...
}

//==============================================================================
// Scan All Dir Sizes Files
//==============================================================================
void CustomFilelist::scanAllDirsSize()
{
    qDebug() << "CustomFilelist::scanAllDirsSize";

    // ...
}
*/
//==============================================================================
// Clear
//==============================================================================
void CustomFilelist::clear()
{
    // Init Mutex
    QMutex mutex;
    // Lock Mutex
    mutex.lock();
    // Check File system Watcher
    if (fileSystemWatcher) {
        // Add Path
        fileSystemWatcher->removePaths(fileSystemWatcher->files() + fileSystemWatcher->directories());
    }
    //qDebug() << "CustomFilelist::clear";
    // Check List Box
    if (ui && ui->fileListBox) {
        // Clear File List
        ui->fileListBox->clear();
    }
    //qDebug() << "CustomFilelist::clear...done";
    // Unlock Mutex
    mutex.unlock();
}

//==============================================================================
// Start File Item Size Scan
//==============================================================================
void CustomFilelist::startFileItemSizeScan(const int& aIndex)
{
    // Check List Box
    if (ui && ui->fileListBox) {
        // Get File Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(aIndex);
        // Check File Item Data
        if (fileItemData) {
            // Start Dir Size Scan
            fileItemData->startCalculatingDirSize();
        }
    }
}

//==============================================================================
// Stop File Item Size Scan
//==============================================================================
void CustomFilelist::stopFileItemSizeScan(const int& aIndex)
{
    // Check List Box
    if (ui && ui->fileListBox) {
        // Get File Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(aIndex);
        // Check File Item Data
        if (fileItemData) {
            // Stop Dir Size Scan
            fileItemData->stopCalculatingDirSize();
        }
    }
}

//==============================================================================
// Start All Items Size Scan
//==============================================================================
void CustomFilelist::startAllItemsSizeScan()
{
    // Check List Box
    if (ui && ui->fileListBox && !dirItemsSizeScanActive) {
        //qDebug() << "CustomFilelist::startAllItemsSizeScan";
        // Set Size Scan Current Item Index
        sizeScanCurrent = 0;

        // Get File Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(sizeScanCurrent);

        // Go To The First Dir Item
        while (sizeScanCurrent < ui->fileListBox->count() && fileItemData && (!fileItemData->getFileInfo().isDir() || fileItemData->getFileInfo().fileName() == QString(".."))) {
            // Inc Size Scan Current Index
            sizeScanCurrent++;
            // Get File Item Data
            fileItemData = ui->fileListBox->getItemData(sizeScanCurrent);
        }

        // Check File Item Data
        if (fileItemData && fileItemData->getFileInfo().isDir() && fileItemData->getFileInfo().fileName() != QString("..")) {
            // Set Dir Items Size Scan Active
            dirItemsSizeScanActive = true;
            // Connect Signals
            connect(fileItemData, SIGNAL(sizeScanFinished(int)), this, SLOT(itemSizeScanFinished(int)));
            // Start Dir Size Scan
            fileItemData->startCalculatingDirSize();
        }
    }
}

//==============================================================================
// Stop All Items Size Scan
//==============================================================================
void CustomFilelist::stopAllItemsSizeScan()
{
    //qDebug() << "CustomFilelist::stopAllItemsSizeScan";
    // Reset Dir Items Size Scan Active
    dirItemsSizeScanActive = false;
    // Check Size Scan Currnt Index
    if (sizeScanCurrent >= 0 && ui && ui->fileListBox) {
        // Get File Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(sizeScanCurrent);
        // Check File Item Data
        if (fileItemData) {
            // Stop Dir Size Scan
            fileItemData->stopCalculatingDirSize();
        }
        // Reset Size Scan Current item Index
        sizeScanCurrent = -1;
    }
}

//==============================================================================
// Start File System Watcher Filter Timer
//==============================================================================
void CustomFilelist::startFSWFilterTimer()
{
    // Stop File System Watcher Filter Timer
    stopFSWFilterTimer();

    // Check Timer ID
    if (fswTimerID == -1) {
        qDebug() << "CustomFilelist::startFSWFilterTimer";
        // Start Timer
        fswTimerID = startTimer(DEFAULT_FILESYSTEM_WATCHER_FILTER_TIMER_INTERVAL);
    }
}

//==============================================================================
// Stop File System Watcher Filter Timer
//==============================================================================
void CustomFilelist::stopFSWFilterTimer()
{
    // Check Timer ID
    if (fswTimerID != -1) {
        qDebug() << "CustomFilelist::stopFSWFilterTimer";
        // Kill Timer
        killTimer(fswTimerID);
        // Reset Timer ID
        fswTimerID = -1;
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void CustomFilelist::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        qDebug() << "CustomFilelist::timerEvent";
        // Check Event Timer ID
        if (aEvent->timerId() == fswTimerID) {
            // Stop File System Watcher Filter Timer
            stopFSWFilterTimer();
/*
            // Check UI
            if (ui && ui->fileListBox) {
                // Check Prev Dir Name
                if (prevDirName.isEmpty()) {
                    // Set Prev File Index
                    prevFileIndex = ui->fileListBox->getCurrentIndex();
                }
            }
*/
            // Reload
            reload();
        }
    }
}

//==============================================================================
// Dir Reader Entry Found Slot
//==============================================================================
void CustomFilelist::dirReaderEntryFound(const QString& aFilePath)
{
    //qDebug() << "CustomFilelist::dirReaderEntryFound - aFilePath: " << aFilePath;

    // Check Need To Clear Flag
    if (needToClear) {
        //qDebug() << "CustomFilelist::dirReaderEntryFound - needToClear: " << needToClear;
        // Reset Need To Clear Flag
        needToClear = false;
        // Clear
        clear();
    }

    // Check UI
    if (ui && ui->fileListBox) {
        // Create New Item Data
        FileItemData* newData = new FileItemData(aFilePath);
        // Append Item To File List
        ui->fileListBox->appendItem(newData);

        // Check Prev Dir Name
        if (!prevDirName.isEmpty()) {
            // Get Prev Dir Index By Name
            int pdIndex = ui->fileListBox->findItemByName(prevDirName);
            // Check Prev Dir Index
            if (pdIndex >= 0) {
                // Clear Prev Dir Name
                prevDirName = QString("");
                // Go To Index
                ui->fileListBox->gotoIndex(pdIndex, false, false);
                // Reset Prev File Index
                prevFileIndex = -1;
            }
        } else if (!prevFileName.isEmpty()) {
            // Get Prev File Index By Name
            int pfIndex = ui->fileListBox->findItemByName(prevFileName);
            // Check Prev File Index
            if (pfIndex >= 0) {
                // Clear Prev File Name
                prevFileName = QString("");
                // Go To Index
                ui->fileListBox->gotoIndex(pfIndex, false, false);
                // Reset Prev File Index
                prevFileIndex = -1;
            }
        } else if (prevFileIndex != -1) {
            // Check Prev File Index
            if (prevFileIndex >= 0 && ui->fileListBox->count() > prevFileIndex) {
                // Go To Index
                ui->fileListBox->gotoIndex(prevFileIndex, false, false);
                // Clear Previous File Index
                prevFileIndex = -1;
            }
        }
    }
}

//==============================================================================
// Dir Reader Started Slot
//==============================================================================
void CustomFilelist::dirReaderStarted()
{
    //qDebug() << "CustomFilelist::dirReaderStarted";

    // ...
}

//==============================================================================
// Dir Reader Stopped Slot
//==============================================================================
void CustomFilelist::dirReaderStopped()
{
    //qDebug() << "CustomFilelist::dirReaderStopped";

    // ...
}

//==============================================================================
// Dir Reader Finished Slot
//==============================================================================
void CustomFilelist::dirReaderFinished()
{
    //qDebug() << "CustomFilelist::dirReaderFinished";

    // Stop Dir Reader
    if (dirReader) {
        // Terminate
        dirReader->stop();
    }

    // Check UI
    if (ui && ui->fileListBox) {
        // Repos
        ui->fileListBox->adjustScrollPos();
        // Update Icons
        ui->fileListBox->updateIcons();
    }

    // Check File system Watcher
    if (fileSystemWatcher) {
        // Add Path
        fileSystemWatcher->addPath(currentDirPath);
    }

    // Emit Curren Dir Read Finished
    emit currentDirReadFinished();
}

//==============================================================================
// Dir Reader Thread Finished Slot
//==============================================================================
void CustomFilelist::dirReaderThreadFinished()
{

    //qDebug() << "CustomFilelist::dirReaderThreadFinished";
/*
    // Check File List Box
    if (ui && ui->fileListBox) {
        // Update Icons
        ui->fileListBox->updateIcons();
    }
*/
}

//==============================================================================
// List Box Key Pressed Slot
//==============================================================================
void CustomFilelist::listBoxKeyPressed(const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    Q_UNUSED(aModifiers);

    // Switch Key
    switch (aKey) {
        default: {
            //qDebug() << "CustomFilelist::listBoxKeyPressed - aKey: " << QString("0x%1").arg(aKey, 8, 16, QChar('0'));
        } break;
    }

    // Emit File List Key Pressed Signal
    emit fileListKeyPressed(panelName, aKey, aModifiers);
}

//==============================================================================
// List Box Key Pressed Slot
//==============================================================================
void CustomFilelist::listBoxKeyReleased(const int& aKey, const Qt::KeyboardModifiers& aModifiers)
{
    // Get Shift Modifier
    bool shiftPressed = aModifiers & Qt::ShiftModifier;
    bool altPressed = aModifiers & Qt::AltModifier;
    bool controlPressed = aModifiers & Qt::ControlModifier;
    bool metaPressed = aModifiers & Qt::MetaModifier;

    // Switch Key
    switch (aKey) {
        case Qt::Key_Left: {
            //qDebug() << "CustomFilelist::listBoxKeyReleased - Backspace";
            goBack();
        } break;

        case Qt::Key_Backspace: {
            //qDebug() << "CustomFilelist::listBoxKeyReleased - Backspace";
            // Go Up
            goUp();
        } break;

        case Qt::Key_Right: {
            //qDebug() << "CustomFilelist::listBoxKeyReleased - Key_Right";
            // Go Forward
            goForward();
        } break;

        case Qt::Key_Insert: {
            //qDebug() << "CustomFilelist::listBoxKeyReleased - Insert";
            // Go Next Item With Selection
            goNextItem(true);
        } break;

        case Qt::Key_Space: {
            //qDebug() << "CustomFilelist::listBoxKeyReleased - Space";
            // Check UI
            if (ui && ui->fileListBox) {
                // Check If Item Selected
                if (!ui->fileListBox->getItemSelected(ui->fileListBox->getCurrentIndex())) {
                    // Start File Item Size Scan
                    startFileItemSizeScan(ui->fileListBox->getCurrentIndex());
                } else {
                    // Stop File Item Seize Scan
                    stopFileItemSizeScan(ui->fileListBox->getCurrentIndex());
                }

                // Toggle File Item Selection
                ui->fileListBox->toggleItemSelected(ui->fileListBox->getCurrentIndex());
            }
        } break;

        case Qt::Key_R: {
#ifdef Q_OS_MAC
            // Check Modifiers
            if (metaPressed) {
#else // Q_OS_MAC
            // Check Modifiers
            if (controlPressed) {
#endif // Q_OS_MAC
/*
                // Check UI
                if (ui && ui->fileListBox) {
                    //qDebug() << "CustomFilelist::listBoxKeyReleased - Ctrl + R";
                    // Set Prev File Index
                    prevFileIndex = ui->fileListBox->getCurrentIndex();
                }
*/
                // Reload
                reload();
            }
        } break;

        case Qt::Key_A: {
#ifdef Q_OS_MAC
            // Check Modifiers
            if (metaPressed) {
#else // Q_OS_MAC
            // Check Modifiers
            if (controlPressed) {
#endif // Q_OS_MAC
                // Check UI
                if (ui && ui->fileListBox) {
                    //qDebug() << "CustomFilelist::listBoxKeyReleased - Ctrl + A";
                    ui->fileListBox->selectAll();
                }
            }
        } break;

        case Qt::Key_Escape: {
            // Check If Items Size Scan Active
            if (dirItemsSizeScanActive) {
                // Stop All Items Size Scan
                stopAllItemsSizeScan();
            }
        } break;

        case Qt::Key_Enter:
        case Qt::Key_Return: {
            // Check Modifier Keys
            if (shiftPressed && altPressed && !metaPressed && !controlPressed) {
                qDebug() << "CustomFilelist::listBoxKeyReleased - Alt + Shift + Enter - Scan All Dirs For Size...";

                // Start All Dir Items  Size Scan
                startAllItemsSizeScan();
            }
        } break;


        default: {
            //qDebug() << "CustomFilelist::listBoxKeyReleased - aKey: " << QString("0x%1").arg(aKey, 8, 16, QChar('0'));
        } break;
    }

    // Emit File List Key Released Signal
    emit fileListKeyReleased(panelName, aKey, aModifiers);
}

//==============================================================================
// List Box Item Selected Slot
//==============================================================================
void CustomFilelist::listBoxItemSelected(const int& aIndex)
{
    qDebug() << "CustomFilelist::listBoxItemSelected - aIndex: " << aIndex;

    // Check UI
    if (ui && ui->fileListBox) {
        // Emit File Selected Signal
        emit fileSelected(ui->fileListBox->getItemData(aIndex));
    }
}

//==============================================================================
// List Box Item Options Slot
//==============================================================================
void CustomFilelist::listBoxItemOptions(const int& aIndex, const QPoint& aPos)
{
    // Check List Box
    if (ui && ui->fileListBox) {
        qDebug() << "CustomFilelist::listBoxItemOptions - aIndex: " << aIndex << " - aPos: " << aPos;
        // Check Popup
        if (!popup) {
            // Create Popup
            popup = new ListPopup();
        }

        // Check Popup
        if (popup) {
            // Clear popup
            popup->clear();
        }

        // Get File Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(aIndex);
        // Check File Item Data
        if (fileItemData) {
            // Check Popup
            if (popup) {
                // Add Popup Items
                popup->addAction(fileItemData->getFileInfo().fileName());

                popup->addSeparator();

                popup->addAction(tr("Copy"));
                popup->addAction(tr("Cut"));
                popup->addAction(tr("Paste"));

                popup->addSeparator();

                popup->addAction(tr("Pack"));

                popup->addSeparator();

                popup->addAction(tr("Compare"));

                popup->addSeparator();

                popup->addAction(tr("Delete"));

            }
        } else {
            // Check Popup
            if (popup) {

                popup->addAction(tr("New Directory"));

                popup->addSeparator();

                popup->addAction(tr("Paste"));
            }
        }

        // Check Popup
        if (popup) {
            // Launch Popup
            popup->exec(aPos);
        }
    }
}

//==============================================================================
// Item Icon Size Changed Slot
//==============================================================================
void CustomFilelist::listBoxItemIconSizeChanged(const bool& aRefresh)
{
    qDebug() << "CustomFilelist::listBoxItemIconSizeChanged";
    // Set Previous Index
    prevFileIndex = getCurrentIndex();
    // Check Refresh
    if (aRefresh) {
        // Reload
        reload();
    }
}

//==============================================================================
// Item Size Scan Finished Slot
//==============================================================================
void CustomFilelist::itemSizeScanFinished(const int& aIndex)
{
    // Check UI
    if (ui && ui->fileListBox) {
        qDebug() << "CustomFilelist::itemSizeScanFinished - aIndex: " << aIndex;

        // Get File Item Data
        FileItemData* fileItemData = ui->fileListBox->getItemData(sizeScanCurrent);

        // Check Item Data
        if (fileItemData) {
            // Disconnect Signal
            disconnect(fileItemData, SIGNAL(sizeScanFinished(int)), this, SLOT(itemSizeScanFinished(int)));
        }

        // Check Dir Items Size Scan Active
        if (dirItemsSizeScanActive) {
            // Set Size Scan Current Index
            sizeScanCurrent = aIndex + 1;

            // Get File Item Data
            fileItemData = ui->fileListBox->getItemData(sizeScanCurrent);

            // Go To The First Dir Item
            while (sizeScanCurrent < ui->fileListBox->count() && fileItemData && (!fileItemData->getFileInfo().isDir() || fileItemData->getFileInfo().fileName() == QString(".."))) {
                // Inc Size Scan Current Index
                sizeScanCurrent++;
                // Get File Item Data
                fileItemData = ui->fileListBox->getItemData(sizeScanCurrent);
            }

            // Check File Item Data
            if (fileItemData && fileItemData->getFileInfo().isDir() && fileItemData->getFileInfo().fileName() != QString("..")) {
                // Set Dir Items Size Scan Active
                //dirItemsSizeScanActive = true;
                // Connect Signals
                connect(fileItemData, SIGNAL(sizeScanFinished(int)), this, SLOT(itemSizeScanFinished(int)));
                // Start Dir Size Scan
                fileItemData->startCalculatingDirSize();
            }
        }
    }
}

//==============================================================================
// File System Directory Changed Slot
//==============================================================================
void CustomFilelist::fsDirectoryChanged(const QString& aPath)
{
    qDebug() << "CustomFilelist::fsDirectoryChanged - aPath: " << aPath;

    // Check Current Dir
    if (getCurrentDir() == aPath) {
        // Start File System Watcher Filter Timer
        startFSWFilterTimer();
    }
}

//==============================================================================
// Feed Search Result To The List Box
//==============================================================================
void CustomFilelist::feedSearchResult()
{
    qDebug() << "CustomFilelist::feedSearchResult";

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
CustomFilelist::~CustomFilelist()
{
    qDebug() << "Deleting CustomFilelist...";

    // Delete UI
    delete ui;

    // Check Directory Reader
    if (dirReader) {
        // Delete Dir Reader
        delete dirReader;
        dirReader = NULL;
    }

    // Check File Utils
    if (fileUtils) {
        // Delete File Utils
        delete fileUtils;
        fileUtils = NULL;
    }

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        settings = NULL;
    }

    // Check Popup
    if (popup) {
        // Delete Popup
        delete popup;
        popup = NULL;
    }

    qDebug() << "Deleting CustomFilelist...done";
}


