#include "aboutdialog.h"
#include "ui_aboutdialog.h"

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
    delete ui;
}
