#ifndef CREATELINKDIALOG_H
#define CREATELINKDIALOG_H

#include <QDialog>

namespace Ui {
class CreateLinkDialog;
}

//==============================================================================
// Create Link Dialog Class
//==============================================================================
class CreateLinkDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit CreateLinkDialog(QWidget* aParent = NULL);

    // Destructor
    virtual ~CreateLinkDialog();

protected slots:

    // On Browse Target Button Clicked Slot
    void on_browseTargetButton_clicked();

private:

    // UI
    Ui::CreateLinkDialog*  ui;
};

#endif // CREATELINKDIALOG_H
