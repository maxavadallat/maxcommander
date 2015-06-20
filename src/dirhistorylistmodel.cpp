#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "dirhistorylistmodel.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
DirHistoryListModel::DirHistoryListModel(const QString& aPanelName, QObject* aParent)
    : QAbstractListModel(aParent)
    , panelName(aPanelName)
{
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void DirHistoryListModel::init()
{
    // Load History
    loadHistory();
}

//==============================================================================
// Load History
//==============================================================================
void DirHistoryListModel::loadHistory()
{
    // Clear Items
    clear();

    qDebug() << "DirHistoryListModel::loadHistory";

    // Init Dir History List File
    QFile dhlFile(QDir::homePath() + "/" + QString(DEFAULT_FILE_LIST_DIR_HSITORY_FILENAME).arg(panelName));

    // Open File
    if (dhlFile.open(QIODevice::ReadOnly)) {
        // Init Text Stream
        QTextStream dhlStream(&dhlFile);

        // Begin Reset Model
        beginResetModel();

        // Go Thru Item List
        while (!dhlStream.atEnd()) {
            // Read Line
            QString line = dhlStream.readLine();
            // Append To List
            items << line;
        }

        // End Reset Model
        endResetModel();

        // Close File
        dhlFile.close();
    }
}

//==============================================================================
// Save History
//==============================================================================
void DirHistoryListModel::saveHistory()
{
    qDebug() << "DirHistoryListModel::saveHistory";

    // Init Dir History List File
    QFile dhlFile(QDir::homePath() + "/" + QString(DEFAULT_FILE_LIST_DIR_HSITORY_FILENAME).arg(panelName));

    // Open File
    if (dhlFile.open(QIODevice::WriteOnly)) {
        // Init Text Stream
        QTextStream dhlStream(&dhlFile);

        // Get Items Count
        int dhlCount = items.count();

        // Go Thru File Search Patterns
        for (int i=0; i<dhlCount; ++i) {
            // Get Item Text
            QString itemText = items[i];
            // Write Item
            dhlStream << itemText;
            // Write New Line
            dhlStream << "\n";
        }

        // Close File
        dhlFile.close();
    }
}

//==============================================================================
// Clear
//==============================================================================
void DirHistoryListModel::clear()
{
    // Check Items Count
    if (items.count() > 0) {
        qDebug() << "DirHistoryListModel::clear";

        // Begin Reset Model
        beginResetModel();
        // Clear Items
        items.clear();
        // End Reset Model
        endResetModel();
    }
}

//==============================================================================
// Add New History Item
//==============================================================================
void DirHistoryListModel::addNewHistoryItem(const QString& aDirPath)
{
    qDebug() << "DirHistoryListModel::addNewHistoryItem - aDirPath: " << aDirPath;


    // Init Local Dir Path
    QString localDirPath = aDirPath;
    // Check Local Dir Path
    if (localDirPath != QString("/") && localDirPath.endsWith("/")) {
        // Truncate
        localDirPath.truncate(localDirPath.length() - 1);
    }

    // Get Index Of Item
    int itemIndex = items.indexOf(aDirPath);

    // Check Item Index
    if (itemIndex >= 0) {
        // Remove Previous Occurence
        items.removeAt(itemIndex);
    }

    // Begin Insert Rows
    beginInsertRows(QModelIndex(), 0, 0);

    // Insert New Item
    items.insert(0, localDirPath);

    // End Insert Rows
    endInsertRows();

    // ...

    // Clear Old Items
    while (items.count() > DEFAULT_DIR_HISTORY_LIST_ITEMS_MAX) {
        // Remove Last
        items.removeLast();
    }
}

//==============================================================================
// Get History Item
//==============================================================================
QString DirHistoryListModel::getItem(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < items.count()) {
        return items[aIndex];
    }

    return "";
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> DirHistoryListModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File Name
    roles[DirPath]         = "dirPath";

    return roles;

}

//==============================================================================
// Get Row Count
//==============================================================================
int DirHistoryListModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return items.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int DirHistoryListModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 1;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant DirHistoryListModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {
        // Switch Role
        switch (aRole) {
            case DirPath: return items[aIndex.row()];

            default:
            break;
        }
    }

    return "";
}

//==============================================================================
// Set Data
//==============================================================================
bool DirHistoryListModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    Q_UNUSED(aIndex);
    Q_UNUSED(aValue);
    Q_UNUSED(aRole);

    return false;
}

//==============================================================================
// Destructor
//==============================================================================
DirHistoryListModel::~DirHistoryListModel()
{
    // Save History
    saveHistory();

    // CLear
    clear();

    // ...
}

