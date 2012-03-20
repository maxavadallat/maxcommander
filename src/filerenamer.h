#ifndef FILERENAMER_H
#define FILERENAMER_H

// INCLUDES

#include <QFrame>
#include <QEventLoop>
#include <QLineEdit>
#include <QKeyEvent>


// FORWARD DECLARATIONS

namespace Ui {
class FileRenamer;
}

class FileListDelegate;


// DECLARATIONS

//==============================================================================
//! @class FileNameEditor Custom Line Editor Class
//==============================================================================
class FileNameEditor : public QLineEdit
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    FileNameEditor(QWidget* aParent = NULL);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

signals:

    //! @brief Editing Finished Signal
    //! @param aAccept Editor Entry Accepted
    void editFinished(const bool& aAccept);
};



//==============================================================================
//! @class FileRenamer Custom File List File Renamer Class
//==============================================================================
class FileRenamer : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aEventLoop Event Loop
    //! @param aParent Parent
    explicit FileRenamer(QEventLoop* aEventLoop, QWidget* aParent = NULL);

    //! @brief Set File Name
    //! @param aFile Name File Name
    void setFileName(const QString& aFileName);

    //! @brief Get File Name
    //! @param none
    //! @return File Name
    QString getFileName();

    //! @brief Show
    //! @param aDelegate File List Item Delegate
    void showRenamer(FileListDelegate* aDelegate);

    //! @brief Hide
    //! @param none
    void hideRenamer();

    //! @brief Get Accepted State
    //! @param none
    //! @return Accepted State
    bool getAccepted();

    //! @brief Destructor
    //! @param none
    virtual ~FileRenamer();

protected slots:

    //! @brief Editor Finished Slot
    //! @param aAccept Editor Entry Accepted
    void editorFinished(const bool& aAccept);

    //! @brief On Close Button Clicked Slot
    //! @param none
    void on_closeButton_clicked();

protected:
    //! UI
    Ui::FileRenamer*    ui;
    //! Event Loop
    QEventLoop*         eventLoop;
    //! Accepted State
    bool                mAccepted;
    //! Parent List Box
    QWidget*            renamerParent;
};

#endif // FILERENAMER_H
