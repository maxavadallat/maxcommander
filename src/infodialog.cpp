
#include "infodialog.h"
#include "ui_infodialog.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
InfoDialog::InfoDialog(const QString& aInfoText, const EInfoDialogType& aType, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::InfoDialog)
{
    // Setup UI
    ui->setupUi(this);
    // Set Text
    setText(aInfoText);
    // Set Type
    setType(aType);
}

//==============================================================================
// Set Text
//==============================================================================
void InfoDialog::setText(const QString& aInfoText)
{
    // Set Text
    ui->infoLabel->setText(aInfoText);
}

//==============================================================================
// Set Type
//==============================================================================
void InfoDialog::setType(const EInfoDialogType& aType)
{
    // Switch Type
    switch (aType) {
        default:
        case EIDTInfo:
            // Set Title
            setWindowTitle(tr(DEFAULT_TITLE_INFO));
            // Set Pixmap
            ui->iconLabel->setPixmap(QPixmap(DEFAULT_ICON_INFO));
        break;

        case EIDTWarning:
            // Set Title
            setWindowTitle(tr(DEFAULT_TITLE_WARNING));
            // Set Pixmap
            ui->iconLabel->setPixmap(QPixmap(DEFAULT_ICON_WARNING));
        break;

        case EIDTError:
            // Set Title
            setWindowTitle(tr(DEFAULT_TITLE_ERROR));
            // Set Pixmap
            ui->iconLabel->setPixmap(QPixmap(DEFAULT_ICON_ERROR));
        break;
    }
}

//==============================================================================
// Destructor
//==============================================================================
InfoDialog::~InfoDialog()
{
    delete ui;
}
