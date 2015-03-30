#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

//==============================================================================
// Preferences Dialog Class
//==============================================================================
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit PreferencesDialog(QWidget* aParent = NULL);

    // Destructor
    virtual ~PreferencesDialog();

public slots:

    // Exec Dialog
    int execDialog();

signals:

    // Settings Has changed Slot
    void settingsHasChanged();

protected slots:

    // Init
    void init();

    // Load Settings
    void loadSettings();

    // Save Settings
    void saveSettings();

    // Apply Settings
    void applySettings();

    // Set Dirty
    void setDirty(const bool& aDirty);

private:
    // UI
    Ui::PreferencesDialog*  ui;
    // Dirty
    bool                    dirty;
};

#endif // PREFERENCESDIALOG_H
