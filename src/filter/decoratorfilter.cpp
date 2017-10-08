#include "decoratorfilter.h"
#include "nullfilter.h"
#include <memory>

using std::make_shared;

DecoratorFilter::DecoratorFilter()
{
    filterVector.push_back(make_shared<NullFilter>());
}

void DecoratorFilter::addFilter(std::shared_ptr<Filter> filter)
{
    filterVector.push_back(filter);
}


bool DecoratorFilter::isValid(std::shared_ptr<DataSample> dataSample)
{
    for (const auto & filter : filterVector)
        if (filter->isValid(dataSample) == false)
            return false;
    return true;
}
