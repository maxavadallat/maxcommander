
// INCLUDES

#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QDebug>

#include "constants.h"
#include "settings.h"
#include "settingswindow.h"
#include "colorpickerbutton.h"
#include "ui_settingswindow.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
SettingsWindow::SettingsWindow(QEventLoop* aEventLoop, QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::SettingsWindow)
    , eventLoop(aEventLoop)
    , settings(NULL)
    , keyPressed(false)
{
    qDebug() << "Creating SettingsWindow...";

    // Set Window Flags
    setWindowFlags(Qt::Dialog);
    // Set Window Modality
    setWindowModality(Qt::ApplicationModal);

    // Setup UI
    ui->setupUi(this);

    // Set Tab Index
    ui->settingsTab->setCurrentIndex(0);

    // Connect Signals
    connect(ui->normalColorButton, SIGNAL(clicked()), this, SLOT(normalColorButtonClicked()));
    connect(ui->normalBackgroundButton, SIGNAL(clicked()), this, SLOT(normalBackgroundButtonClicked()));

    connect(ui->currentColorButton, SIGNAL(clicked()), this, SLOT(currentColorButtonClicked()));
    connect(ui->currentBackgroundButton, SIGNAL(clicked()), this, SLOT(currentBackgroundButtonClicked()));

    connect(ui->selectedColorButton, SIGNAL(clicked()), this, SLOT(selectedColorButtonClicked()));
    connect(ui->selectedBackgroundButton, SIGNAL(clicked()), this, SLOT(selectedBackgroundButtonClicked()));

    connect(ui->currentSelectedColorButton, SIGNAL(clicked()), this, SLOT(currentSelectedColorButtonClicked()));
    connect(ui->currentSelectedBackgroundButton, SIGNAL(clicked()), this, SLOT(currentSelectedBackgroundButtonClicked()));

    connect(ui->linkColorButton, SIGNAL(clicked()), this, SLOT(linkColorButtonClicked()));
    connect(ui->linkBackgroundButton, SIGNAL(clicked()), this, SLOT(linkBackgroundButtonClicked()));

    // ...

    // Fill up Default Icons List
    defaultIcons << QPixmap(QString(":defaultIcon16x16"));
    defaultIcons << QPixmap(QString(":defaultIcon32x32"));
    defaultIcons << QPixmap(QString(":defaultIcon64x64"));

    // Load Settings
    loadSettings();

    qDebug() << "Creating SettingsWindow...done";
}

