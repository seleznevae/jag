#ifndef SECONDARYPROCESSING_HPP
#define SECONDARYPROCESSING_HPP



#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <math.h>
#include "secondaryprocessing/datacontainer.h"



class DummyDouble {
public:
    DummyDouble(double dd = 0) : d(dd) {}
    DummyDouble(const DummyDouble& dd): d(dd.d) {}
    ~DummyDouble() { }

    DummyDouble& operator=(const DummyDouble&arg) {
        d = arg.d;
        return *this;
    }

    DummyDouble& operator=(double arg) {
        d = arg;
        return *this;
    }

//    DummyDouble& operator-() {
//        d = -d;
//        return *this;
//    }

//    DummyDouble& operator+() {
//        return *this;
//    }

    DummyDouble& operator+=(const DummyDouble&arg) {
        d += arg.d;
        return *this;
    }

    DummyDouble& operator-=(const DummyDouble&arg) {
        d -= arg.d;
        return *this;
    }

    DummyDouble& operator*=(const DummyDouble&arg) {
        d *= arg.d;
        return *this;
    }

    DummyDouble& operator/=(const DummyDouble&arg) {
        d /= arg.d;
        return *this;
    }

    DummyDouble operator+(const DummyDouble&arg) {
        DummyDouble res(*this);
        res += arg;
        return res;
    }

    DummyDouble operator-(const DummyDouble&arg) {
        DummyDouble res(*this);
        res -= arg;
        return res;
    }

    DummyDouble operator*(const DummyDouble&arg) {
        DummyDouble res(*this);
        res *= arg;
        return res;
    }

    DummyDouble operator/(const DummyDouble&arg) {
        DummyDouble res(*this);
        res /= arg;
        return res;
    }


    double d;
};

DummyDouble operator-(DummyDouble arg);

DummyDouble operator+(DummyDouble arg);

bool operator==(const DummyDouble& arg1, const DummyDouble& arg2);

bool operator<(const DummyDouble& arg1, const DummyDouble& arg2) ;

bool operator>(const DummyDouble& arg1, const DummyDouble& arg2);






DummyDouble sqrt(const DummyDouble &arg1);

DummyDouble pow(const DummyDouble &arg1, const DummyDouble &arg2);
DummyDouble max(const DummyDouble &arg1, const DummyDouble &arg2);

DummyDouble min(const DummyDouble &arg1, const  DummyDouble &arg2);










#include <cmath>
#include <limits>
#include <iterator>

#include <boost/spirit/version.hpp>
#if !defined(SPIRIT_VERSION) || SPIRIT_VERSION < 0x2010
#error "At least Spirit version 2.1 required"
#endif
#include <boost/math/constants/constants.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace dcNmspc = DataContainerNmspc;

namespace expression {

namespace { // anonymous

struct lazy_pow_
{
    template <typename X, typename Y>
    struct result { typedef X type; };

    template <typename X, typename Y>
    X operator()(X x, Y y) const
    {
        return dcNmspc::pow(x, y);
    }
};

struct lazy_zfunc_
{
    template <typename F, typename A1>
    struct result { typedef A1 type; };

    template <typename F, typename A1>
    A1 operator()(F f, A1 a1) const
    {
        return f(a1);
    }
};



struct lazy_ufunc_
{
    template <typename F, typename A1>
    struct result { typedef A1 type; };

    template <typename F, typename A1>
    A1 operator()(F f, A1 a1) const
    {
        return f(a1);
    }
};

struct lazy_bfunc_
{
    template <typename F, typename A1, typename A2>
    struct result { typedef A1 type; };

    template <typename F, typename A1, typename A2>
    A1 operator()(F f, A1 a1, A2 a2) const
    {
        return f(a1, a2);
    }
};

struct lazy_tfunc_
{
    template <typename F, typename A1, typename A2,  typename A3>
    struct result { typedef A1 type; };

    template <typename F, typename A1, typename A2,  typename A3>
    A1 operator()(F f, A1 a1, A2 a2, A3 a3) const
    {
        return f(a1, a2, a3);
    }
};

//for equlity operators
struct lazy_bfunc_eq_
{
    template <typename F, typename A1, typename A2>
    struct result { typedef A1 type; };

    template <typename F, typename A1, typename A2>
    A1 operator()(F f, A1 a1, A2 a2) const
    {
        return f(a1, a2);
    }
};

struct lazy_qfunc_
{
    template <typename F, typename A1, typename A2, typename A3, typename A4>
    struct result { typedef A1 type; };

