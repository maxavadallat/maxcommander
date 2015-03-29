#ifndef TRANSFERFILEDIALOG_H
#define TRANSFERFILEDIALOG_H

#include <QDialog>

namespace Ui {
class TransferFileDialog;
}

//==============================================================================
// Transfer File Dialog Class
//==============================================================================
class TransferFileDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit TransferFileDialog(QWidget* aParent = NULL);
    // Get Source File Text
    QString getSourceFileText();
    // Set Source File Text
    void setSourceFileText(const QString& aSourceFile);
    // Get Target File Text
    QString getTargetFileText();
    // Set Target Fiel Text
    void setTargetFileText(const QString& aTargetFile);
    // Destructor
    virtual ~TransferFileDialog();

private:

    // UI
    Ui::TransferFileDialog *ui;
};

#endif // TRANSFERFILEDIALOG_H
