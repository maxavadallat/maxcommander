#ifndef BUSYINDICATOR_H
#define BUSYINDICATOR_H

#include <QList>
#include <QTimerEvent>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QQuickPaintedItem>


//==============================================================================
// Busy Indicator Animation Piece Data Class
//==============================================================================
class BusyIndicatorAnimPieceData
{
public:

    // Constructor
    BusyIndicatorAnimPieceData();

    // Destructor
    virtual ~BusyIndicatorAnimPieceData();

protected:
    friend class BusyIndicator;

    // Start Angle
    int         startAngle;
    // Span Angle
    int         spanAngle;
    // Step
    int         step;
    // Counter
    int         counter;
    // Color
    QRgb        color;
    // Pen
    QPen        pen;
};


//==============================================================================
// Busy Indicator Class
//==============================================================================
class BusyIndicator : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning WRITE setRunning NOTIFY runningChanged)

public:
    // Constructor
    BusyIndicator(QQuickItem* aParent = NULL);

    // Get Running
    bool getRunning();
    // Set Running
    void setRunning(const bool& aRunning);

    // Destructor
    virtual ~BusyIndicator();

signals:

    // Running Changed Signals
    void runningChanged(const bool& aRunning);

public slots:

    // Start
    void start();
    // Stop
    void stop();

protected:

    // Init
    void init();
    // Clear
    void clear();

protected: // From QQuickPaintedItem

    // Paint
    virtual void paint(QPainter* aPainter);
    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

protected:

    // Animation Timer ID
    int                                 animTimerID;
    // Animation Pieces
    QList<BusyIndicatorAnimPieceData*>  animPieces;

};

#endif // BUSYINDICATOR_H
