#ifndef DELETEFILEDIALOG_H
#define DELETEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteFileDialog;
}

//==============================================================================
// Delete File Dialog Class
//==============================================================================
class DeleteFileDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit DeleteFileDialog(QWidget* aParent = NULL);
    // Get File Name
    QString getFileName();
    // Set File Name
    void setFileName(const QString& aFileName);
    // Destructor
    virtual ~DeleteFileDialog();

private:

    // UI
    Ui::DeleteFileDialog*   ui;
};

#endif // DELETEFILEDIALOG_H
