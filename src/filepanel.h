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

signals:

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

protected slots:

    // Clear
    void clear();
    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();
    // Update Available Space Label
    void updateAvailableSpaceLabel();

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
};

#endif // FILEPANEL_H
