#ifndef SELECTFILESDIALOG_H
#define SELECTFILESDIALOG_H

#include <QDialog>
#include <QHideEvent>

namespace Ui {
class SelectFilesDialog;
}

enum FileSelectionModeType
{
    EFSMSelect      = 0,
    EFSMDeselect
};

//==============================================================================
// Select File Dialog Class
//==============================================================================
class SelectFilesDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit SelectFilesDialog(QWidget* aParent = NULL);

    // Get Selection Pattern
    QString getPattern();

    // Launch Dialog
    int launchDialog(const FileSelectionModeType& aMode = EFSMSelect);

    // Destructor
    virtual ~SelectFilesDialog();

protected slots:

    // Init
    void init();

    // Restore UI
    void restoreUI();

    // Load File Select Patterns
    void loadFileSelectPatterns();
    // Save File Select Patterns
    void saveFileSelectPatterns();

protected: // From QDialog

    // Exec Dialog
    virtual int exec();

protected: // From QDialog

    // Hide Event
    virtual void hideEvent(QHideEvent* aEvent);

private:

    // UI
    Ui::SelectFilesDialog*  ui;

    // Selection Pattern
    QString                 pattern;

    // Selection Mode
    FileSelectionModeType   mode;
};

#endif // SELECTFILESDIALOG_H
