#ifndef HELPDIALOG_H
#define HELPDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class HelpDialog;
}


// DECLARATIONS

//==============================================================================
//! @class HelpDialog Help Dialog Class
//==============================================================================
class HelpDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit HelpDialog(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~HelpDialog();

protected:

    //! UI
    Ui::HelpDialog*     ui;
};

#endif // HELPDIALOG_H
