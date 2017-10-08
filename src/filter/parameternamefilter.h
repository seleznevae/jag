#ifndef PARAMETERNAMEFILTER_H
#define PARAMETERNAMEFILTER_H

#include "filter.h"
#include <string>
#include <vector>

/*!
 \brief abstract class for all filters that determine validness
 of data if their name contains in particular set of names

*/
class ParameterNameFilter : public Filter
{
public:
    /*!
     \brief determines validness logic

      Logic           | Meaning
      ----------------|----------------
      PositiveLogic   |  data sample is valid if it belongs to the defined set of names
      NegativeLogic   |  data sample is valid if it doesn't belong to the defined set of names


    */
    enum class Logic {PositiveLogic, NegativeLogic};


    template <typename... Args>
    /*!
     \brief constructor

     \param args - valid arguments for validationStringVector construction
    */
    ParameterNameFilter(Args&&... args)
        :validationStringVector(std::forward<Args>(args)...),
          filterLogic(Logic::PositiveLogic)
    {

    }

    /*!
     \brief add new_string that determines set of valid (PositiveLogic) or invalid (NegativeLogic) names

     \param new_string
    */
    virtual void addValidationString(std::string new_string);
    void setFilterLogic(Logic logic);
protected:
    std::vector<std::string> validationStringVector; /*!< vector of strings that determines set of valid (PositiveLogic) or invalid (NegativeLogic) names */
    Logic filterLogic; /*!< logic of filter */
};

#endif // PARAMETERNAMEFILTER_H
