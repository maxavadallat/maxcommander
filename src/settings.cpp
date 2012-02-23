
// INCLUDES

#include <QMutex>
#include <QFile>
#include <QDebug>
#include <QStyle>
#include <QWidget>

#include "constants.h"
#include "settings.h"


// CONSTANTS

//! Settings Singleton
static Settings* settingsSingleton  = NULL;

//! Locking Mutex
static QMutex mutex(QMutex::Recursive);


// IMPLEMENTATION

//==============================================================================
// Static Constructor
//==============================================================================
Settings* Settings::getInstance()
{
    // Check Settings Singleton
    if (!settingsSingleton) {
        qDebug() << "Settings::getInstance - creating settings";
        // Lock
        mutex.lock();
        // Create Settings Singleton
        settingsSingleton = new Settings();
        // Unlock
        mutex.unlock();
    } else {
        //qDebug() << "Settings::getInstance - new instance";

        // Inc Ref Count
        settingsSingleton->ref();
    }

    return settingsSingleton;
}

//==============================================================================
// Constructor
//==============================================================================
Settings::Settings(QObject* aParent)
    : QObject(aParent)
    , refCount(1)
    , settings(QSettings::IniFormat, QSettings::UserScope, DEFAULT_ORGANIZATION_NAME, DEFAULT_APPLICATION_NAME)
{
    qDebug() << "Creating Settings...done";
}

//==============================================================================
// Set Group
//==============================================================================
void Settings::setGroup(const QString& aGroup)
{
    // Lock
    mutex.lock();
    // Check Group
    if (!aGroup.isEmpty()) {
        //qDebug() << "Settings::setGroup - current group: " << settings.group();
        // Check Current Group
        if (settings.group() != aGroup || settings.group().isEmpty()) {
            qDebug() << "Settings::setGroup - endGroup: " << settings.group();
            // Close Previous Group
            settings.endGroup();

            qDebug() << "Settings::setGroup - aGroup: " << aGroup;
            // Set Group
            settings.beginGroup(aGroup);
        }
    }
    // Unlock
    mutex.unlock();
}

//==============================================================================
// Get Current Group
//==============================================================================
QString Settings::getGroup()
{
    return settings.group();
}

//==============================================================================
// Set Value
//==============================================================================
void Settings::setValue(const QString& aKey, const QVariant& aValue)
{
    // Lock
    mutex.lock();

    // Check Key Value
    if (!aKey.isEmpty()) {
        // Check Value
        if (!aValue.isNull()) {
            qDebug() << "Settings::setValue - aKey: " << aKey;
            // Set Value
            settings.setValue(aKey, aValue);
        } else {
            qDebug() << "Settings::setValue - remove aKey: " << aKey;
            // Delete Key
            settings.remove(aKey);
        }
        // Emit Setting Has changed Signal
        emit settingHasChanged(aKey, aValue);
    }

    // Unlock
    mutex.unlock();
}

//==============================================================================
// Get Value
//==============================================================================
QVariant Settings::getValue(const QString& aKey, const QVariant& aDefValue)
{
    // Check Key
    if (!aKey.isEmpty()) {
        return settings.value(aKey, aDefValue);
    } else {
        qDebug() << "Settings::getValue - EMPTY KEY!";
    }

    return QVariant();
}

//==============================================================================
// Get All Keys In Current Group
//==============================================================================
QStringList Settings::getKeys()
{
    return settings.allKeys();
}

