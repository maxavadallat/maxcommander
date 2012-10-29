
// INCLUDES

#include "adminpassdialog.h"
#include "ui_adminpassdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
AdminPassDialog::AdminPassDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::AdminPassDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Password
//==============================================================================
void AdminPassDialog::setPassword(const QString& aPassword)
{
    // Check UI
    if (ui && ui->passwordEdit) {
        // Set Password Text
        ui->passwordEdit->setText(aPassword);
    }
}

//==============================================================================
// Get Password
//==============================================================================
QString AdminPassDialog::getPassword()
{
    // Check UI
    if (ui && ui->passwordEdit) {
        // Return Password Text
        return ui->passwordEdit->text();
    }

    return QString("");
}

//==============================================================================
// Destructor
//==============================================================================
AdminPassDialog::~AdminPassDialog()
{
    // Delete UI
    delete ui;
}
