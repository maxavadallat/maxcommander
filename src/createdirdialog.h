#ifndef CREATEDIRDIALOG_H
#define CREATEDIRDIALOG_H

// INCLUDES

#include <QFrame>
#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class CreateDirDialog;
}


// DECLARATIONS

//==============================================================================
//! @class CreateDirDialog Create Directory Dialog Class
//==============================================================================
class CreateDirDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit CreateDirDialog(QWidget* aParent = NULL);

    //! @brief Set Dir Text
    //! @param aDirPath Directory Path
    void setDirText(const QString& aDirPath);

    //! @brief Get Dir Text
    //! @param none
    //! @return Dir Text
    QString getDirText();

    //! @brief Destructor
    //! @param none
    virtual ~CreateDirDialog();

signals:

    //! @brief Dialog Closed Signal
    //! @param aButton Button Index
    void dialogClosed(const int& aButton);

protected slots:
/*
    //! @brief Ok Button Clicked Slot
    //! @param none
    void on_okButton_clicked();

    //! @brief Cancel Button Clicked Slot
    //! @param none
    void on_cancelButton_clicked();
*/

    //! @brief Button Box Accepted Slot
    //! @param none
    void on_buttonBox_accepted();

    //! @brief Button Box Rejected Slot
    //! @param none
    void on_buttonBox_rejected();

protected: // Data

    //! UI
    Ui::CreateDirDialog*    ui;
};

#endif // CREATEDIRDIALOG_H

