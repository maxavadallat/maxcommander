#include <QDebug>
#include <QFileDialog>

#include "src/comparedialog.h"
#include "ui_comparedialog.h"

#include "constants.h"

//==============================================================================
//  Constructor
//==============================================================================
CompareDialog::CompareDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CompareDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Get Left File Exists
//==============================================================================
bool CompareDialog::leftFileExists()
{
    return leftFileInfo.exists();
}

//==============================================================================
// Get Left File Name
//==============================================================================
QString CompareDialog::getLeftFile()
{
    return ui->leftFileEditor->text();
}

//==============================================================================
// Set Left File Name
//==============================================================================
void CompareDialog::setLeftFile(const QString& aLeftFile)
{
//    // Check Left File
//    if (leftFile != aLeftFile) {
//        // Set Left File
//        leftFile = aLeftFile;
//        // Emit Left File Changed Signal
//        emit leftFileChanged(leftFile);
//    }
    // Set Text
    ui->leftFileEditor->setText(aLeftFile);
}

//==============================================================================
// Get Right File Exists
//==============================================================================
bool CompareDialog::rightFileExists()
{
    return rightFileInfo.exists();
}

//==============================================================================
// Get Right File Name
//==============================================================================
QString CompareDialog::getRightFile()
{
    return ui->rightFileEditor->text();
}

//==============================================================================
// Set Right File Name
//==============================================================================
void CompareDialog::setRightFile(const QString& aRightFile)
{
//    // Check Right File
//    if (rightFile != aRightFile) {
//        // Set Right File
//        rightFile = aRightFile;
//        // Emit Right File Name Changed Signal
//        emit rightFileChanged(rightFile);
//    }
    // Set Text
    ui->rightFileEditor->setText(aRightFile);
}

//==============================================================================
// On Left File Editor Text Changed Slot
//==============================================================================
void CompareDialog::on_leftFileEditor_textChanged(const QString &arg1)
{
    qDebug() << "CompareDialog::on_leftFileEditor_textChanged - arg1: " << arg1;

    // Set Left File Info
    leftFileInfo.setFile(arg1);
    // Check If File Exists
    if (leftFileInfo.exists()) {
        // Clear Style Sheet
        ui->leftFileEditor->setStyleSheet("");
    } else {
        // Set Style Sheet
        ui->leftFileEditor->setStyleSheet(QString(DEFAULT_STYLE_SHEET_BGCOLOR).arg(DEFAULT_BG_COLOR_FILE_DOES_NOT_EXISTS));
    }

    // ...
}

//==============================================================================
// On Right File Editor Text Changed Slot
//==============================================================================
void CompareDialog::on_rightFileEditor_textChanged(const QString &arg1)
{
    qDebug() << "CompareDialog::on_rightFileEditor_textChanged - arg1: " << arg1;

    // Set Right File Info
    rightFileInfo.setFile(arg1);

    // Check If File Exists
    if (rightFileInfo.exists()) {
        // Clear Style Sheet
        ui->rightFileEditor->setStyleSheet("");
    } else {
        // Set Style Sheet
        ui->rightFileEditor->setStyleSheet(QString(DEFAULT_STYLE_SHEET_BGCOLOR).arg(DEFAULT_BG_COLOR_FILE_DOES_NOT_EXISTS));
    }

    // ...
}

//==============================================================================
// Left File Browser Button Pressed Slot
//==============================================================================
void CompareDialog::on_leftBrowseButton_clicked()
{
    // Init Left File Info
    QFileInfo leftFileInfo(ui->leftFileEditor->text());
    // Init File Dialog
    QFileDialog dialog(this, tr("Select Left File/Dir To Compare"), leftFileInfo.absolutePath());
    // Set File Mode
    dialog.setFileMode(QFileDialog::AnyFile);
    // Exec Dialog
    if (dialog.exec() && dialog.selectedFiles().count() > 0) {
        // Get Left File Path
        QString leftFilePath = dialog.selectedFiles()[0];
        // Set Left File Editor Text
        ui->leftFileEditor->setText(leftFilePath);
    }
}

//==============================================================================
// Right File Browser Button Pressed Slot
//==============================================================================
void CompareDialog::on_rightBrowseButton_clicked()
{
    // Init Right File Info
    QFileInfo rightFileInfo(ui->rightFileEditor->text());
    // Init File Dialog
    QFileDialog dialog(this, tr("Select Right File/Dir To Compare"), rightFileInfo.absolutePath());
    // Set File Mode
    dialog.setFileMode(QFileDialog::AnyFile);
    // Exec Dialog
    if (dialog.exec() && dialog.selectedFiles().count() > 0) {
        // Get Right File Path
        QString rightFilePath = dialog.selectedFiles()[0];
        // Set Right File Editor Text
        ui->rightFileEditor->setText(rightFilePath);
    }
}

//==============================================================================
// Destructor
//==============================================================================
CompareDialog::~CompareDialog()
{
    delete ui;
}

