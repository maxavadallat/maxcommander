#ifndef LISTMODEL_H
#define LISTMODEL_H

// INCLUDES

#include <QObject>
#include <QList>


// FORWARD DECLARATIONS

class ListBox;
class ItemDelegate;


// DECLARATIONS

//==============================================================================
//! @class ListModelItemData List Box Model Item Data Class
//==============================================================================
class ListModelItemData : public QObject
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    ListModelItemData(QObject* aParent = NULL);

    //! @brief Set Title
    //! @param aTitle New Title
    void setTitle(const QString& aTitle);

    //! @brief Get Title
    //! @param none
    //! @return Title
    QString getTitle();

    //! @brief Set Current Flag
    //! @param aCurrent Current Flag
    void setCurrent(const bool& aCurrent);

    //! @brief Set Selected Flag
    //! @param aSelected Selected Flag
    virtual void setSelected(const bool& aSelected);

    //! @brief Toggle Selected Flag
    //! @param aSelected Selected Flag
    void toggleSelected();

    //! @brief Destructor
    //! @param none
    virtual ~ListModelItemData();

signals:

    //! @brief Data Updated Signal
    //! @param none
    void dataUpdated();

protected:

    //! @brief Trigger Update
    //! @param none
    void triggerUpdate();

protected: // Data
    friend class ListModel;
    friend class ItemDelegate;
    friend class ListBox;
    friend class FileListDelegate;
    friend class IconScanner;

    //! Index
    int     index;
    //! Selected
    bool    selected;
    //! Current
    bool    current;
    //! Title
    QString title;
};










//==============================================================================
//! @class ListModel List Box Model Class
//==============================================================================
class ListModel : public QObject
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit ListModel(QObject* aParent = NULL);

    //! @brief Append Item
    //! @param aItem New Item
    //! @param aUpdate Update
    void appendItem(ListModelItemData* aItem, const bool& aUpdate = true);

    //! @brief Insert Item
    //! @param aIndex New Item Index
    //! @param aItem New Item
    //! @param aUpdate Update
    void insertItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate = true);

    //! @brief Replace Item
    //! @param aIndex New Item Index
    //! @param aItem New Item
    //! @param aUpdate Update
    void replaceItem(const int& aIndex, ListModelItemData* aItem, const bool& aUpdate = true);

    //! @brief Remove Item
    //! @param aIndex New Item Index
    //! @param aUpdate Update
    void removeItem(const int& aIndex, const bool& aUpdate = true);

    //! @brief Clear
    //! @param none
    void clear();

    //! @brief Get Count
    //! @param none
    //! @return Count
    int count();

    //! @brief Get Item Data
    //! @param aIndex Item Index
    //! @return Item Data
    ListModelItemData* getItem(const int& aIndex);

    //! @brief Destructor
    //! @param none
    virtual ~ListModel();

signals:

    //! @brief Item Added Signal
    //! @param aIndex Item Index
    void itemAdded(const int& aIndex);

    //! @brief Item Removed Signal
    //! @param aIndex Item Index
    void itemRemoved(const int& aIndex);

    //! @brief Item Updated Signal
    //! @param aIndex Item Index
    void itemUpdated(const int& aIndex);

protected:
    friend class ListBox;
    friend class FileListBox;

    //! Model Data
    QList<ListModelItemData*>   data;
};

#endif // LISTMODEL_H
