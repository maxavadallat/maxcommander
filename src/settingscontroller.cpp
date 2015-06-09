#include <QDebug>

#include "settingscontroller.h"
#include "constants.h"
#include "defaultsettings.h"


// Settings Singleton
static SettingsController* settingsSingleton = NULL;


//==============================================================================
// Get Instance - Static Constructor
//==============================================================================
SettingsController* SettingsController::getInstance()
{
    // Check Settings  Singleton
    if (!settingsSingleton) {
        // Create Singleton
        settingsSingleton = new SettingsController();
        // Load Settings
        settingsSingleton->loadSettings();
    } else {
        // Inc Ref Count
        settingsSingleton->refCount++;
    }

    return settingsSingleton;
}

//==============================================================================
// Constructor
//==============================================================================
SettingsController::SettingsController(QObject* aParent)
    : QObject(aParent)
    , refCount(1)
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
    , fontName(DEFAULT_SETTINGS_FONT_NAME)
    , fontSize(DEFAULT_SETTINGS_FONT_SIZE)
    , fontBold(DEFAULT_SETTINGS_FONT_BOLD)
    , fontItalic(DEFAULT_SETTINGS_FONT_ITALIC)

    , thumbWidth(DEFAULT_SETTINGS_THUMB_WIDTH)
    , thumbHeight(DEFAULT_SETTINGS_THUMB_HEIGHT)

    , terminalPath("")
    , viewerPath("")
    , editorPath("")
    , comparePath("")
    , packerPath("")
    , unPackerPath("")

{
    // ...
}

//==============================================================================
// Release
//==============================================================================
void SettingsController::release()
{
    // Dec Ref Count
    refCount--;

    // Check Ref Count
    if (refCount <= 0 && settingsSingleton) {
        // Delete Settings Singleton
        delete settingsSingleton;
        settingsSingleton = NULL;
    }
}

//==============================================================================
// Get Value
//==============================================================================
QVariant SettingsController::value(const QString& aKey, const QVariant& aDefaultValue)
{
    return settings.value(aKey, aDefaultValue);
}

//==============================================================================
// Set Value
//==============================================================================
void SettingsController::setValue(const QString& aKey, const QVariant& aValue)
{
    // Set Value
    settings.setValue(aKey, aValue);

    // Set Dirty
    //setDirty(true);
}

//==============================================================================
// Get Dirty
//==============================================================================
bool SettingsController::getDirty()
{
    return dirty;
}

