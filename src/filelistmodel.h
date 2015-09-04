#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H


#include <QFileInfo>
#include <QAbstractListModel>

#include "utility.h"

class RemoteFileUtilClient;


//==============================================================================
// File List Model Item Class
//==============================================================================
class FileListModelItem
{
public:
    // Constructor
    explicit FileListModelItem(const QString& aPath, const QString& aFileName);
    // Constructor
    explicit FileListModelItem(const QString& aPath, const qint64& aSize, const QDateTime& aDate, const QString& aAttrs, const bool& aIsDir, const bool& aIsLink);

    // File Info
    QFileInfo       fileInfo;
    // Archive File Info
    ArchiveFileInfo archiveFileInfo;
    // Dir Size
    quint64         dirSize;
    // Selected
    bool            selected;
    // Search Result
    bool            searchResult;
};




//==============================================================================
// File List Model Class
//==============================================================================
class FileListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir WRITE setCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(bool busy READ getBusy NOTIFY busyChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int selectedCount READ getSelectedCount NOTIFY selectedCountChanged)
    Q_PROPERTY(bool archiveMode READ getArchiveMode NOTIFY archiveModeChanged)

public:

    // Constructor
    FileListModel(QObject* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();
    // Set Current Dir
    void setCurrentDir(const QString& aDirPath);

    // Set Archive Current Dir
    void setArchiveCurrentDir(const QString& aFilePath, const QString& aDirPath);

    // Find Index by File Name
    int findIndex(const QString& aFileName);
    // Insert Item by File Name - For Newly Create Directory
    void insertDirItem(const QString& aFileName);

    // Get File Info
    QFileInfo getFileInfo(const int& aIndex);
    // Get Archive File Info
    ArchiveFileInfo getArchiveFileInfo(const int& aIndex);

    // Update Item
    void updateItem(const int& aIndex, const QFileInfo& aFileInfo);

    // Update Dir Size
    void updateDirSize(const int& aIndex, const quint64& aSize);

    // Check If Is Dir
    bool isDir(const int& aIndex);
    // Check If Is Bundle
    bool isBundle(const int& aIndex);
    // Check If Archive
    bool isArchive(const int& aIndex);

    // Get Busy
    bool getBusy();

    // Get Archive Mode
    bool getArchiveMode();

    // Set Sorting Order
    void setSorting(const int& aSorting);
    // Set Reverse Mode
    void setReverse(const bool& aReverse);
    // Set Show hidden Files
    void setShowHiddenFiles(const bool& aShow);
    // Set Show Dirs First
    void setShowDirsFirst(const bool& aShowDirsFirst);
    // Set Case Sensitive Sorting
    void setCaseSensitiveSorting(const bool& aCaseSensitiveSort);

    // Get File Count
    int getFileCount();

    // Create Dir
    void createDir(const QString& aDirPath);
    // Create Link
    void createLink(const QString& aLinkPath, const QString& aLinkTarget);

    // Get Last Operation
    QString lastOperation();

    // Send User Response
    void sendUserResponse(const int& aConfirm, const QString& aNewPath = "");

    // Append Item Manually
    void appendItem(const QString& aFilePath, const bool& aSearchResult = false);

    // Add Item Name-Sorted Manually
    void addItem(const QString& aFilePath, const bool& aSearchResult = false);

    // Remove Item
    void removeItem(const int& aIndex);

    // Set Selected Files Count
    void setSelectedCount(const int& aSelectedCount);
    // Get Selected Files Count
    int getSelectedCount();

    // Destructor
    virtual ~FileListModel();

public slots:

    // Clear
    void clear();
    // Reload
    void reload();

    // Get Selected
    bool getSelected(const int& aIndex);
    // Set Selected
    void setSelected(const int& aIndex, const bool& aSelected);

    // Has Selection
    bool hasSelection();

    // Select All
    void selectAll();
    // Deselect All
    void deselectAll();

    // Toggle All Selection
    void toggleAllSelection();

    // Select Files
    void selectFiles(const QString& aPattern);
    // Deselect Files
    void deselectFiles(const QString& aPattern);

    // Get All Selected Files
    QStringList getAllSelected();

    // Rename File
    void renameFile(const QString& aSource, const QString& aTarget);

    // Delete File
    void deleteFile(const QString& aFilePath);

    // Get File Name
    QString getFileName(const int& aIndex);

    // Get Full File Path
    QString getFullPath(const int& aIndex);

    // Get Dir Size
    quint64 getDirSize(const int& aIndex);

    // Check If Path Is External Drive/Volume
    bool isVolume(const QString& aFilePath);

    // Eject Volume
    void ejectVolume(const QString& aFilePath);

signals:

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);

    // Count Changed Signal
    void countChanged(const int& aCount);

    // Dir Fetch Finished Signal
    void dirFetchFinished();

    // Dir Created Signal
    void dirCreated(const QString& aDirPath);

    // Link Created Signal
    void linkCreated(const QString& aLinkPath, const QString& aLinkTarget);

    // Busy State Changed Signal
    void busyChanged(const bool& aBusy);

    // File Renamed Signal
    void fileRenamed(const QString& aSource, const QString& aTarget);

    // Error Signal
    void error(const QString& aPath, const QString aSource, const QString aTarget, const int& aError);

    // Need Confirm Signal
    void needConfirm(const int& aCode, const QString& aPath, const QString& aSource, const QString& aTarget);

    // Selected Count Changed Signal
    void selectedCountChanged(const int& aSelectedCount);

    // Archive Mode Changed Signal
    void archiveModeChanged(const bool& aArchiveMode);

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

