#ifndef DELETEPROGRESSDIALOG_H
#define DELETEPROGRESSDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class DeleteProgressDialog;
}


// DECLARATIONS

//==============================================================================
//! @class DeleteProgressDialog Delete Progress Dialog Class
//==============================================================================
class DeleteProgressDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit DeleteProgressDialog(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~DeleteProgressDialog();

protected:

    //! UI
    Ui::DeleteProgressDialog*   ui;
};

#endif // DELETEPROGRESSDIALOG_H
