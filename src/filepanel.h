#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QTimerEvent>

namespace Ui {
class FilePanel;
}

class FileListModel;
class MainWindow;
class FileListImageProvider;
class ConfirmDialog;
class RemoteFileUtilClient;
class TransferProgressModelItem;
class FileRenamer;
class DirScanner;


//==============================================================================
// File Panel Class
//==============================================================================
class FilePanel : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString panelName READ getPanelName)
    Q_PROPERTY(bool panelHasFocus READ getPanelFocus WRITE setPanelFocus NOTIFY panelFocusChanged)
    Q_PROPERTY(int visualItemsCount READ getvisualItemsCount WRITE setVisualItemsCount NOTIFY visualItemsCountChanged)

    Q_PROPERTY(bool extVisible READ getExtVisible WRITE setExtVisible NOTIFY extVisibleChanged)
    Q_PROPERTY(bool typeVisible READ getTypeVisible WRITE setTypeVisible NOTIFY typeVisibleChanged)
    Q_PROPERTY(bool sizeVisible READ getSizeVisible WRITE setSizeVisible NOTIFY sizeVisibleChanged)
    Q_PROPERTY(bool dateVisible READ getDateVisible WRITE setDateVisible NOTIFY dateVisibleChanged)
    Q_PROPERTY(bool ownerVisible READ getOwnerVisible WRITE setOwnerVisible NOTIFY ownerVisibleChanged)
    Q_PROPERTY(bool permsVisible READ getPermsVisible WRITE setPermsVisible NOTIFY permsVisibleChanged)
    Q_PROPERTY(bool attrsVisible READ getAttrsVisible WRITE setAttrsVisible NOTIFY attrsVisibleChanged)

    Q_PROPERTY(int extWidth READ getExtWidth WRITE setExtWidth NOTIFY extWidthChanged)
    Q_PROPERTY(int typeWidth READ getTypeWidth WRITE setTypeWidth NOTIFY typeWidthChanged)
    Q_PROPERTY(int sizeWidth READ getSizeWidth WRITE setSizeWidth NOTIFY sizeWidthChanged)
    Q_PROPERTY(int dateWidth READ getDateWidth WRITE setDateWidth NOTIFY dateWidthChanged)
    Q_PROPERTY(int ownerWidth READ getOwnerWidth WRITE setOwnerWidth NOTIFY ownerWidthChanged)
    Q_PROPERTY(int permsWidth READ getPermsWidth WRITE setPermsWidth NOTIFY permsWidthChanged)
    Q_PROPERTY(int attrsWidth READ getAttrsWidth WRITE setAttrsWidth NOTIFY attrsWidthChanged)

    Q_PROPERTY(int sorting READ getSorting WRITE setSorting NOTIFY sortingChanged)
    Q_PROPERTY(bool reverseOrder READ getReverseOrder WRITE setReverseOrder NOTIFY reverseOrderChanged)

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(bool loading READ getloading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(bool fileRenameActive READ getFileRenameActive WRITE setFileRenameActive NOTIFY fileRenameActiveChanged)
    Q_PROPERTY(int modifierKeys READ getModifierKeys NOTIFY modifierKeysChanged)

    Q_PROPERTY(bool showHiddenFiles READ getShowHidden WRITE setShowHidden NOTIFY showHiddenChanged)
    Q_PROPERTY(bool showDirsFirst READ getShowDirsFirst WRITE setShowDirsFirst NOTIFY showDirsFirstChanged)
    Q_PROPERTY(bool caseSensitiveSort READ getCaseSensitiveSort WRITE setCaseSensitiveSort NOTIFY caseSensitiveSortChanged)

    Q_PROPERTY(QString textColor READ getTextColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString textBGColor READ gettextBGColor WRITE setTextBGColor NOTIFY textBGColorChanged)

    Q_PROPERTY(QString currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(QString currentBGColor READ getCurrentBGColor WRITE setCurrentBGColor NOTIFY currentBGColorChanged)

    Q_PROPERTY(QString selectedColor READ getSelectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QString selectedBGColor READ getSelectedBGColor WRITE setSelectedBGColor NOTIFY selectedBGColorChanged)

    Q_PROPERTY(QString currentSelectedColor READ getCurrentSelectedColor WRITE setCurrentSelectedColor NOTIFY currentSelectedColorChanged)
    Q_PROPERTY(QString currentSelectedBGColor READ getCurrentSelectedBGColor WRITE setCurrentSelectedBGColor NOTIFY currentSelectedBGColorChanged)

    Q_PROPERTY(QString hiddenColor READ getHiddenColor WRITE setHiddenColor NOTIFY hiddenColorChanged)
    Q_PROPERTY(QString hiddenBGColor READ getHiddenBGColor WRITE setHiddenBGColor NOTIFY hiddenBGColorChanged)

    Q_PROPERTY(QString linkColor READ getLinkColor WRITE setLinkColor NOTIFY linkColorChanged)
    Q_PROPERTY(QString linkBGColor READ getLinkBGColor WRITE setLinkBGColor NOTIFY linkBGColorChanged)

    Q_PROPERTY(QString fontName READ getFontName WRITE setFontName NOTIFY fontNameChanged)
    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool fontBold READ getFontBold WRITE setFontBold NOTIFY fontBoldChanged)
    Q_PROPERTY(bool fontItalic READ getFontItalic WRITE setFontItalic NOTIFY fontItalicChanged)

    Q_PROPERTY(int thumbWidth READ getThumbWidth WRITE setThumbWidth NOTIFY thumbWidthChanged)
    Q_PROPERTY(int thumbHeight READ getThumbHeight WRITE setThumbHeight NOTIFY thumbHeightChanged)

public:
    // Constructor
    explicit FilePanel(QWidget* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();
    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir, const QString& aLastFileName = "");

    // Get Panel Name
    QString getPanelName();
    // Set Panel Name
    void setPanelName(const QString& aPanelName);

    // Get Panel Focus
    bool getPanelFocus();

    // Get Current Index
    int getCurrentIndex();
    // Set Current Index
    void setCurrentIndex(const int& aCurrentIndex);

    // Get Visual Items Count
    int getvisualItemsCount();

    // Get Extension Visible
    bool getExtVisible();
    // Set Extension Visible
    void setExtVisible(const bool& aExtVisible);

    // Get Type Visible
    bool getTypeVisible();
    // Set Type Visible
    void setTypeVisible(const bool& aTypeVisible);

    // Get Size Visible
    bool getSizeVisible();
    // Set Size Visible
    void setSizeVisible(const bool& aSizeVisible);

    // Get Date Visible
    bool getDateVisible();
    // Set Date Visible
    void setDateVisible(const bool& aDateVisible);

    // Get Owner Visible
    bool getOwnerVisible();
    // Set Owner Visible
    void setOwnerVisible(const bool& aOwnerVisible);

    // Get Permissions Visible
    bool getPermsVisible();
    // Set Permissions Visible
    void setPermsVisible(const bool& aPermsVisible);

    // Get Attributes Visible
    bool getAttrsVisible();
    // Set Attributes Visible
    void setAttrsVisible(const bool& aAttrsVisible);

    // Get Extensions Column Width
    int getExtWidth();
    // Set Extensions Column Width
    void setExtWidth(const int& aWidth);

    // Get Type Column Width
    int getTypeWidth();
    // Set Type Column Width
    void setTypeWidth(const int& aWidth);

    // Get Size Column Width
    int getSizeWidth();
    // Set Size Column Width
    void setSizeWidth(const int& aWidth);

    // Get Date Column Width
    int getDateWidth();
    // Set Date Column Width
    void setDateWidth(const int& aWidth);

    // Get Owner Column Width
    int getOwnerWidth();
    // Set Owner Column Width
    void setOwnerWidth(const int& aWidth);

    // Get Permissions Column Width
    int getPermsWidth();
    // Set Permissions Column Width
    void setPermsWidth(const int& aWidth);

    // Get Attributes Column Width
    int getAttrsWidth();
    // Set Attributes Column Width
    void setAttrsWidth(const int& aWidth);

    // Get Sorting
    int getSorting();
    // Set Sorting
    void setSorting(const int& aSorting);

    // Get Reversed Order
    bool getReverseOrder();
    // Set Reversed Order
    void setReverseOrder(const bool& aReverse);

    // Get Show Hidden
    bool getShowHidden();
    // Set Show Hidden
    void setShowHidden(const bool& aHidden);

    // Get Show Directories First
    bool getShowDirsFirst();
    // Set Show Directories First
    void setShowDirsFirst(const bool& aShow);

    // Get Case Sensitive Sorting
    bool getCaseSensitiveSort();
    // Set Case Sensitive Sorting
    void setCaseSensitiveSort(const bool& aCaseSensitive);

    // Get Normal Text Color
    QString getTextColor();
    // Set Normal Text Color
    void setTextColor(const QString& aColor);

    // Get Normal Text BG Color
    QString gettextBGColor();
    // Set Normal Text BG Color
    void setTextBGColor(const QString& aColor);

    // Get Current Text Color
    QString getCurrentColor();
    // Set Current Text Color
    void setCurrentColor(const QString& aColor);

    // Get Current Text BG Color
    QString getCurrentBGColor();
    // Set Current Text BG Color
    void setCurrentBGColor(const QString& aColor);

    // Get Selected Text Color
    QString getSelectedColor();
    // Set Selected Text Color
    void setSelectedColor(const QString& aColor);

    // Get Selected Text BG Color
    QString getSelectedBGColor();
    // Set Selected Text BG Color
    void setSelectedBGColor(const QString& aColor);

    // Get Current Selected Text Color
    QString getCurrentSelectedColor();
    // Set Current Selected Text Color
    void setCurrentSelectedColor(const QString& aColor);

    // Get Current Selected Text BG Color
    QString getCurrentSelectedBGColor();
    // Set Current Selected Text BG Color
    void setCurrentSelectedBGColor(const QString& aColor);

    // Get Hidden Text Color
    QString getHiddenColor();
    // Set Hidden Text Color
    void setHiddenColor(const QString& aColor);

    // Get Hidden Text BG Color
    QString getHiddenBGColor();
    // Set Hidden Text BG Color
    void setHiddenBGColor(const QString& aColor);

    // Get Link Text Color
    QString getLinkColor();
    // Set Link Text Color
    void setLinkColor(const QString& aColor);

    // Get Link Text BG Color
    QString getLinkBGColor();
    // Set Link Text BG Color
    void setLinkBGColor(const QString& aColor);

    // Get Font Name
    QString getFontName();
    // Set Font Name
    void setFontName(const QString& aFontName);

    // Get Font Size
    int getFontSize();
    // Set Font Size
    void setFontSize(const int& aSize);

    // Get Font Bold
    bool getFontBold();
    // Set Font Bold
    void setFontBold(const bool& aBold);

    // Get Font Italic
    bool getFontItalic();
    // Set Font Italic
    void setFontItalic(const bool& aItalic);

    // Get Thumb Width
    int getThumbWidth();
    // Set Thumb Width
    void setThumbWidth(const int& aWidth);

    // Get Thumb Height
    int getThumbHeight();
    // Set Thumb height
    void setThumbHeight(const int& aheight);

    // Get Current File Info
    QFileInfo getCurrFileInfo();

    // Busy
    bool busy();

    // Get Loading
    bool getloading();
    // Set Loading
    void setLoading(const bool& aLoading);

    // Get File Index By File Name
    int getFileIndex(const QString& aFileName);

    // Create Dir
    void createDir(const QString& aDirPath);

    // File Rename Active
    bool getFileRenameActive();
    // Set File Rename Active
    void setFileRenameActive(const bool& aFileRenameActive);

    // Get Modifier Keys
    int getModifierKeys();

    // Destructor
    virtual ~FilePanel();

public slots:

    // Go To Home Directory
    void gotoHome();
    // Go To Root
    void gotoRoot();
    // Go To Drive
    void gotoDrive(const int& aDriveIndex);
    // Go Up To Parent Dir
    void goUp();
    // Go To Next Item Of The List
    void goNext();
    // Go To Prev Item Of The List
    void goPrev();
    // Go To Page Up Of The List
    void pageUp();
    // Go To Page Down Of The List
    void pageDown();
    // Go To Beginning Of The List
    void goFirst();
    // Go To End Of The List
    void goLast();

    // Set Panel Focus
    void setPanelFocus(const bool& aFocus);
    // Set Visual Items Count
    void setVisualItemsCount(const int& aVisualCount);

    // Get Supported Image Formats
    QStringList getSupportedImageFormats();

    // Reload
    void reload(const int& aIndex = -1);

    // Select All Files
    void selectAllFiles();
    // Deselect All Files
    void deselectAllFiles();
    // Toggle Current File Selection
    void toggleCurrentFileSelection();

    // Get Selected Files
    QStringList getSelectedFiles();

    // Rename File
    void renameFile(const QString& aSource, const QString& aTarget);
    // Scan Dir
    void scanDir(const QString& aDirPath);

signals:

    // Focused Panel Changed Signal
    void focusedPanelChanged(FilePanel* aFocusedPanel);

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);
    // Panel Focus Changed Signal
    void panelFocusChanged(const bool& aPanelFocus);

    // Modifier Keys Pressed Signal
    void modifierKeysChanged(const int& aModifierKeys);

    // Exit Key Released Signal
    void exitKeyReleased();

    // Current Index Changed Signal
    void currentIndexChanged(const int& aIndex);
    // Visual Items Count Changed
    void visualItemsCountChanged(const int& aVisualCount);

    // Extension Visible Changed Signal
    void extVisibleChanged(const bool& aExtVisible);
    // Type Visible Changed Signal
    void typeVisibleChanged(const bool& aTypeVisible);
    // Size Visible Changed Signal
    void sizeVisibleChanged(const bool& aSizeVisible);
    // Date Visible Changed Signal
    void dateVisibleChanged(const bool& aDateVisible);
    // Owner Visible Changed Signal
    void ownerVisibleChanged(const bool& aOwnerVisible);
    // Perms Visible Changed Signal
    void permsVisibleChanged(const bool& aPermsVisible);
    // Attributes Visible Changed Signal
    void attrsVisibleChanged(const bool& aAttrsVisible);

    // Extension Width Changed Signal
    void extWidthChanged(const int& aExtWidth);
    // Type Width Changed Signal
    void typeWidthChanged(const int& aTypeWidth);
    // Size Width Changed Signal
    void sizeWidthChanged(const int& aSizeWidth);
    // Date Width Changed Signal
    void dateWidthChanged(const int& aDateWidth);
    // Owner Width Changed Signal
    void ownerWidthChanged(const int& aOwnerWidth);
    // Permissions Width Changed Signal
    void permsWidthChanged(const int& aPermsWidth);
    // Attributes Width Changed Signal
    void attrsWidthChanged(const int& aAttrsWidth);

    // Sorting Changed Signal
    void sortingChanged(const int& aSorting);
    // Reverse Order Changed Signal
    void reverseOrderChanged(const bool& aReverseOrder);
    // Show Hidden Changed Signal
    void showHiddenChanged(const bool& aHidden);
    // Show Directories First Changed Signal
    void showDirsFirstChanged(const bool& aShow);
    // Case Sensitive Sorting Changed Signal
    void caseSensitiveSortChanged(const bool& aCaseSensitive);

    // Text Color Changed Signal
    void textColorChanged(const QString& aColor);
    // BG Color Changed Signal
    void textBGColorChanged(const QString& aColor);
    // Current Text Color Changed Signal
    void currentColorChanged(const QString& aColor);
    // Current BG Color Changed Signal
    void currentBGColorChanged(const QString& aColor);
    // Selected Color Changed Signal
    void selectedColorChanged(const QString& aColor);
    // Selected BG Color Changed Signal
    void selectedBGColorChanged(const QString& aColor);
    // Current Selected Color Changed Signal
    void currentSelectedColorChanged(const QString& aColor);
    // Current Selected BG Color Changed Signal
    void currentSelectedBGColorChanged(const QString& aColor);
    // Hidden Color Changed Signal
    void hiddenColorChanged(const QString& aColor);
    // Hidden BG Color Changed Signal
    void hiddenBGColorChanged(const QString& aColor);
    // Link Color Changed Signal
    void linkColorChanged(const QString& aColor);
    // Link BG Color Changed Signal
    void linkBGColorChanged(const QString& aColor);
    // Font Name Changed Signal
    void fontNameChanged(const QString& aFontName);
    // Font Size Changed Signal
    void fontSizeChanged(const int& aSize);
    // Font Bold Changed Signal
    void fontBoldChanged(const bool& aBold);
    // Font italic Changed Signal
    void fontItalicChanged(const bool& aItalic);

    // Thumb Width Changed Signal
    void thumbWidthChanged(const int& aWidth);
    // Thumb Height Changed Signal
    void thumbHeightChanged(const int& aHeight);

    // Busy Changed Signal
    void busyChanged(const bool& aBusy);

    // Loading Changed Signal
    void loadingChanged(const bool& aLoading);

    // Show Help Signal
    void showHelp();

    // Launch Terminal Signal
    void launchTerminal(const QString& aDirPath);
    // Launch Viewer Signal
    void launchViewer(const bool& aEditMode = false);

    // Launch File Copy Signal
    void launchFileCopy();
    // Launch File Move/Rename Signal
    void launchFileMove();
    // Launch File Rename
    void launchFileRename();

    // Launch Create Dir Signal
    void launchCreateDir();

    // Launch Search
    void launchSearch();

    // Launch Delete Signal
    void launchDelete();

    // Show Preferences Signal
    void showPreferences();

    // Error Occured Signal
    void error(const QString& aPath, const int& aError);

    // File Rename Active Changed Signal
    void fileRenameActiveChanged(const bool& aActive);

