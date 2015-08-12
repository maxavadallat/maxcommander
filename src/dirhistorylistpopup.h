#ifndef DIRHISTORYLISTPOPUP_H
#define DIRHISTORYLISTPOPUP_H

#include <QWidget>
#include <QFocusEvent>

namespace Ui {
class DirHistoryListPopup;
}

class DirHistoryListModel;

//==============================================================================
// Dir History List Popup Class
//==============================================================================
class DirHistoryListPopup : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:

    // Constructor
    explicit DirHistoryListPopup(DirHistoryListModel* aModel, QWidget* aParent = NULL);

    // Launch Popup
    void launchPopup(const QPoint& aPos, const int& aWidth);
    // Hide Popup
    void hidePopup();

    // Get Current Index
    int getCurrentIndex();
    // Set Current Index
    void setCurrentIndex(const int& aCurrentIndex);

    // Destructor
    virtual ~DirHistoryListPopup();

signals:

    // Dir History Item Selected Signal
    void dirHistoryItemSelected(const QString& aDirPath);
    // Resize Needed Signal
    void resizeNeeded();

    void currentIndexChanged(const int& aCurrentIndex);

public slots:

    // Dir History List Item Selected
    void dirHistoryListItemSelected(const int& aIndex);

    // Dir History Clear Clicked
    void dirHistoryClearClicked();

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

private:

    // UI
    Ui::DirHistoryListPopup*    ui;

    // Dir History Model
    DirHistoryListModel*        historyModel;

    // Current Index
    int                         currentIndex;
};



#endif // DIRHISTORYLISTPOPUP_H
