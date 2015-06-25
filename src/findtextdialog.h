#ifndef FINDTEXTDIALOG_H
#define FINDTEXTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QKeyEvent>

namespace Ui {
class FindTextDialog;
}

class SettingsController;

//==============================================================================
// Find Text Dialog Class
//==============================================================================
class FindTextDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit FindTextDialog(QWidget* aParent = NULL);

    // Set Search Term
    void setSearchTerm(const QString& aSearchTerm);
    // Get Search Term
    QString getSearchTerm();

    // Get Case Sensitive Setting
    bool getCaseSensitiveSearch();
    // Get Whole Word Setting
    bool getWholeWordSearch();
    // Get Backward Search Setting
    bool getBackwardSearch();
    // Get Wrap Around Setting
    bool getWrapAroundWhenFinished();

    // Enable Find Next Button
    void enableFindNextButton(const bool& aEnable);

    // Destructor
    virtual ~FindTextDialog();

public: // From QDialog

    // Exec
    virtual int exec();

signals:

    // Find Text Signal
    void findText(const QString& aText);
    // Find Next Occurence Signal
    void findNext();
    // Escape Key Pressed Signal
    void escapePresed();

protected:

    // Init
    void init();

    // Load Settings
    void loadSettings();
    // Save Settings
    void saveSettings();

    // Retore UI
    void restoreUI();

    // Load Search Terms
    void loadSearchTerms();
    // Save Search Terms
    void saveSearchTerms();

    // Store Current Search Term
    void storeCurrentSearchTerm();

protected slots: // Button Box

    // Button Box Button Clicked Slot
    void buttonBoxButtonClicked(QAbstractButton* aButton);

protected slots: // Search Text Edit/Combo

    // Current Search Text Changed Slot
    void currentSearchTextChanged(const QString& aText);

private slots: // UI

    // On Case Sensitive Check Box Clicked Slot
    void on_caseSensitiveCheckBox_clicked();
    // On Whole Word Check Box Clicked Slot
    void on_wholeWordCheckBox_clicked();
    // On Backward Search Check Box Clicked Slot
    void on_backwardSearchCheckBox_clicked();
    // On Wrap Around Check Box Clicked Slot
    void on_wrapAroundCheckBox_clicked();

protected: // From QWidget

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

private:

    // UI
    Ui::FindTextDialog*     ui;

    // Settings Controller
    SettingsController*     settings;

    // Find Button
    QPushButton*            findButton;
    // Find Next Button
    QPushButton*            findNextButton;

    // Search Term
    QString                 searchTerm;

    // Case Sensitive Search
    bool                    caseSensitive;
    // Whole Word
    bool                    wholeWord;
    // Backward Search
    bool                    backward;
    // Wrap Around
    bool                    wrapAround;
};

#endif // FINDTEXTDIALOG_H
