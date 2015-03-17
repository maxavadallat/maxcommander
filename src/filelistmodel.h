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
    FileListModelItem();

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

public:

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

        FileRolesCount
    };

    // Constructor
    FileListModel(QObject* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();
    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);

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

    // Destructor
    virtual ~FileListModel();

public slots:

    // Clear
    void clear();

signals:

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);

protected slots:

    // Init
    void init();

    // Fetch Dir
    void fetchDirItems();

protected slots: // For Remote File Client

    // File Operation Progress Slot
    void fileOpProgress(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aCurrFilePath,
                        const quint64& aCurrProgress,
                        const quint64& aCurrTotal,
                        const quint64& aOverallProgress,
                        const quint64& aOverallTotal,
                        const int& aSpeed);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aSource,
                        const QString& aTarget,
                        const int& aError);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // Need Confirmation Slot
    void fileOpNeedConfirm(const unsigned int& aID,
                           const QString& aOp,
                           const QString& aCode,
                           const QString& aSource,
                           const QString& aTarget);

    // Dir Size Scan Progress Slot
    void dirSizeScanProgress(const unsigned int& aID,
                             const QString& aPath,
                             const quint64& aNumDirs,
                             const quint64& aNumFiles,
                             const quint64& aScannedSize);

    // Dir List Item Found Slot
    void dirListItemFound(const unsigned int& aID,
                          const QString& aPath,
                          const QString& aFileName);

    // File Operation Queue Item Found Slot
    void fileOpQueueItemFound(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aSource,
                              const QString& aTarget);

protected:

    // Current Dir
    QString                     currentDir;

    // File Info List
    QList<FileListModelItem*>   fileInfoList;

    // Remote File Client
    RemoteFileUtilClient*       fileUtil;

    // ...
};

#endif // FILELISTMODEL_H
