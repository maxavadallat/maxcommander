#ifndef LISTPOPUP_H
#define LISTPOPUP_H

// INCLUDES

#include <QFrame>
#include <QMenu>
#include <QAction>


// DECLARATIONS

//==============================================================================
//! @class ListPopup Custom File List Popup Menu Class
//==============================================================================
class ListPopup : public QMenu
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit ListPopup(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~ListPopup();

protected: // Data

};

#endif // LISTPOPUP_H
