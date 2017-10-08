#include "graphstyle.h"
#include "global/global_definitions.h"
#include <QDebug>


GraphStyle::GraphStyle()
    :mLineInterpolation(GraphModel::LineStyle::lsLine), mIsSpecified(false), mIsColorFixed(false)
{

}

void GraphStyle::setPen(QPen newPen)
{
    mPen = newPen;
    mIsSpecified = true;
}

void GraphStyle::setScatterStyle(QCPScatterStyle newStyle)
{
    mScatterStyle = newStyle;
    mIsSpecified = true;
}

void GraphStyle::setLineInterpolation(GraphModel::LineStyle interpolation)
{
    mLineInterpolation = interpolation;
    mIsSpecified = true;
}

void GraphStyle::setSpecifiedProperty(bool specified)
{
    mIsSpecified = specified;
}

void GraphStyle::setColorFixed(bool fixed)
{
    mIsColorFixed = fixed;
    mIsSpecified = true;
}

void GraphStyle::setColor(QColor color)
{
    mPen.setColor(color);
    setColorFixed(true);
}
