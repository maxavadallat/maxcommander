#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "selectfilesdialog.h"
#include "ui_selectfilesdialog.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
SelectFilesDialog::SelectFilesDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::SelectFilesDialog)
    , pattern("")
    , mode(EFSMSelect)
{
    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void SelectFilesDialog::init()
{
    // Load File Select Patterns
    //loadFileSelectPatterns();
}

//==============================================================================
// Restore UI
//==============================================================================
void SelectFilesDialog::restoreUI()
{
    // Check Mode
    if (mode == EFSMSelect) {
        // Set Window Title
        setWindowTitle(tr(DEFAULT_TITLE_SELECT_FILES));
    } else {
        // Set Window Title
        setWindowTitle(tr(DEFAULT_TITLE_DESELECT_FILES));
    }

    // Insert Item
    ui->selectionPatternCombo->insertItem(0, "*.*");

    // Set Current Index
    ui->selectionPatternCombo->setCurrentIndex(0);
}

//==============================================================================
// Get Selection Pattern
//==============================================================================
QString SelectFilesDialog::getPattern()
{
    return pattern;
}

//==============================================================================
// Launch Dialog
//==============================================================================
int SelectFilesDialog::launchDialog(const FileSelectionModeType& aMode)
{
    // Set Mode
    mode = aMode;

    // Load Selection Patterns
    loadFileSelectPatterns();

    // Restore UI
    restoreUI();

    // Exec
    int result = exec();

    // Clear Focus
    ui->selectionPatternCombo->clearFocus();

    return result;
}

//==============================================================================
// Load File Select Patterns
//==============================================================================
void SelectFilesDialog::loadFileSelectPatterns()
{
    //qDebug() << "SelectFilesDialog::loadFileSelectPatterns";

    // Clear File Patterns
    ui->selectionPatternCombo->clear();

    // Init File Select Pattern List File
    QFile fspFile(QDir::homePath() + "/" + DEFAULT_FILE_SELECT_FILE_NAME_PATTERN_LIST);

    // Open File
    if (fspFile.open(QIODevice::ReadOnly)) {
        // Init Text Stream
        QTextStream fspStream(&fspFile);

        // Go Thru File Pattern List
        while (!fspStream.atEnd()) {
            // Read Line
            QString line = fspStream.readLine();
            // Append To List
            ui->selectionPatternCombo->addItem(line);
        }

        // Close File
        fspFile.close();
    }
}

//==============================================================================
// Save File Select Patterns
//==============================================================================
void SelectFilesDialog::saveFileSelectPatterns()
{
    // Get File Select Patterns Count
    int fspCount = ui->selectionPatternCombo->count();

    // Check Count
    if (fspCount <= 0) {
        return;
    }

    //qDebug() << "SelectFilesDialog::saveFileSelectPatterns";

    // Init File Search Pattern List File
    QFile fspFile(QDir::homePath() + "/" + DEFAULT_FILE_SELECT_FILE_NAME_PATTERN_LIST);

    // Open File
    if (fspFile.open(QIODevice::WriteOnly)) {
        // Init Text Stream
        QTextStream fspStream(&fspFile);

        // Go Thru File Select Patterns
        for (int i=0; i<fspCount; ++i) {
            // Get Item Text
            QString itemText = ui->selectionPatternCombo->itemText(i);

            // Check Item Text
            if (itemText == QString("*.*")) {
                continue;
            }

            // Write Item
            fspStream << itemText;
            // Write New Line
            fspStream << "\n";
        }

        // Close File
        fspFile.close();
    }
}

//==============================================================================
// Exec Dialog
//==============================================================================
int SelectFilesDialog::exec()
{
    // Exec Dialog
    int result = QDialog::exec();

    // Check Result
    if (result) {
        // Check File Selection Pattern
        if (!ui->selectionPatternCombo->currentText().isEmpty()) {

            // Find Current File Pattern Text
            int fpcPos = ui->selectionPatternCombo->findText(ui->selectionPatternCombo->currentText());

            // Insert Current Text
            ui->selectionPatternCombo->insertItem(0, ui->selectionPatternCombo->currentText());

            // Check Current Text Pos
            if (fpcPos >= 0) {
                // Remove Item
                ui->selectionPatternCombo->removeItem(fpcPos + 1);
            }
        }

        // Save File Select Patterns
        saveFileSelectPatterns();
    }

    return result;
}

//==============================================================================
// Hide Event
//==============================================================================
void SelectFilesDialog::hideEvent(QHideEvent* aEvent)
{
    Q_UNUSED(aEvent);

    //qDebug() << "SelectFilesDialog::hideEvent";

    // ...

    // Set Pattern
    pattern = ui->selectionPatternCombo->currentText();

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
SelectFilesDialog::~SelectFilesDialog()
{
    delete ui;
}
