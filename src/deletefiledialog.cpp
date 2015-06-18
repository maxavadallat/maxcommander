#include "deletefiledialog.h"
#include "ui_deletefiledialog.h"

//==============================================================================
// Constructor
//==============================================================================
DeleteFileDialog::DeleteFileDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteFileDialog)
    , editorTextChanged(false)
{
    // Setup UI
    ui->setupUi(this);

    // Connect Signal
    connect(ui->fileNameEditor, SIGNAL(textChanged(QString)), this, SLOT(fileNameEditorTextChanged(QString)));
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
// Exec
//==============================================================================
int DeleteFileDialog::exec()
{
    // Reset Editor Text Changed
    editorTextChanged = false;

    return QDialog::exec();
}

//==============================================================================
// Get Editor Text Changed
//==============================================================================
bool DeleteFileDialog::getTextChanged()
{
    return editorTextChanged;
}

//==============================================================================
// File Name Editor Text Changed
//==============================================================================
void DeleteFileDialog::fileNameEditorTextChanged(const QString& aNewText)
{
    Q_UNUSED(aNewText);

    // Set Editor Text Changed
    editorTextChanged = true;
}

//==============================================================================
// Destructor
//==============================================================================
DeleteFileDialog::~DeleteFileDialog()
{
    delete ui;
}
