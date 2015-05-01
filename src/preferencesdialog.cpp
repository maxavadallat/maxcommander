#include <QSettings>
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
#include "utility.h"
#include "constants.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
PreferencesDialog::PreferencesDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PreferencesDialog)
    , dirty(false)
    , showFunctionKeys(DEFAULT_SETTINGS_SHOW_FUNCTION_KEYS)
    , showDirHotKeys(DEFAULT_SETTINGS_SHOW_DIRECTORIY_HOT_KEYS)
    , showDriveButtons(DEFAULT_SETTINGS_SHOW_DRIVE_BUTTONSS)
    , showHiddenFiles(DEFAULT_SETTINGS_SHOW_HIDDEN_FILES)
    , selectDirectories(DEFAULT_SETTINGS_SELECT_DIRECTORIES)
    , showDirsFirst(DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST)
    , caseSensitiveSort(DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING)
    // ...
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
    // ...
    , thumbWidth(DEFAULT_SETTINGS_THUMB_WIDTH)
    , thumbHeight(DEFAULT_SETTINGS_THUMB_HEIGHT)
    // ...
    , viewerPath(DEFAULT_SETTINGS_VIEWER_PATH)
    , editorPath(DEFAULT_SETTINGS_EDITOR_PATH)
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
    qDebug() << "PreferencesDialog::loadSettings";

    // Init Settings
    QSettings settings;

    // Set Normal Text Color
    setTextColor(settings.value(SETTINGS_KEY_PANEL_COLOR_TEXT, DEFAULT_SETTINGS_TEXT_COLOR).toString());
    // Set Normal Text BG Color
    setTextBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_TEXT_BG, DEFAULT_SETTINGS_TEXT_BG_COLOR).toString());
    // Set Current Text Color
    setCurrentColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT, DEFAULT_SETTINGS_CURRENT_COLOR).toString());
    // Set Current Text BG Color
    setCurrentBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_BG, DEFAULT_SETTINGS_CURRENT_BG_COLOR).toString());
    // Set Selected Text Color
    setSelectedColor(settings.value(SETTINGS_KEY_PANEL_COLOR_SELECTED, DEFAULT_SETTINGS_SELECTED_COLOR).toString());
    // Set Selected Text BG Color
    setSelectedBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_SELECTED_BG, DEFAULT_SETTINGS_SELECTED_BG_COLOR).toString());
    // Set Current Selected Text Color
    setCurrentSelectedColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED, DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR).toString());
    // Set Current Selected Text BG Color
    setCurrentSelectedBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG, DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR).toString());
    // Set Hidden Text Color
    setHiddenColor(settings.value(SETTINGS_KEY_PANEL_COLOR_HIDDEN, DEFAULT_SETTINGS_HIDDEN_COLOR).toString());
    // Set Hidden Text BG Color
    setHiddenBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG, DEFAULT_SETTINGS_HIDDEN_BG_COLOR).toString());
    // Set Link Text Color
    setLinkColor(settings.value(SETTINGS_KEY_PANEL_COLOR_LINK, DEFAULT_SETTINGS_LINK_COLOR).toString());
    // Set Link Text BG Color
    setLinkBGColor(settings.value(SETTINGS_KEY_PANEL_COLOR_LINK_BG, DEFAULT_SETTINGS_LINK_BG_COLOR).toString());

    // Set Font Name
    setFontName(settings.value(SETTINGS_KEY_PANEL_FONT_NAME, DEFAULT_SETTINGS_FONT_NAME).toString());
    // Set Font Size
    setFontSize(settings.value(SETTINGS_KEY_PANEL_FONT_SIZE, DEFAULT_SETTINGS_FONT_SIZE).toInt());
    // Set Font Bold
    setFontBold(settings.value(SETTINGS_KEY_PANEL_FONT_BOLD, DEFAULT_SETTINGS_FONT_BOLD).toBool());
    // Set Font Italic
    setFontItalic(settings.value(SETTINGS_KEY_PANEL_FONT_ITALIC, DEFAULT_SETTINGS_FONT_ITALIC).toBool());

    // Set Thumb Width
    setThumbWidth(settings.value(SETTINGS_KEY_THUMBS_WIDTH, DEFAULT_SETTINGS_THUMB_WIDTH).toInt());
    // Set Thumb height
    setThumbHeight(settings.value(SETTINGS_KEY_THUMBS_HEIGHT, DEFAULT_SETTINGS_THUMB_HEIGHT).toInt());

    // Set Show Function Keys
    setShowFunctionKeys(settings.value(SETTINGS_KEY_SHOW_FUNCTION_KEYS, DEFAULT_SETTINGS_SHOW_FUNCTION_KEYS).toBool());
    // Set Show Directory Hot Keys
    setShowDirHotKeys(settings.value(SETTINGS_KEY_SHOW_DIR_HOT_KEYS, DEFAULT_SETTINGS_SHOW_DIRECTORIY_HOT_KEYS).toBool());
    // Set Show Drive Buttons
    setShowDriveButtons(settings.value(SETTINGS_KEY_SHOW_DRIVE_BUTTONS, DEFAULT_SETTINGS_SHOW_DRIVE_BUTTONSS).toBool());

    // Set Show Hidden Files
    setShowHiddenFiles(settings.value(SETTINGS_KEY_SHOW_HIDDEN_FILES, DEFAULT_SETTINGS_SHOW_HIDDEN_FILES).toBool());
    // Set Select Directories
    setSelectDirectories(settings.value(SETTINGS_KEY_SELECT_DIRS, DEFAULT_SETTINGS_SELECT_DIRECTORIES).toBool());
    // Set Show Directories First
    setShowDirsFirst(settings.value(SETTINGS_KEY_DIRFIRST, DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST).toBool());
    // Set Case Sensitive Sorting
    setCaseSensitiveSort(settings.value(SETTINGS_KEY_CASE_SENSITIVE, DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING).toBool());

    // ...

    // Reset Dirty
    setDirty(false);
}

