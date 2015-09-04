#include <QColorDialog>
#include <QFontDialog>
#include <QColor>
#include <QFont>
#include <QDialogButtonBox>
#include <QQmlContext>
#include <QDebug>

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "colorpickerbutton.h"
#include "filelistimageprovider.h"
#include "settingscontroller.h"
#include "utility.h"
#include "constants.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
PreferencesDialog::PreferencesDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PreferencesDialog)
    , settings(SettingsController::getInstance())
    , dirty(false)

    , showFunctionKeys(DEFAULT_SETTINGS_SHOW_FUNCTION_KEYS)
    , showDirHotKeys(DEFAULT_SETTINGS_SHOW_DIRECTORIY_HOT_KEYS)
    , showDriveButtons(DEFAULT_SETTINGS_SHOW_DRIVE_BUTTONS)
    , closeWhenFinished(DEFAULT_SETTINGS_CLOSE_WHEN_FINISHED)

    , selectDirectories(DEFAULT_SETTINGS_SELECT_DIRECTORIES)
    , showHiddenFiles(DEFAULT_SETTINGS_SHOW_HIDDEN_FILES)
    , showDirsFirst(DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST)
    , caseSensitiveSort(DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING)

    , useDefaultIcons(DEAFULT_SETTINGS_USE_DEFAULT_ICONS)
    , showFullSizes(DEFAULT_SETTINGS_SHOW_FULL_FILE_SIZES)
    , copyHiddenFiles(DEFAULT_SETTINGS_COPY_HIDDEN_FILES)
    , followLinks(DEFAULT_SETTINGS_FOLLOW_SYMBOLIC_LINKS)

    , textColor(DEFAULT_SETTINGS_TEXT_COLOR)
    , textBGColor(DEFAULT_SETTINGS_TEXT_BG_COLOR)
    , currentColor(DEFAULT_SETTINGS_CURRENT_COLOR)
    , currentBGColor(DEFAULT_SETTINGS_CURRENT_BG_COLOR)
    , selectedColor(DEFAULT_SETTINGS_SELECTED_COLOR)
    , selectedBGColor(DEFAULT_SETTINGS_SELECTED_BG_COLOR)
    , currentSelectedColor(DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR)
    , currentSelectedBGColor(DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR)
    , hiddenColor(DEFAULT_SETTINGS_HIDDEN_COLOR)
    , hiddenBGColor(DEFAULT_SETTINGS_HIDDEN_BG_COLOR)
    , linkColor(DEFAULT_SETTINGS_LINK_COLOR)
    , linkBGColor(DEFAULT_SETTINGS_LINK_BG_COLOR)
    , archiveColor(DEFAULT_SETTINGS_ARCHIVE_COLOR)
    , archiveBGColor(DEFAULT_SETTINGS_ARCHIVE_BG_COLOR)

    , fontName(DEFAULT_SETTINGS_FONT_NAME)
    , fontSize(DEFAULT_SETTINGS_FONT_SIZE)
    , fontBold(DEFAULT_SETTINGS_FONT_BOLD)
    , fontItalic(DEFAULT_SETTINGS_FONT_ITALIC)

    , thumbWidth(DEFAULT_SETTINGS_THUMB_WIDTH)
    , thumbHeight(DEFAULT_SETTINGS_THUMB_HEIGHT)

    , gridThumbWidth(DEFAULT_SETTINGS_GRID_THUMB_WIDTH)
    , gridThumbHeight(DEFAULT_SETTINGS_GRID_THUMB_HEIGHT)

    , terminalPath(DEFAULT_SETTINGS_TERMINAL_PATH_MAC_OSX)
    , viewerPath(DEFAULT_SETTINGS_VIEWER_PATH_MAC_OSX)
    , editorPath(DEFAULT_SETTINGS_EDITOR_PATH_MAC_OSX)
    , comparePath(DEFAULT_SETTINGS_COMPARE_PATH)
    , packerPath(DEFAULT_SETTINGS_PACKER_PATH)
    , unPackerPath(DEFAULT_SETTINGS_UNPACKER_PATH)

{
    qDebug() << "PreferencesDialog::PreferencesDialog";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void PreferencesDialog::init()
{
    qDebug() << "PreferencesDialog::init";

    // Connect Signals
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBoxButtonPressed(QAbstractButton*)));

    // Set Context Properties
    QQmlContext* ctx = ui->previewWidget->rootContext();
    // Set Context Properties - Dummy Model
    ctx->setContextProperty(DEFAULT_PREVIEW_CONTROLLER, this);

    // Get Engine
    QQmlEngine* engine = ui->previewWidget->engine();
    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_FILE_ICON_PROVIDER_ID), new FileListImageProvider());

    // Set Resize Mode
    ui->previewWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // Set Source
    ui->previewWidget->setSource(QUrl("qrc:/qml/previewMain.qml"));

    // ...
}

