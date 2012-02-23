
// INCLUDES

#include "copyprogressdialog.h"
#include "ui_copyprogressdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
CopyProgressDialog::CopyProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CopyProgressDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
CopyProgressDialog::~CopyProgressDialog()
{
    delete ui;
}
