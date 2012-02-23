#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

// INCLUDES

#include <QDialog>
#include <QDialogButtonBox>


// FORWARD DECLARATIONS

namespace Ui {
class ConfirmationDialog;
}


// DECLARATIONS

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

    //! @brief Destructor
    //! @param none
    virtual ~ConfirmationDialog();

protected:

    // UI
    Ui::ConfirmationDialog* ui;
};

#endif // CONFIRMATIONDIALOG_H
