
// INCLUDES

#include "treewindow.h"
#include "ui_treewindow.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
TreeWindow::TreeWindow(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TreeWindow)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
TreeWindow::~TreeWindow()
{
    // Delete UI
    delete ui;
}