//==============================================================================
// Load Defaults
//==============================================================================
void Settings::loadDefaults(const QString& aGroup)
{
    // Check Group Name
    if (aGroup.isEmpty()) {
        qDebug() << "Settings::loadDefaults - CLEAR ALL";
        // Clear
        settings.clear();
        // Load Defaults For General
        loadDefaultsGeneral();
        // Load Defaults For Appearance
        loadDefaultsAppearance();
        // Load Defaults For Tools
        loadDefaultsTools();
        // Load Defaults For Panel 1
        loadDefaultsPanel1();
        // Load Defaults For Panel 2
        loadDefaultsPanel2();
        // Load Defaults For Search
        loadDefaultsSearch();
        // Load Defaults For Shortcuts
        loadDefaultsShortcuts();
    } else {
        qDebug() << "Settings::loadDefaults - aGroup: " << aGroup;
        // End Group
        settings.endGroup();
        // Begin Group
        settings.beginGroup(aGroup);
        // Remove All Values From Current Group
        settings.remove("");
        // End Group
        settings.endGroup();
        // Check Group Name
        if (aGroup == QString(SETTINGS_GROUP_GENERAL)) {
            // Load Defaults For General
            loadDefaultsGeneral();
        } else if (aGroup == QString(SETTINGS_GROUP_APPEARANCE)) {
            // Load Defaults For Appearance
            loadDefaultsAppearance();
        } else if (aGroup == QString(SETTINGS_GROUP_TOOLS)) {
            // Load Defaults For Tools
            loadDefaultsTools();
        } else if (aGroup == QString(SETTINGS_GROUP_PANEL1)) {
            // Load Defaults For Panel 1
            loadDefaultsPanel1();
        } else if (aGroup == QString(SETTINGS_GROUP_PANEL2)) {
            // Load Defaults For Panel 2
            loadDefaultsPanel2();
        } else if (aGroup == QString(SETTINGS_GROUP_SEARCH)) {
            // Load Defaults For Search
            loadDefaultsSearch();
        } else if (aGroup == QString(SETTINGS_GROUP_SHORTCUTS)) {
            // Load Defaults For Shortcuts
            loadDefaultsShortcuts();
        } else {
            qDebug() << "Settings::loadDefaults - UNHANDLED GROUP";
        }
    }
}

