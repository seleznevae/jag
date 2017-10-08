#include "parameternamefilter.h"

void ParameterNameFilter::addValidationString(std::string newValidationString)
{
    validationStringVector.push_back(newValidationString);
}

void ParameterNameFilter::setFilterLogic(ParameterNameFilter::Logic newLogic)
{
    filterLogic = newLogic;
}