//==============================================================================
// Load Settings
//==============================================================================
void SettingsController::loadSettings()
{
    qDebug() << "SettingsController::loadSettings";

    showFunctionKeys = settings.value(SETTINGS_KEY_SHOW_FUNCTION_KEYS, DEFAULT_SETTINGS_SHOW_FUNCTION_KEYS).toBool();
    showDirHotKeys = settings.value(SETTINGS_KEY_SHOW_DIR_HOT_KEYS, DEFAULT_SETTINGS_SHOW_DIRECTORIY_HOT_KEYS).toBool();
    showDriveButtons = settings.value(SETTINGS_KEY_SHOW_DRIVE_BUTTONS, DEFAULT_SETTINGS_SHOW_DRIVE_BUTTONS).toBool();
    closeWhenFinished = settings.value(SETTINGS_KEY_CLOSE_WHEN_FINISHED, DEFAULT_SETTINGS_CLOSE_WHEN_FINISHED).toBool();

    selectDirectories = settings.value(SETTINGS_KEY_SELECT_DIRS, DEFAULT_SETTINGS_SELECT_DIRECTORIES).toBool();
    showHiddenFiles = settings.value(SETTINGS_KEY_SHOW_HIDDEN_FILES, DEFAULT_SETTINGS_SHOW_HIDDEN_FILES).toBool();
    showDirsFirst = settings.value(SETTINGS_KEY_DIRFIRST, DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST).toBool();
    caseSensitiveSort = settings.value(SETTINGS_KEY_CASE_SENSITIVE, DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING).toBool();

    useDefaultIcons = settings.value(SETTINGS_KEY_PANEL_USE_DEFAULT_ICONS, DEAFULT_SETTINGS_USE_DEFAULT_ICONS).toBool();
    showFullSizes = settings.value(SETTINGS_KEY_SHOW_FULL_SIZES, DEFAULT_SETTINGS_SHOW_FULL_FILE_SIZES).toBool();
    copyHiddenFiles = settings.value(SETTINGS_KEY_PANEL_COPY_HIDDEN_FILES, DEFAULT_SETTINGS_COPY_HIDDEN_FILES).toBool();
    followLinks = settings.value(SETTINGS_KEY_FOLLOW_LINKS, DEFAULT_SETTINGS_FOLLOW_SYMBOLIC_LINKS).toBool();

    textColor = settings.value(SETTINGS_KEY_PANEL_COLOR_TEXT, DEFAULT_SETTINGS_TEXT_COLOR).toString();
    textBGColor = settings.value(SETTINGS_KEY_PANEL_COLOR_TEXT_BG, DEFAULT_SETTINGS_TEXT_BG_COLOR).toString();
    currentColor = settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT, DEFAULT_SETTINGS_CURRENT_COLOR).toString();
    currentBGColor = settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_BG, DEFAULT_SETTINGS_CURRENT_BG_COLOR).toString();
    selectedColor = settings.value(SETTINGS_KEY_PANEL_COLOR_SELECTED, DEFAULT_SETTINGS_SELECTED_COLOR).toString();
    selectedBGColor = settings.value(SETTINGS_KEY_PANEL_COLOR_SELECTED_BG, DEFAULT_SETTINGS_SELECTED_BG_COLOR).toString();
    currentSelectedColor = settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED, DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR).toString();
    currentSelectedBGColor = settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG, DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR).toString();
    hiddenColor = settings.value(SETTINGS_KEY_PANEL_COLOR_HIDDEN, DEFAULT_SETTINGS_HIDDEN_COLOR).toString();
    hiddenBGColor = settings.value(SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG, DEFAULT_SETTINGS_HIDDEN_BG_COLOR).toString();
    linkColor = settings.value(SETTINGS_KEY_PANEL_COLOR_LINK, DEFAULT_SETTINGS_LINK_COLOR).toString();
    linkBGColor = settings.value(SETTINGS_KEY_PANEL_COLOR_LINK_BG, DEFAULT_SETTINGS_LINK_BG_COLOR).toString();
    fontName = settings.value(SETTINGS_KEY_PANEL_FONT_NAME, DEFAULT_SETTINGS_FONT_NAME).toString();
    fontSize = settings.value(SETTINGS_KEY_PANEL_FONT_SIZE, DEFAULT_SETTINGS_FONT_SIZE).toInt();
    fontBold = settings.value(SETTINGS_KEY_PANEL_FONT_BOLD, DEFAULT_SETTINGS_FONT_BOLD).toBool();
    fontItalic = settings.value(SETTINGS_KEY_PANEL_FONT_ITALIC, DEFAULT_SETTINGS_FONT_ITALIC).toBool();

    thumbWidth = settings.value(SETTINGS_KEY_THUMBS_WIDTH, DEFAULT_SETTINGS_THUMB_WIDTH).toInt();
    thumbHeight = settings.value(SETTINGS_KEY_THUMBS_HEIGHT, DEFAULT_SETTINGS_THUMB_HEIGHT).toInt();

    terminalPath = settings.value(SETTINGS_KEY_APPS_TERMINAL, "").toString();
    viewerPath = settings.value(SETTINGS_KEY_APPS_VIEWER, "").toString();
    editorPath = settings.value(SETTINGS_KEY_APPS_EDITOR, "").toString();
    comparePath = settings.value(SETTINGS_KEY_APPS_COMPARE, "").toString();
    packerPath = settings.value(SETTINGS_KEY_APPS_PACKER, "").toString();
    unPackerPath = settings.value(SETTINGS_KEY_APPS_UNPACKER, "").toString();

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void SettingsController::saveSettings()
{
    // Check If Not Dirty
    if (!dirty) {
        return;
    }

    qDebug() << "SettingsController::saveSettings";

    settings.setValue(SETTINGS_KEY_SHOW_FUNCTION_KEYS, showFunctionKeys);
    settings.setValue(SETTINGS_KEY_SHOW_DIR_HOT_KEYS, showDirHotKeys);
    settings.setValue(SETTINGS_KEY_SHOW_DRIVE_BUTTONS, showDriveButtons);
    settings.setValue(SETTINGS_KEY_CLOSE_WHEN_FINISHED, closeWhenFinished);

    settings.setValue(SETTINGS_KEY_SELECT_DIRS, selectDirectories);
    settings.setValue(SETTINGS_KEY_SHOW_HIDDEN_FILES, showHiddenFiles);
    settings.setValue(SETTINGS_KEY_DIRFIRST, showDirsFirst);
    settings.setValue(SETTINGS_KEY_CASE_SENSITIVE, caseSensitiveSort);

    settings.setValue(SETTINGS_KEY_PANEL_USE_DEFAULT_ICONS, useDefaultIcons);
    settings.setValue(SETTINGS_KEY_SHOW_FULL_SIZES, showFullSizes);
    settings.setValue(SETTINGS_KEY_PANEL_COPY_HIDDEN_FILES, copyHiddenFiles);
    settings.setValue(SETTINGS_KEY_FOLLOW_LINKS, followLinks);

    settings.setValue(SETTINGS_KEY_PANEL_COLOR_TEXT, textColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_TEXT_BG, textBGColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT, currentColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT_BG, currentBGColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_SELECTED, selectedColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_SELECTED_BG, selectedBGColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED, currentSelectedColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG, currentSelectedBGColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_HIDDEN, hiddenColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG, hiddenBGColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_LINK, linkColor);
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_LINK_BG, linkBGColor);
    settings.setValue(SETTINGS_KEY_PANEL_FONT_NAME, fontName);
    settings.setValue(SETTINGS_KEY_PANEL_FONT_SIZE, fontSize);
    settings.setValue(SETTINGS_KEY_PANEL_FONT_BOLD, fontBold);
    settings.setValue(SETTINGS_KEY_PANEL_FONT_ITALIC, fontItalic);

    settings.setValue(SETTINGS_KEY_THUMBS_WIDTH, thumbWidth);
    settings.setValue(SETTINGS_KEY_THUMBS_HEIGHT, thumbHeight);

    settings.setValue(SETTINGS_KEY_APPS_TERMINAL, terminalPath);
    settings.setValue(SETTINGS_KEY_APPS_VIEWER, viewerPath);
    settings.setValue(SETTINGS_KEY_APPS_EDITOR, editorPath);
    settings.setValue(SETTINGS_KEY_APPS_COMPARE, comparePath);
    settings.setValue(SETTINGS_KEY_APPS_PACKER, packerPath);
    settings.setValue(SETTINGS_KEY_APPS_UNPACKER, unPackerPath);

    // ...

    // Sync
    settings.sync();
}

