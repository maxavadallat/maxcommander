#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

//==============================================================================
// Info Dialog Type
//==============================================================================
enum EInfoDialogType
{
    EIDTInfo    = 0,
    EIDTWarning,
    EIDTError
};

//==============================================================================
// Info Dialog Class
//==============================================================================
class InfoDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit InfoDialog(const QString& aInfoText, const EInfoDialogType& aType = EIDTInfo, QWidget* aParent = NULL);

    // Set Text
    void setText(const QString& aInfoText);
    // Set Type
    void setType(const EInfoDialogType& aType);

    // Destructor
    virtual ~InfoDialog();

private:
    // UI
    Ui::InfoDialog*     ui;
};

#endif // INFODIALOG_H
