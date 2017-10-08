#ifndef DECORATORFILTER_H
#define DECORATORFILTER_H

#include "filter.h"
#include <vector>

/**
 * @brief class that can contain many other Filters. It declares datum valid if all
 * internal  filters declares it valid.
 *
 */
class DecoratorFilter : public Filter
{
public:
    /**
     * @brief
     *
     */
    DecoratorFilter();
    /**
     * @brief add filter
     *
     * @param std::shared_ptr<Filter>
     */
    void addFilter(std::shared_ptr<Filter>);
    /**
     * @brief declares datum valid if all
     * internal  filters declares it valid.
     *
     * @param std::shared_ptr<DataSample>
     * @return bool
     */
    virtual bool isValid(std::shared_ptr<DataSample>);
protected:
    std::vector< std::shared_ptr<Filter> > filterVector; /**< vector of internal filters.
 If all internal filters consider datum valid, then  DecoratorFilter considers it valid, too.*/
};

#endif // DECORATORFILTER_H
