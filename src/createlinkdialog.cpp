#include "src/createlinkdialog.h"
#include "ui_createlinkdialog.h"

//==============================================================================
// Constructor
//==============================================================================
CreateLinkDialog::CreateLinkDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CreateLinkDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// On Browse Target Button Clicked Slot
//==============================================================================
void CreateLinkDialog::on_browseTargetButton_clicked()
{

}

//==============================================================================
// Destructor
//==============================================================================
CreateLinkDialog::~CreateLinkDialog()
{
    delete ui;
}

