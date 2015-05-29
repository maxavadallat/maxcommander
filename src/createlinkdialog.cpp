#include <QFileDialog>

#include "createlinkdialog.h"
#include "ui_createlinkdialog.h"
#include "utility.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
CreateLinkDialog::CreateLinkDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CreateLinkDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Launch Dialog
//==============================================================================
int CreateLinkDialog::launchDialog(const QString& aStartDir)
{
    // Set Start Dir
    startDir = aStartDir;

    // Chekc Dir Path
    if (!startDir.endsWith("/")) {
        // Adjust Dir Path
        startDir += "/";
    }

    // Reset Link Name
    ui->linkNameEdit->setText("");

    // Set Link Target
    ui->linkTargetEdit->setText(startDir);

    return exec();
}

//==============================================================================
// Get Link Name
//==============================================================================
QString CreateLinkDialog::getLinkName()
{
    return ui->linkNameEdit->text();
}

//==============================================================================
// Get Link Target
//==============================================================================
QString CreateLinkDialog::getLinkTarget()
{
    return ui->linkTargetEdit->text();
}

//==============================================================================
// On Browse Target Button Clicked Slot
//==============================================================================
void CreateLinkDialog::on_browseTargetButton_clicked()
{
    // Get Current Dir
    QString currentDir = getDirPath(ui->linkTargetEdit->text());

    // Init New File Dialog
    QFileDialog fileDialog(NULL, tr(DEFAULT_TITLE_SELECT_LINK_TARGET), currentDir);

    // Set File Mode
    fileDialog.setFileMode(QFileDialog::AnyFile);
    // Set Accept Mode
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    // Set Options
    fileDialog.setOptions(QFileDialog::ReadOnly);

    // Exec
    if (fileDialog.exec()) {
        // Get New Target Name
        QString newTargetName = fileDialog.selectedFiles().count() > 0 ? fileDialog.selectedFiles()[0] : "";

        // Check New Target Name
        if (!newTargetName.isEmpty()) {
            // Set Link Target
            ui->linkTargetEdit->setText(newTargetName);
        }
    }

    // ...
}

//==============================================================================
// Exec Dialog
//==============================================================================
int CreateLinkDialog::exec()
{
    return QDialog::exec();
}

//==============================================================================
// Destructor
//==============================================================================
CreateLinkDialog::~CreateLinkDialog()
{
    delete ui;
}