//==============================================================================
// Load Settings
//==============================================================================
void PreferencesDialog::loadSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    qDebug() << "PreferencesDialog::loadSettings";

    // Set Normal Text Color
    setTextColor(settings->getTextColor());
    // Set Normal Text BG Color
    setTextBGColor(settings->gettextBGColor());
    // Set Current Text Color
    setCurrentColor(settings->getCurrentColor());
    // Set Current Text BG Color
    setCurrentBGColor(settings->getCurrentBGColor());
    // Set Selected Text Color
    setSelectedColor(settings->getSelectedColor());
    // Set Selected Text BG Color
    setSelectedBGColor(settings->getSelectedBGColor());
    // Set Current Selected Text Color
    setCurrentSelectedColor(settings->getCurrentSelectedColor());
    // Set Current Selected Text BG Color
    setCurrentSelectedBGColor(settings->getCurrentSelectedBGColor());
    // Set Hidden Text Color
    setHiddenColor(settings->getHiddenColor());
    // Set Hidden Text BG Color
    setHiddenBGColor(settings->getHiddenBGColor());
    // Set Link Text Color
    setLinkColor(settings->getLinkColor());
    // Set Link Text BG Color
    setLinkBGColor(settings->getLinkBGColor());
    // Set Archive Text Color
    setArchiveColor(settings->getArchiveColor());
    // Set Archive Text BG Color
    setArchiveBGColor(settings->getArchiveBGColor());

    // Set Font Name
    setFontName(settings->getFontName());
    // Set Font Size
    setFontSize(settings->getFontSize());
    // Set Font Bold
    setFontBold(settings->getFontBold());
    // Set Font Italic
    setFontItalic(settings->getFontItalic());

    // Set Thumb Width
    setThumbWidth(settings->getThumbWidth());
    // Set Thumb height
    setThumbHeight(settings->getThumbHeight());

    // Set Grid Thumb Width
    setGridThumbWidth(settings->getGridThumbWidth());
    // Set Grid Thumb Height
    setGridThumbHeight(settings->getGridThumbHeight());

    // Set Show Function Keys
    setShowFunctionKeys(settings->getShowFunctionKeys());
    // Set Show Directory Hot Keys
    setShowDirHotKeys(settings->getShowDirHotKeys());
    // Set Show Drive Buttons
    setShowDriveButtons(settings->getShowDriveButtons());
    // Set Close When Finished
    setCloseWhenFinished(settings->getCloseWhenFinished());

    // Set Show Hidden Files
    setShowHiddenFiles(settings->getShowHiddenFiles());
    // Set Select Directories
    setSelectDirectories(settings->getSelectDirectories());
    // Set Show Directories First
    setShowDirsFirst(settings->getShowDirsFirst());
    // Set Case Sensitive Sorting
    setCaseSensitiveSort(settings->getCaseSensitiveSort());

    // Set Copy Hidden Files
    setCopyHiddenFiles(settings->getCopyHiddenFiles());
    // Set Show Full File Sizes
    setShowFullSizes(settings->getShowFullSizes());
    // Set Use Default File Icons
    setUseDefaultIcons(settings->getUseDefaultIcons());
    // Set Follow Links
    setFollowLinks(settings->getFollowLinks());

    // ...

    // Reset Dirty
    setDirty(false);
}

//==============================================================================
// Save Settings
//==============================================================================
void PreferencesDialog::saveSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    qDebug() << "PreferencesDialog::saveSettings";

    // Begin Global Settings Update
    settings->beginGlobalSettingsUpdate();

    // Set Normal Text Color
    settings->setTextColor(textColor);
    // Set Normal Text BG Color
    settings->setTextBGColor(textBGColor);
    // Set Current Text Color
    settings->setCurrentColor(currentColor);
    // Set Current Text BG Color
    settings->setCurrentBGColor(currentBGColor);
    // Set Selected Text Color
    settings->setSelectedColor(selectedColor);
    // Set Selected Text BG Color
    settings->setSelectedBGColor(selectedBGColor);
    // Set Current Selected Text Color
    settings->setCurrentSelectedColor(currentSelectedColor);
    // Set Current Selected Text BG Color
    settings->setCurrentSelectedBGColor(currentSelectedBGColor);
    // Set Hidden Text Color
    settings->setHiddenColor(hiddenColor);
    // Set Hidden Text BG Color
    settings->setHiddenBGColor(hiddenBGColor);
    // Set Link Text Color
    settings->setLinkColor(linkColor);
    // Set Link Text BG Color
    settings->setLinkBGColor(linkBGColor);
    // Set Archive Text Color
    settings->setArchiveColor(archiveColor);
    // Set Archive Text BG Color
    settings->setArchiveBGColor(archiveBGColor);

    // Set Font Name
    settings->setFontName(fontName);
    // Set Font Size
    settings->setFontSize(fontSize);
    // Set Font Bold
    settings->setFontBold(fontBold);
    // Set Font Italic
    settings->setFontItalic(fontItalic);

    // Set Thumb Width
    settings->setThumbWidth(thumbWidth);
    // Set Thumb Height
    settings->setThumbHeight(thumbHeight);

    // Set Grid Thumb Width
    settings->setGridThumbWidth(gridThumbWidth);
    // Set Grid Thumb Height
    settings->setGridThumbHeight(gridThumbHeight);

    // Set Show Function Keys
    settings->setShowFunctionKeys(showFunctionKeys);
    // Set Show Directory Hot Keys
    settings->setShowDirHotKeys(showDirHotKeys);
    // Set Show Drive Buttons
    settings->setShowDriveButtons(showDriveButtons);
    // Set Close When Finished
    settings->setCloseWhenFinished(closeWhenFinished);

    // Set Show Hidden Files
    settings->setShowHiddenFiles(showHiddenFiles);
    // Set Select Directories
    settings->setSelectDirectories(selectDirectories);
    // Set Show Directories First
    settings->setShowDirsFirst(showDirsFirst);
    // Set Case Sensitive Sorting
    settings->setCaseSensitiveSort(caseSensitiveSort);

    // Set Copy Hidden Files
    settings->setCopyHiddenFiles(copyHiddenFiles);
    // Set Show Full File Sizes
    settings->setShowFullSizes(showFullSizes);
    // Set Use Default File Icons
    settings->setUseDefaultIcons(useDefaultIcons);
    // Set Follow Links
    settings->setFollowLinks(followLinks);

    // Set Terminal Path
    settings->setTerminalPath(terminalPath);
    // Set Viewer Path
    settings->setViewerPath(viewerPath);
    // Set Editor Path
    settings->setEditorPath(editorPath);
    // Set Compare Path
    settings->setComparePath(comparePath);
    // Set Packer Path
    settings->setPackerPath(packerPath);
    // Set Unpacker Path
    settings->setUnPackerPath(unPackerPath);


    // Ginish Global Settings Update
    settings->finishGlobalSettingsUpdate();

    // ...

    // Reset Dirty
    setDirty(false);
}

