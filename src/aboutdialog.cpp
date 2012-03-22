
// INCLUDES

#include "aboutdialog.h"
#include "ui_aboutdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
AboutDialog::AboutDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::AboutDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
AboutDialog::~AboutDialog()
{
    // Delete UI
    delete ui;
}