//==============================================================================
// Save Settings
//==============================================================================
void PreferencesDialog::saveSettings()
{
    qDebug() << "PreferencesDialog::saveSettings";

    // Init Settings
    QSettings settings;

    // Set Normal Text Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_TEXT, textColor);
    // Set Normal Text BG Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_TEXT_BG, textBGColor);
    // Set Current Text Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT, currentColor);
    // Set Current Text BG Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT_BG, currentBGColor);
    // Set Selected Text Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_SELECTED, selectedColor);
    // Set Selected Text BG Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_SELECTED_BG, selectedBGColor);
    // Set Current Selected Text Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED, currentSelectedColor);
    // Set Current Selected Text BG Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG, currentSelectedBGColor);
    // Set Hidden Text Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_HIDDEN, hiddenColor);
    // Set Hidden Text BG Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG, hiddenBGColor);
    // Set Link Text Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_LINK, linkColor);
    // Set Link Text BG Color
    settings.setValue(SETTINGS_KEY_PANEL_COLOR_LINK_BG, linkBGColor);

    // Set Font Name
    settings.setValue(SETTINGS_KEY_PANEL_FONT_NAME, fontName);
    // Set Font Size
    settings.setValue(SETTINGS_KEY_PANEL_FONT_SIZE, fontSize);
    // Set Font Bold
    settings.setValue(SETTINGS_KEY_PANEL_FONT_BOLD, fontBold);
    // Set Font Italic
    settings.setValue(SETTINGS_KEY_PANEL_FONT_ITALIC, fontItalic);

    // Set Thumb Width
    settings.setValue(SETTINGS_KEY_THUMBS_WIDTH, thumbWidth);
    // Set Thumb height
    settings.setValue(SETTINGS_KEY_THUMBS_HEIGHT, thumbHeight);

    // Set Show Function Keys
    settings.setValue(SETTINGS_KEY_SHOW_FUNCTION_KEYS, showFunctionKeys);
    // Set Show Directory Hot Keys
    settings.setValue(SETTINGS_KEY_SHOW_DIR_HOT_KEYS, showDirHotKeys);
    // Set Show Drive Buttons
    settings.setValue(SETTINGS_KEY_SHOW_DRIVE_BUTTONS, showDriveButtons);

    // Set Show Hidden Files
    settings.setValue(SETTINGS_KEY_SHOW_HIDDEN_FILES, showHiddenFiles);
    // Set Select Directories
    settings.setValue(SETTINGS_KEY_SELECT_DIRS, selectDirectories);
    // Set Show Directories First
    settings.setValue(SETTINGS_KEY_DIRFIRST, showDirsFirst);
    // Set Case Sensitive Sorting
    settings.setValue(SETTINGS_KEY_CASE_SENSITIVE, caseSensitiveSort);


    // ...

    // Reset Dirty
    setDirty(false);
}