//==============================================================================
// Load Settings
//==============================================================================
void SettingsWindow::loadSettings()
{
    // Check Settings
    if (!settings) {
        // Get Settings Instance
        settings = Settings::getInstance();
    }

    // Check UI
    if (settings && ui) {
        qDebug() << "SettingsWindow::loadSettings";

        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_GENERAL));

        // Set Checkboxes Checked State
        ui->showSplashCheckBox->setChecked(settings->getValue(QString(SETTINGS_KEY_SHOW_SPLASH), true).toBool());
        ui->showToolbarCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_SHOW_TOOL_BAR), true).toBool());
        ui->showDirButtonsCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_SHOW_DIR_BUTTONS), true).toBool());
        ui->showCommandBarCheckBox->setChecked(settings->getValue(QString(SETTINGS_KEY_SHOW_COMMAND_BAR), true).toBool());
        ui->showStatusBarCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_SHOW_STATUS_BAR), true).toBool());
        ui->showHiddenCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_SHOW_HIDDEN_FILES), false).toBool());
        ui->execBundlesCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_EXECUTE_BUNDLES), true).toBool());
        ui->deleteMovesCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_USE_RECYCLE_BIN), false).toBool());
        ui->saveOnExitCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_SAVE_ON_EXIT), true).toBool());
        ui->followLinksCheckBox->setChecked(settings->getValue(QString(SETTIGNS_KEY_FOLLOW_LINKS), true).toBool());
        ui->archivesAsDirsCheckBox->setChecked(settings->getValue(QString(SETTINGS_KEY_ARCHIVES_AS_DIRS), true).toBool());


        // ...

        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_APPEARANCE));

        // Get Icon Size
        iconSize = settings->getValue(QString(SETTINGS_KEY_ICONSIZE), SETTINGS_VALUE_ICONSIZE_32X32).toInt();
        // Set Icon Size Combo Box
        ui->iconSizeComboBox->setCurrentIndex(iconSize);
        //ui->iconSizeComboBox->setEditText(ui->iconSizeComboBox->itemText(ui->iconSizeComboBox->currentIndex()));

        // Get Font settings
        font.setFamily(settings->getValue(QString(SETTINGS_KEY_FONTFAMILY), QString(SETTINGS_VALUE_FONTFAMILY)).toString());
        font.setPointSize(settings->getValue(QString(SETTINGS_KEY_FONTSIZE), SETTINGS_VALUE_FONTSIZE).toInt());
        font.setBold(settings->getValue(QString(SETTINGS_KEY_FONTBOLD), SETTINGS_VALUE_FONTBOLD).toBool());
        font.setItalic(settings->getValue(QString(SETTINGS_KEY_FONTITALIC), SETTINGS_VALUE_FONTITALIC).toBool());
        font.setUnderline(false);
        font.setStrikeOut(false);

        // Set Change Button Font
        //ui->changefontButton->setFont(font);
        // Set Example Text
        ui->changefontButton->setText(QString(CHANGE_FONT_BUTTON_EXAMPLE_TEXT_TEMPLATE).arg(font.family()).arg(font.pointSize()));

        // Set Button Color
        ui->normalColorButton->setColor(settings->getValue(QString(SETTINGS_KEY_NORMAL_TEXT_COLOR), SETTINGS_VALUE_NORMAL_TEXT_COLOR).toInt());
        // Set Button Color
        ui->normalBackgroundButton->setColor(settings->getValue(QString(SETTINGS_KEY_NORMAL_BACKGROUND_COLOR), SETTINGS_VALUE_NORMAL_BACKGROUND_COLOR).toInt());

        // Set Button Color
        ui->currentColorButton->setColor(settings->getValue(QString(SETTINGS_KEY_CURRENT_TEXT_COLOR), SETTINGS_VALUE_CURRENT_TEXT_COLOR).toInt());
        // Set Button Color
        ui->currentBackgroundButton->setColor(settings->getValue(QString(SETTINGS_KEY_CURRENT_BACKGROUND_COLOR), SETTINGS_VALUE_CURRENT_BACKGROUND_COLOR).toInt());

        // Set Button Color
        ui->selectedColorButton->setColor(settings->getValue(QString(SETTINGS_KEY_SELECTED_TEXT_COLOR), SETTINGS_VALUE_SELECTED_TEXT_COLOR).toInt());
        // Set Button Color
        ui->selectedBackgroundButton->setColor(settings->getValue(QString(SETTINGS_KEY_SELECTED_BACKGROUND_COLOR), SETTINGS_VALUE_SELECTED_BACKGROUND_COLOR).toInt());

        // Set Button Color
        ui->currentSelectedColorButton->setColor(settings->getValue(QString(SETTINGS_KEY_CURRENT_SELECTED_TEXT_COLOR), SETTINGS_VALUE_CURRENT_SELECTED_TEXT_COLOR).toInt());
        // Set Button Color
        ui->currentSelectedBackgroundButton->setColor(settings->getValue(QString(SETTINGS_KEY_CURRENT_SELECTED_BACKGROUND_COLOR), SETTINGS_VALUE_CURRENT_SELECTED_BACKGROUND_COLOR).toInt());

        // Set Button Color
        ui->linkColorButton->setColor(settings->getValue(QString(SETTINGS_KEY_LINK_TEXT_COLOR), SETTINGS_VALUE_LINK_TEXT_COLOR).toInt());
        // Set Button Color
        ui->linkBackgroundButton->setColor(settings->getValue(QString(SETTINGS_KEY_LINK_BACKGROUND_COLOR), SETTINGS_VALUE_LINK_BACKGROUND_COLOR).toInt());
