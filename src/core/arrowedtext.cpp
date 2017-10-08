#include "arrowedtext.h"
#include "cmath"
#include "global/global_definitions.h"

ArrowedText::ArrowedText(QObject *parent) :
    QObject(parent)
{
    mArrowedTextPrefs.mArrowEndingStyle = QCPLineEnding::esSpikeArrow;
}

QString ArrowedText::text() const
{
    return mText;
}

QPointF ArrowedText::textCenter() const
{
    return mTextCenter;
}

QPointF ArrowedText::arrowFinish() const
{
    return mArrowFinish;
}

ArrowedText::ArrowBasis ArrowedText::arrowBasisInRanges(double xRange, double yRange) const
{
    //TODO: algorithm doesn't seem to be correct, do it better
    if (xRange == 0. || yRange == 0)
        return ArrowBasis::Bottom;
    QPointF direction = mArrowFinish - mTextCenter;
    double angle_deg = std::atan2(direction.y()/yRange, direction.x()/xRange) * 180. / M_PI;
    if (angle_deg <= 45. && angle_deg > -45) {
        return ArrowBasis::Right;
    } else if (angle_deg <= -45. && angle_deg > -135) {
        return ArrowBasis::Bottom;
    } else if (angle_deg <= 135. &&  angle_deg > 45.){
        return ArrowBasis::Top;
    } else {
        return ArrowBasis::Left;
    }
}

QPen ArrowedText::pen() const
{
    return mArrowedTextPrefs.mPen;
}

QColor ArrowedText::backgroundColor() const
{
    return mArrowedTextPrefs.mBackgroundColor;
}

void ArrowedText::setText(const QString &text)
{
    if (mText != text) {
        mText = text;
        emit arrowedTextChanged();
    }
}

void ArrowedText::setTextCenter(const QPointF &textCenter)
{
    if (mTextCenter != textCenter) {
        mTextCenter = textCenter;
        emit arrowedTextChanged();
    }
}

void ArrowedText::setArrowFinish(const QPointF &arrowFinish)
{
    if (mArrowFinish != arrowFinish) {
        mArrowFinish = arrowFinish;
        emit arrowedTextChanged();
    }
}

void ArrowedText::setPen(const QPen &pen)
{
    if (mArrowedTextPrefs.mPen != pen) {
        mArrowedTextPrefs.mPen = pen;
        emit arrowedTextChanged();
    }
}

void ArrowedText::setBackgroundColor(QColor color)
{
    if (mArrowedTextPrefs.mBackgroundColor != color) {
        mArrowedTextPrefs.mBackgroundColor = color;
        emit arrowedTextChanged();
    }
}

void ArrowedText::setArrowStyle(QCPLineEnding::EndingStyle arrowStyle)
{
    if (mArrowedTextPrefs.mArrowEndingStyle != arrowStyle) {
        mArrowedTextPrefs.mArrowEndingStyle = arrowStyle;
        emit arrowedTextChanged();
    }
}

void ArrowedText::setArrowedTextPrefs(const style::ArrowedTextPref &prefs)
{
    if (mArrowedTextPrefs != prefs) {
        mArrowedTextPrefs = prefs;
        emit arrowedTextChanged();
    }
}


