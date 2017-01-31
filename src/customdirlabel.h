#ifndef CUSTOMDIRLABEL_H
#define CUSTOMDIRLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

//==============================================================================
// Custom Dir Label Class
//==============================================================================
class CustomDirLabel : public QLabel
{
    Q_OBJECT

public:
    // Constructor
    explicit CustomDirLabel(QWidget* aParent = NULL, Qt::WindowFlags aFlags = 0);

    // Destructor
    virtual ~CustomDirLabel();

signals:

    // Right Mouse Clicked
    void rightMouseClicked(const QPoint& aPos);
    // Mouse Double Clicked Signal
    void mouseDoubleClicked();

protected: // From QWidget

    // Mouse Press Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);
    // Mouse Double Clicked Event
    virtual void mouseDoubleClickEvent(QMouseEvent* aEvent);

protected: // Data

    // Muse Pressed
    bool        mousePressed;
};

#endif // CUSTOMDIRLABEL_H
