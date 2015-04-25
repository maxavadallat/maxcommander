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
    explicit FileListModelItem(const QString& aPath, const QString& aFileName);

    // File Info
    QFileInfo       fileInfo;
    // Selected
    bool            selected;
};


//==============================================================================
// File List Model Class
//==============================================================================
class FileListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir WRITE setCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(bool busy READ getBusy NOTIFY busyChanged)

public:

    // Constructor
    FileListModel(QObject* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();
    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);

    // Find Index
    int findIndex(const QString& aFileName);
    // Insert Item by File Name - For Newly Create Directory
    void insertItem(const QString& aFileName);

    // Get File Info
    QFileInfo getFileInfo(const int& aIndex);
    // Check If Is Dir
    bool isDir(const int& aIndex);
    // Check If Is Bundle
    bool isBundle(const int& aIndex);

    // Get Busy
    bool getBusy();

    // Set Sorting Order
    void setSorting(const int& aSorting);
    // Set Reverse Mode
    void setReverse(const bool& aReverse);

    // Get File Count
    int getFileCount();

    // Get File Index By FileName
    int getFileIndex(const QString& aFileName);

    // Create Dir
    void createDir(const QString& aDirPath);

    // Has Selection
    bool hasSelection();

    // Get Last Operation
    QString lastOperation();

    // Send User Response
    void sendUserResponse(const int& aConfirm, const QString& aNewPath = "");

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

    // Select All
    void selectAll();
    // Deselect All
    void deselectAll();

    // Toggle All Selection
    void toggleAllSelection();

    // Get All Selected Files
    QStringList getAllSelected();

    // Rename File
    void renameFile(const QString& aSource, const QString& aTarget);

    // Get File Name
    QString getFileName(const int& aIndex);

signals:

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);

    // Dir Fetch Finished Signal
    void dirFetchFinished();

    // Dir Created Signal
    void dirCreated(const QString& aDirPath);

    // Busy State Changed Signal
    void busyChanged(const bool& aBusy);

    // File Renamed Signal
    void fileRenamed(const QString& aSource, const QString& aTarget);

    // Error
    void error(const QString& aPath, const int& aError);

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

    // Delete Item
    void deleteItem(const int& aIndex);

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
        FileIsHidden,
        FileIsLink,
        FileFullName,

        FileRolesCount
    };

    // Current Dir
    QString                     currentDir;

    // File Info List
    QList<FileListModelItem*>   itemList;
    // File Name List - For Quicker Search
    QStringList                 fileNameList;

    // Remote File Client
    RemoteFileUtilClient*       fileUtil;

    // Sorting Mode
    int                         sorting;

    // Reverse Order
    bool                        reverseOrder;

    // Selected Count
    int                         selectedCount;

    // ...
};

#endif // FILELISTMODEL_H
