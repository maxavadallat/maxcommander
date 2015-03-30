#ifndef DELETEPROGRESSDIALOG_H
#define DELETEPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteProgressDialog;
}

class DeleteProgressModel;


//==============================================================================
// Delete Progress Dialog Class
//==============================================================================
class DeleteProgressDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit DeleteProgressDialog(QWidget* aParent = NULL);

    // Set Current File Name
    void setCurrentFileName(const QString& aCurrentFileName);
    // Set Current File Progress
    void setCurrentProgress(const quint64& aProgress, const quint64& aTotal);

    // Destructor
    virtual ~DeleteProgressDialog();

private:
    // UI
    Ui::DeleteProgressDialog*   ui;

    // Queue Model
    DeleteProgressModel*        queueModel;
};

#endif // DELETEPROGRESSDIALOG_H
