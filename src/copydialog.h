#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>

namespace Ui {
class CopyDialog;
}

class CopyDialog : public QDialog
{
    Q_OBJECT

public:

    explicit CopyDialog(QWidget* aParent = NULL);

    virtual ~CopyDialog();

protected:

    //! UI
    Ui::CopyDialog*     ui;
};

#endif // COPYDIALOG_H