//==============================================================================
// Load Defaults For General Settings
//==============================================================================
void Settings::loadDefaultsGeneral()
{
    // Begin Group - General
    settings.beginGroup(QString(SETTINGS_GROUP_GENERAL));

    // Set Default Values
    settings.setValue(QString(SETTINGS_KEY_SHOW_SPLASH), true);

    settings.setValue(QString(SETTINGS_KEY_ARCHIVES_AS_DIRS), true);

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Load Defaults For Appearance Settings
//==============================================================================
void Settings::loadDefaultsAppearance()
{
    // Begin Group - Appearance
    settings.beginGroup(QString(SETTINGS_GROUP_APPEARANCE));

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Load Defaults For Tools Settings
//==============================================================================
void Settings::loadDefaultsTools()
{
    // Begin Group - Tools
    settings.beginGroup(QString(SETTINGS_GROUP_TOOLS));

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Load Defaults For Panel1 Settings
//==============================================================================
void Settings::loadDefaultsPanel1()
{
    // Begin Group - Panel 1
    settings.beginGroup(QString(SETTINGS_GROUP_PANEL1));

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Load Defaults For Panel2 Settings
//==============================================================================
void Settings::loadDefaultsPanel2()
{
    // Begin Group - Panel 2
    settings.beginGroup(QString(SETTINGS_GROUP_PANEL2));

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Load Defaults For Search Settings
//==============================================================================
void Settings::loadDefaultsSearch()
{
    // Begin Group - Search
    settings.beginGroup(QString(SETTINGS_GROUP_SEARCH));

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Load Defaults For Shortcuts Settings
//==============================================================================
void Settings::loadDefaultsShortcuts()
{
    // Begin Group - Search
    settings.beginGroup(QString(SETTINGS_GROUP_SHORTCUTS));

    // ...

    // End Group
    settings.endGroup();
}

//==============================================================================
// Export Settings
//==============================================================================
void Settings::exportSettings(const QString& aFilePath)
{
    // Check File Path
    if (!aFilePath.isEmpty()) {
        qDebug() << "Settings::exportSettings - aFilePath: " << aFilePath;

        // ...

    } else {
        qDebug() << "Settings::exportSettings - aFilePath: INVALID FILE NAME!";
    }
}

//==============================================================================
// Import Settings
//==============================================================================
void Settings::importSettings(const QString& aFilePath)
{
    // Check File Path
    if (QFile::exists(aFilePath)) {
        qDebug() << "Settings::importSettings - aFilePath: " << aFilePath;

        // ...

    } else {
        qDebug() << "Settings::importSettings - aFilePath: " << aFilePath << " FILE DOESN'T EXIST!";
    }
}

//==============================================================================
// Update Widget Style Sheet
//==============================================================================
void Settings::updateWidgetStyleSheet(QWidget* aWidget, const QString& aFgColorKey, const QString& aBgColorKey, const bool& aHidden)
{
    // Check Widget
    if (aWidget) {
        // Set Group
        setGroup(QString(SETTINGS_GROUP_APPEARANCE));
        // Init Foreground Color
        QString fgColor;
        // Check If Hidden
        if (aHidden &&
            aFgColorKey != QString(SETTINGS_KEY_SELECTED_TEXT_COLOR) &&
            aFgColorKey != QString(SETTINGS_KEY_CURRENT_SELECTED_TEXT_COLOR)) {
            // set Foreground Color
            fgColor = QColor(getValue(aFgColorKey).toInt() + QColor(Qt::gray).rgb()).name();
            //qDebug() << "Settings::updateWidgetStyleSheet - HIDDEN fgColor: " << fgColor;
        } else {
            // Set Foreground Color
            fgColor = QColor(getValue(aFgColorKey).toInt()).name();
            //qDebug() << "Settings::updateWidgetStyleSheet - fgColor: " << fgColor;
        }

        // Get Background Color
        QString bgColor = QColor(getValue(aBgColorKey).toInt()).name();
        // Get Font Family
        QString fontFamily = getValue(SETTINGS_KEY_FONTFAMILY).toString();
        // Get Font Bold
        bool fontBold = getValue(SETTINGS_KEY_FONTBOLD).toBool();
        // Get Font Italic
        bool fontItalic = getValue(SETTINGS_KEY_FONTITALIC).toBool();
        // Get Font Size
        int fontSize = getValue(SETTINGS_KEY_FONTSIZE).toInt();

        // Set Widget Style Sheet
        aWidget->setStyleSheet(QString(STYLE_SHEET_ITEM_PREVIEW_TEMPLATE).arg(fgColor)
                                                                         .arg(bgColor)
                                                                         .arg(fontBold ? QString(STYLESHEET_VALUE_FONTBOLD) : QString(""))
                                                                         .arg(fontItalic ? QString(STYLESHEET_VALUE_FONTITALIC) : QString(""))
                                                                         .arg(fontSize)
                                                                         .arg(fontFamily));
    }
}

//==============================================================================
// Save
//==============================================================================
void Settings::save()
{
    // Lock
    mutex.lock();
    qDebug() << "Settings::save - fileName: " << settings.fileName();
    // Sync
    settings.sync();
    // Unlock
    mutex.unlock();
}

//==============================================================================
// Release Instance
//==============================================================================
void Settings::release()
{
    //qDebug() << "Settings::release";
    // Unref
    unref();
}

//==============================================================================
// Increase Reference Count
//==============================================================================
void Settings::ref()
{
    // Lock
    mutex.lock();
    // Increase Ref Count
    refCount++;
    // Unlock
    mutex.unlock();
}

//==============================================================================
// Decrease Reference Count
//==============================================================================
void Settings::unref()
{
    // Lock
    mutex.lock();
    // Decrease Ref Count
    refCount--;
    // Check Ref Count
    if (refCount <= 0) {
        // Save
        //save();
        // Delete Singleton
        delete settingsSingleton;
        // Reset Pointer
        settingsSingleton = NULL;
    }
    // Unlock
    mutex.unlock();
}

//==============================================================================
// Destructor
//==============================================================================
Settings::~Settings()
{
    qDebug() << "Deleting Settings...";

    // Save Settings
    save();

    qDebug() << "Deleting Settings...done";
}

