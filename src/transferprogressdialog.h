#ifndef TRANSFERPROGRESSDIALOG_H
#define TRANSFERPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class TransferProgressDialog;
}

//==============================================================================
// Transfer Progress Dialog Class
//==============================================================================
class TransferProgressDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit TransferProgressDialog(QWidget* aParent = NULL);

    // Set Title
    void setTitle(const QString& aTitle);

    // Set Current File Name
    void setCurrentFileName(const QString& aCurrentFileName);
    // Set Current File Progress
    void setCurrentProgress(const quint64& aProgress, const quint64& aTotal);
    // Set Overall Progress
    void setOverallProgress(const quint64& aProgress, const quint64& aTotal);

    // Destructor
    virtual ~TransferProgressDialog();

private:
    // UI
    Ui::TransferProgressDialog*     ui;
};

#endif // TRANSFERPROGRESSDIALOG_H
