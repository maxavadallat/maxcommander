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

    // Launch Dialog
    int launchDialog(const QString& aStartDir);

    // Get Link Name
    QString getLinkName();
    // Get Link Target
    QString getLinkTarget();

    // Destructor
    virtual ~CreateLinkDialog();

protected:

    // Exec Dialog
    virtual int exec();

protected slots:

    // On Browse Target Button Clicked Slot
    void on_browseTargetButton_clicked();

private:

    // UI
    Ui::CreateLinkDialog*   ui;
    // Start Dir
    QString                 startDir;
};

#endif // CREATELINKDIALOG_H