protected slots:

    // Init
    void init();
    // Clear
    void clear();
    // Restore UI
    void restoreUI(const bool& aReload = true);
    // Save Settings
    void saveSettings();
    // Update Available Space Label
    void updateAvailableSpaceLabel();
    // Update Supported Image Formates
    void updateSupportedImageFormats();

    // Start Dir Watcher Slot
    void startDirWatcher();
    // Stop Dir Watcher Slot
    void stopDirWatcher();

    // Handle Item Selection - Dir/Execution
    void handleItemSelection();
    // Handle Item Select
    void handleItemSelect();

protected slots: // From File Model

    // File List Model Busy Changed Slot
    void fileModelBusyChanged(const bool& aBusy);
    // File List Model Fetch Ready Slot
    void fileModelDirFetchFinished();
    // File List Model Dir Created Slot
    void fileModelDirCreated(const QString& aDirPath);
    // File List Model File Renames Slot
    void fileModelFileRenamed(const QString& aSource, const QString& aTarget);
    // File Model Error Slot
    void fileModelError(const QString& aPath, const int& aError);
    // File Model Need Confirm Found Slot
    void fileModelNeedConfirm(const int& aCode, const QString& aPath, const QString& aSource, const QString& aTarget);

protected slots: // From QFileSystemWatcher

    // Directory Changed Slot
    void directoryChanged(const QString& aDirPath);
    // File Changed Slot
    void fileChanged(const QString& aFilePath);

    // Refresh File List Model
    void refreshFileListModel(const QString& aFilePath);

