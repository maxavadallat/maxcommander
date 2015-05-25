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
QVariant SettingsController::value(const QString& aKey)
{
    return settings.value(aKey);
}

//==============================================================================
// Set Value
//==============================================================================
void SettingsController::setValue(const QString& aKey, const QVariant& aValue)
{
    settings.setValue(aKey, aValue);
}

//==============================================================================
// Restore Defaults
//==============================================================================
void SettingsController::restoreDefaults()
{
    // ...
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
// Destructor
//==============================================================================
SettingsController::~SettingsController()
{
    // Sync
    settings.sync();
}

