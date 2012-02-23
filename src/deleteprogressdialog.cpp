
// INCLUDES

#include "deleteprogressdialog.h"
#include "ui_deleteprogressdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
DeleteProgressDialog::DeleteProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteProgressDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressDialog::~DeleteProgressDialog()
{
    delete ui;
}
