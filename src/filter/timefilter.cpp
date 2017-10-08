#include "timefilter.h"




TimeFilter::TimeFilter(double lowSaturation, double highSaturation)
    :lowMargin(lowSaturation), highMargin(highSaturation)
{

}

bool TimeFilter::isValid(std::shared_ptr<DataSample> dataSample)
{
    if (dataSample->getTime() > highMargin
            || dataSample->getTime() < lowMargin)
        return false;  //invalid dataSample
    else
        return true;  //valid dataSample

}
