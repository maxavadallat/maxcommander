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

signals:

    //! @brief Info Dialog Finished Signal
    //! @param aDialog Dialog
    void infoDialogFinished(QDialog* aDialog);

protected slots:

    //! @brief Dialog Finished Slot
    //! @param aResult Dialog Result
    void on_InfoDialog_finished(int aResult);

protected:

    //! UI
    Ui::InfoDialog* ui;
};

#endif // INFODIALOG_H
