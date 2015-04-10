#include <QDebug>

#include "createdirdialog.h"
#include "ui_createdirdialog.h"

//==============================================================================
// Constructor
//==============================================================================
CreateDirDialog::CreateDirDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CreateDirDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Get Directory Name
//==============================================================================
QString CreateDirDialog::getDirectory()
{
    return ui->directoryPathEdit->text();
}

//==============================================================================
// Set Directory Path
//==============================================================================
void CreateDirDialog::setDirPath(const QString& aDirPath)
{
    // Set Text
    ui->directoryPathEdit->setText(aDirPath);
    // Set Cursor Position
    ui->directoryPathEdit->setCursorPosition(aDirPath.length());
}

//==============================================================================
// Exec
//==============================================================================
int CreateDirDialog::exec()
{
    // Set Selection
    ui->directoryPathEdit->setSelection(ui->directoryPathEdit->text().length(), ui->directoryPathEdit->text().length());
    // Clear Focus
    ui->directoryPathEdit->clearFocus();

    // Exec Dialog
    int result = QDialog::exec();

    qDebug() << "CreateDirDialog::exec - result: " << result;

    return result;
}

//==============================================================================
// Destructor
//==============================================================================
CreateDirDialog::~CreateDirDialog()
{
    delete ui;
}