//==============================================================================
// Restore Defaults
//==============================================================================
void SettingsController::restoreDefaults()
{
    // Emit Begin Global Settings Update
    emit globalSettingsUpdateBegin();

    // ...

    // Reset Show Function Keys
    setShowFunctionKeys(DEFAULT_SETTINGS_SHOW_FUNCTION_KEYS);
    // Reset Show Directory Hot Keys
    setShowDirHotKeys(DEFAULT_SETTINGS_SHOW_DIRECTORIY_HOT_KEYS);
    // Reset Show Drive Buttons
    setShowDriveButtons(DEFAULT_SETTINGS_SHOW_DRIVE_BUTTONS);
    // Reset Close When Finished
    setCloseWhenFinished(DEFAULT_SETTINGS_CLOSE_WHEN_FINISHED);
    // Reset Select Directories
    setSelectDirectories(DEFAULT_SETTINGS_SELECT_DIRECTORIES);
    // Reset Show Hidden Files
    setShowHiddenFiles(DEFAULT_SETTINGS_SHOW_HIDDEN_FILES);
    // Reset Show Directories First
    setShowDirsFirst(DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST);
    // Reset Case Sensitive Sorting
    setCaseSensitiveSort(DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING);
    // Reset Use Default Icons
    setUseDefaultIcons(DEAFULT_SETTINGS_USE_DEFAULT_ICONS);
    // Reset Show Full File Sizes
    setShowFullSizes(DEFAULT_SETTINGS_SHOW_FULL_FILE_SIZES);
    // Reset Copy Hidden Files
    setCopyHiddenFiles(DEFAULT_SETTINGS_COPY_HIDDEN_FILES);
    // Reset Follow Links
    setFollowLinks(DEFAULT_SETTINGS_FOLLOW_SYMBOLIC_LINKS);
    // Reset Normal Text Color
    setTextColor(DEFAULT_SETTINGS_TEXT_COLOR);
    // Reset Normal Text BG Color
    setTextBGColor(DEFAULT_SETTINGS_TEXT_BG_COLOR);
    // Reset Current Text Color
    setCurrentColor(DEFAULT_SETTINGS_CURRENT_COLOR);
    // Reset Current Text BG Color
    setCurrentBGColor(DEFAULT_SETTINGS_CURRENT_BG_COLOR);
    // Reset Selected Text Color
    setSelectedColor(DEFAULT_SETTINGS_SELECTED_COLOR);
    // Reset Selected Text BG Color
    setSelectedBGColor(DEFAULT_SETTINGS_SELECTED_BG_COLOR);
    // Reset Current Selected Text Color
    setCurrentSelectedColor(DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR);
    // Reset Current Selected Text BG Color
    setCurrentSelectedBGColor(DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR);
    // Reset Hidden Text Color
    setHiddenColor(DEFAULT_SETTINGS_HIDDEN_COLOR);
    // Reset Hidden Text BG Color
    setHiddenBGColor(DEFAULT_SETTINGS_HIDDEN_BG_COLOR);
    // Reset Link Text Color
    setLinkColor(DEFAULT_SETTINGS_LINK_COLOR);
    // Reset Link Text BG Color
    setLinkBGColor(DEFAULT_SETTINGS_LINK_BG_COLOR);
    // Reset Font Name
    setFontName(DEFAULT_SETTINGS_FONT_NAME);
    // Reset Font Size
    setFontSize(DEFAULT_SETTINGS_FONT_SIZE);
    // Reset Font Bold
    setFontBold(DEFAULT_SETTINGS_FONT_BOLD);
    // Reset Font Italic
    setFontItalic(DEFAULT_SETTINGS_FONT_ITALIC);
    // Reset Thumb Width
    setThumbWidth(DEFAULT_SETTINGS_THUMB_WIDTH);
    // Reset Thumb height
    setThumbHeight(DEFAULT_SETTINGS_THUMB_HEIGHT);

    // ...

    // Reset Dirty
    setDirty(false);

    // Emit Global Settings Update Finished
    emit globalSettingsUpdateFinished();
}