//==============================================================================
// Restore Defaults
//==============================================================================
void PreferencesDialog::restoreDefaults()
{
    // Check Settings
    if (!settings) {
        return;
    }

    qDebug() << "PreferencesDialog::restoreDefaults";

    // Restore Default Settings
    settings->restoreDefaults();

    // Load Settings
    loadSettings();

    // Restore UI
    restoreUI();
}

//==============================================================================
// Apply Settings
//==============================================================================
void PreferencesDialog::applySettings()
{
    // Check Dirty
    if (dirty) {
        qDebug() << "PreferencesDialog::applySettings";
        // Save settings
        saveSettings();
        // Emit Settings Changed Signal
        emit settingsHasChanged();
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void PreferencesDialog::restoreUI()
{
    qDebug() << "PreferencesDialog::restoreUI";

    // Set Button Color
    ui->textColorButton->setColor(string2Rgb(textColor));
    // Set Button Color
    ui->textBgButton->setColor(string2Rgb(textBGColor));
    // Set Button Color
    ui->currentColorButton->setColor(string2Rgb(currentColor));
    // Set Button Color
    ui->currentBGButton->setColor(string2Rgb(currentBGColor));
    // Set Button Color
    ui->selectedColorButton->setColor(string2Rgb(selectedColor));
    // Set Button Color
    ui->selectedBGButton->setColor(string2Rgb(selectedBGColor));
    // Set Button Color
    ui->currentSelectedColorButton->setColor(string2Rgb(currentSelectedColor));
    // Set Button Color
    ui->currentSelectedBGButton->setColor(string2Rgb(currentSelectedBGColor));
    // Set Button Color
    ui->hiddenTextColorButton->setColor(string2Rgb(hiddenColor));
    // Set Button Color
    ui->hiddenTextBgButton->setColor(string2Rgb(hiddenBGColor));
    // Set Button Color
    ui->linkTextColorButton->setColor(string2Rgb(linkColor));
    // Set Button Color
    ui->linkTextBgButton->setColor(string2Rgb(linkBGColor));
    // Set Button Color
    ui->archiveTextColorButton->setColor(string2Rgb(archiveColor));
    // Set Button Color
    ui->archiveTextBgButton->setColor(string2Rgb(archiveBGColor));


    // ...

    // Update Font Button
    updateFontButtonText();

    // ...

    // Set CheckBox Checked
    ui->showHiddenFilesCheckBox->setChecked(showHiddenFiles);
    // Set CheckBox Checked
    ui->selectDirsCheckBox->setChecked(selectDirectories);
    // Set CheckBox Checked
    ui->showDirsFirstCheckBox->setChecked(showDirsFirst);
    // Set CheckBox Checked
    ui->functionKeysCheckBox->setChecked(showFunctionKeys);
    // Set CheckBox Checked
    ui->driveButtonsCheckBox->setChecked(showDriveButtons);
    // Set CheckBox Checked
    ui->dirHotKeysCheckBox->setChecked(showDirHotKeys);
    // Set CheckBox Checked
    ui->closeWhenFinishedCheckBox->setChecked(closeWhenFinished);
    // Set Check Box Checked
    ui->caseSensitiveSortCheckBox->setChecked(caseSensitiveSort);

    // Set Check Box Checked
    ui->copyHiddenFilesCheckBox->setChecked(copyHiddenFiles);
    // Set Check Box Checked
    ui->showFullFileSizesCheckBox->setChecked(showFullSizes);
    // Set Check Box Checked
    ui->defaultFileIconsCheckBox->setChecked(useDefaultIcons);
    // Set Check Box Checked
    ui->followLinksCheckBox->setChecked(followLinks);

    // Switch Thumb Width
    switch (thumbWidth) {
        case DEFAULT_ICON_WIDTH_16:
            // Set Current Index
            ui->iconSizeComboBox->setCurrentIndex(0);
        break;

        default:
        case DEFAULT_ICON_WIDTH_32:
            // Set Current Index
            ui->iconSizeComboBox->setCurrentIndex(1);
        break;

        case DEFAULT_ICON_WIDTH_64:
            // Set Current Index
            ui->iconSizeComboBox->setCurrentIndex(2);
        break;
    }

    // Set Terminal Path
    ui->terminalEditor->setText(terminalPath);
    // Set Viewer Path
    ui->vieverEditor->setText(viewerPath);
    // Set Editor Path
    ui->editorEditor->setText(editorPath);
    // Set Compare Path
    ui->compareEditor->setText(comparePath);
    // Set Packer Path
    ui->packerEditor->setText(packerPath);
    // Set Unpacker Path
    ui->unpackerEditor->setText(unPackerPath);

    // ...
}

//==============================================================================
// Set Dirty
//==============================================================================
void PreferencesDialog::setDirty(const bool& aDirty)
{
    // Check Dirty
    if (dirty != aDirty) {
        // Set Dirty
        dirty = aDirty;

        // Check Dirty
        if (dirty) {
            // Set Standard Buttons
            ui->buttonBox->setStandardButtons(QDialogButtonBox::Close | QDialogButtonBox::Apply | QDialogButtonBox::Reset);
        } else {
            // Set Standard Buttons
            ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
        }
    }
}

//==============================================================================
// Get Use Default Icons
//==============================================================================
bool PreferencesDialog::getUseDefaultIcons()
{
    return useDefaultIcons;
}

//==============================================================================
// Set Use Default Icons
//==============================================================================
void PreferencesDialog::setUseDefaultIcons(const bool& aUseDefaultIcons)
{
    // Check Use Default Icons
    if (useDefaultIcons != aUseDefaultIcons) {
        // Set Use Default Icons
        useDefaultIcons = aUseDefaultIcons;
        // Set Dirty
        setDirty(true);
        // Emit Signal
        emit useDefaultIconsChanged(useDefaultIcons);
    }
}

//==============================================================================
// Get Show Full File Sizes
//==============================================================================
bool PreferencesDialog::getShowFullSizes()
{
    return showFullSizes;
}

//==============================================================================
// Set Show Full File Sizes
//==============================================================================
void PreferencesDialog::setShowFullSizes(const bool& aShowFullSizes)
{
    // Check Show Full Sizes
    if (showFullSizes != aShowFullSizes) {
        // Set Show Full Sizes
        showFullSizes = aShowFullSizes;
        // Set Dirty
        setDirty(true);
        // Emit Signal
        emit showFullSizesChanged(showFullSizes);
    }
}

//==============================================================================
// Get Copy Hidden Files
//==============================================================================
bool PreferencesDialog::getCopyHiddenFiles()
{
    return copyHiddenFiles;
}

//==============================================================================
// Set Copy Hidden Files
//==============================================================================
void PreferencesDialog::setCopyHiddenFiles(const bool& aCopyHiddenFile)
{
    // Check Copy Hidden Files
    if (copyHiddenFiles != aCopyHiddenFile) {
        // Set Copy Hidden Files
        copyHiddenFiles = aCopyHiddenFile;
        // Set Dirty
        setDirty(true);
        // Emit Signal
        emit copyHiddenFilesChanged(copyHiddenFiles);
    }
}

//==============================================================================
// Get Follow Links
//==============================================================================
bool PreferencesDialog::getFollowLinks()
{
    return followLinks;
}

//==============================================================================
// Set Follow Links
//==============================================================================
void PreferencesDialog::setFollowLinks(const bool& aFollowLinks)
{
    // Check Follow Links
    if (followLinks != aFollowLinks) {
        // Set Follow Links
        followLinks = aFollowLinks;
        // Set Dirty
        setDirty(true);
        // Emit Signal
        emit followLinksChanged(followLinks);
    }
}

//==============================================================================
// Get Show Function Keys
//==============================================================================
bool PreferencesDialog::getShowFunctionKeys()
{
    return showFunctionKeys;
}

//==============================================================================
// Set Show Function Keys
//==============================================================================
void PreferencesDialog::setShowFunctionKeys(const bool& aShow)
{
    // Check Show Function Keys
    if (showFunctionKeys != aShow) {
        // Set Show Function Keys
        showFunctionKeys = aShow;
        // Set Dirty
        setDirty(true);
        // Emit Show Function Keys Changed Signal
        emit showFunctionKeysChanged(showFunctionKeys);
    }
}

//==============================================================================
// Get Show Directory Hot Keys
//==============================================================================
bool PreferencesDialog::getShowDirHotKeys()
{
    return showDirHotKeys;
}

//==============================================================================
// Set Show Directory Hot Keys
//==============================================================================
void PreferencesDialog::setShowDirHotKeys(const bool& aShow)
{
    // Check Show Directory Hot Keys
    if (showDirHotKeys != aShow) {
        // Set Show Directory Hot Keya
        showDirHotKeys = aShow;
        // Set Dirty
        setDirty(true);
        // Emit Show Directory Hot Keys Changed Signal
        emit showDirHotKeysChanged(showDirHotKeys);
    }
}

//==============================================================================
// Get Show Drive Buttons
//==============================================================================
bool PreferencesDialog::getShowDriveButtons()
{
    return showDriveButtons;
}

//==============================================================================
// Set Show Drive Buttons
//==============================================================================
void PreferencesDialog::setShowDriveButtons(const bool& aShow)
{
    // Check Show Drive Buttons
    if (showDriveButtons != aShow) {
        // Set Show Drive Buttons
        showDriveButtons = aShow;
        // Set Dirty
        setDirty(true);
        // Emit Show Drive Buttons Changed Signal
        emit showDriveButtonsChanged(showDriveButtons);
    }
}

//==============================================================================
// Get Close When Finished
//==============================================================================
bool PreferencesDialog::getCloseWhenFinished()
{
    return closeWhenFinished;
}

//==============================================================================
// Set Close When Finished
//==============================================================================
void PreferencesDialog::setCloseWhenFinished(const bool& aClose)
{
    // Check Close When Finished
    if (closeWhenFinished != aClose) {
        // Set Close When Finished
        closeWhenFinished = aClose;
        // Set Dirty
        setDirty(true);
        // Emit Close When Finished Changed Signal
        emit closeWhenFinishedChanged(closeWhenFinished);
    }
}

//==============================================================================
// Get Show Hidden Files
//==============================================================================
bool PreferencesDialog::getShowHiddenFiles()
{
    return showHiddenFiles;
}

//==============================================================================
// Set Show Hidden Files
//==============================================================================
void PreferencesDialog::setShowHiddenFiles(const bool& aShow)
{
    // Check Show Hidden Files
    if (showHiddenFiles != aShow) {
        // Set Show Hidden Files
        showHiddenFiles = aShow;
        // Set Dirty
        setDirty(true);
        // Emit Show hidden Files Changed Signal
        emit showHiddenFilesChanged(showHiddenFiles);
    }
}

//==============================================================================
// Get Select Directories
//==============================================================================
bool PreferencesDialog::getSelectDirectories()
{
    return selectDirectories;
}

//==============================================================================
// Set Select Directories
//==============================================================================
void PreferencesDialog::setSelectDirectories(const bool& aSelect)
{
    // Check Select Directories
    if (selectDirectories != aSelect) {
        // Set Select Directories
        selectDirectories = aSelect;
        // Set Dirty
        setDirty(true);
        // Emit Select Directories Changed Signal
        emit selectDirectoriesChanged(selectDirectories);
    }
}

//==============================================================================
// Get Show Directories First
//==============================================================================
bool PreferencesDialog::getShowDirsFirst()
{
    return showDirsFirst;
}

//==============================================================================
// Set Show Directories First
//==============================================================================
void PreferencesDialog::setShowDirsFirst(const bool& aShow)
{
    // Check Show Dirs First
    if (showDirsFirst != aShow) {
        // Set Show Dirs First
        showDirsFirst = aShow;
        // Set Dirty
        setDirty(true);
        // Emit Show Dirs First Changed Signal
        emit showDirsFirstChanged(showDirsFirst);
    }
}

//==============================================================================
// Get Case Sensitive Sorting
//==============================================================================
bool PreferencesDialog::getCaseSensitiveSort()
{
    return caseSensitiveSort;
}

//==============================================================================
// Set Case Sensitive Sorting
//==============================================================================
void PreferencesDialog::setCaseSensitiveSort(const bool& aCaseSensitive)
{
    // Check Case Sensitive Sorting
    if (caseSensitiveSort != aCaseSensitive) {
        // Set Case Sensitive Sorting
        caseSensitiveSort = aCaseSensitive;
        // Set Dirty
        setDirty(true);
        // Emit Case Sensitive Sorting hanged Signal
        emit caseSensitiveSortChanged(caseSensitiveSort);
    }
}

//==============================================================================
// Get Normal Text Color
//==============================================================================
QString PreferencesDialog::getTextColor()
{
    return textColor;
}

//==============================================================================
// Set Normal Text Color
//==============================================================================
void PreferencesDialog::setTextColor(const QString& aColor)
{
    // Check Text Color
    if (textColor != aColor) {
        // Set Text Color
        textColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Text Color Changed Signal
        emit textColorChanged(textColor);
    }
}

//==============================================================================
// Get Normal Text BG Color
//==============================================================================
QString PreferencesDialog::gettextBGColor()
{
    return textBGColor;
}

//==============================================================================
// Set Normal Text BG Color
//==============================================================================
void PreferencesDialog::setTextBGColor(const QString& aColor)
{
    // Check Text BG Color
    if (textBGColor != aColor) {
        // Set Text BG Color
        textBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Text BG Color Changed Signal
        emit textBGColorChanged(textBGColor);
    }
}

//==============================================================================
// Get Current Text Color
//==============================================================================
QString PreferencesDialog::getCurrentColor()
{
    return currentColor;
}

//==============================================================================
// Set Current Text Color
//==============================================================================
void PreferencesDialog::setCurrentColor(const QString& aColor)
{
    // Check Current Color
    if (currentColor != aColor) {
        // Set Current Color
        currentColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Current color Changed Signal
        emit currentColorChanged(currentColor);
    }
}

//==============================================================================
// Get Current Text BG Color
//==============================================================================
QString PreferencesDialog::getCurrentBGColor()
{
    return currentBGColor;
}

//==============================================================================
// Set Current Text BG Color
//==============================================================================
void PreferencesDialog::setCurrentBGColor(const QString& aColor)
{
    // Check Current BG Color
    if (currentBGColor != aColor) {
        // Set Current BG Color
        currentBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Current BG Color Changed Signal
        emit currentBGColorChanged(currentBGColor);
    }
}

//==============================================================================
// Get Selected Text Color
//==============================================================================
QString PreferencesDialog::getSelectedColor()
{
    return selectedColor;
}

//==============================================================================
// Set Selected Text Color
//==============================================================================
void PreferencesDialog::setSelectedColor(const QString& aColor)
{
    // Check Selected Color
    if (selectedColor != aColor) {
        // Set Selected Color
        selectedColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Selected Color Changed Signal
        emit selectedColorChanged(selectedColor);
    }
}

//==============================================================================
// Get Selected Text BG Color
//==============================================================================
QString PreferencesDialog::getSelectedBGColor()
{
    return selectedBGColor;
}

//==============================================================================
// Set Selected Text BG Color
//==============================================================================
void PreferencesDialog::setSelectedBGColor(const QString& aColor)
{
    // Check Selected BG Color
    if (selectedBGColor != aColor) {
        // Set Selected BG Color
        selectedBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Selected BG Color Changed Signal
        emit selectedBGColorChanged(selectedBGColor);
    }
}

//==============================================================================
// Get Current Selected Text Color
//==============================================================================
QString PreferencesDialog::getCurrentSelectedColor()
{
    return currentSelectedColor;
}

//==============================================================================
// Set Current Selected Text Color
//==============================================================================
void PreferencesDialog::setCurrentSelectedColor(const QString& aColor)
{
    // Check Current Selected Color
    if (currentSelectedColor != aColor) {
        // Set Current Selected Color
        currentSelectedColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Current Selected Color Changed Signal
        emit currentSelectedColorChanged(currentSelectedColor);
    }
}

//==============================================================================
// Get Current Selected Text BG Color
//==============================================================================
QString PreferencesDialog::getCurrentSelectedBGColor()
{
    return currentSelectedBGColor;
}

//==============================================================================
// Set Current Selected Text BG Color
//==============================================================================
void PreferencesDialog::setCurrentSelectedBGColor(const QString& aColor)
{
    // Check Current Selected BG Color
    if (currentSelectedBGColor != aColor) {
        // Set Current Selected BG Color
        currentSelectedBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Currnt Selected BG Color Changed Signal
        emit currentSelectedBGColorChanged(currentSelectedBGColor);
    }
}

//==============================================================================
// Get Hidden Text Color
//==============================================================================
QString PreferencesDialog::getHiddenColor()
{
    return hiddenColor;
}

//==============================================================================
// Set Hidden Text Color
//==============================================================================
void PreferencesDialog::setHiddenColor(const QString& aColor)
{
    // Check Hidden Color
    if (hiddenColor != aColor) {
        // Set Hidden Color
        hiddenColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Hidden Color Changed Signal
        emit hiddenColorChanged(hiddenColor);
    }
}

//==============================================================================
// Get Hidden Text BG Color
//==============================================================================
QString PreferencesDialog::getHiddenBGColor()
{
    return hiddenBGColor;
}

//==============================================================================
// Set Hidden Text BG Color
//==============================================================================
void PreferencesDialog::setHiddenBGColor(const QString& aColor)
{
    // Check Hidden BG Color
    if (hiddenBGColor != aColor) {
        // Set Hidden BG Color
        hiddenBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit hidden BG Color Changed Signal
        emit hiddenBGColorChanged(hiddenBGColor);
    }
}

//==============================================================================
// Get Link Text Color
//==============================================================================
QString PreferencesDialog::getLinkColor()
{
    return linkColor;
}

//==============================================================================
// Set Link Text Color
//==============================================================================
void PreferencesDialog::setLinkColor(const QString& aColor)
{
    // Check Link Color
    if (linkColor != aColor) {
        // Set Link Color
        linkColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Link Color Changed Signal
        emit linkColorChanged(linkColor);
    }
}

//==============================================================================
// Get Link Text BG Color
//==============================================================================
QString PreferencesDialog::getLinkBGColor()
{
    return linkBGColor;
}

//==============================================================================
// Set Link Text BG Color
//==============================================================================
void PreferencesDialog::setLinkBGColor(const QString& aColor)
{
    // Check Link BG Color
    if (linkBGColor != aColor) {
        // Set Link BG Color
        linkBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit hidden BG Color Changed Signal
        emit linkBGColorChanged(linkBGColor);
    }
}

//==============================================================================
// Get Archive Text Color
//==============================================================================
QString PreferencesDialog::getArchiveColor()
{
    return archiveColor;
}

//==============================================================================
// Set Archive Text Color
//==============================================================================
void PreferencesDialog::setArchiveColor(const QString& aColor)
{
    // Check Archive Color
    if (archiveColor != aColor) {
        // Set Archive Color
        archiveColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Archive Color Changed Signal
        emit archiveColorChanged(archiveColor);
    }
}

//==============================================================================
// Get Archive Text BG Color
//==============================================================================
QString PreferencesDialog::getArchiveBGColor()
{
    return archiveBGColor;
}

//==============================================================================
// Set Archive Text BG Color
//==============================================================================
void PreferencesDialog::setArchiveBGColor(const QString& aColor)
{
    // Check Archive BG Color
    if (archiveBGColor != aColor) {
        // Set Archive BG Color
        archiveBGColor = aColor;
        // Set Dirty Flag
        setDirty(true);
        // Emit Archive BG Color Changed Signal
        emit archiveBGColorChanged(archiveBGColor);
    }
}

//==============================================================================
// Get Font Name
//==============================================================================
QString PreferencesDialog::getFontName()
{
    return fontName;
}

//==============================================================================
// Set Font Name
//==============================================================================
void PreferencesDialog::setFontName(const QString& aFontName)
{
    // Check Font Name
    if (fontName != aFontName) {
        // Set Fonr name
        fontName = aFontName;
        // Set Dirty Flag
        setDirty(true);
        // Emit Font name Changed Signal
        fontNameChanged(fontName);
    }
}

//==============================================================================
// Get Font Size
//==============================================================================
int PreferencesDialog::getFontSize()
{
    return fontSize;
}

//==============================================================================
// Set Font Size
//==============================================================================
void PreferencesDialog::setFontSize(const int& aSize)
{
    // Check Font Size
    if (fontSize != aSize) {
        // Set Font Size
        fontSize = aSize;
        // Set Dirty Flag
        setDirty(true);
        // Emit Font Size Changed
        emit fontSizeChanged(fontSize);
    }
}

//==============================================================================
// Get Font Bold
//==============================================================================
bool PreferencesDialog::getFontBold()
{
    return fontBold;
}

//==============================================================================
// Set Font Bold
//==============================================================================
void PreferencesDialog::setFontBold(const bool& aBold)
{
    // Check Font Bold
    if (fontBold != aBold) {
        // Set Font Bold
        fontBold = aBold;
        // Set Dirty Flag
        setDirty(true);
        // Emit Font Bold Changed
        emit fontBoldChanged(fontBold);
    }
}

//==============================================================================
// Get Font Italic
//==============================================================================
bool PreferencesDialog::getFontItalic()
{
    return fontItalic;
}

//==============================================================================
// Set Font Italic
//==============================================================================
void PreferencesDialog::setFontItalic(const bool& aItalic)
{
    // Check Font Italic
    if (fontItalic != aItalic) {
        // Set Font Italic
        fontItalic = aItalic;
        // Set Dirty Flag
        setDirty(true);
        // Emit Font Italic Changed Signal
        emit fontItalicChanged(fontItalic);
    }
}

//==============================================================================
// Get Thumb Width
//==============================================================================
int PreferencesDialog::getThumbWidth()
{
    return thumbWidth;
}

//==============================================================================
// Set Thumb Width
//==============================================================================
void PreferencesDialog::setThumbWidth(const int& aWidth)
{
    // Check Thumb Width
    if (thumbWidth != aWidth) {
        // Set Thumb Width
        thumbWidth = aWidth;
        // Set Dirty Flag
        setDirty(true);
        // Emit Thumb Width Changed
        emit thumbWidthChanged(thumbWidth);
    }
}

//==============================================================================
// Get Thumb Height
//==============================================================================
int PreferencesDialog::getThumbHeight()
{
    return thumbHeight;
}

//==============================================================================
// Set Thumb height
//==============================================================================
void PreferencesDialog::setThumbHeight(const int& aHeight)
{
    // Check Thumb Height
    if (thumbHeight != aHeight) {
        // Set Thumb Height
        thumbHeight = aHeight;
        // Set Dirty Flag
        setDirty(true);
        // Emit Thumb Height Changed Signal
        emit thumbHeightChanged(thumbHeight);
    }
}

//==============================================================================
// Get Grid Thumb Width
//==============================================================================
int PreferencesDialog::getGridThumbWidth()
{
    return gridThumbWidth;
}

//==============================================================================
// Set Grid Thumb Width
//==============================================================================
void PreferencesDialog::setGridThumbWidth(const int& aWidth)
{
    // Chek Grid Thumb Width
    if (gridThumbWidth != aWidth) {
        // Set Grid Thumb Width
        gridThumbWidth = aWidth;
        // Emit Signal
        emit gridThumbWidthChanged(gridThumbWidth);
    }
}

//==============================================================================
// Get Grid Thumb Height
//==============================================================================
int PreferencesDialog::getGridThumbHeight()
{
    return gridThumbHeight;
}

//==============================================================================
// Set Grid Thumb Height
//==============================================================================
void PreferencesDialog::setGridThumbHeight(const int& aHeight)
{
    // Check Grid Thumb Height
    if (gridThumbHeight != aHeight) {
        // Set Grid Thumb Height
        gridThumbHeight = aHeight;
        // Emit Signal
        emit gridThumbHeightChanged(gridThumbHeight);
    }
}


//==============================================================================
// Update Preview
//==============================================================================
void PreferencesDialog::updatePreview()
{
    // ...
}

//==============================================================================
// Update Font Button Text
//==============================================================================
void PreferencesDialog::updateFontButtonText()
{
    // Init Updaed Font
    QFont updatedFont(fontName, 12);
    // Set Bold
    updatedFont.setBold(fontBold);
    // Set Italic
    updatedFont.setItalic(fontItalic);
    // Set Font
    ui->fontButton->setFont(updatedFont);
    // Set Button Text
    ui->fontButton->setText(fontName + " - " + tr(DEFAULT_TITLE_FONT_SIZE) + QString("%1").arg(fontSize));
}

//==============================================================================
// Exec
//==============================================================================
int PreferencesDialog::exec()
{
    return QDialog::exec();
}

// Exec Dialog
//==============================================================================
int PreferencesDialog::execDialog()
{
    // Load Settings
    loadSettings();

    // Restore UI
    restoreUI();

    // ...

    return QDialog::exec();
}

//==============================================================================
// On Show Function Keys Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_functionKeysCheckBox_clicked()
{
    // Set Show Function Keys
    setShowFunctionKeys(ui->functionKeysCheckBox->isChecked());
}

//==============================================================================
// On Show Drive Buttons Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_driveButtonsCheckBox_clicked()
{
    // Set Show Drive Buttons
    setShowDriveButtons(ui->driveButtonsCheckBox->isChecked());
}

//==============================================================================
// On Show Directory Hot Keys Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_dirHotKeysCheckBox_clicked()
{
    // Set Show Dir Hot Keys
    setShowDirHotKeys(ui->dirHotKeysCheckBox->isChecked());
}

//==============================================================================
// On Close When Finished Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_closeWhenFinishedCheckBox_clicked()
{
    // Set Close When Finished
    setCloseWhenFinished(ui->closeWhenFinishedCheckBox->isChecked());
}

//==============================================================================
// On Show Hidden Files Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_showHiddenFilesCheckBox_clicked()
{
    // Set Show Hidden Files
    setShowHiddenFiles(ui->showHiddenFilesCheckBox->isChecked());
}

//==============================================================================
// On Select Dirs Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_selectDirsCheckBox_clicked()
{
    // Set Select Directories
    setSelectDirectories(ui->selectDirsCheckBox->isChecked());
}

//==============================================================================
// On Show Dirs First Check Box Clicked Slot
//==============================================================================
void PreferencesDialog::on_showDirsFirstCheckBox_clicked()
{
    // Set Show Dirs First
    setShowDirsFirst(ui->showDirsFirstCheckBox->isChecked());
}

//==============================================================================
// On Case Sensitive Sort Checkbox Clicked Slot
//==============================================================================
void PreferencesDialog::on_caseSensitiveSortCheckBox_clicked()
{
    // Set Case Sensitive Sorting
    setCaseSensitiveSort(ui->caseSensitiveSortCheckBox->isChecked());
}

//==============================================================================
// Show Full File Sizes Checkbox Clicked Slot
//==============================================================================
void PreferencesDialog::on_showFullFileSizesCheckBox_clicked()
{
    // Set Show Full File Sizes
    setShowFullSizes(ui->showFullFileSizesCheckBox->isChecked());
}

//==============================================================================
// Copy Hidden Files Checkbox Clicked Slot
//==============================================================================
void PreferencesDialog::on_copyHiddenFilesCheckBox_clicked()
{
    // Set Copy Hidden Files
    setCopyHiddenFiles(ui->copyHiddenFilesCheckBox->isChecked());
}

//==============================================================================
// Follow Symbolic Links Checkbox Clicked Slot
//==============================================================================
void PreferencesDialog::on_followLinksCheckBox_clicked()
{
    // Set Follow Links
    setFollowLinks(ui->followLinksCheckBox->isChecked());
}

//==============================================================================
// Use Default File Icons Checkbox Clicked Slot
//==============================================================================
void PreferencesDialog::on_defaultFileIconsCheckBox_clicked()
{
    // Set Use Default File Icons
    setUseDefaultIcons(ui->defaultFileIconsCheckBox->isChecked());
}

//==============================================================================
// On Text Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_textColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setTextColor(rgb2string(currentColor));
        // Set Button Color
        ui->textColorButton->setColor(currentColor);
    }
}

