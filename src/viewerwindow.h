#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

// INCLUDES

#include <QMainWindow>
#include <QHideEvent>
#include <QPlainTextEdit>


// FORWARD DECLARATIONS

namespace Ui {
class ViewerWindow;
}

class Settings;

// DECLARATIONS


//==============================================================================
//! @class CustomPlainTextEditor Application File Viewer Custom Editor Class
//==============================================================================
class CustomPlainTextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    CustomPlainTextEditor(QWidget* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~CustomPlainTextEditor();

signals:

    //! @brief Esc Key Pressed Signal
    //! @param none
    void escPressed();

protected: // From QPlainTextEdit

    //! @brief Key Pressed
    //! @param aEvent Key Event
    virtual void keyPressEvent(QKeyEvent* aEvent);

    //! @brief Key Released
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

protected: // Data

};









//==============================================================================
//! @class ViewerWindow Application File Viewer Window Class
//==============================================================================
class ViewerWindow : public QMainWindow
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit ViewerWindow(QWidget* aParent = NULL);

    //! @brief Load File
    //! @param aFilePath File Path
    //! @param aReadOnly Read Only Settings
    //! @return true If Successful
    bool loadFile(const QString& aFilePath, const bool& aReadOnly = true);

    //! @brief Close Viewer
    //! @param none
    void closeViewer();

    //! @brief Print File
    //! @param none
    void printFile();

    //! @brief Set Word Wrap
    //! @param aWordWrap Word Wrap
    void setWordWrap(const bool& aWordWrap);

    //! @brief Find Text
    //! @param aSearchTerm Search Term
    //! @param aCaseSensitive Case Sensitive
    //! @param aWholeWord Whole Word
    //! @param aDirection Search Direction
    //! @param aWrapAround Search Wrap Around
    void findText(const QString& aSearchTerm, const bool& aCaseSensitive = false, const bool& aWholeWord = false, const bool& aDirection = true, const bool& aWrapAround = true);

    //! @brief Find Next Matching Text
    //! @param none
    void findNext();

    //! @brief Set Read Only
    //! @param aReadOnly Read Only Settings
    void setReadOnly(const bool& aReadOnly);

    //! @brief Destructor
    //! @param none
    virtual ~ViewerWindow();

protected:

    //! @brief Load Settings
    //! @param none
    void loadSettings();

    //! @brief Save Settings
    //! @param none
    void saveSettings();

protected slots:

    //! @brief Action Open Triggered
    //! @param none
    void on_actionOpen_triggered();

    //! @brief Action Find Triggered
    //! @param none
    void on_actionFind_triggered();

    //! @brief Action Find Next Triggered
    //! @param none
    void on_actionFind_Next_triggered();

    //! @brief Action Print Triggered
    //! @param none
    void on_actionPrint_triggered();

    //! @brief Action Word Wrap Triggered
    //! @param none
    void on_actionWord_Wrap_triggered();

    //! @brief Action Font Triggered
    //! @param none
    void on_actionFont_triggered();

    //! @brief Action Close Triggered
    //! @param none
    void on_actionClose_triggered();

    //! @brief Editor Esc Key Pressed Slot
    //! @param none
    void editorEscPressed();

protected: // From QMainWindow

    //! @brief Hide Event
    //! @param aHideEvent Hide Event
    virtual void hideEvent(QHideEvent* aHideEvent);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

protected: // Data

    //! Ui
    Ui::ViewerWindow*   ui;
    //! Current File Name
    QString             fileName;
    //! Settings
    Settings*           settings;
    //! Font
    QFont               editorFont;
    //! Word Wrap
    bool                wordWrap;
    //! Search Term
    QString             searchTerm;
    //! Case Sensitive Word
    bool                caseSensitive;
    //! Whole Word Search
    bool                wholeWord;
    //! Search Direction
    bool                searchDirection;
    //! Wrap Around
    bool                searchWrapAround;


};

#endif // VIEWERWINDOW_H
