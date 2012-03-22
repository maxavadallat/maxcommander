#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class AboutDialog;
}


// DECLARATIONS

//==============================================================================
//! @class AboutDialog About Dialog Class
//==============================================================================
class AboutDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit AboutDialog(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~AboutDialog();

protected:

    //! UI
    Ui::AboutDialog*    ui;
};

#endif // ABOUTDIALOG_H
