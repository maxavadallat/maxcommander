#include <QDir>
#include <QTimer>
#include <QQmlContext>
#include <QImageReader>
#include <QDebug>

#include <mcwinterface.h>

#include "filepanel.h"
#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "remotefileutilclient.h"
#include "filelistimageprovider.h"
#include "utility.h"
#include "constants.h"
#include "defaultsettings.h"


//==============================================================================
// Constructor
//==============================================================================
SearchDialog::SearchDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::SearchDialog)
    , fileUtil(new RemoteFileUtilClient)
    , resultModel(new SearchResultModel)
    , focusedPanel(NULL)
    , caseSensitiveSearch(false)
    , wholeWordSearch(false)
    , dialogShown(false)
    , searchActive(false)
    , searchFinished(false)
    , resultsVisible(true)
    , startButton(NULL)
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
    qDebug() << "SearchDialog::init";

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

    //ui->buttonBox
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
        emit searchResultView(currentFilePath, aEdit);
    }
}

//==============================================================================
// Load Settings
//==============================================================================
void SearchDialog::loadSettings()
{
    qDebug() << "SearchDialog::loadSettings";

    // ...

}

//==============================================================================
// Save Settings
//==============================================================================
void SearchDialog::saveSettings()
{
    qDebug() << "SearchDialog::saveSettings";

    // ...

}

//==============================================================================
// restore UI
//==============================================================================
void SearchDialog::restoreUI()
{
    qDebug() << "SearchDialog::restoreUI";

    // Hide Results
    hideresults();

    // Set Up Button Box
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

    // Add Start Button
    startButton = ui->buttonBox->addButton(tr(DEFAULT_BUTTON_TEXT_START), QDialogButtonBox::ActionRole);

    // Set Current Dir Label
    ui->currentDirLabel->setText(currentDir);

    // Reset File Pattern Combo Current Text
    ui->filePatternComboBox->setCurrentText("");

    // Reset Content Pattern Combo Current Text
    ui->contentPatternComboBox->setCurrentText("");

    // Check Results Model
    if (resultModel) {
        // Clear
        resultModel->clear();
    }

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
    setMinimumHeight(480);
    // Set Maximum Height
    setMaximumHeight(1200);

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
    setMinimumHeight(190);
    // Set Maximum Height
    setMaximumHeight(190);

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
    }

    // Get Standard Button
    QDialogButtonBox::StandardButton button = ui->buttonBox->standardButton(aButton);

    // Switch Standard Button
    switch (button) {

        case QDialogButtonBox::Cancel:
        case QDialogButtonBox::Close:
            // Reset Dialog Shown
            //dialogShown = false;
            // Close
            close();
        break;

        case QDialogButtonBox::Abort:
            // Check File Util
            if (fileUtil && fileUtil->getStatus() == ECSTBusy) {
                // Abort
                fileUtil->abort();
            }

            // Reset Dialog Shown
            //dialogShown = false;
            // Close
            close();
        break;

        case QDialogButtonBox::Reset:
            // Restore UI
            restoreUI();

            // ...

            // Check Result Model
            if (resultModel) {
                // Clear
                resultModel->clear();
            }

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
    qDebug() << "SearchDialog::showDialog";

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
    qDebug() << "SearchDialog::hideDialog";

    // Save Settings
    saveSettings();

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
    // Check File Util
    if (fileUtil) {
        // Get File Name Pattern
        fileNamePattern = ui->filePatternComboBox->currentText();
        // Get Content Pattern
        contentPattern = ui->contentPatternComboBox->currentText();

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
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "SearchDialog::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

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

        // Check Results
        if (resultModel && resultModel->rowCount() > 0) {
            // Show Results
            showResults();
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
        qDebug() << "SearchDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

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
    qDebug() << "SearchDialog::fileSearchResultItemFound - aID: " << aID << " - aPath: " << aPath << " - aFilePath: " << aFilePath;

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
            case Qt::Key_Home:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // Set Current Index
                    setCurrentIndex(0);
                }
            break;

            case Qt::Key_End:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // Set Current index
                    setCurrentIndex(resultModel ? resultModel->rowCount() - 1 : 0);
                }
            break;

            case Qt::Key_PageUp:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // Set Current Index
                    setCurrentIndex(currentIndex - (visualItemCount - 1));
                }
            break;

            case Qt::Key_PageDown:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // Set Current Index
                    setCurrentIndex(currentIndex + (visualItemCount - 1));
                }
            break;

            case Qt::Key_Enter:
            case Qt::Key_Return:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // ItemSelected
                    itemSelected();
                }
            break;

            case Qt::Key_F3:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // Item View
                    itemView(false);
                }
            break;

            case Qt::Key_F4:
                // Check Focus
                if (resultListKeyEvent && ui->searchResultView->hasFocus()) {
                    // Item View
                    itemView(true);
                }
            break;

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

    qDebug() << "SearchDialog::closeEvent";

    // ...

}

//==============================================================================
// Hide Event
//==============================================================================
void SearchDialog::hideEvent(QHideEvent* aEvent)
{
    Q_UNUSED(aEvent);

    qDebug() << "SearchDialog::hideEvent";

    // Reset Dialog Shown
    dialogShown = false;

    // Set Visibility
    setVisible(false);

    // Raise
    lower();

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
// Destructor
//==============================================================================
SearchResultModel::~SearchResultModel()
{
    // Clear
    clear();

    // ...
}