/*
        // Init RGB Color
        QRgb colorRgb;
        // Init Foreground Color
        QColor fgColor;
        // Init Background Color
        QColor bgColor;

        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_NORMAL_TEXT_COLOR), SETTINGS_VALUE_NORMAL_TEXT_COLOR).toInt();
        // Set Color RGBA
        fgColor.setRgba(colorRgb);
        // Set Button Color
        ui->normalColorButton->setColor(fgColor);

        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_NORMAL_BACKGROUND_COLOR), SETTINGS_VALUE_NORMAL_BACKGROUND_COLOR).toInt();
        // Set Color RGBA
        bgColor.setRgba(colorRgb);
        // Set Button Color
        ui->normalBackgroundButton->setColor(bgColor);


        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_CURRENT_TEXT_COLOR), SETTINGS_VALUE_CURRENT_TEXT_COLOR).toInt();
        // Set Color RGBA
        fgColor.setRgba(colorRgb);
        // Set Button Color
        ui->currentColorButton->setColor(fgColor);

        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_CURRENT_BACKGROUND_COLOR), SETTINGS_VALUE_CURRENT_BACKGROUND_COLOR).toInt();
        // Set Color RGBA
        bgColor.setRgba(colorRgb);
        // Set Button Color
        ui->currentBackgroundButton->setColor(bgColor);


        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_SELECTED_TEXT_COLOR), SETTINGS_VALUE_SELECTED_TEXT_COLOR).toInt();
        // Set Color RGBA
        fgColor.setRgba(colorRgb);
        // Set Button Color
        ui->selectedColorButton->setColor(fgColor);

        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_SELECTED_BACKGROUND_COLOR), SETTINGS_VALUE_SELECTED_BACKGROUND_COLOR).toInt();
        // Set Color RGBA
        bgColor.setRgba(colorRgb);
        // Set Button Color
        ui->selectedBackgroundButton->setColor(bgColor);



        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_CURRENT_SELECTED_TEXT_COLOR), SETTINGS_VALUE_CURRENT_SELECTED_TEXT_COLOR).toInt();
        // Set Color RGBA
        fgColor.setRgba(colorRgb);
        // Set Button Color
        ui->currentSelectedColorButton->setColor(fgColor);

        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_CURRENT_SELECTED_BACKGROUND_COLOR), SETTINGS_VALUE_CURRENT_SELECTED_BACKGROUND_COLOR).toInt();
        // Set Color RGBA
        bgColor.setRgba(colorRgb);
        // Set Button Color
        ui->currentSelectedBackgroundButton->setColor(bgColor);



        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_LINK_TEXT_COLOR), SETTINGS_VALUE_LINK_TEXT_COLOR).toInt();
        // Set Color RGBA
        fgColor.setRgba(colorRgb);
        // Set Button Color
        ui->linkColorButton->setColor(fgColor);

        // Get Color
        colorRgb = settings->getValue(QString(SETTINGS_KEY_LINK_BACKGROUND_COLOR), SETTINGS_VALUE_LINK_BACKGROUND_COLOR).toInt();
        // Set Color RGBA
        bgColor.setRgba(colorRgb);
        // Set Button Color
        ui->linkBackgroundButton->setColor(bgColor);

*/
        // Set Spacing Spin Box Value
        ui->spacingSpinBox->setValue(settings->getValue(QString(SETTINGS_KEY_ITEM_SPACING), SETTINGS_VALUE_DEFAULT_ITEM_SPACING).toInt());

        // Update Preview Frame
        updatePreviewFrame();


        // ...


        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_TOOLS));

        // ...


        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_SEARCH));

        // ...


    }
}