protected slots: // From File Renamer

    // Rename Finished Slot
    void renamerFinished(const QString& aSource, const QString& aTarget);

protected slots: // From Dir Scanner

    // Scan Size Changed Slot
    void scanSizeChanged(const QString& aDirPath, const quint64& aSize);

protected slots:

    // Handle Modifier Key Press Event
    bool handleModifierKeyPressEvent(QKeyEvent* aEvent);
    // Handle Modifier Key Release Event
    bool handleModifierKeyReleaseEvent(QKeyEvent* aEvent);

private slots:

    // Home Button Clicked Slot
    void on_homeButton_clicked();
    // Root Button Clicked Slot
    void on_rootButton_clicked();

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

private:
    friend class MainWindow;

    // UI
    Ui::FilePanel*          ui;

    // Current Dir
    QString                 currentDir;

    // Panel Name
    QString                 panelName;

    // Panel Focus
    bool                    panelHasFocus;

    // File List Model
    FileListModel*          fileListModel;

    // Modifier Keys
    int                     modifierKeys;

    // Current Index
    int                     currentIndex;

    // Visual Items Count
    int                     visualItemsCount;

    // Last Directory Name To Jump After CD UP
    QString                 lastDirName;
    // Last File Name To Jump After Reload
    QString                 lastFileName;
    // Last Index Before Reload
    int                     lastIndex;

    // Supported Image Formats
    QStringList             supportedImageFormats;

    // Extensions Column Visible
    bool                    extVisible;
    // Type Column Visible
    bool                    typeVisible;
    // Size Column Visible
    bool                    sizeVisible;
    // Date Column Visible
    bool                    dateVisible;
    // Owner Column Visible
    bool                    ownerVisible;
    // Permissions Column Visible
    bool                    permsVisible;
    // Attributes Column Visible
    bool                    attrsVisible;

    // Extensions Column Width
    int                     extWidth;
    // Type Column Width
    int                     typeWidth;
    // Size Column Width
    int                     sizeWidth;
    // Date Column Width
    int                     dateWidth;
    // Owner Column Width
    int                     ownerWidth;
    // Permissions Column Width
    int                     permsWidth;
    // Attributes Column Width
    int                     attrsWidth;

    // Sorting Type
    int                     sorting;
    // Reverse Order
    bool                    reverseOrder;
    // Show Hidden Files
    bool                    showHiddenFiles;
    // Show Dirs First
    bool                    showDirsFirst;
    // Case Sensitive Sorting
    bool                    caseSensitiveSort;

    // Normal Text Color
    QString                 textColor;
    // Normal Text BG Color
    QString                 textBGColor;
    // Current Item Text Color
    QString                 currentColor;
    // Current Item BG Color
    QString                 currentBGColor;
    // Selected Item Text Color
    QString                 selectedColor;
    // Selected Item BG Color
    QString                 selectedBGColor;
    // Current Selected Item Text Color
    QString                 currentSelectedColor;
    // Current Selected Item BG Color
    QString                 currentSelectedBGColor;
    // Hidden Item Text Color
    QString                 hiddenColor;
    // Hidden Item BG Color
    QString                 hiddenBGColor;
    // Link Item Text Color
    QString                 linkColor;
    // Link Item BG Color
    QString                 linkBGColor;
    // Font Name
    QString                 fontName;
    // Font Size
    int                     fontSize;
    // Font Bold
    bool                    fontBold;
    // Font Italic
    bool                    fontItalic;

    // Thumb Width
    int                     thumbWidth;
    // Thumb Height
    int                     thumbHeight;


    // Dir File System Watcher
    QFileSystemWatcher      dirWatcher;
    // Dir File System Watcher Timer ID
    int                     dirWatcherTimerID;
    // Dir Changed
    bool                    dwDirChanged;
    // File Changed
    bool                    dwFileChanged;

    // Own Key Press
    bool                    ownKeyPress;

    // File Rename Active
    bool                    fileRenameActive;
    // File Ranemer
    FileRenamer*            fileRenamer;
    // File Renamer Update Is Done
    bool                    fileRenamerUpdate;
    // File Transfer Update
    bool                    fileTransferUpdate;
    // File Delete Update
    bool                    fileDeleteUpdate;

    // Dir Scanner
    DirScanner*             dirScanner;

    // Loading State Indicator
    bool                    loading;
};

















