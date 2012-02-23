
// INCLUDES

#include <QDebug>

#include "copydialog.h"
#include "ui_copydialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
CopyDialog::CopyDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CopyDialog)
{
    // Setup UI
    ui->setupUi(this);

    qDebug() << "Creating CopyDialog...done";
}

//==============================================================================
// Set Dialog Title Text
//==============================================================================
void CopyDialog::setTitle(const QString& aTitle)
{
    // Check Title
    if (!aTitle.isEmpty()) {
        // Set Window Title
        setWindowTitle(aTitle);
    }
}

//==============================================================================
// Set Dialog Copy Msg Text
//==============================================================================
void CopyDialog::setCopyMsg(const QString& aCopyMsg)
{
    // Check Copy Message
    if (!aCopyMsg.isEmpty() && ui && ui->copyMsgLabel) {
        // Set Text
        ui->copyMsgLabel->setText(aCopyMsg);
    }
}

//==============================================================================
// Set Target Text
//==============================================================================
void CopyDialog::setTarget(const QString& aTarget)
{
    // Check UI
    if (ui && ui->targetEdit) {
        // Set Target Edit Text
        ui->targetEdit->setText(aTarget);
    }
}

//==============================================================================
// Get Target Text
//==============================================================================
QString CopyDialog::getTarget()
{
    // Check UI
    if (ui && ui->targetEdit) {
        // Return Target
        return ui->targetEdit->text();
    }

    return QString("");
}

//==============================================================================
// Destructor
//==============================================================================
CopyDialog::~CopyDialog()
{
    // Delete UI
    delete ui;
}