    template <typename F, typename A1, typename A2, typename A3, typename A4>
    A1 operator()(F f, A1 a1, A2 a2, A3 a3, A4 a4) const
    {
        return f(a1, a2, a3, a4);
    }
};



template <class T>
T max_by_value ( const T &a, const T &b ) {
    return std::max(a, b);
}

template <class T>
T min_by_value ( const T &a, const T &b ) {
    return std::min(a, b);
}


} // end namespace anonymous



template <typename FPT, typename Iterator>
struct grammar
    : boost::spirit::qi::grammar<
            Iterator, FPT(), boost::spirit::ascii::space_type
        >
{

    // symbol table for constants like "pi"
    struct constant_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
                FPT
            >
    {
        constant_()
        {
            this->add

                    ("pi",       FPT(3.1415926535897932384626433832795))
                    ("e" ,       FPT(2.7182818284590452353602874713527))
            ;
        }
    } constant;


    // symbol table for unary functions like "abs"
    struct ufunc_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
//                FPT (*)(FPT)
            FPT (*)(const FPT&)
            >
    {
        ufunc_()
        {
            this->add


                    ("abs"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::abs  ))
                    ("bartlett"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::bartlett  ))
                    ("log"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::log  ))
                    ("log10", static_cast<FPT (*)(const FPT&)>(&dcNmspc::log10))
                    ("log2" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::log2 ))
                    ("exp"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::exp  ))
                    ("sqrt" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::sqrt ))
                    ("cbrt" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::cbrt ))
                    ("sin"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::sin  ))
                    ("cos"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::cos  ))
                    ("tan"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::tan  ))
                    ("asin" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::asin ))
                    ("acos" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::acos ))
                    ("atan" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::atan ))
                    ("sinh" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::sinh ))
                    ("cosh" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::cosh ))
                    ("tanh" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::tanh ))
                    ("asinh", static_cast<FPT (*)(const FPT&)>(&dcNmspc::asinh))
                    ("acosh", static_cast<FPT (*)(const FPT&)>(&dcNmspc::acosh))
                    ("atanh", static_cast<FPT (*)(const FPT&)>(&dcNmspc::atanh))
                    ("ceil" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::ceil ))
                    ("floor", static_cast<FPT (*)(const FPT&)>(&dcNmspc::floor))
                    ("round", static_cast<FPT (*)(const FPT&)>(&dcNmspc::round))
                    ("trunc", static_cast<FPT (*)(const FPT&)>(&dcNmspc::trunc))
                    ("deriv", static_cast<FPT (*)(const FPT&)>(&dcNmspc::deriv))
                    ("ffta"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::fftA))
                    ("fftr"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::fftR))
                    ("ffti"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::fftI))
                    ("fftan"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::fftAN))
                    ("fftes"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::fftES))
                    ("fftps"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::fftPS))
                    ("hamming"  ,static_cast<FPT (*)(const FPT&)>(&dcNmspc::hamming))
                    ("hann"  ,static_cast<FPT (*)(const FPT&)>(&dcNmspc::hann))
                    ("integr",static_cast<FPT (*)(const FPT&)>(&dcNmspc::integr))
                    ("nanto0",static_cast<FPT (*)(const FPT&)>(&dcNmspc::nanTo0))
                    ("negtonan",static_cast<FPT (*)(const FPT&)>(&dcNmspc::negToNan))
                    ("negtonaninterp",static_cast<FPT (*)(const FPT&)>(&dcNmspc::negToNanInterp))
                    ("nonnegtonan",static_cast<FPT (*)(const FPT&)>(&dcNmspc::nonnegToNan))
                    ("postonan",static_cast<FPT (*)(const FPT&)>(&dcNmspc::posToNan))
                    ("nonpostonan",static_cast<FPT (*)(const FPT&)>(&dcNmspc::nonposToNan))
                    ("min",static_cast<FPT (*)(const FPT&)>(&dcNmspc::minElemInCont))
                    ("max",static_cast<FPT (*)(const FPT&)>(&dcNmspc::maxElemInCont))
                    ("mean",static_cast<FPT (*)(const FPT&)>(&dcNmspc::mean))
                    ("sign",static_cast<FPT (*)(const FPT&)>(&dcNmspc::sign))
                    ("heavis",static_cast<FPT (*)(const FPT&)>(&dcNmspc::heavis))
                    ("heavis_t",static_cast<FPT (*)(const FPT&)>(&dcNmspc::heavis_t))
                    ("welch"  , static_cast<FPT (*)(const FPT&)>(&dcNmspc::welch))




                    ("usastatm76_htorho",static_cast<FPT (*)(const FPT&)>(&dcNmspc::USAStAtm76_HToRho))
                    ("usastatm76_htoa"  ,static_cast<FPT (*)(const FPT&)>(&dcNmspc::USAStAtm76_HToA  ))
                    ("usastatm76_htog"  ,static_cast<FPT (*)(const FPT&)>(&dcNmspc::USAStAtm76_HToG  ))
                    ("usastatm76_htot"  ,static_cast<FPT (*)(const FPT&)>(&dcNmspc::USAStAtm76_HToT  ))
                    ("usastatm76_htop"  ,static_cast<FPT (*)(const FPT&)>(&dcNmspc::USAStAtm76_HToP  ))

            ;
        }
    } ufunc;

    struct zfunc_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
                FPT (*)(const FPT&)
            >
    {
    zfunc_()
        {
            this->add
                    ("g1" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g1))
                    ("g2" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g2))
                    ("g3" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g3))
                    ("g4" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g4))
                    ("g5" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g5))
                    ("g6" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g6))
                    ("g7" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g7))
                    ("g8" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g8))
                    ("g9" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g9))
                    ("g10" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g10))
                    ("g11" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g11))
                    ("g12" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g12))
                    ("g13" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g13))
                    ("g14" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g14))
                    ("g15" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g15))
                    ("g16" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g16))
                    ("g17" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g17))
                    ("g18" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g18))
                    ("g19" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g19))
                    ("g20" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g20))
                    ("g21" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g21))
                    ("g22" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g22))
                    ("g23" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g23))
                    ("g24" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g24))
                    ("g25" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g25))
                    ("g26" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g26))
                    ("g27" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g27))
                    ("g28" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g28))
                    ("g29" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g29))
                    ("g30" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g30))
                    ("g31" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g31))
                    ("g32" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g32))
                    ("g33" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g33))
                    ("g34" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g34))
                    ("g35" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g35))
                    ("g36" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g36))
                    ("g37" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g37))
                    ("g38" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g38))
                    ("g39" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g39))
                    ("g40" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g40))
                    ("g41" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g41))
                    ("g42" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g42))
                    ("g43" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g43))
                    ("g44" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g44))
                    ("g45" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g45))
                    ("g46" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g46))
                    ("g47" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g47))
                    ("g48" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g48))
                    ("g49" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g49))
                    ("g50" , static_cast<FPT (*)(const FPT&)>(&dcNmspc::g50))
            ;
        }
    } zfunc;

    // symbol table for binary functions like "pow"
    struct bfunc_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
