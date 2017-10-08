#include "timeevent.h"
#include <cmath>

TimeEvent::TimeEvent(QObject *parent)
    :QObject(parent), mEventTime(NAN),
     mBrushBasis(BrushBasis::Right), mLabelDeviation(0),  mBrushDuration(std::numeric_limits<double>::max())
{
    mBrush.setStyle(Qt::NoBrush);
}

TimeEvent::TimeEvent(const TimeEvent &timeEvent):
    QObject(timeEvent.parent()), mName(timeEvent.mName),
    mDescription(timeEvent.mDescription), mEventTime(timeEvent.mEventTime), mBrush(timeEvent.mBrush), mBrushBasis(BrushBasis::Right),
    mLabelDeviation(timeEvent.mLabelDeviation),  mBrushDuration(timeEvent.mBrushDuration)
{

}

void TimeEvent::setBrushBasis(TimeEvent::BrushBasis basis)
{
    if (mBrushBasis != basis) {
        mBrushBasis = basis;
        emit timeEventChanged();
    }
}

void TimeEvent::setBrush(const QBrush &newBrush)
{
    if (mBrush != newBrush) {
        mBrush = newBrush;
        emit timeEventChanged();
    }

}

void TimeEvent::setBrushColor(QColor newColor)
{
    if (mBrush.color() != newColor) {
        mBrush.setColor(newColor);
        emit timeEventChanged();
    }

}

void TimeEvent::setName(const QString &newName)
{
    if (mName != newName) {
        mName = newName;
        emit timeEventChanged();
    }
}

void TimeEvent::setEventTime(double newEventTime)
{
    if (mEventTime != newEventTime) {
        mEventTime = newEventTime;
        emit timeEventChanged();
    }
}

void TimeEvent::setDescription(const QString &newDecription)
{
    if (mDescription != newDecription) {
        mDescription = newDecription;
        //no use at he moment in signal
//        emit timeEventChanged();
    }
}

void TimeEvent::setLabelDeviation(int deviation)
{
    if (mLabelDeviation != deviation) {
        mLabelDeviation = deviation;
        emit timeEventChanged();
    }
}

void TimeEvent::setBrushDuration(double duration)
{
    duration = std::max(duration, 0.);
    if (mBrushDuration != duration) {
        mBrushDuration = duration;
        emit timeEventChanged();
    }
}

void TimeEvent::setPen(const QPen &new_pen)
{
    if (mPen != new_pen) {
        mPen = new_pen;
        emit timeEventChanged();
    }
}
