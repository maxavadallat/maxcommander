#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

// INCLUDES

#include <QFrame>
#include <QEventLoop>


// FORWARD DECLARATIONS

namespace Ui {
class SettingsWindow;
}

class Settings;
class ColorPickerButton;


// DECLARATIONS

//==============================================================================
//! @class SettingsWindow Application Settings Window Class
//==============================================================================
class SettingsWindow : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aEventLoop Event Loop
    //! @param aParent Parent
    explicit SettingsWindow(QEventLoop* aEventLoop = NULL, QWidget* aParent = NULL);

    //! @brief Load Settings
    //! @param none
    void loadSettings();

    //! @brief Save Settings
    //! @param none
    void saveSettings();

    //! @brief Destructor
    //! @param none
    virtual ~SettingsWindow();

protected:

    //! @brief Color Button Clicked
    //! @param aButton Color Picker Button
    void colorButtonClicked(ColorPickerButton* aButton);

    //! @brief Update Preview Frame
    //! @param none
    void updatePreviewFrame();

protected slots:

    //! @brief On Apply Button Clicked Slot
    //! @param none
    void on_applyButton_clicked();

    //! @brief On Close Button Clicked Slot
    //! @param none
    void on_closeButton_clicked();

    //! @brief On Default Button Clicked Slot
    //! @param none
    void on_defaultsButton_clicked();

    //! @brief On Export Button Clicked Slot
    //! @param none
    void on_exportButton_clicked();

    //! @brief On Import Button Clicked Slot
    //! @param none
    void on_importButton_clicked();

    //! @brief Change Font Button Clicked Slot
    //! @param none
    void on_changefontButton_clicked();

    //! @brief Icon Size Selector Combo Box Current Index Changed Slot
    //! @param none
    void on_iconSizeComboBox_currentIndexChanged(int index);

    //! @brief Normal Item Text Color Button Clicked Slot
    //! @param none
    void normalColorButtonClicked();

    //! @brief Normal Item Background Color Button Clicked Slot
    //! @param none
    void normalBackgroundButtonClicked();

    //! @brief Current Item Text Color Button Clicked Slot
    //! @param none
    void currentColorButtonClicked();

    //! @brief Current Item Background Color Button Clicked Slot
    //! @param none
    void currentBackgroundButtonClicked();

    //! @brief Selected Item Text Color Button Clicked Slot
    //! @param none
    void selectedColorButtonClicked();

    //! @brief Selected Item Background Color Button Clicked Slot
    //! @param none
    void selectedBackgroundButtonClicked();

    //! @brief Current Selected Item Text Color Button Clicked Slot
    //! @param none
    void currentSelectedColorButtonClicked();

    //! @brief Current Selected Item Background Color Button Clicked Slot
    //! @param none
    void currentSelectedBackgroundButtonClicked();

    //! @brief Link Item Text Color Button Clicked Slot
    //! @param none
    void linkColorButtonClicked();

    //! @brief Link Item Background Color Button Clicked Slot
    //! @param none
    void linkBackgroundButtonClicked();

    //! @brief Spacing List Box Value Changed
    //! @param arg1 Argument
    void on_spacingSpinBox_valueChanged(const QString& arg1);

protected: // From QFrame

    //! @brief Change Event
    //! @param aEvent Event
    void changeEvent(QEvent* aEvent);

    //! @brief Key Press Event
    //! @param aEvent Key Event
    virtual void keyPressEvent(QKeyEvent* aEvent);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

protected: // Data

    //! UI
    Ui::SettingsWindow* ui;
    //! Event Loop
    QEventLoop*         eventLoop;
    //! Settings
    Settings*           settings;
    //! Font
    QFont               font;
    //! Default Icon Pixmaps
    QList<QPixmap>      defaultIcons;
    //! Icon Size Index
    int                 iconSize;
    //! Key Pressed
    bool                keyPressed;
};

#endif // SETTINGSWINDOW_H