//==============================================================================
// Set Dirty
//==============================================================================
void SettingsController::setDirty(const bool& aDirty)
{
    // Check Dirty
    if (dirty != aDirty) {
        // Set Dirty
        dirty = aDirty;

        // ...

        // Emit Dirty Changed Signal
        emit dirtyChanged(dirty);
    }
}

//==============================================================================
// Get Show Function Keys
//==============================================================================
bool SettingsController::getShowFunctionKeys()
{
    return showFunctionKeys;
}

//==============================================================================
// Set Show Function Keys
//==============================================================================
void SettingsController::setShowFunctionKeys(const bool& aShow)
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
bool SettingsController::getShowDirHotKeys()
{
    return showDirHotKeys;
}

//==============================================================================
// Set Show Directory Hot Keys
//==============================================================================
void SettingsController::setShowDirHotKeys(const bool& aShow)
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
bool SettingsController::getShowDriveButtons()
{
    return showDriveButtons;
}

//==============================================================================
// Set Show Drive Buttons
//==============================================================================
void SettingsController::setShowDriveButtons(const bool& aShow)
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
bool SettingsController::getCloseWhenFinished()
{
    return closeWhenFinished;
}

//==============================================================================
// Set Close When Finished
//==============================================================================
void SettingsController::setCloseWhenFinished(const bool& aClose)
{
    // Check Close When Finished
    if (closeWhenFinished != aClose) {
        // Set Close When Finished
        closeWhenFinished = aClose;
        // Emit Close When Finished Changed Signal
        emit closeWhenFinishedChanged(closeWhenFinished);
    }
}

//==============================================================================
// Get Show Hidden Files
//==============================================================================
bool SettingsController::getShowHiddenFiles()
{
    return showHiddenFiles;
}

//==============================================================================
// Set Show Hidden Files
//==============================================================================
void SettingsController::setShowHiddenFiles(const bool& aShow)
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
bool SettingsController::getSelectDirectories()
{
    return selectDirectories;
}

//==============================================================================
// Set Select Directories
//==============================================================================
void SettingsController::setSelectDirectories(const bool& aSelect)
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
bool SettingsController::getShowDirsFirst()
{
    return showDirsFirst;
}