//==============================================================================
// On Text BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_textBgButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setTextBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->textBgButton->setColor(currentColor);
    }
}

//==============================================================================
// On Current Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_currentColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setCurrentColor(rgb2string(currentColor));
        // Set Button Color
        ui->currentColorButton->setColor(currentColor);
    }
}

//==============================================================================
// On Current BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_currentBGButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setCurrentBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->currentBGButton->setColor(currentColor);
    }
}

//==============================================================================
// On Selected Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_selectedColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setSelectedColor(rgb2string(currentColor));
        // Set Button Color
        ui->selectedColorButton->setColor(currentColor);
    }
}

//==============================================================================
// On Selected BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_selectedBGButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setSelectedBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->selectedBGButton->setColor(currentColor);
    }
}

//==============================================================================
// On Current Selected Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_currentSelectedColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setCurrentSelectedColor(rgb2string(currentColor));
        // Set Button Color
        ui->currentSelectedColorButton->setColor(currentColor);
    }
}

//==============================================================================
// On Current Selected BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_currentSelectedBGButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setCurrentSelectedBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->currentSelectedBGButton->setColor(currentColor);
    }
}

//==============================================================================
// On Hidden Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_hiddenTextColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setHiddenColor(rgb2string(currentColor));
        // Set Button Color
        ui->hiddenTextColorButton->setColor(currentColor);
    }
}

