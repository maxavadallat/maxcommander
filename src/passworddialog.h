#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

//==============================================================================
// Password Dialog Class
//==============================================================================
class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit PasswordDialog(QWidget* aParent = NULL);

    // Get Pass
    QString getPass();

    // Destructor
    virtual ~PasswordDialog();

private:
    // UI
    Ui::PasswordDialog* ui;
};

#endif // PASSWORDDIALOG_H
