#ifndef PROGRESSBARWIDGET_H
#define PROGRESSBARWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>


//==============================================================================
// Progress Bar Widget Class
//==============================================================================
class ProgressBarWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qint64 value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qint64 minimum READ getMinimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(qint64 maximum READ getMaximum WRITE setMaximum NOTIFY maximumChanged)

public:
    // Constructor
    explicit ProgressBarWidget(QWidget* aParent = NULL);

    // Get Value
    qint64 getValue();
    // Set Value
    void setValue(const qint64& aValue);

    // Get Minimum
    qint64 getMinimum();
    // Set Minimum
    void setMinimum(const qint64& aMinimum);

    // Get Maximum
    qint64 getMaximum();
    // Set Maximum
    void setMaximum(const qint64& aMaximum);

    // Destructor
    virtual ~ProgressBarWidget();

signals:
    // Value Changed Signal
    void valueChanged(const qint64& aValue);
    // Minimum Changed Signal
    void minimumChanged(const qint64& aMinimum);
    // Maximum Changed Signal
    void maximumChanged(const qint64& aMaximum);

protected:

    // Calculate Scale
    void calculateScale();
    // Update Scaled Values
    void updateScaledValues();

protected: // From QWidget

    // Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected:

    // Current Value
    qint64          value;
    // Minimum
    qint64          minimum;
    // Maximum
    qint64          maximum;

    // Scale
    int             scale;

    // Get Scaled Value
    int             sValue;
    // Get Scaled Minimum Value
    int             sMin;
    // Get Scaled MAximum Value
    int             sMax;

    // Progress Width
    int             progressWidth;

    // Progress Bar Color
    QColor          color;
};

#endif // PROGRESSBARWIDGET_H
