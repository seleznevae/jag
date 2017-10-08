#ifndef NAMEREGEXFILTER_H
#define NAMEREGEXFILTER_H

#include "parameternamefilter.h"
#include <QRegExp>
#include <QCoreApplication>
#include <set>

/*!
 \brief class for filtering data by name accordint to particular regular expression


*/
class NameRegExFilter : public ParameterNameFilter
{
public:

    template <typename... Args>
    /*!
     \brief

     \param args - arguments for validationStringVector construction
    */
    NameRegExFilter(Args&&... args)
        :ParameterNameFilter(std::forward<Args>(args)...)
    {
        for (auto validationString : validationStringVector) {
            regExpList << QRegExp(validationString.c_str());
        }
    }

    /*!
     \brief add new string, representing regular expression, for validation

     \param new_string
    */
    virtual void addValidationString(std::string new_string);
    /*!
     \brief

     \param dataSample
     \return bool
    */
    virtual bool isValid(std::shared_ptr<DataSample> dataSample);
private:
    QList <QRegExp> regExpList; /*!< TODO */
    std::set<unsigned int> validIdsSet; /*!< TODO */
};

#endif // NAMEREGEXFILTER_H
