#include <QDir>
#include <QTimer>
#include <QQmlContext>
#include <QImageReader>
#include <QFile>
#include <QTextStream>
#include <QLineEdit>
#include <QDebug>

#include <mcwinterface.h>

#include "filepanel.h"
#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "remotefileutilclient.h"
#include "filelistimageprovider.h"
#include "settingscontroller.h"
#include "utility.h"
#include "constants.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
SearchDialog::SearchDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::SearchDialog)
    , settings(SettingsController::getInstance())
    , feedToListButton(NULL)
    , startButton(NULL)
    , fileUtil(new RemoteFileUtilClient)
    , resultModel(new SearchResultModel)
    , focusedPanel(NULL)
    , caseSensitiveSearch(false)
    , wholeWordSearch(false)
    , dialogShown(false)
    , searchActive(false)
    , searchFinished(false)
    , resultsVisible(true)
    , currentIndex(-1)
    , visualItemCount(0)
    , resultListKeyEvent(false)
{
    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void SearchDialog::init()
{
    //qDebug() << "SearchDialog::init";

    // Connect Signals
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBoxClicked(QAbstractButton*)));

    // Check File Util
    if (fileUtil) {
        // Connect Signals
        connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileSearchResultItemFound(uint,QString,QString)), this, SLOT(fileSearchResultItemFound(uint,QString,QString)));
    }

    // ...

    // Get Supported Image Formats Bye Array
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    // Get Count
    int flCount = formats.count();
    // Go Thru Formats
    for (int i=0; i<flCount; ++i) {
        // Add Format String
        supportedImageFormats << QString(formats[i]);
    }

    // Set Context Properties
    QQmlContext* ctx = ui->searchResultView->rootContext();
    // Set Context Properties - Search Result Controller
    ctx->setContextProperty(DEFAULT_SEARCH_RESULT_CONTROLLER, this);
    // Set Context Properties - Search Result Model
    ctx->setContextProperty(DEFAULT_SEARCH_RESULT_MODEL, resultModel);

    // Get Engine
    QQmlEngine* engine = ui->searchResultView->engine();
    // Add Image Provider
    engine->addImageProvider(QLatin1String(DEFAULT_FILE_ICON_PROVIDER_ID), new FileListImageProvider());

    // Set Resize Mode
    ui->searchResultView->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // Set Source
    ui->searchResultView->setSource(QUrl("qrc:/qml/SearchResults.qml"));

    // ...

    // Set Duplicates Enabled
    ui->filePatternComboBox->setDuplicatesEnabled(false);
    // Set Duplicates Enabled
    ui->contentPatternComboBox->setDuplicatesEnabled(false);
}

//==============================================================================
// Get Supported Image Formats
//==============================================================================
QStringList SearchDialog::getSupportedImageFormats()
{
    return supportedImageFormats;
}

//==============================================================================
// Item Selected
//==============================================================================
void SearchDialog::itemSelected()
{
    qDebug() << "SearchDialog::itemSelected - currentIndex: " << currentIndex;

    // Get Current File Name
    QString currentFilePath = resultModel ? resultModel->getItem(currentIndex) : "";

    // Check Current File Path
    if (!currentFilePath.isEmpty()) {
        // Emit Search REsult Item Selected Signal
        emit searchResultSelected(currentFilePath);
    }
}

//==============================================================================
// Item View
//==============================================================================
void SearchDialog::itemView(const bool& aEdit)
{
    qDebug() << "SearchDialog::itemView - currentIndex: " << currentIndex << " - aEdit: " << aEdit;

    // Get Current File Name
    QString currentFilePath = resultModel ? resultModel->getItem(currentIndex) : "";

    // Check Current File Path
    if (!currentFilePath.isEmpty()) {
        // Emit Search Result Item View Signal
        emit searchResultView(currentFilePath, aEdit, contentPattern);
    }
}