//                FPT (*)(FPT, FPT)
               FPT (*)(const FPT&, const FPT&)
            >
    {
        bfunc_()
        {
            this->add


                    ("atan2"  , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::atan2 ))
                    ("decim"  , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::decimation ))
                    ("delay"  , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::delay ))
                    ("hypot"  , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::hypot ))
                    ("lininterp", static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::linInterp ))
                    ("getbit" , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::getBit))
                    ("paramcurve" , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::paramCurve))
                    ("pow"    , static_cast<FPT (*)(const FPT&, const FPT&)>(&dcNmspc::pow   ))
            ;
        }
    } bfunc;

    // symbol table for functions with 3 parameters
    struct tfunc_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
//                FPT (*)(FPT, FPT)
               FPT (*)(const FPT&, const FPT&, const FPT&)
            >
    {
        tfunc_()
        {
            this->add
                    ("getnbits" , static_cast<FPT (*)(const FPT&, const FPT&, const FPT&)>(&dcNmspc::getNBits ))
                    ("clamp"    , static_cast<FPT (*)(const FPT&, const FPT&, const FPT&)>(&dcNmspc::clamp    ))
                    ("time"     , static_cast<FPT (*)(const FPT&, const FPT&, const FPT&)>(&dcNmspc::time     ))


            ;
        }
    } tfunc;

    // symbol table for functions with 4 parameters
    struct qfunc_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