//==============================================================================
// Set Show Directories First
//==============================================================================
void SettingsController::setShowDirsFirst(const bool& aShow)
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
bool SettingsController::getCaseSensitiveSort()
{
    return caseSensitiveSort;
}

//==============================================================================
// Set Case Sensitive Sorting
//==============================================================================
void SettingsController::setCaseSensitiveSort(const bool& aCaseSensitive)
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
// Get Use Default Icons
//==============================================================================
bool SettingsController::getUseDefaultIcons()
{
    return useDefaultIcons;
}

//==============================================================================
// Set Use Default Icons
//==============================================================================
void SettingsController::setUseDefaultIcons(const bool& aUseDefaultIcons)
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
bool SettingsController::getShowFullSizes()
{
    return showFullSizes;
}

//==============================================================================
// Set Show Full File Sizes
//==============================================================================
void SettingsController::setShowFullSizes(const bool& aShowFullSizes)
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
bool SettingsController::getCopyHiddenFiles()
{
    return copyHiddenFiles;
}

//==============================================================================
// Set Copy Hidden Files
//==============================================================================
void SettingsController::setCopyHiddenFiles(const bool& aCopyHiddenFile)
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
bool SettingsController::getFollowLinks()
{
    return followLinks;
}

//==============================================================================
// Set Follow Links
//==============================================================================
void SettingsController::setFollowLinks(const bool& aFollowLinks)
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
// Get Normal Text Color
//==============================================================================
QString SettingsController::getTextColor()
{
    return textColor;
}

//==============================================================================
// Set Normal Text Color
//==============================================================================
void SettingsController::setTextColor(const QString& aColor)
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
QString SettingsController::gettextBGColor()
{
    return textBGColor;
}

//==============================================================================
// Set Normal Text BG Color
//==============================================================================
void SettingsController::setTextBGColor(const QString& aColor)
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
QString SettingsController::getCurrentColor()
{
    return currentColor;
}

//==============================================================================
// Set Current Text Color
//==============================================================================
void SettingsController::setCurrentColor(const QString& aColor)
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
QString SettingsController::getCurrentBGColor()
{
    return currentBGColor;
}

//==============================================================================
// Set Current Text BG Color
//==============================================================================
void SettingsController::setCurrentBGColor(const QString& aColor)
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
QString SettingsController::getSelectedColor()
{
    return selectedColor;
}

//==============================================================================
// Set Selected Text Color
//==============================================================================
void SettingsController::setSelectedColor(const QString& aColor)
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
QString SettingsController::getSelectedBGColor()
{
    return selectedBGColor;
}

//==============================================================================
// Set Selected Text BG Color
//==============================================================================
void SettingsController::setSelectedBGColor(const QString& aColor)
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
QString SettingsController::getCurrentSelectedColor()
{
    return currentSelectedColor;
}

//==============================================================================
// Set Current Selected Text Color
//==============================================================================
void SettingsController::setCurrentSelectedColor(const QString& aColor)
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
QString SettingsController::getCurrentSelectedBGColor()
{
    return currentSelectedBGColor;
}

//==============================================================================
// Set Current Selected Text BG Color
//==============================================================================
void SettingsController::setCurrentSelectedBGColor(const QString& aColor)
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
QString SettingsController::getHiddenColor()
{
    return hiddenColor;
}

//==============================================================================
// Set Hidden Text Color
//==============================================================================
void SettingsController::setHiddenColor(const QString& aColor)
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
QString SettingsController::getHiddenBGColor()
{
    return hiddenBGColor;
}

//==============================================================================
// Set Hidden Text BG Color
//==============================================================================
void SettingsController::setHiddenBGColor(const QString& aColor)
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
QString SettingsController::getLinkColor()
{
    return linkColor;
}

//==============================================================================
// Set Link Text Color
//==============================================================================
void SettingsController::setLinkColor(const QString& aColor)
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
QString SettingsController::getLinkBGColor()
{
    return linkBGColor;
}

//==============================================================================
// Set Link Text BG Color
//==============================================================================
void SettingsController::setLinkBGColor(const QString& aColor)
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
// Get Font Name
//==============================================================================
QString SettingsController::getFontName()
{
    return fontName;
}

//==============================================================================
// Set Font Name
//==============================================================================
void SettingsController::setFontName(const QString& aFontName)
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
int SettingsController::getFontSize()
{
    return fontSize;
}

