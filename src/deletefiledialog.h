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
    // Exec
    virtual int exec();
    // Get Editor Text Changed
    bool getTextChanged();
    // Destructor
    virtual ~DeleteFileDialog();

protected slots:

    // File Name Editor Text Changed
    void fileNameEditorTextChanged(const QString& aNewText);

private:

    // UI
    Ui::DeleteFileDialog*   ui;

    // Editor Text Changed
    bool                    editorTextChanged;
};

#endif // DELETEFILEDIALOG_H


