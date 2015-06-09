#ifndef TRANSFERFILEDIALOG_H
#define TRANSFERFILEDIALOG_H

#include <QDialog>

namespace Ui {
class TransferFileDialog;
}

class SettingsController;

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
    void setSourceFileText(const QString& aSourceFile, const bool& aReadOnly = false);
    // Get Target File Text
    QString getTargetFileText();
    // Set Target Fiel Text
    void setTargetFileText(const QString& aTargetFile, const bool& aReadOnly = false);
    // Get Copy Hidden Files
    bool getCopyHidden();
    // Set Copy Hidden Files
    void setCopyHidden(const bool& aCopyHidden);

    // Destructor
    virtual ~TransferFileDialog();

private slots:

    // On Copy Hidden Files Checkbox Clicked Slot
    void on_copyHiddenFilesCheckBox_clicked();

private:

    // UI
    Ui::TransferFileDialog* ui;

    // Settings
    SettingsController*     settings;
};

#endif // TRANSFERFILEDIALOG_H