//                FPT (*)(FPT, FPT)
               FPT (*)(const FPT&, const FPT&, const FPT&, const FPT&)
            >
    {
        qfunc_()
        {
            this->add
                    ("geo_cc2fi" , static_cast<FPT (*)(const FPT&, const FPT&, const FPT&, const FPT&)>(&dcNmspc::geo_cc2fi ))
                    ("geo_cc2lam", static_cast<FPT (*)(const FPT&, const FPT&, const FPT&, const FPT&)>(&dcNmspc::geo_cc2lam))
                    ("geo_cc2h"  , static_cast<FPT (*)(const FPT&, const FPT&, const FPT&, const FPT&)>(&dcNmspc::geo_cc2h  ))
            ;
        }
    } qfunc;


    struct bfunc_eq_
        : boost::spirit::qi::symbols<
                typename std::iterator_traits<Iterator>::value_type,
               FPT (*)(const FPT&, const FPT&)
            >
    {
        bfunc_eq_()
        {
            this->add
                    ("=="  , static_cast<FPT (*)(const FPT&, const FPT&)>(&dataEqualOperator ))
                    ("!=" , static_cast<FPT (*)(const FPT&, const FPT&)>(&dataNotEqualOperator))
            ;
        }
    } bfunc_eq;

    boost::spirit::qi::rule<
            Iterator, FPT(), boost::spirit::ascii::space_type
        > expression, term, factor, primary, compare_expression, equal_logic_expression;

    grammar() : grammar::base_type(equal_logic_expression)
    {
        using boost::spirit::qi::real_parser;
        using boost::spirit::qi::real_policies;
        real_parser<FPT,real_policies<FPT> > real;

        using boost::spirit::qi::_1;
        using boost::spirit::qi::_2;
        using boost::spirit::qi::_3;
        using boost::spirit::qi::_4;
        using boost::spirit::qi::_5;
        using boost::spirit::qi::no_case;
        using boost::spirit::qi::_val;
        using boost::spirit::qi::lit;

        boost::phoenix::function<lazy_pow_>   lazy_pow;
        boost::phoenix::function<lazy_ufunc_> lazy_ufunc;
        boost::phoenix::function<lazy_bfunc_> lazy_bfunc;
        boost::phoenix::function<lazy_zfunc_> lazy_zfunc;
        boost::phoenix::function<lazy_tfunc_> lazy_tfunc;
        boost::phoenix::function<lazy_qfunc_> lazy_qfunc;
        boost::phoenix::function<lazy_bfunc_eq_> lazy_bfunc_eq;



        equal_logic_expression =
                            compare_expression                   [_val =  _1]
                            >> * (  ( no_case[bfunc_eq] >>  compare_expression )
                                                        [_val = lazy_bfunc_eq(_1, _val, _2)]
                                )
                            ;

        compare_expression =
                        expression                   [_val =  _1]
                        >> *(  ('<' >> expression    [_val =  _val < _1])
                            |  ('>' >> expression    [_val =  _val > _1])
                            |  (lit(">=") >> expression    [_val =  _val >= _1])
                            |  (lit("<=") >> expression    [_val =  _val <= _1])
                            )
                        ;

        expression =
            term                   [_val =  _1]
            >> *(  ('+' >> term    [_val += _1])
                |  ('-' >> term    [_val -= _1])
                )
            ;

        term =
            factor                 [_val =  _1]
            >> *(  ('*' >> factor  [_val *= _1])
                |  ('/' >> factor  [_val /= _1])
                )
            ;

        factor =
            primary                [_val =  _1]
            >> *(  ("^" >> factor [_val = lazy_pow(_val, _1)])
                )
            ;



       primary =
                '(' >> equal_logic_expression [_val =  _1] >> ')'
           |   ('-' >> primary    [_val = -_1])
           |   ('+' >> primary    [_val =  _1])
           |   no_case[zfunc]     [_val = lazy_zfunc(_1, FPT())]
           |   (no_case[ufunc] >> '(' >> equal_logic_expression >> ')')
                                  [_val = lazy_ufunc(_1, _2)]
           |   (no_case[bfunc] >> '(' >> equal_logic_expression >> ','
                                      >> equal_logic_expression >> ')')
                                  [_val = lazy_bfunc(_1, _2, _3)]
           |   (no_case[tfunc] >> '(' >> equal_logic_expression >> ',' >> equal_logic_expression >> ','
                                      >> equal_logic_expression >>  ')')
                                  [_val = lazy_tfunc(_1, _2, _3, _4)]
           |   (no_case[qfunc] >> '(' >> equal_logic_expression >> ',' >> equal_logic_expression >> ','
                                      >> equal_logic_expression >>  ',' >> equal_logic_expression >> ')')
                                  [_val = lazy_qfunc(_1, _2, _3, _4, _5)]
           |   no_case[constant]  [_val =  _1]
           |   real               [_val =  _1]
           ;

    }
};



template <typename FPT, typename Iterator>
bool parse(Iterator &iter,
           Iterator end,
           const grammar<FPT,Iterator> &g,
           FPT &result)
{
    return boost::spirit::qi::phrase_parse(
                iter, end, g, boost::spirit::ascii::space, result);
}

} // end namespace expression





















#endif // SECONDARYPROCESSING_HPP
