#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>
#include <QMap>
#include <QAbstractButton>
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
    // Add Button With Action Index
    void addButton(const QString& aText, const QDialogButtonBox::ButtonRole& aButtonRole, const int& aActionIndex = 0);
    // Clear Buttons
    void clearButtons();
    // Get Action Index
    int getActionIndex();
    // Set Path
    void setPath(const QString& aPath);
    // Get Path
    QString getPath();

    // Exec
    virtual int exec();

    // Destructor
    virtual ~ConfirmDialog();

protected slots:

    // Button Clicked Slot
    void buttonClicked(QAbstractButton* aButton);

private:
    // UI
    Ui::ConfirmDialog*          ui;

    // Action Index
    int                         actionIndex;

    // Custom Buttons
    QMap<QAbstractButton*, int> customButtons;

};

#endif // CONFIRMDIALOG_H