protected slots:

    // Init
    void init();
    // Fetch Dir
    void fetchDirItems();
    // Fetch Archive Dir Items
    void fetchArchiveDirItems();
    // Set Archive Mode
    void setArchiveMode(const bool& aArchiveMode);

protected slots: // For Remote File Client

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
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

    // Dir List Item Found Slot
    void dirListItemFound(const unsigned int& aID,
                          const QString& aPath,
                          const QString& aFileName);

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

    // Archive File List Item Found Slot
    void archiveListItemFound(const unsigned int& aID,
                              const QString& aArchive,
                              const QString& aFilePath,
                              const qint64& aSize,
                              const QDateTime& aDate,
                              const QString& aAttribs,
                              const bool& aIsDir,
                              const bool& aIsLink);

protected:

    // Roles
    enum Roles {
        FileName = Qt::UserRole + 1,
        FileExtension,
        FileType,
        FileAttributes,
        FileSize,
        FileDateTime,
        FileOwner,
        FilePerms,
        FileSelected,
        FileSearchResult,
        FileIsHidden,
        FileIsLink,
        FileIsDir,
        FileIsArchive,
        FileFullName,
        FileDirSize,

        FileRolesCount
    };

    // Current Dir
    QString                             currentDir;
    // Prev Current Dir
    QString                             prevCurrentDir;
    // File Info List
    QList<FileListModelItem*>           itemList;
    // File Name List - For Quicker Search
    QStringList                         fileNameList;

    // Remote File Client
    RemoteFileUtilClient*               fileUtil;

    // Sorting Mode
    int                                 sorting;
    // Reverse Order
    bool                                reverseOrder;
    // Show Hidden Files
    bool                                showHiddenFiles;
    // Show Dirs First
    bool                                showDirsFirst;
    // Case Sensitive Sorting
    bool                                caseSensitiveSort;

    // Selected Count
    int                                 selectedCount;

    // Fetch Dir On Connection
    bool                                fetchOnConnection;

    // Archive Mode
    bool                                archiveMode;
    // Archive Path
    QString                             archivePath;

    // ...
};

#endif // FILELISTMODEL_H
