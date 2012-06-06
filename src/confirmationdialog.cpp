
// INCLUDES

#include <QPushButton>
#include <QDebug>

#include "confirmationdialog.h"
#include "ui_confirmationdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ConfirmationDialog::ConfirmationDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmationDialog)
    , buttonGroup(NULL)
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
// Add Button
//==============================================================================
void ConfirmationDialog::addButton(const QString& aButtonText, QDialogButtonBox::ButtonRole aButtonRole, const int& aID)
{
    // Check UI
    if (ui && ui->buttonBox && !aButtonText.isEmpty()) {
        // Add Button
        QPushButton* newButton = ui->buttonBox->addButton(aButtonText, aButtonRole);
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
        }
    }
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

        // Clear Custom Buttons
        clearCustomButtons();
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
