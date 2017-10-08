#include "nameregexfilter.h"
#include <data/data_sample.h>
#include <iostream>

void NameRegExFilter::addValidationString(std::string newValidationString)
{
    ParameterNameFilter::addValidationString(newValidationString);
    regExpList << QRegExp(newValidationString.c_str());
}

bool NameRegExFilter::isValid(std::shared_ptr<DataSample> dataSample)
{
    // checking if identifiactor of datum is already in the set ov valid IDs
    if (validIdsSet.find(dataSample->getDataAttribute()->getId()) != validIdsSet.end()) {
        return (filterLogic == Logic::PositiveLogic) ? true : false;
    }


    auto parameterName = dataSample->getDataAttribute()->getName();
    QString qPrameterName(parameterName.c_str());
    for (auto i = regExpList.begin(); i != regExpList.end(); i++) {
        if (i->indexIn ( qPrameterName ) != -1) {
            validIdsSet.insert(dataSample->getDataAttribute()->getId());
            return (filterLogic == Logic::PositiveLogic) ? true : false;
        }
    }

    return (filterLogic == Logic::PositiveLogic) ? false : true;
}