//==============================================================================
// Save Settings
//==============================================================================
void SettingsWindow::saveSettings()
{
    // Check Settings
    if (settings && ui) {
        qDebug() << "SettingsWindow::saveSettings";

        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_GENERAL));

        // Set Values
        settings->setValue(QString(SETTINGS_KEY_SHOW_SPLASH), ui->showSplashCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_SHOW_TOOL_BAR), ui->showToolbarCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_SHOW_DIR_BUTTONS), ui->showDirButtonsCheckBox->isChecked());
        settings->setValue(QString(SETTINGS_KEY_SHOW_COMMAND_BAR), ui->showCommandBarCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_SHOW_STATUS_BAR), ui->showStatusBarCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_SHOW_HIDDEN_FILES), ui->showHiddenCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_EXECUTE_BUNDLES), ui->execBundlesCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_USE_RECYCLE_BIN), ui->deleteMovesCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_SAVE_ON_EXIT), ui->saveOnExitCheckBox->isChecked());
        settings->setValue(QString(SETTIGNS_KEY_FOLLOW_LINKS), ui->followLinksCheckBox->isChecked());
        settings->setValue(QString(SETTINGS_KEY_ARCHIVES_AS_DIRS), ui->archivesAsDirsCheckBox->isChecked());

        // ...

        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_APPEARANCE));

        // Set Values
        settings->setValue(QString(SETTINGS_KEY_NORMAL_TEXT_COLOR), ui->normalColorButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_NORMAL_BACKGROUND_COLOR), ui->normalBackgroundButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_CURRENT_TEXT_COLOR), ui->currentColorButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_CURRENT_BACKGROUND_COLOR), ui->currentBackgroundButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_SELECTED_TEXT_COLOR), ui->selectedColorButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_SELECTED_BACKGROUND_COLOR), ui->selectedBackgroundButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_CURRENT_SELECTED_TEXT_COLOR), ui->currentSelectedColorButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_CURRENT_SELECTED_BACKGROUND_COLOR), ui->currentSelectedBackgroundButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_LINK_TEXT_COLOR), ui->linkColorButton->getColor());
        settings->setValue(QString(SETTINGS_KEY_LINK_BACKGROUND_COLOR), ui->linkBackgroundButton->getColor());

        // Icon Size
        settings->setValue(QString(SETTINGS_KEY_ICONSIZE), ui->iconSizeComboBox->currentIndex());

        // Font
        settings->setValue(QString(SETTINGS_KEY_FONTFAMILY), font.family());
        settings->setValue(QString(SETTINGS_KEY_FONTSIZE), font.pointSize());
        settings->setValue(QString(SETTINGS_KEY_FONTBOLD), font.bold());
        settings->setValue(QString(SETTINGS_KEY_FONTITALIC), font.italic());

        // Item Spacing
        settings->setValue(QString(SETTINGS_KEY_ITEM_SPACING), ui->spacingSpinBox->value());

        // ...

        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_TOOLS));

        // ...

        // Set Group
        settings->setGroup(QString(SETTINGS_GROUP_SEARCH));

        // ...

        // Save Settings
        settings->save();
    }
}

//==============================================================================
// Color Button Clicked
//==============================================================================
void SettingsWindow::colorButtonClicked(ColorPickerButton* aButton)
{
    // Check Button
    if (aButton) {
        // Init Color Dialog
        QColorDialog colorDialog;
        // Set Window Modality
        colorDialog.setWindowModality(Qt::ApplicationModal);
        // Set Color Dialog Current Color
        colorDialog.setCurrentColor(aButton->getColor());
        // Execute Color Dialog
        if (colorDialog.exec()) {

            qDebug() << "SettingsWindow::colorButtonClicked - currentColor: " << QString("#%1").arg(colorDialog.currentColor().rgba(), 8, 16, QChar('0'));

            // Set Color
            aButton->setColor(colorDialog.currentColor().rgba());

            // Check Button
            if (aButton == ui->normalBackgroundButton) {
                // Set Selected Background Color
                ui->selectedBackgroundButton->setColor(colorDialog.currentColor().rgba());
                // Set Link Background Color
                ui->linkBackgroundButton->setColor(colorDialog.currentColor().rgba());
            } else if (aButton == ui->currentBackgroundButton) {
                // Set Current Selected Backgound Color
                ui->currentSelectedBackgroundButton->setColor(colorDialog.currentColor().rgba());
            }
        }

        // Update Preview Frame
        updatePreviewFrame();
    }
}

