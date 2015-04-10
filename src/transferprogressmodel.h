#ifndef TRANSFERPROGRESSMODEL_H
#define TRANSFERPROGRESSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QList>


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

    // Operation
    QString     op;
    // Source
    QString     source;
    // Target
    QString     target;
    // Done
    bool        done;
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
    // Remove Item
    void removeItem(const int& aIndex);
    // Set Completed
    void setDone(const int& aIndex, const bool& aDone = true);

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

protected:

    // Init
    void init();

protected:

    // Role ID's
    enum RolesIDs
    {
        ERIDOp      = 0,
        ERIDSource,
        ERIDTarget,
        ERIDDone
    };

    // Items
    QList<TransferProgressModelItem*>   items;
};

#endif // TRANSFERPROGRESSMODEL_H

