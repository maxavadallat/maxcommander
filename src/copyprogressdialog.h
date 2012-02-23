#ifndef COPYPROGRESSDIALOG_H
#define COPYPROGRESSDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class CopyProgressDialog;
}


// DECLARATIONS

//==============================================================================
//! @class CopyProgressDialog Copy Progress Dialog Class
//==============================================================================
class CopyProgressDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit CopyProgressDialog(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~CopyProgressDialog();

protected:

    //! UI
    Ui::CopyProgressDialog* ui;
};

#endif // COPYPROGRESSDIALOG_H
