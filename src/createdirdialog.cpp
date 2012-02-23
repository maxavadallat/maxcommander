
// INCLUDES

#include "constants.h"
#include "createdirdialog.h"
#include "ui_createdirdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
CreateDirDialog::CreateDirDialog(QWidget* aParent)
    //: QFrame(aParent)
    : QDialog(aParent)
    , ui(new Ui::CreateDirDialog)
{
    // Set Window Modality
    setWindowModality(Qt::ApplicationModal);
    // Set Window Flags
    setWindowFlags(Qt::Dialog);
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Dir Text
//==============================================================================
void CreateDirDialog::setDirText(const QString& aDirPath)
{
    // Check UI
    if (ui && ui->dirNameEdit) {
        // Set Dir Text
        ui->dirNameEdit->setText(aDirPath);
    }
}

//==============================================================================
// Get Dir Text
//==============================================================================
QString CreateDirDialog::getDirText()
{
    // Check UI
    if (ui && ui->dirNameEdit) {
        // Return Dir Name
        return ui->dirNameEdit->text();
    }

    return QString("");
}

//==============================================================================
// Ok Button Clicked Slot
//==============================================================================
void CreateDirDialog::on_okButton_clicked()
{
    // Done
    done(DEFAULT_BUTTON_INDEX_OK);
}

//==============================================================================
// Cancel Button Clicked Slot
//==============================================================================
void CreateDirDialog::on_cancelButton_clicked()
{
    // Done
    done(DEFAULT_BUTTON_INDEX_CANCEL);
}

//==============================================================================
// Destructor
//==============================================================================
CreateDirDialog::~CreateDirDialog()
{
    // Delete UI
    delete ui;
}


