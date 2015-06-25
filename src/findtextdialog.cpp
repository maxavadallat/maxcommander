#include <QDir>
#include <QFile>
#include <QDebug>

#include "findtextdialog.h"
#include "ui_findtextdialog.h"
#include "settingscontroller.h"
#include "constants.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
FindTextDialog::FindTextDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::FindTextDialog)
    , settings(SettingsController::getInstance())
    , findButton(NULL)
    , findNextButton(NULL)
    , searchTerm("")
    , caseSensitive(false)
    , wholeWord(false)
    , backward(false)
    , wrapAround(true)
{
    // Setup UI
    ui->setupUi(this);

    // Connect Signals
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBoxButtonClicked(QAbstractButton*)));
    connect(ui->findTextCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(currentSearchTextChanged(QString)));

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void FindTextDialog::init()
{
    // Load Settings
    loadSettings();

    // Load Search Terms
    loadSearchTerms();
}

//==============================================================================
// Load Settings
//==============================================================================
void FindTextDialog::loadSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    // Set Case Sensitive Search
    caseSensitive = settings->value(SETTINGS_KEY_SEARCH_CASE_SENSITIVE, DEFAULT_SETTINGS_SEARCH_CASE_SENSITIVE).toBool();
    // Set Whole Word Search
    wholeWord = settings->value(SETTINGS_KEY_SEARCH_WHOLE_WORD, DEFAULT_SETTINGS_SEARCH_WHOLE_WORD).toBool();

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void FindTextDialog::saveSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    // Save Case Sensitive Setting
    settings->setValue(SETTINGS_KEY_SEARCH_CASE_SENSITIVE, caseSensitive);
    // Save Whole Word Search
    settings->setValue(SETTINGS_KEY_SEARCH_WHOLE_WORD, wholeWord);

    // ...
}

//==============================================================================
// Retore UI
//==============================================================================
void FindTextDialog::restoreUI()
{
    // Clear Button Box
    ui->buttonBox->clear();

    // Set Standard Button
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

    // Add Find Button
    findButton = ui->buttonBox->addButton(tr(DEFAULT_BUTTON_TEXT_FIND), QDialogButtonBox::ActionRole);

    // Set Case Sensitive Check Box
    ui->caseSensitiveCheckBox->setChecked(caseSensitive);
    // Set Whole Word Check Box
    ui->wholeWordCheckBox->setChecked(wholeWord);
    // Set Wrap Around Check Box
    ui->wrapAroundCheckBox->setChecked(wrapAround);
}

//==============================================================================
// Load Search Terms
//==============================================================================
void FindTextDialog::loadSearchTerms()
{
    qDebug() << "FindTextDialog::loadSearchTerms";

    // Clear Content Patterns
    ui->findTextCombo->clear();

    // Init Content Search Pattern List File
    QFile cspFile(QDir::homePath() + "/" + DEFAULT_FILE_SEARCH_CONTENT_PATTERN_LIST);

    // Open File
    if (cspFile.open(QIODevice::ReadOnly)) {
        // Init Text Stream
        QTextStream cspStream(&cspFile);

        // Go Thru Content Patterns List File
        while (!cspStream.atEnd()) {
            // Read Line
            QString line = cspStream.readLine();
            // Append To List
            ui->findTextCombo->addItem(line);
        }

        // Close File
        cspFile.close();
    }

}

//==============================================================================
// Save Search Terms
//==============================================================================
void FindTextDialog::saveSearchTerms()
{
    qDebug() << "FindTextDialog::saveSearchTerms";

    // Init Content Search Pattern List File
    QFile cspFile(QDir::homePath() + "/" + DEFAULT_FILE_SEARCH_CONTENT_PATTERN_LIST);

    // Open File
    if (cspFile.open(QIODevice::WriteOnly)) {
        // Init Text Stream
        QTextStream cspStream(&cspFile);

        // Get Content Search Patterns Count
        int cspCount = ui->findTextCombo->count();

        // Go Thru File Search Patterns
        for (int i=0; i<cspCount; ++i) {
            // Get Item Text
            QString itemText = ui->findTextCombo->itemText(i);
            // Write Item
            cspStream << itemText;
            // Write New Line
            cspStream << "\n";
        }

        // Close File
        cspFile.close();
    }
}

//==============================================================================
// Store Current Search Term
//==============================================================================
void FindTextDialog::storeCurrentSearchTerm()
{
    // Check Find Text Combo Combo Current Text
    if (!ui->findTextCombo->currentText().isEmpty()) {

        qDebug() << "FindTextDialog::storeCurrentSearchTerm - currentText: " << ui->findTextCombo->currentText();

        // Find Current Text
        int fpcPos = ui->findTextCombo->findText(ui->findTextCombo->currentText());

        // Insert Current Text
        ui->findTextCombo->insertItem(0, ui->findTextCombo->currentText());

        // Check Current Text Pos
        if (fpcPos >= 0) {
            // Remove Item
            ui->findTextCombo->removeItem(fpcPos + 1);
        }

        // Set Current Index
        ui->findTextCombo->setCurrentIndex(0);
    }
}

//==============================================================================
// Exec
//==============================================================================
int FindTextDialog::exec()
{
    // Restore UI
    restoreUI();

    return QDialog::exec();
}

