#include "src/passworddialog.h"
#include "ui_passworddialog.h"


//==============================================================================
// Constructor
//==============================================================================
PasswordDialog::PasswordDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PasswordDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
PasswordDialog::~PasswordDialog()
{
    delete ui;
}
