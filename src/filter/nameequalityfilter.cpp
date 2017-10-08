#include "nameequalityfilter.h"
#include <algorithm>



bool NameEqualityFilter::isValid(std::shared_ptr<DataSample> dataSample)
{
//    auto parameterName = dataSample->getDataAttribute()->getName();
//    for (auto name : validationStringVector) {
//        if (parameterName == name)
//            return (filterLogic == Logic::PositiveLogic) ? true : false;
//    }
//    return (filterLogic == Logic::PositiveLogic) ? false : true;

    auto parameterName = dataSample->getDataAttribute()->getName();
    if (std::find(validationStringVector.begin(), validationStringVector.end(), parameterName)
            != validationStringVector.end()) {
        return (filterLogic == Logic::PositiveLogic) ? true : false;
    } else {
        return (filterLogic == Logic::PositiveLogic) ? false : true;
    }

}