//==============================================================================
// Load Settings
//==============================================================================
void SearchDialog::loadSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    //qDebug() << "SearchDialog::loadSettings";

    // Case Senstive Search
    caseSensitiveSearch = settings->value(SETTINGS_KEY_SEARCH_CASE_SENSITIVE, DEFAULT_SETTINGS_SEARCH_CASE_SENSITIVE).toBool();

    // Whole Word Search
    wholeWordSearch = settings->value(SETTINGS_KEY_SEARCH_WHOLE_WORD, DEFAULT_SETTINGS_SEARCH_WHOLE_WORD).toBool();

    // ...

}

//==============================================================================
// Save Settings
//==============================================================================
void SearchDialog::saveSettings()
{
    // Check Settings
    if (!settings) {
        return;
    }

    //qDebug() << "SearchDialog::saveSettings";

    // Save Case Sensitive Setting
    settings->setValue(SETTINGS_KEY_SEARCH_CASE_SENSITIVE, caseSensitiveSearch);

    // Save Whole Word Search
    settings->setValue(SETTINGS_KEY_SEARCH_WHOLE_WORD, wholeWordSearch);

    // ...

    // Save File Search Patterns
    saveFileSearchPatterns();
    // Save Content Search Patterns
    saveContentSearchPatterns();

    // ...
}

//==============================================================================
// restore UI
//==============================================================================
void SearchDialog::restoreUI()
{
    //qDebug() << "SearchDialog::restoreUI";

    // Hide Results
    hideresults();

    // Clear Button Box
    ui->buttonBox->clear();

    // Reset Feed To List Button
    feedToListButton = NULL;

    // Set Up Button Box
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

    // Add Start Button
    startButton = ui->buttonBox->addButton(tr(DEFAULT_BUTTON_TEXT_START), QDialogButtonBox::AcceptRole);

    // Set Current Dir Label
    ui->currentDirLabel->setText(currentDir);

    // Reset File Pattern Combo Current Text
    ui->filePatternComboBox->setCurrentText("");

    // Reset Content Pattern Combo Current Text
    ui->contentPatternComboBox->setCurrentText("");

    // Case Sensitive Search
    ui->caseSensitiveCheckBox->setChecked(caseSensitiveSearch);
    // Whole Word Search
    ui->wholeWordCheckBox->setChecked(wholeWordSearch);

    // Reset Content Search Check Box
    ui->contentSearchCheckBox->setChecked(false);

    // Set Case Sensitive Check Box Enabled
    ui->caseSensitiveCheckBox->setEnabled(false);
    // Set Whole Word Check Box Enabled
    ui->wholeWordCheckBox->setEnabled(false);
    // Set Content Pattern Combo Enabled
    ui->contentPatternComboBox->setEnabled(false);
    // Enable Results Button
    ui->showResultsButton->setEnabled(false);

    // Check Results Model
    if (resultModel) {
        // Clear
        resultModel->clear();
    }

    // ...

    // Load File Search Patterns
    loadFileSearchPatterns();
    // Load Content Search Patterns
    loadContentSearchPatterns();

    // ...
}

//==============================================================================
// Show Results
//==============================================================================
void SearchDialog::showResults()
{
    //qDebug() << "SearchDialog::showResults";

    // Set Visibility
    ui->currentDirLabel->setVisible(true);
    // Set Visibility
    ui->resultsFrame->setVisible(true);

    // Set Minimum Height
    setMinimumHeight(DEFAULT_FILE_SEARCH_DIALOG_MIN_HEIGHT_RESULTS);
    // Set Maximum Height
    setMaximumHeight(DEFAULT_FILE_SEARCH_DIALOG_MAX_HEIGHT_RESULTS);

    // ...

    // Set Results Shown/Visible
    resultsVisible = true;

    // Set Focus
    ui->searchResultView->setFocus();
}

//==============================================================================
// Hide Results
//==============================================================================
void SearchDialog::hideresults()
{
    //qDebug() << "SearchDialog::hideresults";

    // Set Visibility
    ui->currentDirLabel->setVisible(false);
    // Set Visibility
    ui->resultsFrame->setVisible(false);

    // Set Minimum Height
    setMinimumHeight(DEFAULT_FILE_SEARCH_DIALOG_MIN_HEIGHT_NO_RESULTS);
    // Set Maximum Height
    setMaximumHeight(DEFAULT_FILE_SEARCH_DIALOG_MAX_HEIGHT_NO_RESULTS);

    // ...

    // Set Results Shown/Visible
    resultsVisible = false;
}

