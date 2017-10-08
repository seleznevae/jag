#ifndef AXISSTATE_H
#define AXISSTATE_H

#include "axismodel.h"
#include <iostream>

/*!
 \brief AxisState is  a class that is used for storing axis states(axis range, axis mode)

AxisState is basically used for history implementation for axis states, so that user can return to
previous axis view scope and then go back or forward in the history.

*/

class AxisState
{
public:
    AxisState(const AxisModel* = NULL);

    AxisModel::ScalingMode xScalingMode() const {return mXScalingMode;}
    AxisModel::ScalingMode yScalingMode() const {return mYScalingMode;}
    double beginXRange() const { return mXRangeBegin; }
    double endXRange() const { return mXRangeEnd; }
    double beginYRange() const { return mYRangeBegin; }
    double endYRange() const { return mYRangeEnd; }

    friend bool operator==(const AxisState& arg1, const AxisState& arg2);
    friend bool operator!=(const AxisState& arg1, const AxisState& arg2);
    friend std::ostream& operator<<(std::ostream& os, const AxisState& arg);

private:

    double mXRangeBegin;
    double mXRangeEnd;
    double mYRangeBegin;
    double mYRangeEnd;
    AxisModel::ScalingMode mXScalingMode;
    AxisModel::ScalingMode mYScalingMode;

};

#endif // AXISSTATE_H
