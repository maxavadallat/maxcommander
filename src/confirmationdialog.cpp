
// INCLUDES

#include "confirmationdialog.h"
#include "ui_confirmationdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
ConfirmationDialog::ConfirmationDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmationDialog)
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
void ConfirmationDialog::setButtons(const QDialogButtonBox::StandardButtons& aButtons)
{
    // Check UI
    if (ui && ui->buttonBox) {
        // Set Buttons
        ui->buttonBox->setStandardButtons(aButtons);
    }
}

//==============================================================================
// Destructor
//==============================================================================
ConfirmationDialog::~ConfirmationDialog()
{
    // Delete UI
    delete ui;
}
