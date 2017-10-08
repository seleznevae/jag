#ifndef ABSTRACTFILTERCONTAINER_H
#define ABSTRACTFILTERCONTAINER_H

#include "dataconverter/dataconverter.h"

class AbstractFilterContainer
{
public:
    AbstractFilterContainer();
    virtual ~AbstractFilterContainer();


    virtual void addFilter(std::shared_ptr<DataConverter> newConverter) = 0;
    virtual QVector<std::shared_ptr<DataConverter>> getFilterVector() const = 0;
    virtual void removeFilter(int index) = 0;
    virtual void updateOutputData() = 0;
};

#endif // ABSTRACTFILTERCONTAINER_H
