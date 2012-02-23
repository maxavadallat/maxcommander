#ifndef INFODIALOG_H
#define INFODIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class InfoDialog;
}


// DECLARATIONS

//==============================================================================
//! @class InfoDialog Info Dialog Class
//==============================================================================
class InfoDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit InfoDialog(QWidget* aParent = NULL);

    //! @brief Set Title
    //! @param aTitle Dialog Title
    void setTitle(const QString& aTitle);

    //! @brief Set Info Message
    //! @param aInfoMsg Info Message
    void setInfoMsg(const QString& aInfoMsg);

    //! @brief Set Icon
    //! @param aPixmap Icon Pixmap
    void setIcon(const QPixmap& aPixmap);

    //! @brief Destructor
    //! @param none
    virtual ~InfoDialog();

protected:

    //! UI
    Ui::InfoDialog* ui;
};

#endif // INFODIALOG_H
