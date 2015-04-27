#ifndef TRANSFERPROGRESSMODEL_H
#define TRANSFERPROGRESSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QList>


//==============================================================================
// Transfer Progress Item State
//==============================================================================
enum TransferProgressState
{
    ETPIdle         = 0,
    ETPRunning,
    ETPFinished,
    ETPSkipped,
    ETPError
};


//==============================================================================
// File Transfer Progress Queue Model Item Class
//==============================================================================
class TransferProgressModelItem
{
public:
    // Constructor
    TransferProgressModelItem(const QString& aOp, const QString& aSource, const QString& aTarget);
    // Destructor
    virtual ~TransferProgressModelItem();

protected:
    friend class TransferProgressModel;
    friend class FileRenamer;

    // Operation
    QString                 op;
    // Source
    QString                 source;
    // Target
    QString                 target;
    // State
    TransferProgressState   state;
};



//==============================================================================
// File Transfer Progress Queue Model Class
//==============================================================================
class TransferProgressModel : public QAbstractListModel
{
    Q_OBJECT

public:

    // Constructor
    explicit TransferProgressModel(QObject* aParent = NULL);

    // Add Item
    void addItem(const QString& aOp, const QString& aSource, const QString& aTarget);
    // Insert Item
    void insertItem(const int& aIndex, const QString& aOp, const QString& aSource, const QString& aTarget);
    // Remove Item
    void removeItem(const int& aIndex);
    // Set Progress State
    void setProgressState(const int& aIndex, const TransferProgressState& aState);

    // Get Operation
    QString getOperation(const int& aIndex);
    // Get Source File Name
    QString getSourceFileName(const int& aIndex);
    // Get Target File Name
    QString getTargetFileName(const int& aIndex);
    // Get Progress State
    TransferProgressState getProgressState(const int& aIndex);

    // Find Index
    int findIndex(const QString& aSourceFileName);

    // Destructor
    virtual ~TransferProgressModel();

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
        ERIDOp      = Qt::UserRole + 1,
        ERIDSource,
        ERIDTarget,
        ERIDState
    };

    // Items
    QList<TransferProgressModelItem*>   items;
};

#endif // TRANSFERPROGRESSMODEL_H

