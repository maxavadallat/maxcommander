#ifndef DIRHISTORYLISTMODEL_H
#define DIRHISTORYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>


//==============================================================================
// Dir History List Model Class
//==============================================================================
class DirHistoryListModel : public QAbstractListModel
{
public:

    // Constructor
    DirHistoryListModel(const QString& aPanelName, QObject* aParent = NULL);

    // Add New History Item
    void addNewHistoryItem(const QString& aDirPath);

    // Get History Item
    QString getItem(const int& aIndex);

    // Clear
    void clear();

    // Destructor
    virtual ~DirHistoryListModel();

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
    // Load History
    void loadHistory();
    // Save History
    void saveHistory();

protected:

    // Roles
    enum Roles {
        DirPath = Qt::UserRole + 1,
        RolesCount
    };


    // Panel Name
    QString         panelName;

    // Model Items
    QStringList     items;
};

#endif // DIRHISTORYLISTMODEL_H
