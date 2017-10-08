#ifndef NULLFILTER_H
#define NULLFILTER_H

#include "filter.h"

/**
 * @brief NullFilter is a fake filter that declares all data to be valid
 *
 */
class NullFilter : public Filter
{
public:
    /**
     * @brief
     *
     */
    NullFilter();
    /**
     * @brief
     *
     * @param std::shared_ptr<DataSample>
     * @return bool
     */
    virtual bool isValid(std::shared_ptr<DataSample>) { return true; }
};

#endif // NULLFILTER_H