//==============================================================================
// Update Preview Frame
//==============================================================================
void SettingsWindow::updatePreviewFrame()
{
    // Check UI
    if (ui) {
        // Check Normal Item Background Color Button Color Value
        if (ui->normalBackgroundButton->getColor() != -1) {
            // Set Preview Label Style Sheet
            ui->normalPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->normalColorButton->getColor()).name())
                                                                                            .arg(QColor(ui->normalBackgroundButton->getColor()).name())
                                                                                            .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                            .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                            .arg(font.pointSize())
                                                                                            .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->normalPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->normalColorButton->getColor()).name())
                                                                                           .arg(QColor(ui->normalBackgroundButton->getColor()).name())
                                                                                           .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                           .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                           .arg(font.pointSize())
                                                                                           .arg(font.family()));
        } else {
            // Set Preview Label Style Sheet
            ui->normalPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->normalColorButton->getColor()).name())
                                                                                            .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                            .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                            .arg(font.pointSize())
                                                                                            .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->normalPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->normalColorButton->getColor()).name())
                                                                                           .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                           .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                           .arg(font.pointSize())
                                                                                           .arg(font.family()));
        }

        // Set Icon
        ui->normalPreviewIcon->setPixmap(defaultIcons[iconSize]);

        // Set Whole Preview Frame Style Sheet As Normal Item
        ui->previewFrame->setStyleSheet(ui->normalPreviewLabel->styleSheet());

        if (ui->currentBackgroundButton->getColor() != -1) {
            // Set Preview Label Style Sheet
            ui->currentPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->currentColorButton->getColor()).name())
                                                                                             .arg(QColor(ui->currentBackgroundButton->getColor()).name())
                                                                                             .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                             .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                             .arg(font.pointSize())
                                                                                             .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->currentPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->currentColorButton->getColor()).name())
                                                                                            .arg(QColor(ui->currentBackgroundButton->getColor()).name())
                                                                                            .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                            .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                            .arg(font.pointSize())
                                                                                            .arg(font.family()));
        } else {
            // Set Preview Label Style Sheet
            ui->currentPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->currentColorButton->getColor()).name())
                                                                                             .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                             .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                             .arg(font.pointSize())
                                                                                             .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->currentPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->currentColorButton->getColor()).name())
                                                                                            .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                            .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                            .arg(font.pointSize())
                                                                                            .arg(font.family()));
        }

        // Set Icon
        ui->currentPreviewIcon->setPixmap(defaultIcons[iconSize]);

        // Check Selected Background Color
        if (ui->selectedBackgroundButton->getColor() != -1) {
            // Set Preview Label Style Sheet
            ui->selectedPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->selectedColorButton->getColor()).name())
                                                                                              .arg(QColor(ui->selectedBackgroundButton->getColor()).name())
                                                                                              .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                              .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                              .arg(font.pointSize())
                                                                                              .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->selectedPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->selectedColorButton->getColor()).name())
                                                                                             .arg(QColor(ui->selectedBackgroundButton->getColor()).name())
                                                                                             .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                             .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                             .arg(font.pointSize())
                                                                                             .arg(font.family()));
        } else {
            // Set Preview Label Style Sheet
            ui->selectedPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->selectedColorButton->getColor()).name())
                                                                                              .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                              .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                              .arg(font.pointSize())
                                                                                              .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->selectedPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->selectedColorButton->getColor()).name())
                                                                                             .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                             .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                             .arg(font.pointSize())
                                                                                             .arg(font.family()));
        }

        // Set Icon
        ui->selectedPreviewIcon->setPixmap(defaultIcons[iconSize]);

        // Check Current Selected Background
        if (ui->currentSelectedBackgroundButton->getColor() != -1) {
            // Set Preview Label Style Sheet
            ui->currentSelectedPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->currentSelectedColorButton->getColor()).name())
                                                                                                     .arg(QColor(ui->currentSelectedBackgroundButton->getColor()).name())
                                                                                                     .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                                     .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                                     .arg(font.pointSize())
                                                                                                     .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->currentSelectedPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->currentSelectedColorButton->getColor()).name())
                                                                                                    .arg(QColor(ui->currentSelectedBackgroundButton->getColor()).name())
                                                                                                    .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                                    .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                                    .arg(font.pointSize())
                                                                                                    .arg(font.family()));
        } else {
            // Set Preview Label Style Sheet
            ui->currentSelectedPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->currentSelectedColorButton->getColor()).name())
                                                                                                     .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                                     .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                                     .arg(font.pointSize())
                                                                                                     .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->currentSelectedPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->currentSelectedColorButton->getColor()).name())
                                                                                                    .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                                    .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                                    .arg(font.pointSize())
                                                                                                    .arg(font.family()));
        }

        // Set Icon
        ui->currentSelectedPreviewIcon->setPixmap(defaultIcons[iconSize]);

        // Check Link Background Color
        if (ui->linkBackgroundButton->getColor() != -1) {
            // Set Preview Label Style Sheet
            ui->linkPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->linkColorButton->getColor()).name())
                                                                                          .arg(QColor(ui->linkBackgroundButton->getColor()).name())
                                                                                          .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                          .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                          .arg(font.pointSize())
                                                                                          .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->linkPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(QColor(ui->linkColorButton->getColor()).name())
                                                                                         .arg(QColor(ui->linkBackgroundButton->getColor()).name())
                                                                                         .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                         .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                         .arg(font.pointSize())
                                                                                         .arg(font.family()));
        } else {
            // Set Preview Label Style Sheet
            ui->linkPreviewLabel->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->linkColorButton->getColor()).name())
                                                                                          .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                          .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                          .arg(font.pointSize())
                                                                                          .arg(font.family()));

            // Set Preview Icon Style Sheet
            ui->linkPreviewIcon->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE).arg(QColor(ui->linkColorButton->getColor()).name())
                                                                                         .arg(font.bold() ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                                         .arg(font.italic() ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                                         .arg(font.pointSize())
                                                                                         .arg(font.family()));
        }

        // Set Icon
        ui->linkPreviewIcon->setPixmap(defaultIcons[iconSize]);

        // Set Layout
        ui->previewFrameLayout->setVerticalSpacing(ui->spacingSpinBox->value());
        ui->previewFrameLayout->setHorizontalSpacing(ui->spacingSpinBox->value());
        ui->previewFrameLayout->setMargin(ui->spacingSpinBox->value());
    }
}

