#ifndef DELETEPROGRESSMODEL_H
#define DELETEPROGRESSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>


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
    QString     fileName;
    // Done
    bool        done;
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
    // Remove Item
    void removeItem(const int& aIndex);

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

protected:

    // Init
    void init();

protected:

    // Role ID's
    enum RolesIDs
    {
        ERIDFileName    = 0,
        ERIDDone
    };

    // Items
    QList<DeleteProgressModelItem*>     items;

};

#endif // DELETEPROGRESSMODEL_H
