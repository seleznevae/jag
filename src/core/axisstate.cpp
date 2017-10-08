#include "axisstate.h"

AxisState::AxisState(const AxisModel* axisModel)
{
//    auto xRange = axisModel->getXRange();
//    auto yRange = axisModel->getYRange();
    if (axisModel) {
        std::tie(mXRangeBegin , mXRangeEnd) = axisModel->getXRange();
        std::tie(mYRangeBegin , mYRangeEnd) = axisModel->getYRange();
        std::tie(mXScalingMode, mYScalingMode) = axisModel->scalingModes();
    }

}

std::ostream &operator<<(std::ostream &os, const AxisState &arg)
{
    os << "mXScalingMode = " << (arg.mXScalingMode == AxisModel::ScalingMode::Auto ? "Auto" : "Manual");
    os << " " << arg.mXRangeBegin << " " << arg.mXRangeEnd << "\n";
    os << "mYScalingMode = " << (arg.mYScalingMode == AxisModel::ScalingMode::Auto ? "Auto" : "Manual");
    os << " " << arg.mYRangeBegin << " " << arg.mYRangeEnd ;
    return os;
}

bool operator==(const AxisState &arg1, const AxisState &arg2)
{
    return   arg1.mXRangeBegin == arg2.mXRangeBegin &&
             arg1.mXRangeEnd == arg2.mXRangeEnd &&
             arg1.mYRangeBegin == arg2.mYRangeBegin &&
             arg1.mYRangeEnd     == arg2.mYRangeEnd &&
             arg1.mXScalingMode  == arg2.mXScalingMode &&
             arg1.mYScalingMode  == arg2.mYScalingMode ;
}

bool operator!=(const AxisState &arg1, const AxisState &arg2)
{
    return !(arg1 == arg2);
}