//==============================================================================
// Abort
//==============================================================================
void SearchDialog::abort()
{
    qDebug() << "SearchDialog::abort";

}

//==============================================================================
// Suspend
//==============================================================================
void SearchDialog::suspend()
{
    qDebug() << "SearchDialog::suspend";

}

//==============================================================================
// Resume
//==============================================================================
void SearchDialog::resume()
{
    qDebug() << "SearchDialog::resume";

}

//==============================================================================
// Shut Down
//==============================================================================
void SearchDialog::shutDown()
{
    // Check File Util
    if (fileUtil) {
        // Check Status
        if (fileUtil->getStatus() == ECSTBusy) {
            // Abort
            fileUtil->abort();
        }
    }

    // ...
}

//==============================================================================
// Load File Search Patterns
//==============================================================================
void SearchDialog::loadFileSearchPatterns()
{
    //qDebug() << "SearchDialog::loadFileSearchPatterns";

    // Clear File Patterns
    ui->filePatternComboBox->clear();

    // Init File Search Pattern List File
    QFile fspFile(QDir::homePath() + "/" + DEFAULT_FILE_SEARCH_FILE_NAME_PATTERN_LIST);

    // Open File
    if (fspFile.open(QIODevice::ReadOnly)) {
        // Init Text Stream
        QTextStream fspStream(&fspFile);

        // Go Thru File Pattern List
        while (!fspStream.atEnd()) {
            // Read Line
            QString line = fspStream.readLine();
            // Append To List
            ui->filePatternComboBox->addItem(line);
        }

        // Close File
        fspFile.close();
    }
}

//==============================================================================
// Load Content Search Patterns
//==============================================================================
void SearchDialog::loadContentSearchPatterns()
{
    //qDebug() << "SearchDialog::loadContentSearchPatterns";

    // Clear Content Patterns
    ui->contentPatternComboBox->clear();

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
            ui->contentPatternComboBox->addItem(line);
        }

        // Close File
        cspFile.close();
    }
}

