#include <QSettings>
#include <QDebug>

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

//==============================================================================
// Constructor
//==============================================================================
PreferencesDialog::PreferencesDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PreferencesDialog)
    , dirty(false)
{
    qDebug() << "PreferencesDialog::PreferencesDialog";

    // Setup UI
    ui->setupUi(this);
    // Init
    init();
    // Load Settings
    loadSettings();
}

//==============================================================================
// Init
//==============================================================================
void PreferencesDialog::init()
{
    qDebug() << "PreferencesDialog::init";

}

//==============================================================================
// Load Settings
//==============================================================================
void PreferencesDialog::loadSettings()
{
    qDebug() << "PreferencesDialog::loadSettings";

    // Init Settings
    QSettings settings;

    // ...

}

//==============================================================================
// Save Settings
//==============================================================================
void PreferencesDialog::saveSettings()
{
    qDebug() << "PreferencesDialog::saveSettings";

    // Init Settings
    QSettings settings;

    // ...
}

//==============================================================================
// Apply Settings
//==============================================================================
void PreferencesDialog::applySettings()
{
    // Check Dirty
    if (dirty) {
        qDebug() << "PreferencesDialog::applySettings";
        // Set Dirty
        setDirty(false);
        // Save settings
        saveSettings();
        // Emit Settings Changed Signal
        emit settingsHasChanged();
    }
}

//==============================================================================
// Set Dirty
//==============================================================================
void PreferencesDialog::setDirty(const bool& aDirty)
{
    // Check Dirty
    if (dirty != aDirty) {
        // Set Dirty
        dirty = false;

        // Check Dirty
        if (dirty) {
            // Enable Apply Button

        } else {
            // Disable Apply Button

        }
    }
}

//==============================================================================
// Exec Dialog
//==============================================================================
int PreferencesDialog::execDialog()
{
    // Load Settings
    loadSettings();

    // Reset Dirty
    setDirty(false);

    // ...

    return exec();
}

//==============================================================================
// Destructor
//==============================================================================
PreferencesDialog::~PreferencesDialog()
{
    // ...

    delete ui;

    qDebug() << "PreferencesDialog::~PreferencesDialog";
}