//==============================================================================
// On Hidden BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_hiddenTextBgButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setHiddenBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->hiddenTextBgButton->setColor(currentColor);
    }
}

//==============================================================================
// Link Text Color Button Pressed Slot
//==============================================================================
void PreferencesDialog::on_linkTextColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setLinkColor(rgb2string(currentColor));
        // Set Button Color
        ui->linkTextColorButton->setColor(currentColor);
    }
}

//==============================================================================
// Link Text BG Color Button Pressed Slot
//==============================================================================
void PreferencesDialog::on_linkTextBgButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setLinkBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->linkTextBgButton->setColor(currentColor);
    }
}

//==============================================================================
// Archive Text Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_archiveTextColorButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setArchiveColor(rgb2string(currentColor));
        // Set Button Color
        ui->archiveTextColorButton->setColor(currentColor);
    }
}

//==============================================================================
// On Archive Text BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_archiveTextBgButton_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();
        // Set Color
        setArchiveBGColor(rgb2string(currentColor));
        // Set Button Color
        ui->archiveTextBgButton->setColor(currentColor);
    }
}

//==============================================================================
// On Font Select Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_fontButton_clicked()
{
    // Init Font Dialog
    QFontDialog fontDialog;
    // Exec Font Dialog
    if (fontDialog.exec()) {
        // Get Current Font
        QFont currentFont = fontDialog.currentFont();
        // Set Font Name
        setFontName(currentFont.family());
        // Set Font Size
        setFontSize(currentFont.pointSize());
        // Set Font bold
        setFontBold(currentFont.bold());
        // Set Font Italic
        setFontItalic(currentFont.italic());
        // Update Font Button Text
        updateFontButtonText();
    }
}

