#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

// INCLUDES

#include <QDialog>
#include <QDialogButtonBox>
#include <QButtonGroup>


// FORWARD DECLARATIONS

namespace Ui {
class ConfirmationDialog;
}


// DECLARATIONS

//==============================================================================
//! @class ConfirmationDialog Confirmation Dialog Class
//==============================================================================
class ConfirmationDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit ConfirmationDialog(QWidget* aParent = NULL);

    //! @brief Set Title
    //! @param aTitle Dialog Title
    void setTitle(const QString& aTitle);

    //! @brief Set Confirm Message
    //! @param aConfirmMsg Confirm Message
    void setConfirmMsg(const QString& aConfirmMsg);

    //! @brief Set Icon
    //! @param aPixmap Icon Pixmap
    void setIcon(const QPixmap& aPixmap);

    //! @brief Set Buttons
    //! @param aButtons Buttons
    //! @param aButtons Default Button
    void setButtons(const QDialogButtonBox::StandardButtons& aButtons, const QDialogButtonBox::StandardButton& aDefault);

    //! @brief Add Button
    //! @param aButtonText Button Text
    //! @param aButtonRole Button Role
    //! @param aID Button ID
    void addButton(const QString& aButtonText, QDialogButtonBox::ButtonRole aButtonRole, const int& aID);

    //! @brief Destructor
    //! @param none
    virtual ~ConfirmationDialog();

protected slots:

    //! @brief Button Box Clicked Slot
    //! @param aButton Button
    void on_buttonBox_clicked(QAbstractButton* aButton);

protected:

    //! @brief Clear Custom Buttons
    //! @param none
    void clearCustomButtons();

protected: // Data

    //! UI
    Ui::ConfirmationDialog* ui;

    //! Button Group For Custom Buttons
    QButtonGroup*           buttonGroup;
};

#endif // CONFIRMATIONDIALOG_H
