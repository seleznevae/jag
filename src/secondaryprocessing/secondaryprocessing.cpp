#include <QDebug>
#include "secondaryprocessing/datacontainer.h"
#include "secondaryprocessing/secondaryprocessing.hpp"
#include "secondaryprocessing/secondaryprocessing.h"
#include "core/graphmodel.h"
#include "global/global_definitions.h"


//bool parse(Iterator &iter,
//           Iterator end,
//           const grammar<FPT,Iterator> &g,
//           FPT &result)
//{
//    return boost::spirit::qi::phrase_parse(
//                iter, end, g, boost::spirit::ascii::space, result);
//}

bool parse_func_string_and_count(std::string::iterator &iter, std::string::iterator end, DataContainer &result)
{
    expression::grammar<DataContainer, std::string::iterator> gram;
    return parse(iter, end, gram, result);
}


DummyDouble operator-(DummyDouble arg) {
    DummyDouble res(-arg.d);
    return res;
}

DummyDouble operator+(DummyDouble arg) {
    return arg;
}

bool operator==(const DummyDouble& arg1, const DummyDouble& arg2) {
    return arg1.d == arg2.d;
}

bool operator<(const DummyDouble& arg1, const DummyDouble& arg2) {
    return arg1.d < arg2.d;
}

bool operator>(const DummyDouble& arg1, const DummyDouble& arg2) {
    return arg1.d > arg2.d;
}




DummyDouble sqrt(const DummyDouble &arg)
{
    return sqrt(arg.d);
}

DummyDouble pow(const DummyDouble &arg1, const DummyDouble &arg2)
{
    return DummyDouble(pow(arg1.d, arg2.d));
}

DummyDouble max(const DummyDouble &arg1, const DummyDouble &arg2)
{
    return DummyDouble(std::max(arg1.d, arg2.d));
}

DummyDouble min(const DummyDouble &arg1, const  DummyDouble &arg2)
{
    return DummyDouble(std::min(arg1.d, arg2.d));
}



