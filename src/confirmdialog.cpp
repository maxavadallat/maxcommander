#include <QPushButton>
#include <QDebug>

#include "confirmdialog.h"
#include "ui_confirmdialog.h"

//==============================================================================
// Constructor
//==============================================================================
ConfirmDialog::ConfirmDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmDialog)
    , actionIndex(-1)
{
    qDebug() << "ConfirmDialog::ConfirmDialog";

    // Setup UI
    ui->setupUi(this);

    // Connect Signal
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
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
    ui->buttonBox->setStandardButtons(aButtons);
}

//==============================================================================
// Add Button
//==============================================================================
void ConfirmDialog::addButton(const QString& aText, const QDialogButtonBox::ButtonRole& aButtonRole, const int& aActionIndex)
{
    // Add Button
    QPushButton* pushButton = ui->buttonBox->addButton(aText, aButtonRole);

    // Add Button To Custom Buttons
    customButtons[pushButton] = aActionIndex;
}

//==============================================================================
// Clear Buttons
//==============================================================================
void ConfirmDialog::clearButtons()
{
    // Clear Button Box
    ui->buttonBox->clear();
    // Clear Custom Buttons
    customButtons.clear();
}

//==============================================================================
// Button Clicked Slot
//==============================================================================
void ConfirmDialog::buttonClicked(QAbstractButton* aButton)
{
    // Check Button
    if (aButton) {

        // Get Action Index
        actionIndex = customButtons[aButton];

        qDebug() << "ConfirmDialog::buttonClicked - actionIndex: " << actionIndex;

    } else {
        // Reset Action Index
        actionIndex = -1;
    }
}

//==============================================================================
// Get Action Index
//==============================================================================
int ConfirmDialog::getActionIndex()
{
    return actionIndex;
}

//==============================================================================
// Set Path
//==============================================================================
void ConfirmDialog::setPath(const QString& aPath)
{
    // Set Text
    ui->pathEdit->setText(aPath);
}

//==============================================================================
// Get Path
//==============================================================================
QString ConfirmDialog::getPath()
{
    return ui->pathEdit->text();
}

//==============================================================================
// Exec
//==============================================================================
int ConfirmDialog::exec()
{
    // Check Path Edit Tewxt Length
    if (ui->pathEdit->text().length() <= 0) {
        // Set Visibility
        ui->pathEdit->setVisible(false);
    } else {
        // Set Visibility
        ui->pathEdit->setVisible(true);
    }

    // Exec Dialog
    int result = QDialog::exec();

    qDebug() << "ConfirmDialog::exec - result: " << result;

    // Check Result

    // Clear Buttons
    clearButtons();

    return result;
}

//==============================================================================
// Destructor
//==============================================================================
ConfirmDialog::~ConfirmDialog()
{
    // Clear Buttons
    clearButtons();

    delete ui;

    qDebug() << "ConfirmDialog::~ConfirmDialog";
}
