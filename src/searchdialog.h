#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QStringList>
#include <QCloseEvent>
#include <QHideEvent>
#include <QAbstractButton>
#include <QPushButton>


namespace Ui {
class SearchDialog;
}

class RemoteFileUtilClient;
class SearchResultModel;
class FileListImageProvider;
class FilePanel;
class SettingsController;

//==============================================================================
// Search Dialog Class
//==============================================================================
class SearchDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int visualItemCount READ getVisualItemCount WRITE setVisualItemCount NOTIFY visualItemCountChanged)

public:
    // Constructor
    explicit SearchDialog(QWidget* aParent = NULL);

    // Show Dialog
    void showDialog(const QString& aDirPath, FilePanel* aFocusedPanel);
    // Hide Dialog
    void hideDialog();

    // Is Dialog Shown
    bool isDialogShown();

    // Get Current Index
    int getCurrentIndex();
    // Set Current index
    void setCurrentIndex(const int& aIndex);

    // Get Visual Item Count
    int getVisualItemCount();
    // Set Visual item Count
    void setVisualItemCount(const int& aVisualCount);

    // Get Focused Panel
    FilePanel* getFocusedPanel();

    // Destructor
    virtual ~SearchDialog();

signals:

    // Current Index Changed Signal
    void currentIndexChanged(const int& aIndex);
    // Visual Item Count Changed Signal
    void visualItemCountChanged(const int& aVisualCount);

    // Search Result Item Selected Signal
    void searchResultSelected(const QString& aFilePath);
    // Search Result Item View Signal
    void searchResultView(const QString& aFilePath, const bool& aEdit, const QString& aSearchTerm);

public slots:

    // Get Supported Image Formats
    QStringList getSupportedImageFormats();

    // Item Selected
    void itemSelected();
    // Item View
    void itemView(const bool& aEdit);

protected slots:

    // Init
    void init();
    // Load Settings
    void loadSettings();
    // Save Settings
    void saveSettings();
    // Restore UI
    void restoreUI();
    // Show Results
    void showResults();
    // Hide Results
    void hideresults();

    // Start Search
    void startSearch();

    // Abort
    void abort();
    // Suspend
    void suspend();
    // Resume
    void resume();

    // Shut Down
    void shutDown();

    // Load File Search Patterns
    void loadFileSearchPatterns();
    // Load Content Search Patterns
    void loadContentSearchPatterns();

    // Save File Search Patterns
    void saveFileSearchPatterns();
    // Save Content Search Patterns
    void saveContentSearchPatterns();

    // Check If Path IS Valid
    bool isPathValid(const QString& aDirPath);

    // Button Box Clicked Slot
    void buttonBoxClicked(QAbstractButton* aButton);

    // Show Results Button Clicked Slot
    void on_showResultsButton_clicked();

    // On Content Search Checkbox Clicked Slot
    void on_contentSearchCheckBox_clicked();

protected: // From QDialog

    // Exec
    virtual int exec();

protected slots: // For RemoteFileUtilClient

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Started Slot
    void fileOpStarted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Skipped Slot
    void fileOpSkipped(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Slot
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // File Search Result Item Found Slot
    void fileSearchResultItemFound(const unsigned int& aID,
                                   const QString& aPath,
                                   const QString& aFilePath);

protected: // From QDialog

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);
    // Close Event
    virtual void closeEvent(QCloseEvent* aEvent);
    // Hide Event
    virtual void hideEvent(QHideEvent* aEvent);

private:

    // UI
    Ui::SearchDialog*       ui;
    // Settings Controller
    SettingsController*     settings;

    // Feed To List Button
    QPushButton*            feedToListButton;
    // Start Button
    QPushButton*            startButton;
    // Reset Button
    QPushButton*            resetButton;

    // Remote File Util Client
    RemoteFileUtilClient*   fileUtil;
    // Search Result Model
    SearchResultModel*      resultModel;

    // Focused Panel
    FilePanel*              focusedPanel;

    // Current Dir
    QString                 currentDir;

    // File Name Pattern
    QString                 fileNamePattern;
    // Content Pattern
    QString                 contentPattern;

    // Case Sensitive Search
    bool                    caseSensitiveSearch;
    // Whole Words Search
    bool                    wholeWordSearch;

    // Search Dialog Shown
    bool                    dialogShown;

    // Search Active
    bool                    searchActive;
    // Search Finished
    bool                    searchFinished;

    // Results Visible
    bool                    resultsVisible;

    // Supported Image Formats
    QStringList             supportedImageFormats;

    // Search Result List Current Index
    int                     currentIndex;

    // Search Result List Visual Item Count
    int                     visualItemCount;

    // Result List Key Event
    bool                    resultListKeyEvent;
};























//==============================================================================
// Search Result Model Class
//==============================================================================
class SearchResultModel : public QAbstractListModel
{
    Q_OBJECT

public:

    // Constructor
    explicit SearchResultModel(QObject* aParent = NULL);

    // Add Item
    void addItem(const QString& aNewItem);
    // Remove Item
    void removeItem(const int& aIndex);
    // Get Item
    QString getItem(const int& aIndex);
    // Clear
    void clear();

    // Get Search Results
    QStringList& results();

    // Destructor
    virtual ~SearchResultModel();

public: // From QAbstractListModel

    // Get Role Names
    virtual QHash<int, QByteArray> roleNames() const;
    // Get Row Count
    virtual int rowCount(const QModelIndex& aParent = QModelIndex()) const;
    // Get Column Count
    virtual int columnCount(const QModelIndex& aParent = QModelIndex()) const;
    // Get Data
    virtual QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const;
    // Set Data
    virtual bool setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole = Qt::EditRole);
    // Header Data
    virtual QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole = Qt::DisplayRole) const;
    // Flags
    virtual Qt::ItemFlags flags(const QModelIndex& aIndex) const;

protected:

    // Role ID's
    enum RolesIDs {
        ERIDFilePath    = Qt::UserRole + 1,
        ERIDIsDir
    };

    // Result List
    QStringList     resultList;
};


#endif // SEARCHDIALOG_H
