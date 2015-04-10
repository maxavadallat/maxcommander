#ifndef CREATEDIRDIALOG_H
#define CREATEDIRDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirDialog;
}

//==============================================================================
// Create Dir Dialog Class
//==============================================================================
class CreateDirDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit CreateDirDialog(QWidget* aParent = NULL);
    // Get Directory Name
    QString getDirectory();
    // Set Directory Path
    void setDirPath(const QString& aDirPath);

    // Exec
    virtual int exec();

    // Destructor
    virtual ~CreateDirDialog();

private:
    // UI
    Ui::CreateDirDialog*    ui;
};

#endif // CREATEDIRDIALOG_H
