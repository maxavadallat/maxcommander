#ifndef COPYDIALOG_H
#define COPYDIALOG_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class CopyDialog;
}


// DECLARATIONS

//==============================================================================
//! @class CopyDialog Copy Dialog Class
//==============================================================================
class CopyDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit CopyDialog(QWidget* aParent = NULL);

    //! @brief Set Dialog Title Text
    //! @param aTitle Title Text
    void setTitle(const QString& aTitle);

    //! @brief Set Dialog Copy Msg Text
    //! @param aCopyMsg Copy Msg Text
    void setCopyMsg(const QString& aCopyMsg);

    //! @brief Set Target Text
    //! @param aTarget Target Path
    void setTarget(const QString& aTarget);

    //! @brief Get Target Text
    //! @param none
    //! @return Target Text
    QString getTarget();

    //! @brief Destructor
    //! @param none
    virtual ~CopyDialog();

protected:

    //! UI
    Ui::CopyDialog*     ui;
};

#endif // COPYDIALOG_H
