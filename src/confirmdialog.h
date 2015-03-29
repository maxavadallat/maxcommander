#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class ConfirmDialog;
}

//==============================================================================
// Confirm Dialog Class
//==============================================================================
class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit ConfirmDialog(QWidget* aParent = NULL);

    // Set Confirm Text
    void setConfirmText(const QString& aConfirmText);
    // Configure Buttons
    void configureButtons(const QDialogButtonBox::StandardButtons& aButtons = QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    // Add Button
    void addButton(const QString& aText, const QDialogButtonBox::ButtonRole& aButtonRole);
    // Clear Buttons
    void clearButtons();

    // Destructor
    virtual ~ConfirmDialog();

private:
    Ui::ConfirmDialog *ui;
};

#endif // CONFIRMDIALOG_H
