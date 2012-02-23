#ifndef SETTINGS_H
#define SETTINGS_H

// INCLUDES

#include <QSettings>
#include <QObject>
#include <QVariant>
#include <QStringList>


// DECLARATIONS

//==============================================================================
//! @class Settings Settings Class
//==============================================================================
class Settings : public QObject
{
    Q_OBJECT

public:

    //! @brief Static Constructor
    //! @param none
    //! @return New Settings Instance
    static Settings* getInstance();

    //! @brief Set Settings Group
    //! @param aGroup Settings Group Name
    void setGroup(const QString& aGroup);

    //! @brief Get Current Settings Group Name
    //! @param none
    //! @return Current Settings Group Name
    QString getGroup();

    //! @brief Set Settings Value
    //! @param aKey Key
    //! @param aValue Value
    void setValue(const QString& aKey, const QVariant& aValue);

    //! @brief Get Settings Value
    //! @param aKey Key
    //! @param aDefValue Default Value
    //! @return Settings Value
    QVariant getValue(const QString& aKey, const QVariant& aDefValue = QVariant());

    //! @brief Get All Keys In Current Group
    //! @param none
    //! @return All Keys In Current Group
    QStringList getKeys();

    //! @brief Load Defaults
    //! @param aGroup Settings Group Name
    void loadDefaults(const QString& aGroup = QString());

    //! @brief Export Settings
    //! @param aFilePath Exported Settings File Path
    void exportSettings(const QString& aFilePath);

    //! @brief Import Settings
    //! @param aFilePath Imported Settings File Path
    void importSettings(const QString& aFilePath);

    //! @brief Update Widget Style Sheet
    //! @param aWidget Widget
    //! @param aFgColorKey Foreground Color Key
    //! @param aBgColorKey Background Color Key
    //! @param aHidden Hidden
    void updateWidgetStyleSheet(QWidget* aWidget, const QString& aFgColorKey, const QString& aBgColorKey, const bool& aHidden);

    //! @brief Save
    //! @param none
    void save();

    //! @brief Release Current Instance
    //! @param none
    void release();

signals:

    //! @brief Settings Has Changed Slot
    //! @param aKey Key
    //! @param aValue Value
    void settingHasChanged(const QString& aKey, const QVariant& aValue);

protected:

    //! @brief Constructor
    //! @param aParent Parent
    explicit Settings(QObject* aParent = NULL);

    //! @brief Increase Reference Count
    //! @param none
    void ref();

    //! @brief Decrease Reference Count
    //! @param none
    void unref();

    //! @brief Load Defaults For General Settings
    //! @param none
    void loadDefaultsGeneral();

    //! @brief Load Defaults For Appearance Settings
    //! @param none
    void loadDefaultsAppearance();

    //! @brief Load Defaults For Tools Settings
    //! @param none
    void loadDefaultsTools();

    //! @brief Load Defaults For Panel1 Settings
    //! @param none
    void loadDefaultsPanel1();

    //! @brief Load Defaults For Panel2 Settings
    //! @param none
    void loadDefaultsPanel2();

    //! @brief Load Defaults For Search Settings
    //! @param none
    void loadDefaultsSearch();

    //! @brief Load Defaults For Shortcuts Settings
    //! @param none
    void loadDefaultsShortcuts();

    //! @brief Destructor
    //! @param none
    virtual ~Settings();

protected: // Data
    //! Reference Count
    int         refCount;
    //! Settings
    QSettings   settings;
};

#endif // SETTINGS_H
