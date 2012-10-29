
// INCLUDES

#include <QPushButton>
#include <QDebug>

#include "confirmationdialog.h"
#include "ui_confirmationdialog.h"
#include "constants.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ConfirmationDialog::ConfirmationDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmationDialog)
    , buttonGroup(NULL)
    , clearCustomButtonsOnConfirm(true)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Title
//==============================================================================
void ConfirmationDialog::setTitle(const QString& aTitle)
{
    // Set Window Title
    setWindowTitle(aTitle);
}

//==============================================================================
// Set Confirmation Message
//==============================================================================
void ConfirmationDialog::setConfirmMsg(const QString& aInfoMsg)
{
    // Check UI
    if (ui && ui->confirmLabel) {
        // Set Confirm Label Text
        ui->confirmLabel->setText(aInfoMsg);
    }

    // Update Size
    updateSize();
}

//==============================================================================
// Set Icon
//==============================================================================
void ConfirmationDialog::setIcon(const QPixmap& aPixmap)
{
    // Check UI
    if (ui && ui->confirmIcon) {
        // Set Pixmap
        ui->confirmIcon->setPixmap(aPixmap);
    }
}

//==============================================================================
// Set Buttons
//==============================================================================
void ConfirmationDialog::setButtons(const QDialogButtonBox::StandardButtons& aButtons, const QDialogButtonBox::StandardButton& aDefault)
{
    // Check UI
    if (ui && ui->buttonBox) {
        // Set Buttons
        ui->buttonBox->setStandardButtons(aButtons);
        // Get Button
        QPushButton* button = ui->buttonBox->button(aDefault);
        // Check Button
        if (button) {
            // Set Default
            button->setDefault(true);
        }
    }
}

//==============================================================================
// Clear Buttons
//==============================================================================
void ConfirmationDialog::clearButtons()
{
    // Clear Custom Buttons
    clearCustomButtons();

    // Check UI
    if (ui && ui->buttonBox) {
        // Clear Buttons
        ui->buttonBox->clear();
    }
}

//==============================================================================
// Add Button
//==============================================================================
void ConfirmationDialog::addButton(const QString& aButtonText, QDialogButtonBox::ButtonRole aButtonRole, const int& aID, const bool& aDefault)
{
    // Check UI
    if (ui && ui->buttonBox && !aButtonText.isEmpty()) {
        // Add Button
        QPushButton* newButton = ui->buttonBox->addButton(aButtonText, aButtonRole);
        // Set Button Minimal Width
        newButton->setMinimumWidth(DEFAULT_BUTTON_MINIMAL_WIDTH);
        // Set Default
        newButton->setDefault(aDefault);
        // Set Focus Policy
        newButton->setFocusPolicy(Qt::StrongFocus);

        // Check Button Group
        if (!buttonGroup) {
            // Create Button Group
            buttonGroup = new QButtonGroup();
        }
        // Check Button Group
        if (buttonGroup) {
            // Add Button
            buttonGroup->addButton(newButton, aID);
        } else {
            // Remove New Button
            ui->buttonBox->removeButton(newButton);
            // Delete New Button
            delete newButton;
            newButton = NULL;
        }

        // Update Size
        updateSize();
    }
}

//==============================================================================
// Set Clear Custom Buttons On Confirm
//==============================================================================
void ConfirmationDialog::setClearCustomButtonsOnConfirm(const bool& aClear)
{
    // Set Clear Custom Buttons On Confirm
    clearCustomButtonsOnConfirm = aClear;
}

//==============================================================================
// On Button Clicked
//==============================================================================
void ConfirmationDialog::on_buttonBox_clicked(QAbstractButton* aButton)
{
    // Check Button
    if (aButton) {
        // Get Button Group
        QButtonGroup* buttonGroup = aButton->group();
        // Check Button Group
        if (buttonGroup) {
            // Get ID
            int buttonID = buttonGroup->id(aButton);

            //qDebug() << "ConfirmationDialog::on_buttonBox_clicked - buttonID: " << buttonID;

            // Set Dialog Result
            setResult(buttonID);
            // Dialog Done
            done(buttonID);
        }

        // Check Clear Custom Buttons On Confirm
        if (clearCustomButtonsOnConfirm) {
            // Clear Custom Buttons
           clearCustomButtons();
        }
    }
}

//==============================================================================
// Clear Custom Buttons
//==============================================================================
void ConfirmationDialog::clearCustomButtons()
{
    // Check Button Group
    if (buttonGroup) {
        //qDebug() << "ConfirmationDialog::clearCustomButtons";
        // Get Custom Buttons Count
        int cbCount = buttonGroup->buttons().count();

        // Go Thru Buttons
        for (int i=cbCount-1; i>=0; --i) {
            // Get Button
            QAbstractButton* button = buttonGroup->buttons().at(i);

            // Check Button
            if (button) {
                // UI & Check Button Box
                if (ui && ui->buttonBox) {
                    // Remove Button From Button Box
                    ui->buttonBox->removeButton(button);
                }

                // Remove Button From Buttong Group
                buttonGroup->removeButton(button);

                // Delete Button
                delete button;
                button = NULL;
            }
        }

        // Clear Buttons
        //buttonGroup->buttons().clear();

        // Delete Button Group
        delete buttonGroup;
        buttonGroup = NULL;
    }
}

//==============================================================================
// Update Size
//==============================================================================
void ConfirmationDialog::updateSize()
{
    qDebug() << "ConfirmationDialog::updateSize";

    // Check UI
    if (ui && ui->buttonBox && ui->confirmIcon && ui->confirmLabel) {
        // Calculate Minimum Width
        int minWidth = qMax(ui->buttonBox->buttons().count() * (DEFAULT_BUTTON_MINIMAL_WIDTH + DEFAULT_BUTTON_GAP) + DEFAULT_BUTTON_GAP * 2,
                            ui->confirmIcon->width() + ui->confirmLabel->minimumWidth() + DEFAULT_BUTTON_GAP * 3);

        // Set Minimum Width
        setMinimumWidth(minWidth);

        // Set Width
        resize(minWidth, minimumHeight());
    }
}

//==============================================================================
// Destructor
//==============================================================================
ConfirmationDialog::~ConfirmationDialog()
{
    //qDebug() << "Deleting ConfirmationDialog";

    // Clear Custom Buttons
    clearCustomButtons();
    // Delete UI
    delete ui;

    //qDebug() << "Deleting ConfirmationDialog...done";
}
