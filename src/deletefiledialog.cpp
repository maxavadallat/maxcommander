#include "deletefiledialog.h"
#include "ui_deletefiledialog.h"

//==============================================================================
// Constructor
//==============================================================================
DeleteFileDialog::DeleteFileDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteFileDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Get File Name
//==============================================================================
QString DeleteFileDialog::getFileName()
{
    return ui->fileNameEditor->text();
}

//==============================================================================
// Set File Name
//==============================================================================
void DeleteFileDialog::setFileName(const QString& aFileName)
{
    // Set Text
    ui->fileNameEditor->setText(aFileName);
}

//==============================================================================
// Destructor
//==============================================================================
DeleteFileDialog::~DeleteFileDialog()
{
    delete ui;
}
