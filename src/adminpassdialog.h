#ifndef ADMINPASSDIALOG_H
#define ADMINPASSDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class AdminPassDialog;
}


// DECLARATIONS

//==============================================================================
//! @class AdminPassDialog Admin Pass Query Dialog Class
//==============================================================================
class AdminPassDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit AdminPassDialog(QWidget* aParent = NULL);

    //! @brief Set Password
    //! @param aPassword Password
    void setPassword(const QString& aPassword);

    //! @brief Get Password
    //! @param nonw
    //! @return Password
    QString getPassword();

    //! @brief Destructor
    //! @param none
    virtual ~AdminPassDialog();

protected:

    //! UI
    Ui::AdminPassDialog*    ui;
};

#endif // ADMINPASSDIALOG_H
