#include "src/confirmdialog.h"
#include "ui_confirmdialog.h"

//==============================================================================
// Constructor
//==============================================================================
ConfirmDialog::ConfirmDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Set Confirm Text
//==============================================================================
void ConfirmDialog::setConfirmText(const QString& aConfirmText)
{
    ui->confirmTextLabel->setText(aConfirmText);
}

//==============================================================================
// Configure Buttons
//==============================================================================
void ConfirmDialog::configureButtons(const QDialogButtonBox::StandardButtons& aButtons)
{
    ui->buttonBox1->setStandardButtons(aButtons);
}

//==============================================================================
// Add Button
//==============================================================================
void ConfirmDialog::addButton(const QString& aText, const QDialogButtonBox::ButtonRole& aButtonRole)
{
    ui->buttonBox1->addButton(aText, aButtonRole);
}

//==============================================================================
// Clear Buttons
//==============================================================================
void ConfirmDialog::clearButtons()
{
    ui->buttonBox1->clear();
}

//==============================================================================
// Destructor
//==============================================================================
ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}
