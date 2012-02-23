#ifndef CUSTOMPANEL_H
#define CUSTOMPANEL_H

// INCLUDES

#include <QFrame>
#include <QMenu>
#include <QAction>


// FORWARD DECLARATIONS

namespace Ui {
class CustomPanel;
}

class FileItemData;
class Settings;
class HeaderPopup;


// DECLARATIONS

//==============================================================================
//! @class CustomPanel Custom File Panel Class
//==============================================================================
class CustomPanel : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit CustomPanel(QWidget* aParent = NULL);

    //! @brief Set Panel Name
    //! @param aPanelName Panel Name
    void setPanelName(const QString& aPanelName);

    //! @brief Set Active State
    //! @param aActive Active State
    void setActive(const bool& aActive);

    //! @brief Get Active State
    //! @param none
    //! @return Active State
    bool isActive();

    //! @brief Set Show Hidden Files State
    //! @param aShowHidden Show Hidden Files State
    void setShowHiddenFiles(const bool& aShowHidden);

    //! @brief Get Show Hidden Files State
    //! @param none
    //! @return Show Hidden File State
    bool getShowHiddenFiles();

    //! @brief Get Selected Files Count
    //! @param none
    //! @return Selected Files Count
    int getSelectedCount();

    //! @brief Load Panel Settings
    //! @param none
    void loadSettings();

    //! @brief Save Panel Settings
    //! @param none
    void saveSettings();

    //! @brief Destructor
    //! @param none
    virtual ~CustomPanel();

protected:

    //! @brief Update Current Dir Lable Text
    //! @param aCurrDir Current Dir
    void updateCurrDirLabelText(const QString& aCurrDir);

    //! @brief Update Status Panel Text
    //! @param none
    void updateStatusPanelText();

    //! @brief Update Selection Panel Text
    //! @param none
    void updateSelectionPanelText();

protected slots:

    //! @brief File List Focus Changed
    //! @param aPanelName Panel Name
    //! @param aFocused Focused State
    void fileListFocusChanged(const QString& aPanelName, const bool& aFocused);

    //! @brief Header Activated Slot
    //! @param none
    void headerActivated();

    //! @brief Order Changed Slot
    //! @param aIndex Header Item Index
    //! @param aReversed Reverse Order
    void headerOrderChanged(const int& aIndex, const bool& aReversed);

    //! @brief Header Options Launch Slot
    //! @param aPos Mosue Position
    void headerOptionsLaunch(const QPoint& aPos);

    //! @brief Current Directory Changed Slot
    //! @param aCurrDir Current Directory
    void currentDirChanged(const QString& aCurrDir);

    //! @brief Current Directory Changed Slot
    //! @param none
    void currentDirReadFinished();

    //! @brief File List File Selected Slot
    //! @param aFileItemData File Item Data
    void fileListFileSelected(FileItemData* aFileItemData);

    //! @brief Up Dir Button Pressed Slot
    //! @param none
    void on_upDirButton_clicked();

    //! @brief Home Dir Button Pressed Slot
    //! @param none
    void on_homeButton_clicked();

    //! @brief Root Dir Button Pressed Slot
    //! @param none
    void on_rootButton_clicked();

    //! @brief Header Popup Close Button Clicked Slot
    //! @param none
    void headerPopupCloseButtonClicked();

protected: // From QFrame

    //! @brief Key Press Event
    //! @param aEvent Key Event
    virtual void keyPressEvent(QKeyEvent* aEvent);

    //! @brief Key Release Event
    //! @param aEvent Key Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

    //! @brief Resize Event
    //! @param aEvent Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);

protected: // Data
    friend class MainWindow;

    //! UI
    Ui::CustomPanel*    ui;
    //! Panel Name
    QString             panelName;
    //! Active
    bool                active;
    //! Settings
    Settings*           settings;
    //! Execue Bundles
    bool                execBundles;
    //! Show Hidden Files
    bool                showHidden;
    //! Header Popup
    HeaderPopup*        headerPopup;
};

#endif // CUSTOMPANEL_H
