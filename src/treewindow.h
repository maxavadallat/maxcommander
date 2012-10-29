#ifndef TREEWINDOW_H
#define TREEWINDOW_H

// INCLUDES

#include <QDialog>


// FORWARD DECLARATIONS

namespace Ui {
class TreeWindow;
}


// DECLARATIONS

//==============================================================================
//! @class TreeWindow Tree Window Class
//==============================================================================
class TreeWindow : public QDialog
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit TreeWindow(QWidget* aParent = NULL);

    //! @brief Set Tree Root
    //! @param aRoot Tree Root
    void setRoot(const QString& aRoot);

    //! @brief Destructor
    //! @param none
    virtual ~TreeWindow();

protected:

    //! UI
    Ui::TreeWindow*     ui;

    //! Root Path
    QString             root;
};

#endif // TREEWINDOW_H
