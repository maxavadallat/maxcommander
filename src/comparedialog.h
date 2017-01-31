#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

#include <QDialog>
#include <QFileInfo>

namespace Ui {
class CompareDialog;
}

//==============================================================================
// Compare Dialog Class
//==============================================================================
class CompareDialog : public QDialog
{
    Q_OBJECT

public:
    //  Constructor
    explicit CompareDialog(QWidget* aParent = NULL);

    // Get Left File Exists
    bool leftFileExists();
    // Get Left File Name
    QString getLeftFile();
    // Set Left File Name
    void setLeftFile(const QString& aLeftFile);

    // Get Right File Exists
    bool rightFileExists();
    // Get Right File Name
    QString getRightFile();
    // Set Right File Name
    void setRightFile(const QString& aRightFile);

    // Destructor
    ~CompareDialog();

private slots:
    // On Left File Editor Text Changed Slot
    void on_leftFileEditor_textChanged(const QString &arg1);
    // On Right File Editor Text Changed Slot
    void on_rightFileEditor_textChanged(const QString &arg1);
    // Left File Browser Button Pressed Slot
    void on_leftBrowseButton_clicked();
    // Right File Browser Button Pressed Slot
    void on_rightBrowseButton_clicked();

private:
    // UI
    Ui::CompareDialog* ui;

    // Left File Info
    QFileInfo leftFileInfo;
    // Right File Info
    QFileInfo rightFileInfo;
};

#endif // COMPAREDIALOG_H
