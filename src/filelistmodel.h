#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H


#include <QFileInfo>
#include <QAbstractListModel>

#include "utility.h"

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

protected:

    // Current Dir
    QString                     currentDir;

    // File Info List
    QList<FileListModelItem*>   fileInfoList;

    // ...
};

#endif // FILELISTMODEL_H