//==============================================================================
// Restore Defaults
//==============================================================================
void PreferencesDialog::restoreDefaults()
{
    qDebug() << "PreferencesDialog::restoreDefaults";

    // Set Normal Text Color
    setTextColor(DEFAULT_SETTINGS_TEXT_COLOR);
    // Set Normal Text BG Color
    setTextBGColor(DEFAULT_SETTINGS_TEXT_BG_COLOR);
    // Set Current Text Color
    setCurrentColor(DEFAULT_SETTINGS_CURRENT_COLOR);
    // Set Current Text BG Color
    setCurrentBGColor(DEFAULT_SETTINGS_CURRENT_BG_COLOR);
    // Set Selected Text Color
    setSelectedColor(DEFAULT_SETTINGS_SELECTED_COLOR);
    // Set Selected Text BG Color
    setSelectedBGColor(DEFAULT_SETTINGS_SELECTED_BG_COLOR);
    // Set Current Selected Text Color
    setCurrentSelectedColor(DEFAULT_SETTINGS_CURRENT_SELECTED_COLOR);
    // Set Current Selected Text BG Color
    setCurrentSelectedBGColor(DEFAULT_SETTINGS_CURRENT_SELECTED_BG_COLOR);
    // Set Hidden Text Color
    setHiddenColor(DEFAULT_SETTINGS_HIDDEN_COLOR);
    // Set Hidden Text BG Color
    setHiddenBGColor(DEFAULT_SETTINGS_HIDDEN_BG_COLOR);
    // Set Link Text Color
    setLinkColor(DEFAULT_SETTINGS_LINK_COLOR);
    // Set Link Text BG Color
    setLinkBGColor(DEFAULT_SETTINGS_LINK_BG_COLOR);

    // Set Font Name
    setFontName(DEFAULT_SETTINGS_FONT_NAME);
    // Set Font Size
    setFontSize(DEFAULT_SETTINGS_FONT_SIZE);
    // Set Font Bold
    setFontBold(DEFAULT_SETTINGS_FONT_BOLD);
    // Set Font Italic
    setFontItalic(DEFAULT_SETTINGS_FONT_ITALIC);

    // Set Thumb Width
    setThumbWidth(DEFAULT_SETTINGS_THUMB_WIDTH);
    // Set Thumb height
    setThumbHeight(DEFAULT_SETTINGS_THUMB_HEIGHT);

    // Set Show Function Keys
    setShowFunctionKeys(DEFAULT_SETTINGS_SHOW_FUNCTION_KEYS);
    // Set Show Directory Hot Keys
    setShowDirHotKeys(DEFAULT_SETTINGS_SHOW_DIRECTORIY_HOT_KEYS);
    // Set Show Drive Buttons
    setShowDriveButtons(DEFAULT_SETTINGS_SHOW_DRIVE_BUTTONSS);

    // Set Show Hidden Files
    setShowHiddenFiles(DEFAULT_SETTINGS_SHOW_HIDDEN_FILES);
    // Set Select Directories
    setSelectDirectories(DEFAULT_SETTINGS_SELECT_DIRECTORIES);
    // Set Show Directories First
    setShowDirsFirst(DEFAULT_SETTINGS_SHOW_DIRECTORIES_FIRST);
    // Set Case Sensitive Sorting
    setCaseSensitiveSort(DEFAULT_SETTINGS_CASE_SENSITIVE_SORTING);

    // ...

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
    // Set Check Box Checked
    ui->caseSensitiveSortCheckBox->setChecked(caseSensitiveSort);

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
            // Enable Apply Button

        } else {
            // Disable Apply Button

        }
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
        // Emit Thumb Height Changed Signal
        emit thumbHeightChanged(thumbHeight);
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
    QFont updatedFont(fontName, 12, -1, fontItalic);
    // Set Bold
    updatedFont.setBold(fontBold);
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

    qDebug() << "PreferencesDialog::~PreferencesDialog";
}