//==============================================================================
// Icon/Thumb Size Combo Box Current Index Changed Slot
//==============================================================================
void PreferencesDialog::on_iconSizeComboBox_currentIndexChanged(const QString& arg1)
{
    //qDebug() << "PreferencesDialog::on_iconSizeComboBox_currentIndexChanged - arg1: " << arg1;

    // Set Thumb Width
    setThumbWidth(arg1.left(2).toInt());

    // Set Thumb Height
    setThumbHeight(arg1.right(2).toInt());
}

//==============================================================================
// Clear Current BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_clearCurrentBGButton_clicked()
{
    // Set Current BG Color
    setCurrentBGColor(DEFAULT_SETTINGS_CURRENT_BG_COLOR);
}

//==============================================================================
// Clear Selected BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_clearSelectedBGButton_clicked()
{
    // Set Selected BG Color
    setSelectedBGColor(DEFAULT_TRANSPARENT_COLOR);
}

//==============================================================================
// Clear Current Selected BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_clearCurrentSelectedBGButton_clicked()
{
    // Set Current Selected BG Color
    setCurrentSelectedBGColor(DEFAULT_TRANSPARENT_COLOR);
}

//==============================================================================
// Clear Hidden BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_clearHiddenBGButton_clicked()
{
    // Set Hidden BG Color
    setHiddenBGColor(DEFAULT_TRANSPARENT_COLOR);
}

