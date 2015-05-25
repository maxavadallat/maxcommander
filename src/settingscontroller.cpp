#include <QDebug>

#include "settingscontroller.h"
#include "constants.h"
#include "defaultsettings.h"


// Settings Singleton
static SettingsController* settingsSingleton = NULL;


//==============================================================================
// Get Instance - Static Constructor
//==============================================================================
SettingsController* SettingsController::getInstance()
{
    // Check Settings  Singleton
    if (!settingsSingleton) {
        // Create Singleton
        settingsSingleton = new SettingsController();
    } else {
        // Inc Ref Count
        settingsSingleton->refCount++;
    }

    return settingsSingleton;
}

//==============================================================================
// Constructor
//==============================================================================
SettingsController::SettingsController(QObject* aParent)
    : QObject(aParent)
    , refCount(1)
{
    // ...
}

//==============================================================================
// Release
//==============================================================================
void SettingsController::release()
{
    // Dec Ref Count
    refCount--;

    // Check Ref Count
    if (refCount <= 0 && settingsSingleton) {
        // Delete Settings Singleton
        delete settingsSingleton;
        settingsSingleton = NULL;
    }
}

//==============================================================================
// Get Value
//==============================================================================
QVariant SettingsController::value(const QString& aKey)
{
    return settings.value(aKey);
}

//==============================================================================
// Set Value
//==============================================================================
void SettingsController::setValue(const QString& aKey, const QVariant& aValue)
{
    settings.setValue(aKey, aValue);
}

//==============================================================================
// Restore Defaults
//==============================================================================
void SettingsController::restoreDefaults()
{
    // ...
}

//==============================================================================
// Destructor
//==============================================================================
SettingsController::~SettingsController()
{
    // Sync
    settings.sync();
}

