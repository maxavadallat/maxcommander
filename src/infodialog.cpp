
// INCLUDES

#include <QDebug>

#include "infodialog.h"
#include "ui_infodialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
InfoDialog::InfoDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::InfoDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Title
//==============================================================================
void InfoDialog::setTitle(const QString& aTitle)
{
    // Set Window Title
    setWindowTitle(aTitle);
}

//==============================================================================
// Set Info Message
//==============================================================================
void InfoDialog::setInfoMsg(const QString& aInfoMsg)
{
    // Check UI
    if (ui && ui->infoLabel) {
        // Set Info Label Text
        ui->infoLabel->setText(aInfoMsg);
    }
}

//==============================================================================
// Set Icon
//==============================================================================
void InfoDialog::setIcon(const QPixmap& aPixmap)
{
    // Check UI
    if (ui && ui->infoIcon) {
        // Set Pixmap
        ui->infoIcon->setPixmap(aPixmap);
    }
}

//==============================================================================
// Info Dialog Finished
//==============================================================================
void InfoDialog::on_InfoDialog_finished(int aResult)
{
    qDebug() << "InfoDialog::on_InfoDialog_finished - aResult: " << aResult;

    // Emit Info Dialog Finished Signal
    emit infoDialogFinished(this);
}

//==============================================================================
// Destructor
//==============================================================================
InfoDialog::~InfoDialog()
{
    delete ui;
}

