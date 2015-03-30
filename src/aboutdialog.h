#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

//==============================================================================
// About Dialog Class
//==============================================================================
class AboutDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit AboutDialog(QWidget* aParent = NULL);
    // Destructor
    virtual ~AboutDialog();

private:
    // UI
    Ui::AboutDialog*    ui;
};

#endif // ABOUTDIALOG_H
