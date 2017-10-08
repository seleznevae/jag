#ifndef FILTER_h
#define FILTER_h

#include <memory>
#include "../data/data_sample.h"
#include "../data/data_sample.h"

/**
 * @brief Filter - abstract base class for all filters
 *
 *  Filter object recieve data samples from Director and
 * determine if they are valid for following treatment ant passing
 * to Builder for writing.
 */
class Filter {

public:
    /**
     * @brief Determines if data sample is valid for following treatment ant passing
     * to Builder for writing.
     *
     * @param std::shared_ptr<DataSample>
     * @return bool
     */
    virtual bool isValid(std::shared_ptr<DataSample>) = 0;
};


#endif //FILTER_h
