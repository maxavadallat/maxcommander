#ifndef DELETEPROGRESSMODEL_H
#define DELETEPROGRESSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>


//==============================================================================
// Delete Progress Queue Item State
//==============================================================================
enum DeleteProgressState
{
    EDPIdle         = 0,
    EDPRunning,
    EDPFinished,
    EDPSkipped,
    EDPError
};


//==============================================================================
// File Delete Progress Queue Model Item Class
//==============================================================================
class DeleteProgressModelItem
{
public:

    // Constructor
    explicit DeleteProgressModelItem(const QString& aFileName);
    // Destructor
    virtual ~DeleteProgressModelItem();

protected:
    friend class DeleteProgressModel;

    // File Name
    QString             fileName;
    // Progress State
    DeleteProgressState state;
};


//==============================================================================
// File Delete Progress Queue Model Class
//==============================================================================
class DeleteProgressModel : public QAbstractListModel
{
    Q_OBJECT

public:

    // Constructor
    explicit DeleteProgressModel(QObject* aParent = NULL);

    // Add Item
    void addItem(const QString& aFileName);
    // Insert Item
    void insertItem(const int& aIndex, const QString& aFileName);
    // Remove Item
    void removeItem(const int& aIndex);
    // Set Progress State
    void setProgressState(const int& aIndex, const DeleteProgressState& aState);

    // Get File Name
    QString getFileName(const int& aIndex);
    // Get Progress State
    DeleteProgressState getProgressState(const int& aIndex);

    // Find Index
    int findIndex(const QString& aFileName);

    // Destructor
    virtual ~DeleteProgressModel();

public slots:

    // Clear
    void clear();

signals:


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

    // Init
    void init();

protected:

    // Role ID's
    enum RolesIDs
    {
        ERIDFileName    = Qt::UserRole + 1,
        ERIDState
    };

    // Items
    QList<DeleteProgressModelItem*>     items;

};

#endif // DELETEPROGRESSMODEL_H
