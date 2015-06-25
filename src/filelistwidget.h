#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QQuickWidget>
#include <QTimerEvent>

//==============================================================================
// File List Widget
//==============================================================================
class FileListWidget : public QQuickWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit FileListWidget(QWidget* aParent = NULL);

    // Destructor
    virtual ~FileListWidget();

signals:

    // Focus Changed Signal
    void focusChanged(const bool& aFocus);

    // Drag Started Signal
    void dragStarted(const int& aPosX, const int aPosY);

    // Drag Dropped
    void dragDropped();

protected:

    // Start Drag Timer
    void startDragTimer();
    // Stop Drag Timer
    void stopDragTimer();

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Mouse Press Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Move Event
    virtual void mouseMoveEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

private:
    // Focused
    bool        focused;

    // Drag & Drop Timer ID
    int         dragTimerID;

    // Drag On
    bool        dragOn;

    // Inside Press
    bool        insidePress;

    // Drag Pos X
    int         dragPosX;
    // Drag Pos Y
    int         dragPosY;
};

#endif // FILELISTWIDGET_H

