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

    //! @brief Set Standard Buttons
    //! @param aButtons Buttons
    //! @param aDefault Default Button
    void setStandardButtons(const QDialogButtonBox::StandardButtons& aButtons, const QDialogButtonBox::StandardButton& aDefault);

    //! @brief Set Default Standard Button
    //! @param aDefault Default Button
    void setDefaultStandardButton(const QDialogButtonBox::StandardButton& aDefault);

    //! @brief Clear Buttons
    //! @param none
    void clearButtons();

    //! @brief Add Button
    //! @param aButtonText Button Text
    //! @param aButtonRole Button Role
    //! @param aID Button ID
    //! @param aDefault Default Button
    void addButton(const QString& aButtonText, QDialogButtonBox::ButtonRole aButtonRole, const int& aID, const bool& aDefault = false);

    //! @brief Set Clear Custom Buttons On Confirm
    //! @param aClear Clear Custom Buttons
    void setClearCustomButtonsOnConfirm(const bool& aClear);

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

    //! @brief Update Size
    //! @param none
    void updateSize();

protected: // Data

    //! UI
    Ui::ConfirmationDialog* ui;

    //! Button Group For Custom Buttons
    QButtonGroup*           buttonGroup;

    //! Clear Custom Buttons On Selection
    bool                    clearCustomButtonsOnConfirm;
};

#endif // CONFIRMATIONDIALOG_H
