#include <QPushButton>
#include <QDebug>

#include "confirmdialog.h"
#include "ui_confirmdialog.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
ConfirmDialog::ConfirmDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmDialog)
    , actionIndex(-1)
{
    //qDebug() << "ConfirmDialog::ConfirmDialog";

    // Setup UI
    ui->setupUi(this);

    // Connect Signal
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

//==============================================================================
// Set Confirm Title
//==============================================================================
void ConfirmDialog::setConfirmTitle(const QString& aTitleText)
{
    // Set Window Title
    setWindowTitle(aTitleText);
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
    // Set Standard Buttons
    ui->buttonBox->setStandardButtons(aButtons);

    // Set Minimal Width
    setMinimumWidth(ui->buttonBox->buttons().count() * DEFAULT_CONFIRM_BUTTON_MINIMAL_WIDTH);
}

//==============================================================================
// Add Button
//==============================================================================
void ConfirmDialog::addCustomButton(const QString& aText, const QDialogButtonBox::ButtonRole& aButtonRole, const int& aActionIndex)
{
    //qDebug() << "ConfirmDialog::addCustomButton - aText: " << aText << " - aActionIndex: " << aActionIndex;

    // Add Button
    QPushButton* pushButton = ui->buttonBox->addButton(aText, aButtonRole);

    // Add Button To Custom Buttons
    customButtons[pushButton] = aActionIndex;

    // Set Minimal Width
    setMinimumWidth(ui->buttonBox->buttons().count() * DEFAULT_CONFIRM_BUTTON_MINIMAL_WIDTH);
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

    // Set Button Box Minimal Size
    //ui->buttonBox->setMinimumWidth(0);
}

//==============================================================================
// Button Clicked Slot
//==============================================================================
void ConfirmDialog::buttonClicked(QAbstractButton* aButton)
{
    // Check Button
    if (aButton) {
        // Check Custom Buttons Keys
        if (customButtons.uniqueKeys().count() > 0 && customButtons.uniqueKeys().indexOf(aButton) >= 0) {

            // Get Action Index
            actionIndex = customButtons[aButton];

            //qDebug() << "ConfirmDialog::buttonClicked - customButtons.keys: " << customButtons.keys();
            //qDebug() << "ConfirmDialog::buttonClicked - customButtons.values: " << customButtons.values();

            //qDebug() << "ConfirmDialog::buttonClicked - actionIndex: " << actionIndex;
        } else {

            // Reset Action Index
            actionIndex = -1;

            //qDebug() << "ConfirmDialog::buttonClicked - NO MATCHING CUSTOM BUTTON!! - actionIndex: " << actionIndex;
        }
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
        // Set Geometry
        ui->verticalSpacer->setGeometry(QRect(0, 0, 0, 0));
    } else {
        // Set Visibility
        ui->pathEdit->setVisible(true);
        // Set Geometry
        ui->verticalSpacer->setGeometry(QRect(0, 0, 20, 8));
    }

    // Exec Dialog
    int result = QDialog::exec();

    //qDebug() << "ConfirmDialog::exec - result: " << result;

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

    //qDebug() << "ConfirmDialog::~ConfirmDialog";
}

