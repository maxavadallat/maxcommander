#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class SearchDialog;
}


// DECLARATIONS

//==============================================================================
//! @class SearchDialog Search Dialog Class
//==============================================================================
class SearchDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit SearchDialog(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~SearchDialog();

protected:

    //! UI
    Ui::SearchDialog*   ui;
};

#endif // SEARCHDIALOG_H
