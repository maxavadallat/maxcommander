#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QFrame>
#include <QString>

namespace Ui {
class FilePanel;
}

class FileListModel;
class MainWindow;
class FileListImageProvider;

//==============================================================================
// File Panel Class
//==============================================================================
class FilePanel : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString panelName READ getPanelName)
    Q_PROPERTY(bool panelHasFocus READ getPanelFocus WRITE setPanelFocus NOTIFY panelFocusChanged)
    Q_PROPERTY(int visualItemsCount READ getvisualItemsCount WRITE setVisualItemsCount NOTIFY visualItemsCountChanged)

    Q_PROPERTY(bool showHidden READ getShowHidden WRITE setShowHidden NOTIFY showHiddenChanged)

    Q_PROPERTY(bool extVisible READ getExtVisible WRITE setExtVisible NOTIFY extVisibleChanged)
    Q_PROPERTY(bool typeVisible READ getTypeVisible WRITE setTypeVisible NOTIFY typeVisibleChanged)
    Q_PROPERTY(bool sizeVisible READ getSizeVisible WRITE setSizeVisible NOTIFY sizeVisibleChanged)
    Q_PROPERTY(bool dateVisible READ getDateVisible WRITE setDateVisible NOTIFY dateVisibleChanged)
    Q_PROPERTY(bool ownerVisible READ getOwnerVisible WRITE setOwnerVisible NOTIFY ownerVisibleChanged)
    Q_PROPERTY(bool permsVisible READ getPermsVisible WRITE setPermsVisible NOTIFY permsVisibleChanged)
    Q_PROPERTY(bool attrsVisible READ getAttrsVisible WRITE setAttrsVisible NOTIFY attrsVisibleChanged)

public:
    // Constructor
    explicit FilePanel(QWidget* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();
    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);

    // Get Panel Name
    QString getPanelName();
    // Set Panel Name
    void setPanelName(const QString& aPanelName);

    // Get Panel Focus
    bool getPanelFocus();
    // Get Current Index
    int getCurrentIndex();
    // Get Visual Items Count
    int getvisualItemsCount();

    // Get Show Hidden
    bool getShowHidden();
    // Set Show Hidden
    void setShowHidden(const bool& aHidden);

    // Get Extension Visible
    bool getExtVisible();
    // Set Extension Visible
    void setExtVisible(const bool& aExtVisible);

    // Get Type Visible
    bool getTypeVisible();
    // Set Type Visible
    void setTypeVisible(const bool& aTypeVisible);

    // Get Size Visible
    bool getSizeVisible();
    // Set Size Visible
    void setSizeVisible(const bool& aSizeVisible);

    // Get Date Visible
    bool getDateVisible();
    // Set Date Visible
    void setDateVisible(const bool& aDateVisible);

    // Get Owner Visible
    bool getOwnerVisible();
    // Set Owner Visible
    void setOwnerVisible(const bool& aOwnerVisible);

    // Get Permissions Visible
    bool getPermsVisible();
    // Set Permissions Visible
    void setPermsVisible(const bool& aPermsVisible);

    // Get Attributes Visible
    bool getAttrsVisible();
    // Set Attributes Visible
    void setAttrsVisible(const bool& aAttrsVisible);

    // Destructor
    virtual ~FilePanel();

public slots:

    // Go To Home Directory
    void gotoHome();
    // Go To Root
    void gotoRoot();
    // Go To Drive
    void gotoDrive(const int& aDriveIndex);
    // Go Up To Parent Dir
    void goUp();
    // Go To Next Item Of The List
    void goNext();
    // Go To Prev Item Of The List
    void goPrev();
    // Go To Page Up Of The List
    void pageUp();
    // Go To Page Down Of The List
    void pageDown();
    // Go To Beginning Of The List
    void goFirst();
    // Go To End Of The List
    void goLast();

    // Handle Item Select
    void handleItemSelect();
    // Rename
    void renameCurrent();

    // Set Panel Focus
    void setPanelFocus(const bool& aFocus);
    // Set Current Index
    void setCurrentIndex(const int& aCurrentIndex);
    // Set Visual Items Count
    void setVisualItemsCount(const int& aVisualCount);

    // Get Supported Image Formats
    QStringList getSupportedImageFormats();

    // Reload
    void reload(const int& aIndex);

signals:

    // Focused Panel Changed Signal
    void focusedPanelChanged(FilePanel* aFocusedPanel);

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);
    // Panel Focus Changed Signal
    void panelFocusChanged(const bool& aPanelFocus);

    // Modifier Keys Pressed Signal
    void modifierKeysChanged(const int& aModifierKeys);

    // Exit Key Released Signal
    void exitKeyReleased();

    // Current Index Changed Signal
    void currentIndexChanged(const int& aIndex);
    // Visual Items Count Changed
    void visualItemsCountChanged(const int& aVisualCount);

    // Show Hidden Changed Signal
    void showHiddenChanged(const bool& aHidden);

    // Extension Visible Changed Signal
    void extVisibleChanged(const bool& aExtVisible);
    // Type Visible Changed Signal
    void typeVisibleChanged(const bool& aTypeVisible);
    // Size Visible Changed Signal
    void sizeVisibleChanged(const bool& aSizeVisible);
    // Date Visible Changed Signal
    void dateVisibleChanged(const bool& aDateVisible);
    // Owner Visible Changed Signal
    void ownerVisibleChanged(const bool& aOwnerVisible);
    // Perms Visible Changed Signal
    void permsVisibleChanged(const bool& aPermsVisible);
    // Attributes Visible Changed Signal
    void attrsVisibleChanged(const bool& aAttrsVisible);

protected slots:

    // Init
    void init();
    // Clear
    void clear();
    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();
    // Update Available Space Label
    void updateAvailableSpaceLabel();
    // Update Supported Image Formates
    void updateSupportedImageFormats();

protected slots: // From File Model

    // File Model Fetch Ready
    void fileModelDirFetchFinished();

private slots:

    // Home Button Clicked Slot
    void on_homeButton_clicked();
    // Root Button Clicked Slot
    void on_rootButton_clicked();

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);


private:
    friend class MainWindow;

    // UI
    Ui::FilePanel*          ui;

    // Current Dir
    QString                 currentDir;

    // Panel Name
    QString                 panelName;

    // Panel Focus
    bool                    panelHasFocus;

    // File List Model
    FileListModel*          fileListModel;

    // File List image Provider
    FileListImageProvider*  fileListImageProvider;

    // Modifier Keys
    int                     modifierKeys;

    // Current Index
    int                     currentIndex;

    // Visual Items Count
    int                     visualItemsCount;

    // Last Directory Name To Jump After CD UP
    QString                 lastDirName;
    // Last Index Before Reload
    int                     lastIndex;

    // Supported Image Formats
    QStringList             supportedImageFormats;

    // Show Hidden Files
    bool                    showHidden;

    // Extensions Visible
    bool                    extVisible;
    // Type Visible
    bool                    typeVisible;
    // Size Visible
    bool                    sizeVisible;
    // Date Visible
    bool                    dateVisible;
    // Owner Visible
    bool                    ownerVisible;
    // Permissions Visible
    bool                    permsVisible;
    // Attributes Visible
    bool                    attrsVisible;
};

#endif // FILEPANEL_H
