#ifndef CREATEDIRDIALOG_H
#define CREATEDIRDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirDialog;
}

//==============================================================================
// Create Dialog Class
//==============================================================================
class CreateDirDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit CreateDirDialog(QWidget* aParent = NULL);
    // Get Directory Name
    QString getDirectory();
    // Destructor
    virtual ~CreateDirDialog();

private:
    // UI
    Ui::CreateDirDialog*    ui;
};

#endif // CREATEDIRDIALOG_H