//==============================================================================
// File Renamer Controller Class
//==============================================================================
class FileRenamer : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit FileRenamer(QWidget* aParent = NULL);

    // Rename File
    void renameFile(const QString& aSource, const QString& aTarget);

    // Destructor
    virtual ~FileRenamer();

signals:

    // Renamer Finished Signal
    void finished(const QString& aSource, const QString& aTarget);

protected slots:

    // Add Item
    void addItem(const QString& aSource, const QString& aTarget);
    // Insert Item
    void insertItem(const int& aIndex, const QString& aSource, const QString& aTarget);
    // Set Item State
    void setItemState(const int& aIndex, const int& aState);

    // Process Queue
    void processQueue();
    // Abort
    void abort();
    // Clear Queue
    void clearQueue();

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

    // Need Confirmation Slot
    void fileOpNeedConfirm(const unsigned int& aID,
                           const QString& aOp,
                           const int& aCode,
                           const QString& aPath,
                           const QString& aSource,
                           const QString& aTarget);

    // File Operation Queue Item Found Slot
    void fileOpQueueItemFound(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aPath,
                              const QString& aSource,
                              const QString& aTarget);

protected:

    // File Util
    RemoteFileUtilClient*               fileUtil;
    // Rename Items Queue
    QList<TransferProgressModelItem*>   renameQueue;
    // Queue Index
    int                                 queueIndex;

    // ...

};