//==============================================================================
// Change Event
//==============================================================================
void SettingsWindow::changeEvent(QEvent* aEvent)
{
    QFrame::changeEvent(aEvent);

    // Switch Event Type
    switch (aEvent->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
        break;

        default:
        break;
    }
}

//==============================================================================
// Apply Button Clicked Slot
//==============================================================================
void SettingsWindow::on_applyButton_clicked()
{
    // Save Settings
    saveSettings();
}

//==============================================================================
// Close Button Clicked Slot
//==============================================================================
void SettingsWindow::on_closeButton_clicked()
{
    // Close
    close();

    // Check Event Loop
    if (eventLoop) {
        // Exit Event Loop
        eventLoop->exit(0);
    }
}

//==============================================================================
// Defaults Button Clicked Slot
//==============================================================================
void SettingsWindow::on_defaultsButton_clicked()
{
    // Check Settings
    if (settings) {
        // Load Defaults
        settings->loadDefaults();
    }

    // Load Settings
    loadSettings();

    // Save Settings
    saveSettings();
}

//==============================================================================
// Export Button Clicked Slot
//==============================================================================
void SettingsWindow::on_exportButton_clicked()
{

}

//==============================================================================
// Import Button Clicked Slot
//==============================================================================
void SettingsWindow::on_importButton_clicked()
{

}

