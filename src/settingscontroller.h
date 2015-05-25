#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QVariant>

//==============================================================================
// Settings Settings Controller
//==============================================================================
class SettingsController : public QObject
{
    Q_OBJECT

public:

    // Get Instance - Static Constructor
    static SettingsController* getInstance();

    // Release
    void release();

    // Get Value
    QVariant value(const QString& aKey);

    // Set Value
    void setValue(const QString& aKey, const QVariant& aValue);

    // Restore Defaults
    void restoreDefaults();

signals:

protected:

    // Constructor
    explicit SettingsController(QObject* aParent = NULL);

    // Destructor
    virtual ~SettingsController();

protected:

    // Int Ref Counter
    int                 refCount;

    // Settings
    QSettings           settings;

};

#endif // SETTINGSCONTROLLER_H
