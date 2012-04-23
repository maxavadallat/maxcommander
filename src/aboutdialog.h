#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// INCLUDES

#include <QDialog>
#include <QPaintEvent>
#include <QColor>


// FORWARD DECLARATIONS

namespace Ui {
class AboutDialog;
}


// DECLARATIONS

//==============================================================================
//! @class AboutDialog About Dialog Class
//==============================================================================
class AboutDialog : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit AboutDialog(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~AboutDialog();

protected: // From QDialog

    //! @brief Paint Eventr
    //! @param aEvent Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected:

    //! UI
    Ui::AboutDialog*    ui;
    //! Background Color
    QColor              bgColor;
};

#endif // ABOUTDIALOG_H