//==============================================================================
// Set Font Size
//==============================================================================
void SettingsController::setFontSize(const int& aSize)
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
bool SettingsController::getFontBold()
{
    return fontBold;
}

//==============================================================================
// Set Font Bold
//==============================================================================
void SettingsController::setFontBold(const bool& aBold)
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
bool SettingsController::getFontItalic()
{
    return fontItalic;
}

//==============================================================================
// Set Font Italic
//==============================================================================
void SettingsController::setFontItalic(const bool& aItalic)
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
int SettingsController::getThumbWidth()
{
    return thumbWidth;
}

//==============================================================================
// Set Thumb Width
//==============================================================================
void SettingsController::setThumbWidth(const int& aWidth)
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
int SettingsController::getThumbHeight()
{
    return thumbHeight;
}

//==============================================================================
// Set Thumb height
//==============================================================================
void SettingsController::setThumbHeight(const int& aHeight)
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
// Get Terminal Path
//==============================================================================
QString SettingsController::getTerminalPath()
{
    return terminalPath;
}

//==============================================================================
// Set Terminal Path
//==============================================================================
void SettingsController::setTerminalPath(const QString& aTerminalPath)
{
    // Check Terminal Path
    if (terminalPath != aTerminalPath) {
        // Set Terminal Path
        terminalPath = aTerminalPath;
        // Set Dirty
        setDirty(true);
        // Emit Terminal Path Changed Signal
        emit terminalPathChanged(terminalPath);
    }
}

//==============================================================================
// Get Viewer Path
//==============================================================================
QString SettingsController::getViewerPath()
{
    return viewerPath;
}

//==============================================================================
// Set Viewer Path
//==============================================================================
void SettingsController::setViewerPath(const QString& aViewerPath)
{
    // Check Viewer Path
    if (viewerPath != aViewerPath) {
        // Set Viewer Path
        viewerPath = aViewerPath;
        // Set Dirty
        setDirty(true);
        // Emit Viewer Path Changed Signal
        emit viewerPathChanged(viewerPath);
    }
}

//==============================================================================
// Get Editor Path
//==============================================================================
QString SettingsController::getEditorPath()
{
    return editorPath;
}

//==============================================================================
// Set Editor Path
//==============================================================================
void SettingsController::setEditorPath(const QString& aEditorPath)
{
    // Check Editor Path
    if (editorPath != aEditorPath) {
        // Set Editor Path
        editorPath = aEditorPath;
        // Set Dirty
        setDirty(true);
        // Emit Editor Path Changed Signal
        emit editorPathChanged(editorPath);
    }
}

//==============================================================================
// Get Compare Path
//==============================================================================
QString SettingsController::getComparePath()
{
    return comparePath;
}

//==============================================================================
// Set Compare Path
//==============================================================================
void SettingsController::setComparePath(const QString& aComparePath)
{
    // Check Compare Path
    if (comparePath != aComparePath) {
        // Set Compare Path
        comparePath = aComparePath;
        // Set Dirty
        setDirty(true);
        // Emit Compare Path Changed Signal
        emit comparePathChanged(comparePath);
    }
}

//==============================================================================
// Get Packer Path
//==============================================================================
QString SettingsController::getPackerPath()
{
    return packerPath;
}

//==============================================================================
// Set Packer Path
//==============================================================================
void SettingsController::setPackerPath(const QString& aPackerPath)
{
    // Check Packer Path
    if (packerPath != aPackerPath) {
        // Set Packer Path
        packerPath = aPackerPath;
        // Set Dirty
        setDirty(true);
        // Emit Packer Path Changed Signal
        emit packerPathChanged(packerPath);
    }
}

//==============================================================================
// Get Unpacker Path
//==============================================================================
QString SettingsController::getUnPackerPath()
{
    return unPackerPath;
}

//==============================================================================
// Set Unpacker Path
//==============================================================================
void SettingsController::setUnPackerPath(const QString& aUnpackerPath)
{
    // Check Un Packer Path
    if (unPackerPath != aUnpackerPath) {
        // Set Un Packer Path
        unPackerPath = aUnpackerPath;
        // Set Dirty
        setDirty(true);
        // Emit Un Packer Path Changed Signal
        emit unpackerPathChanged(unPackerPath);
    }
}

//==============================================================================
// Destructor
//==============================================================================
SettingsController::~SettingsController()
{
    // Save Settings
    saveSettings();
}