//==============================================================================
// Change Font Button Clicked Slot
//==============================================================================
void SettingsWindow::on_changefontButton_clicked()
{
    // Init Font Dialog
    QFontDialog fontDialog;
    // Set Window Modality
    fontDialog.setWindowModality(Qt::ApplicationModal);
    // Set Font
    fontDialog.setCurrentFont(font);
    // Exec Font Dialog
    if (fontDialog.exec()) {
        // Get Font
        font = fontDialog.currentFont();

        // Check UI
        if (ui) {
            // Set Font
            //ui->changefontButton->setFont(font);
            // Set Button Text
            ui->changefontButton->setText(QString(CHANGE_FONT_BUTTON_EXAMPLE_TEXT_TEMPLATE).arg(font.family()).arg(font.pointSize()));
        }
/*
        // Check Preview Frame
        if (ui && ui->previewFrame) {
            // Set Preview Frame Font
            ui->previewFrame->setFont(font);
        }
*/
        // Update Preview Frame
        updatePreviewFrame();
    }
}

//==============================================================================
// Icon Size Selector Combo Box Current Index Changed Slot
//==============================================================================
void SettingsWindow::on_iconSizeComboBox_currentIndexChanged(int index)
{
    // Set Icon Size
    iconSize = index;
    // Update Preview Frame
    updatePreviewFrame();
}

//==============================================================================
// Normal Item Text Color Button Clicked Slot
//==============================================================================
void SettingsWindow::normalColorButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->normalColorButton);
}

//==============================================================================
// Normal Item Background Color Button Clicked Slot
//==============================================================================
void SettingsWindow::normalBackgroundButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->normalBackgroundButton);
}

//==============================================================================
// Current Item Text Color Button Clicked Slot
//==============================================================================
void SettingsWindow::currentColorButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->currentColorButton);
}

//==============================================================================
// Current Item Background Color Button Clicked Slot
//==============================================================================
void SettingsWindow::currentBackgroundButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->currentBackgroundButton);
}

//==============================================================================
// Selected Item Text Color Button Clicked Slot
//==============================================================================
void SettingsWindow::selectedColorButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->selectedColorButton);
}

//==============================================================================
// Selected Item Background Color Button Clicked Slot
//==============================================================================
void SettingsWindow::selectedBackgroundButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->selectedBackgroundButton);
}

//==============================================================================
// Current Selected Item Text Color Button Clicked Slot
//==============================================================================
void SettingsWindow::currentSelectedColorButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->currentSelectedColorButton);
}

//==============================================================================
// Current Selected Item Background Color Button Clicked Slot
//==============================================================================
void SettingsWindow::currentSelectedBackgroundButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->currentSelectedBackgroundButton);
}

//==============================================================================
// Spacing List Box Value Changed
//==============================================================================
void SettingsWindow::on_spacingSpinBox_valueChanged(const QString& arg1)
{
    Q_UNUSED(arg1);

    // Update Preview Frame
    updatePreviewFrame();
}

//==============================================================================
// Link Item Text Color Button Clicked Slot
//==============================================================================
void SettingsWindow::linkColorButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->linkColorButton);
}

//==============================================================================
// Link Item Background Color Button Clicked Slot
//==============================================================================
void SettingsWindow::linkBackgroundButtonClicked()
{
    // Color Button Clicked
    colorButtonClicked(ui->linkBackgroundButton);
}

//==============================================================================
// Key Press Event
//==============================================================================
void SettingsWindow::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Key Pressed
        if (!keyPressed) {
            // Set Key Pressed
            keyPressed = true;

            // ...

            //qDebug() << "SettingsWindow::keyPressEvent - key: " << aEvent->key();
        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void SettingsWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Key Pressed
        if (keyPressed) {
            // Set Key Pressed
            keyPressed = false;
            // Check Key
            if (aEvent->key() == Qt::Key_Escape) {
                // Close
                on_closeButton_clicked();
            } else {
                //qDebug() << "SettingsWindow::keyReleaseEvent - key: " << aEvent->key();

                // ...
            }
        }
    }
}


//==============================================================================
// Destructor
//==============================================================================
SettingsWindow::~SettingsWindow()
{
    qDebug() << "Deleting SettingsWindow...";

    // Delete UI
    delete ui;

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        settings = NULL;
    }

    // Clear Default Icons
    defaultIcons.clear();

    qDebug() << "Deleting SettingsWindow...done";
}
