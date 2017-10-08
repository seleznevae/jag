#ifndef NAMEEQUALITYFILTER_H
#define NAMEEQUALITYFILTER_H

#include "parameternamefilter.h"

class NameEqualityFilter : public ParameterNameFilter
{
public:

    template <typename... Args>
    NameEqualityFilter(Args&&... args)
        :ParameterNameFilter(std::forward<Args>(args)...)
    {

    }

    virtual bool isValid(std::shared_ptr<DataSample>);
};

#endif // NAMEEQUALITYFILTER_H
