
// INCLUDES

#include "searchdialog.h"
#include "ui_searchdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
SearchDialog::SearchDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
SearchDialog::~SearchDialog()
{
    delete ui;
}