//==============================================================================
// Set Search Term
//==============================================================================
void FindTextDialog::setSearchTerm(const QString& aSearchTerm)
{
    // Check Search Term
    if (searchTerm != aSearchTerm) {
        // Set Search Ter,
        searchTerm = aSearchTerm;

        // Set Current Text
        ui->findTextCombo->setCurrentText(searchTerm);

        // ...
    }
}

//==============================================================================
// Get Search Term
//==============================================================================
QString FindTextDialog::getSearchTerm()
{
    return searchTerm;
}

//==============================================================================
// Get Case Sensitive Setting
//==============================================================================
bool FindTextDialog::getCaseSensitiveSearch()
{
    return caseSensitive;
}

//==============================================================================
// Get Whole Word Setting
//==============================================================================
bool FindTextDialog::getWholeWordSearch()
{
    return wholeWord;
}

//==============================================================================
// Get Backward Search Setting
//==============================================================================
bool FindTextDialog::getBackwardSearch()
{
    return backward;
}

//==============================================================================
// Get Wrap Around Setting
//==============================================================================
bool FindTextDialog::getWrapAroundWhenFinished()
{
    return wrapAround;
}

//==============================================================================
// Enable Find Next Button
//==============================================================================
void FindTextDialog::enableFindNextButton(const bool& aEnable)
{
    // Check Enable
    if (aEnable) {
        // Check Find Button
        if (findButton) {
            // Remove Button
            ui->buttonBox->removeButton(findButton);
            // Delete Button
            delete findButton;
            findButton = NULL;
        }

        // Check Find Next Button
        if (!findNextButton) {
            // Add Button
            findNextButton = ui->buttonBox->addButton(tr(DEFAULT_BUTTON_TEXT_FIND_NEXT), QDialogButtonBox::ActionRole);
        }

        // Set Focus
        findNextButton->setFocus();

    } else {

        // Check Find Next Button
        if (findNextButton) {
            // Remove Button
            ui->buttonBox->removeButton(findNextButton);
            // Delete Button
            delete findNextButton;
            findNextButton = NULL;
        }

        // Check Find Button
        if (!findButton) {
            // Add Button
            findButton = ui->buttonBox->addButton(tr(DEFAULT_BUTTON_TEXT_FIND), QDialogButtonBox::ActionRole);
        }

        // Set Focus
        findButton->setFocus();
    }
}

//==============================================================================
// Button Box Button Clicked Slot
//==============================================================================
void FindTextDialog::buttonBoxButtonClicked(QAbstractButton* aButton)
{
    // Check Button
    if (findButton && findButton == aButton) {

        // Emit Find Text Signal
        emit findText(searchTerm);

    } else if (findNextButton && findNextButton == aButton) {

        // Emit Find Next Signal
        emit findNext();
    }
}

//==============================================================================
// Current Search Text Changed Slot
//==============================================================================
void FindTextDialog::currentSearchTextChanged(const QString& aText)
{
    // Set Search Term
    searchTerm = aText;
}

//==============================================================================
// Key Press Event
//==============================================================================
void FindTextDialog::keyPressEvent(QKeyEvent* aEvent)
{
    QDialog::keyPressEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Accept Event
        aEvent->accept();

        // Switch Event Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                //qDebug() << "FindTextDialog::keyPressEvent - ESCAPE";

                // Emit Escape Pressed Signal
                emit escapePresed();

                // ...

            break;

            default:
            break;
        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void FindTextDialog::keyReleaseEvent(QKeyEvent* aEvent)
{
    QDialog::keyReleaseEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Accept Event
        aEvent->accept();

        // Switch Event Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                //qDebug() << "FindTextDialog::keyReleaseEvent - ESCAPE";

                // ...

            break;

            default:
            break;
        }
    }
}

//==============================================================================
// On Case Sensitive Check Box Clicked Slot
//==============================================================================
void FindTextDialog::on_caseSensitiveCheckBox_clicked()
{
    // Set Case Sensitive
    caseSensitive = ui->caseSensitiveCheckBox->isChecked();
}

//==============================================================================
// On Whole Word Check Box Clicked Slot
//==============================================================================
void FindTextDialog::on_wholeWordCheckBox_clicked()
{
    // Set Whole Word
    wholeWord = ui->wholeWordCheckBox->isChecked();
}

//==============================================================================
// On Backward Search Check Box Clicked Slot
//==============================================================================
void FindTextDialog::on_backwardSearchCheckBox_clicked()
{
    // Set Backward Search
    backward = ui->backwardSearchCheckBox->isChecked();
}

//==============================================================================
// On Wrap Around Check Box Clicked Slot
//==============================================================================
void FindTextDialog::on_wrapAroundCheckBox_clicked()
{
    // Set Wrap Around
    wrapAround = ui->wrapAroundCheckBox->isChecked();
}

//==============================================================================
// Destructor
//==============================================================================
FindTextDialog::~FindTextDialog()
{
    // Save Settings
    saveSettings();
    // Save Search Terms
    saveSearchTerms();

    // Check Settings
    if (settings) {
        // Release Instance
        settings->release();
        // Reset Settings Pointer
        settings = NULL;
    }

    // Delete UI
    delete ui;
}

