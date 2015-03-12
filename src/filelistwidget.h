#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QQuickWidget>

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

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);

    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

private:
    // Focused
    bool        focused;
};

#endif // FILELISTWIDGET_H