//==============================================================================
// Dir Scan Progess State Enum
//==============================================================================
enum DirScanProgressState
{
    EDSSIdle         = 0,
    EDSSRunning,
    EDSSPaused,
    EDSSFinished,
    EDSSSkipped,
    EDSSError
};




//==============================================================================
// Dir Scan Queue Item Class
//==============================================================================
class DirScannerQueueItem
{
public:

    // Constructor
    explicit DirScannerQueueItem(const QString& aDirPath);
    // Destructor
    virtual ~DirScannerQueueItem();

protected:
    friend class DirScanner;

    // Dir Path
    QString                 dirPath;
    // Size
    quint64                 dirSize;
    // Progress State
    DirScanProgressState    state;
};












//==============================================================================
// Dir Scanner Controller Class
//==============================================================================
class DirScanner : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit DirScanner(QObject* aParent = NULL);

    // Scan Dir
    void scanDir(const QString& aDirPath);

    // Abort
    void abort();

    // Clear Queue
    void clearQueue();

    // Destructor
    virtual ~DirScanner();

signals:

    // Scan Finished Signal
    void scanFinished(const QString& aDirPath);

    // Scan Size Changed
    void scanSizeChanged(const QString& aDirPath, const quint64& aSize);

protected slots:

    // Add Item
    void addItem(const QString& aDirPath);
    // Set Item State
    void setItemState(const int& aIndex, const int& aState);

    // Find Index
    int findIndex(const QString& aDirPath);

    // Process Queue
    void processQueue();

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

    // Dir Size Scan Progress Signal
    void fileOpDirSizeScanProgress(const unsigned int& aID,
                                   const QString& aPath,
                                   const quint64& aNumDirs,
                                   const quint64& aNumFiles,
                                   const quint64& aScannedSize);

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

protected: // For RemoteFileUtilClient

    // File Util
    RemoteFileUtilClient*               fileUtil;

    // Rename Items Queue
    QList<DirScannerQueueItem*>         scanQueue;

    // Queue Index
    int                                 queueIndex;
};


#endif // FILEPANEL_H
