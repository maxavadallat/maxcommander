
#include <QColor>
#include <QDateTime>
#include <QDebug>

#include "busyindicator.h"
#include "constants.h"



//==============================================================================
// Constructor
//==============================================================================
BusyIndicatorAnimPieceData::BusyIndicatorAnimPieceData()
{
    // Auto Generate Values
    startAngle  = (qrand() % 360);
    spanAngle   = (qrand() % 120) + 80;
    step        = (qrand() % 10) + 5;
    counter     = startAngle;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(qrand() % 3 + 1);
    pen.setColor(QColor::fromRgb(0, 0, (qrand() % 128) + 128, 128));

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
BusyIndicatorAnimPieceData::~BusyIndicatorAnimPieceData()
{
}










//==============================================================================
// Constructor
//==============================================================================
BusyIndicator::BusyIndicator(QQuickItem* aParent)
    : QQuickPaintedItem(aParent)
    , animTimerID(-1)
{
    //qDebug() << "BusyIndicator::BusyIndicator";
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void BusyIndicator::init()
{
    //qDebug() << "BusyIndicator::init";

    // Init Random Generator
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // Set Smooth
    setSmooth(true);
    // Set Antialiasing
    setAntialiasing(true);
    // Set Mipmap
    setMipmap(true);

    // Init Direction
    bool ccw = (qrand() % 2) == 1;

    // Generate Pieces
    for (int i=0; i<DEFAULT_ANIM_PIECES_MAX; ++i) {
        // Create New Anim Piece Data
        BusyIndicatorAnimPieceData* newPiece = new BusyIndicatorAnimPieceData();
        // Adjust Direction
        newPiece->step = ccw ? newPiece->step : -newPiece->step;
        // Append To Anim Pieces
        animPieces << newPiece;
        // Change Direction
        ccw = !ccw;
    }
}

//==============================================================================
// Get Running
//==============================================================================
bool BusyIndicator::getRunning()
{
    return animTimerID != -1;
}

//==============================================================================
// Set Running
//==============================================================================
void BusyIndicator::setRunning(const bool& aRunning)
{
    // Check Running
    if (aRunning) {
        // Start
        start();
    } else {
        // Stop
        stop();
    }
}

//==============================================================================
// Start
//==============================================================================
void BusyIndicator::start()
{
    // Check Timer ID
    if (animTimerID == -1) {
        //qDebug() << "BusyIndicator::start";
        // Start Timer ~30 FPS
        animTimerID = startTimer(DEFAULT_ONE_SEC / DEFAULT_ANIM_FRAME_RATE, Qt::PreciseTimer);
        // Emit Running Changed Signal
        emit runningChanged(animTimerID != -1);
    }
}

//==============================================================================
// Stop
//==============================================================================
void BusyIndicator::stop()
{
    // Check Timer ID
    if (animTimerID != -1) {
        //qDebug() << "BusyIndicator::stop";
        // Kill Timer
        killTimer(animTimerID);
        // Reset Anim Timer ID
        animTimerID = -1;
        // Emit Running Changed Signal
        emit runningChanged(animTimerID != -1);
    }
}

//==============================================================================
// Paint
//==============================================================================
void BusyIndicator::paint(QPainter* aPainter)
{
    // Check Painter
    if (aPainter) {
        // Get Pieces Count
        int apCount = animPieces.count();
        // Go Thru Animation Pieces
        for (int i=0; i<apCount; ++i) {
            // Calc Rect Step
            int rectStep = i * 4 + 3;
            // Get Piece
            BusyIndicatorAnimPieceData* piece = animPieces[i] ;
            // Check Piece
            if (piece) {
                // Set Pen
                aPainter->setPen(piece->pen);
                // Draw Arc Piece
                aPainter->drawArc(boundingRect().adjusted(rectStep, rectStep, -rectStep, -rectStep), piece->counter << 4, piece->spanAngle << 4);
            }
        }
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void BusyIndicator::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Timer ID
        if (aEvent->timerId() == animTimerID) {
            //qDebug() << "BusyIndicator::timerEvent";

            // Get Pieces Count
            int apCount = animPieces.count();
            // Go Thru Animation Pieces
            for (int i=0; i<apCount; ++i) {
                // Get Piece
                BusyIndicatorAnimPieceData* piece = animPieces[i] ;
                // Check Piece
                if (piece) {
                    // Adjust Counter
                    piece->counter = (piece->counter + piece->step) % 360;
                }
            }

            // Update
            update();
        }
    }
}

//==============================================================================
// Clear
//==============================================================================
void BusyIndicator::clear()
{
    //qDebug() << "BusyIndicator::clear";

    // Go Thru List
    while (animPieces.count() > 0) {
        // Take Last
        delete animPieces.takeLast();
    }
}

//==============================================================================
// Destructor
//==============================================================================
BusyIndicator::~BusyIndicator()
{
    // Stop
    stop();
    // Clear
    clear();

    //qDebug() << "BusyIndicator::~BusyIndicator";
}