//==============================================================================
// Clear Link BG Color Button Pressed Slot
//==============================================================================
void PreferencesDialog::on_clearLinkBGButton_clicked()
{
    // Set Link BG Color
    setLinkBGColor(DEFAULT_TRANSPARENT_COLOR);
}

//==============================================================================
// Clear Archive Text BG Color Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_clearArchiveBGButton_clicked()
{
    // Set Archive BG Color
    setArchiveBGColor(DEFAULT_TRANSPARENT_COLOR);
}

//==============================================================================
// On Browse Terminal Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_browseTerminalButton_clicked()
{

}

//==============================================================================
// On Browse Viewer Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_browseViewerButton_clicked()
{

}

//==============================================================================
// On Browse Editor Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_browseEditorButton_clicked()
{

}

//==============================================================================
// On Browse Compare Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_browseCompareButton_clicked()
{

}

//==============================================================================
// On Browse Packer Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_browsePackerButton_clicked()
{

}

//==============================================================================
// On Browse Unpacker Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_browseUnpackerButton_clicked()
{

}

//==============================================================================
// Button Box Button Clicked
//==============================================================================
void PreferencesDialog::buttonBoxButtonPressed(QAbstractButton* aButton)
{
    // Get Standard Button
    QDialogButtonBox::StandardButton standardButton = ui->buttonBox->standardButton(aButton);

    // Switch Standard Button
    switch (standardButton) {
        case QDialogButtonBox::Apply:
            //qDebug() << "PreferencesDialog::buttonBoxButtonPressed - Apply";
            // Apply Settings
            applySettings();
        break;

        case QDialogButtonBox::Reset:
            //qDebug() << "PreferencesDialog::buttonBoxButtonPressed - Reset";
            // Restore Default Settings
            restoreDefaults();
        break;

        default:
        break;
    }
}

//==============================================================================
// Destructor
//==============================================================================
PreferencesDialog::~PreferencesDialog()
{
    // ...

    // Delete Preview Widget Explicitly To Avoid Crash
    delete ui->previewWidget;
    // Delete UI
    delete ui;

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        // Reset Settings
        settings = NULL;
    }


    qDebug() << "PreferencesDialog::~PreferencesDialog";
}









