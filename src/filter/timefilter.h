#ifndef TIMEFILTER_H
#define TIMEFILTER_H


#include <float.h>
#include "filter.h"

/**
 * @brief TimeFilter filters data by time.
 *
 * TimeFilter considers datum valid only if datum sample has time between lowMargin and highMargin.
 *
 */
class TimeFilter: public Filter
{
public:
    /**
     * @brief
     *
     * @param lowSaturation
     * @param highSaturation
     */
    TimeFilter(double lowSaturation = -DBL_MAX, double highSaturation = DBL_MAX);
    /**
     * @brief
     *
     * @param std::shared_ptr<DataSample>
     * @return bool
     */
    virtual bool isValid(std::shared_ptr<DataSample>);
private:
    double lowMargin; /**< datum sample with time lower than lowMargin are considered as invalid*/
    double highMargin; /**< datum sample with time higher than highMargin are considered as invalid*/
};

#endif // TIMEFILTER_H