//==============================================================================
// Save File Search Patterns
//==============================================================================
void SearchDialog::saveFileSearchPatterns()
{
    //qDebug() << "SearchDialog::saveFileSearchPatterns";

    // Init File Search Pattern List File
    QFile fspFile(QDir::homePath() + "/" + DEFAULT_FILE_SEARCH_FILE_NAME_PATTERN_LIST);

    // Open File
    if (fspFile.open(QIODevice::WriteOnly)) {
        // Init Text Stream
        QTextStream fspStream(&fspFile);

        // Get File Search Patterns Count
        int fspCount = ui->filePatternComboBox->count();

        // Go Thru File Search Patterns
        for (int i=0; i<fspCount; ++i) {
            // Get Item Text
            QString itemText = ui->filePatternComboBox->itemText(i);
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
// Save Content Search Patterns
//==============================================================================
void SearchDialog::saveContentSearchPatterns()
{
    //qDebug() << "SearchDialog::saveContentSearchPatterns";

    // Init Content Search Pattern List File
    QFile cspFile(QDir::homePath() + "/" + DEFAULT_FILE_SEARCH_CONTENT_PATTERN_LIST);

    // Open File
    if (cspFile.open(QIODevice::WriteOnly)) {
        // Init Text Stream
        QTextStream cspStream(&cspFile);

        // Get Content Search Patterns Count
        int cspCount = ui->contentPatternComboBox->count();

        // Go Thru File Search Patterns
        for (int i=0; i<cspCount; ++i) {
            // Get Item Text
            QString itemText = ui->contentPatternComboBox->itemText(i);
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
// Check If Path IS Valid
//==============================================================================
bool SearchDialog::isPathValid(const QString& aDirPath)
{
    // Check If Exists
    if (!QDir(aDirPath).exists()) {
        return false;
    }

    // ...

    return true;
}

//==============================================================================
// Button Box Clicked Slot
//==============================================================================
void SearchDialog::buttonBoxClicked(QAbstractButton* aButton)
{
    // Check Button
    if (startButton && startButton == aButton) {

        // Check File Util
        if (fileUtil) {
            // Check If Connected
            if (fileUtil->isConnected()) {
                // Start Search
                QTimer::singleShot(1, this, SLOT(startSearch()));
            } else {
                // Connect To Server
                fileUtil->connectToFileServer();
            }
        }

        // Remove Button
        ui->buttonBox->removeButton(startButton);

        // Reset Start Button
        delete startButton;
        startButton = NULL;

        return;

    } else if (feedToListButton && feedToListButton == aButton) {

        // Check Focused Panel & Result Model
        if (focusedPanel && resultModel) {
            // Feed To List
            focusedPanel->feedSearchResults(resultModel->results(), contentPattern);
        }

        // Remove Button
        ui->buttonBox->removeButton(feedToListButton);

        // Reset Feed To List Button
        delete feedToListButton;
        feedToListButton = NULL;

        // Accept & Close
        accept();

        return;
    }

    // Get Standard Button
    QDialogButtonBox::StandardButton button = ui->buttonBox->standardButton(aButton);

    // Switch Standard Button
    switch (button) {

        case QDialogButtonBox::Cancel:
        case QDialogButtonBox::Close:
            // Reject/Close
            reject();
        break;

        case QDialogButtonBox::Abort:
            // Check File Util
            if (fileUtil && fileUtil->getStatus() == ECSTBusy) {
                // Abort
                fileUtil->abort();
            }
            // reject/Close
            reject();
        break;

        case QDialogButtonBox::Reset:
            // Restore UI
            restoreUI();
        break;

        default:
        break;
    }
}

//==============================================================================
// Show Dialog
//==============================================================================
void SearchDialog::showDialog(const QString& aDirPath, FilePanel* aFocusedPanel)
{
    //qDebug() << "SearchDialog::showDialog";

    // Set Current Dir
    currentDir = aDirPath;
    // Set Focused Panel
    focusedPanel = aFocusedPanel;

    // Check Current Dir
    if (!currentDir.endsWith("/")) {
        // Adjust Current Dir
        currentDir += "/";
    }

    // Load Settings
    loadSettings();
    // Restore UI
    restoreUI();

    // Reset Seach Active
    searchActive = false;
    // Reset Search Finished
    searchFinished = false;

    // Set Visibility
    setVisible(true);
    // Show Dialog
    showNormal();
    // Raise
    raise();

    // Set Dialog Shown
    dialogShown = true;

    // Set Focus
    ui->filePatternComboBox->setFocus();
}

//==============================================================================
// Hide Dialog
//==============================================================================
void SearchDialog::hideDialog()
{
    //qDebug() << "SearchDialog::hideDialog";

    // Close
    close();

    // ...
}

//==============================================================================
// Is Dialog Shown
//==============================================================================
bool SearchDialog::isDialogShown()
{
    return dialogShown;
}

//==============================================================================
// Get Current Index
//==============================================================================
int SearchDialog::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Set Current index
//==============================================================================
void SearchDialog::setCurrentIndex(const int& aIndex)
{
    // Get Bounded Index
    int boundedIndex = qBound(0, aIndex, (resultModel ? resultModel->rowCount() - 1 : 0));

    // Check Current Index
    if (currentIndex != boundedIndex) {
        // Set Current Index
        currentIndex = boundedIndex;

        // ...

        // Emit Current Index Changed Signal
        emit currentIndexChanged(currentIndex);
    }
}

//==============================================================================
// Get Visual Item Count
//==============================================================================
int SearchDialog::getVisualItemCount()
{
    return visualItemCount;
}

//==============================================================================
// Set Visual item Count
//==============================================================================
void SearchDialog::setVisualItemCount(const int& aVisualCount)
{
    // Check Visual Item Count
    if (visualItemCount != aVisualCount) {
        // Set Visual Item Count
        visualItemCount = aVisualCount;

        // ...

        // Emit Visual Item Count Changed Signal
        emit visualItemCountChanged(visualItemCount);
    }
}

//==============================================================================
// Get Focused Panel
//==============================================================================
FilePanel* SearchDialog::getFocusedPanel()
{
    return focusedPanel;
}

//==============================================================================
// Start Search
//==============================================================================
void SearchDialog::startSearch()
{
    // Check File Pattern Combo Current Text
    if (!ui->filePatternComboBox->currentText().isEmpty()) {
        // Find Current File Pattern Text
        int fpcPos = ui->filePatternComboBox->findText(ui->filePatternComboBox->currentText());

        // Insert Current Text
        ui->filePatternComboBox->insertItem(0, ui->filePatternComboBox->currentText());

        // Check Current Text Pos
        if (fpcPos >= 0) {
            // Remove Item
            ui->filePatternComboBox->removeItem(fpcPos + 1);
        }

        // Set Current Index
        ui->filePatternComboBox->setCurrentIndex(0);
    }

    // Check Content Pattern Combo Current Text
    if (!ui->contentPatternComboBox->currentText().isEmpty()) {
        // Find Current Content Pattern Text
        int cpcPos = ui->contentPatternComboBox->findText(ui->contentPatternComboBox->currentText());

        // Insert Current Text
        ui->contentPatternComboBox->insertItem(0, ui->contentPatternComboBox->currentText());

        // Check Current Text Pos
        if (cpcPos >= 0) {
            // Remove Item
            ui->contentPatternComboBox->removeItem(cpcPos + 1);
        }

        // Set Current Index
        ui->contentPatternComboBox->setCurrentIndex(0);
    }

    // Check File Util
    if (fileUtil) {
        // Get File Name Pattern
        fileNamePattern = ui->filePatternComboBox->currentText();
        // Get Content Pattern
        contentPattern = ui->contentSearchCheckBox->isChecked() ? ui->contentPatternComboBox->currentText() : "";

        // Init Options
        int searchOptions = 0;

        // Check Case Sensitive Check Box
        if (ui->caseSensitiveCheckBox->isChecked()) {
            // Adjust Search Options
            searchOptions |= DEFAULT_SEARCH_OPTION_CASE_SENSITIVE;
        }

        // Check Whole Word Check Box
        if (ui->wholeWordCheckBox->isChecked()) {
            searchOptions |= DEFAULT_SEARCH_OPTION_WHOLE_WORD;
        }

        qDebug() << "SearchDialog::startSearch - fileNamePattern: " << fileNamePattern << " - contentPattern: " << contentPattern << " - searchOptions: " << searchOptions;

        // Search File
        fileUtil->searchFile(fileNamePattern, currentDir, contentPattern, searchOptions);
    }
}

//==============================================================================
// Exec
//==============================================================================
int SearchDialog::exec()
{
    return QDialog::exec();
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void SearchDialog::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    Q_UNUSED(aID);

    qDebug() << "SearchDialog::clientConnectionChanged - aConnected: " << aConnected;

    // Check Connected
    if (aConnected) {
        // Start Search
        QTimer::singleShot(1, this, SLOT(startSearch()));
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void SearchDialog::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    Q_UNUSED(aID);
    Q_UNUSED(aStatus);

    //qDebug() << "SearchDialog::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...
}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void SearchDialog::fileOpStarted(const unsigned int& aID,
                                 const QString& aOp,
                                 const QString& aPath,
                                 const QString& aSource,
                                 const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SEARCH_FILE) {
        qDebug() << "SearchDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

        // Set Search Active
        searchActive = true;

        // Set Button Box
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Abort);

        // ...
    }
}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void SearchDialog::fileOpSkipped(const unsigned int& aID,
                                 const QString& aOp,
                                 const QString& aPath,
                                 const QString& aSource,
                                 const QString& aTarget)
{
    Q_UNUSED(aID);
    Q_UNUSED(aOp);
    Q_UNUSED(aPath);
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    //qDebug() << "SearchDialog::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void SearchDialog::fileOpFinished(const unsigned int& aID,
                                  const QString& aOp,
                                  const QString& aPath,
                                  const QString& aSource,
                                  const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SEARCH_FILE) {
        qDebug() << "SearchDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

        // Reset Search Active
        searchActive = false;

        // Set Search Finished
        searchFinished = true;

        // Set Button Box
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Reset | QDialogButtonBox::Close);

        // Enable Results Button
        ui->showResultsButton->setEnabled(true);

        // Check Results
        if (resultModel && resultModel->rowCount() > 0) {
            // Show Results
            showResults();
            // Add Custom Button
            feedToListButton = ui->buttonBox->addButton(tr("Feed to List"), QDialogButtonBox::AcceptRole);
        }
    }
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void SearchDialog::fileOpAborted(const unsigned int& aID,
                                 const QString& aOp,
                                 const QString& aPath,
                                 const QString& aSource,
                                 const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SEARCH_FILE) {
        qDebug() << "SearchDialog::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

        // ...

        // Reset Search Active
        searchActive = false;

        // Set Search Finished
        searchFinished = false;

        // Set Button Box
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
    }
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void SearchDialog::fileOpError(const unsigned int& aID,
                               const QString& aOp,
                               const QString& aPath,
                               const QString& aSource,
                               const QString& aTarget,
                               const int& aError)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    // Check Operation
    if (aOp == DEFAULT_OPERATION_SEARCH_FILE) {
        qDebug() << "SearchDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

        // ...

        // Reset Search Active
        searchActive = false;

        // Set Search Finished
        searchFinished = false;

        // Set Button Box
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
    }
}

//==============================================================================
// File Search Result Item Found Slot
//==============================================================================
void SearchDialog::fileSearchResultItemFound(const unsigned int& aID,
                                             const QString& aPath,
                                             const QString& aFilePath)
{
    Q_UNUSED(aID);

    //qDebug() << "SearchDialog::fileSearchResultItemFound - aID: " << aID << " - aPath: " << aPath << " - aFilePath: " << aFilePath;

    // Check Result Model
    if (resultModel) {
        // Check PAth
        if (isPathValid(aPath)) {
            // Add Item
            resultModel->addItem(aFilePath);
        }
    }
}

//==============================================================================
// Key Press Event
//==============================================================================
void SearchDialog::keyPressEvent(QKeyEvent* aEvent)
{
    QDialog::keyPressEvent(aEvent);

    // Set Result List Key Event
    resultListKeyEvent = ui->searchResultView->hasFocus();

    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            default:

                // ...

            break;
        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void SearchDialog::keyReleaseEvent(QKeyEvent* aEvent)
{
    QDialog::keyReleaseEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {

            default:

                // Unhandled Keys

                // ...

            break;
        }
    }

    // Reset Result List Key Event
    resultListKeyEvent = false;
}

//==============================================================================
// Close Event
//==============================================================================
void SearchDialog::closeEvent(QCloseEvent* aEvent)
{
    Q_UNUSED(aEvent);

    //qDebug() << "SearchDialog::closeEvent";

    // ...

}

//==============================================================================
// Hide Event
//==============================================================================
void SearchDialog::hideEvent(QHideEvent* aEvent)
{
    Q_UNUSED(aEvent);

    //qDebug() << "SearchDialog::hideEvent";

    // Reset Dialog Shown
    dialogShown = false;

    // Set Visibility
    setVisible(false);

    // Save Settings
    saveSettings();

    // Check Start Button
    if (startButton) {
        // Remove Button
        ui->buttonBox->removeButton(startButton);

        // Reset Start Button
        delete startButton;
        startButton = NULL;
    }

    // ...
}

//==============================================================================
// Show Results Button Clicked Slot
//==============================================================================
void SearchDialog::on_showResultsButton_clicked()
{
    // Check If Results Shown/Visible
    if (resultsVisible) {
        // Hide Results
        hideresults();
    } else {

        // ...

        // Show Results
        showResults();
    }
}

//==============================================================================
// On Content Search Checkbox Clicked Slot
//==============================================================================
void SearchDialog::on_contentSearchCheckBox_clicked()
{
    // Check Check Box
    if (ui->contentSearchCheckBox->isChecked()) {

        // Set Case Sensitive Check Box Enabled
        ui->caseSensitiveCheckBox->setEnabled(true);
        // Set Whole Word Check Box Enabled
        ui->wholeWordCheckBox->setEnabled(true);
        // Set Content Pattern Combo Enabled
        ui->contentPatternComboBox->setEnabled(true);

        // Set Focus
        ui->contentPatternComboBox->setFocus();
        // Set Current Index
        ui->contentPatternComboBox->setCurrentIndex(0);
        // Select all
        ui->contentPatternComboBox->lineEdit()->selectAll();

    } else {

        // Set Case Sensitive Check Box Enabled
        ui->caseSensitiveCheckBox->setEnabled(false);
        // Set Whole Word Check Box Enabled
        ui->wholeWordCheckBox->setEnabled(false);
        // Set Content Pattern Combo Enabled
        ui->contentPatternComboBox->setEnabled(false);

    }
}

//==============================================================================
// Destructor
//==============================================================================
SearchDialog::~SearchDialog()
{

    // Delete Result View First Explicitly to Avoid Crash
    delete ui->searchResultView;

    // Delete UI
    delete ui;

    // Check File Util
    if (fileUtil) {
        // Close
        fileUtil->close();

        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    // Check Search Result Model
    if (resultModel) {
        // Delete Search Result Model
        delete resultModel;
        resultModel = NULL;
    }

    // Check Settings
    if (settings) {
        // RElease Instance
        settings->release();
        // Reset Pointer
        settings = NULL;
    }
}



















//==============================================================================
// Constructor
//==============================================================================
SearchResultModel::SearchResultModel(QObject* aParent)
    : QAbstractListModel(aParent)
{

}

//==============================================================================
// Add Item
//==============================================================================
void SearchResultModel::addItem(const QString& aNewItem)
{
    // Begin Insert Rows
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Append Item
    resultList << aNewItem;

    // End Insert Rows
    endInsertRows();
}

//==============================================================================
// Remove Item
//==============================================================================
void SearchResultModel::removeItem(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Begin Remove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);

        // Remove Item
        resultList.removeAt(aIndex);

        // End Remove Rows
        endRemoveRows();
    }
}

//==============================================================================
// Get Item
//==============================================================================
QString SearchResultModel::getItem(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        return resultList[aIndex];
    }

    return "";
}

//==============================================================================
// Clear
//==============================================================================
void SearchResultModel::clear()
{
    // Begin Reset Model
    beginResetModel();
    // Clear
    resultList.clear();
    // End Reset Model
    endResetModel();
}

//==============================================================================
// Get Search Results
//==============================================================================
QStringList& SearchResultModel::results()
{
    return resultList;
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> SearchResultModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File PAth
    roles[ERIDFilePath] = "filePath";

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int SearchResultModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return resultList.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int SearchResultModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 1;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant SearchResultModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        QString filePath = resultList[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case Qt::DisplayRole:
                // Switch Column
                switch (aIndex.column()) {
                    default:
                    case 0: return filePath;
                }
            break;

            case ERIDFilePath:  return filePath;

            default:
            break;
        }
    }

    return "";
}

//==============================================================================
// Set Data
//==============================================================================
bool SearchResultModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    Q_UNUSED(aIndex);
    Q_UNUSED(aValue);
    Q_UNUSED(aRole);

    return false;
}

//==============================================================================
// Header Data
//==============================================================================
QVariant SearchResultModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    return QAbstractItemModel::headerData(aSection, aOrientation, aRole);
}

//==============================================================================
// Flags
//==============================================================================
Qt::ItemFlags SearchResultModel::flags(const QModelIndex& aIndex) const
{
    Q_UNUSED(aIndex);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//==============================================================================
// Destructor
//==============================================================================
SearchResultModel::~SearchResultModel()
{
    // Clear
    clear();

    // ...
}





