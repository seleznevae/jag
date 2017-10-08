#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QVector>
#include <float.h>
#include "core/graphmodel.h"
#include <boost/operators.hpp>

/*!
 \brief Basic class that is used in secondary processing.

 DataContainer contains xData, yData, interoplationType (to know how evaluate values between points) - all information
 that is necessary for calculations in secondary processing.
*/
class DataContainer: boost::arithmetic<DataContainer>
{
public:
    enum class GraphType { Function,
                           Curve
                          };

    DataContainer(QVector<double> x, QVector<double> y): mXData(x), mYData(y), lineInterpolation(GraphModel::LineStyle::lsLine), mLastIndexFoundInGetValue(0) { }
    DataContainer(double dd = 0): lineInterpolation(GraphModel::LineStyle::lsLine), mLastIndexFoundInGetValue(0) {mXData << mLowLimit << mHighLimit; mYData << dd << dd; }
    DataContainer(const DataContainer& arg): mXData(arg.mXData), mYData(arg.mYData), lineInterpolation(arg.lineInterpolation), mType(arg.mType), mLastIndexFoundInGetValue(0) { }
    DataContainer(DataContainer&& arg): mXData(std::move(arg.mXData)), mYData(std::move(arg.mYData)), lineInterpolation(arg.lineInterpolation), mType(arg.mType), mLastIndexFoundInGetValue(0) { }
    DataContainer(QString str);

    DataContainer& operator=(const DataContainer&arg) {
        mXData = arg.mXData;
        mYData = arg.mYData;
        lineInterpolation = arg.lineInterpolation;
        mType = arg.mType;
        mLastIndexFoundInGetValue = 0;
        return *this;
    }

    DataContainer& operator=(DataContainer&&arg) {
        mXData = std::move(arg.mXData);
        mYData = std::move(arg.mYData);
        lineInterpolation = arg.lineInterpolation;
        mType = arg.mType;
        mLastIndexFoundInGetValue = 0;
        return *this;

    }

//    DataContainer& operator=(DataContainer arg) {
//        mXData = arg.mXData;
//        mYData = arg.mYData;
//        lineInterpolation = arg.lineInterpolation;
//        return *this;
//    }


    DataContainer& operator=(double arg) {
        *this = DataContainer(arg);
        return *this;
    }

    void stripData();

    DataContainer& operator+=(const DataContainer&arg);
    DataContainer& operator-=(const DataContainer&arg);
    DataContainer& operator*=(const DataContainer&arg);
    DataContainer& operator/=(const DataContainer&arg);


    QVector<double> mXData;
    QVector<double> mYData;
    GraphModel::LineStyle lineInterpolation;
    GraphType mType = GraphType::Function;
    mutable int mLastIndexFoundInGetValue = 0;

public:
    double getValue(double x, bool useLastFountIndexInSearch = false) const;
    static QVector<double> combineXVectors(const QVector<double> x1, const QVector<double> x2);

    static double mLowLimit;
    static double mHighLimit;
    static void setConstraints(double low, double high);
    static void setDefaultConstraints();
private:

};

DataContainer operator<(const DataContainer& arg1, const DataContainer& arg2);
DataContainer operator>(const DataContainer& arg1, const DataContainer& arg2);
DataContainer operator<=(const DataContainer& arg1, const DataContainer& arg2);
DataContainer operator>=(const DataContainer& arg1, const DataContainer& arg2);
DataContainer dataEqualOperator(const DataContainer& arg1, const DataContainer& arg2);
DataContainer dataNotEqualOperator(const DataContainer& arg1, const DataContainer& arg2);

DataContainer operator-(DataContainer arg);

DataContainer operator+(DataContainer arg);

bool operator==(const DataContainer& arg1, const DataContainer& arg2);

//bool operator<(const DataContainer& arg1, const DataContainer& arg2) ;

//bool operator>(const DataContainer& arg1, const DataContainer& arg2);






namespace DataContainerNmspc {



DataContainer abs(const DataContainer &arg1);
DataContainer acos(const DataContainer &arg1);
DataContainer acosh(const DataContainer &arg1);
DataContainer asin(const DataContainer &arg1);
DataContainer asinh(const DataContainer &arg1);
DataContainer atan(const DataContainer &arg1);
DataContainer atanh(const DataContainer &arg1);
DataContainer ceil(const DataContainer &arg1);
DataContainer cbrt(const DataContainer &arg1);
DataContainer cos(const DataContainer &arg1);
DataContainer cosh(const DataContainer &arg1);
DataContainer deriv(const DataContainer &arg1);
DataContainer exp(const DataContainer &arg1);
DataContainer floor(const DataContainer &arg1);
DataContainer fftA(const DataContainer &arg1);
DataContainer fftAN(const DataContainer &arg1);
DataContainer fftR(const DataContainer &arg1);
DataContainer fftI(const DataContainer &arg1);
DataContainer fftES(const DataContainer &container);
DataContainer fftPS(const DataContainer &container);
DataContainer heavis(const DataContainer &arg1);
DataContainer heavis_t(const DataContainer &arg1);
DataContainer integr(const DataContainer &arg1);
DataContainer log(const DataContainer &arg1);
DataContainer log10(const DataContainer &arg1);
DataContainer log2(const DataContainer &arg1);
DataContainer maxElemInCont(const DataContainer &arg1);
DataContainer mean(const DataContainer &arg1);
DataContainer minElemInCont(const DataContainer &arg1);
DataContainer nanTo0(const DataContainer &arg1);
DataContainer negToNan(const DataContainer &arg1);
DataContainer nonnegToNan(const DataContainer &arg1);
DataContainer posToNan(const DataContainer &arg1);
DataContainer nonposToNan(const DataContainer &arg1);
DataContainer negToNanInterp(const DataContainer &arg1);
DataContainer nonnegToNanInterp(const DataContainer &arg1);
DataContainer posToNanInterp(const DataContainer &arg1);
DataContainer nonposToNanInterp(const DataContainer &arg1);
DataContainer round(const DataContainer &arg1);
DataContainer sign(const DataContainer &arg1);
DataContainer sin(const DataContainer &arg1);
DataContainer sinh(const DataContainer &arg1);
DataContainer sqrt(const DataContainer &arg1);
DataContainer tan(const DataContainer &arg1);
DataContainer tanh(const DataContainer &arg1);
DataContainer trunc(const DataContainer &arg1);


//windows
DataContainer bartlett(const DataContainer &arg1);
DataContainer hamming(const DataContainer &arg1);
DataContainer hann(const DataContainer &arg1);
DataContainer welch(const DataContainer &arg1);





DataContainer USAStAtm76_HToRho(const DataContainer &arg1);
DataContainer USAStAtm76_HToA(const DataContainer &arg1);
DataContainer USAStAtm76_HToG(const DataContainer &arg1);
DataContainer USAStAtm76_HToT(const DataContainer &arg1);
DataContainer USAStAtm76_HToP(const DataContainer &arg1);





DataContainer atan2(const DataContainer &arg1, const DataContainer &arg2);
DataContainer decimation(const DataContainer &arg1, const DataContainer &arg2);
DataContainer delay(const DataContainer &arg1, const DataContainer &arg2);
DataContainer getBit(const DataContainer &arg1, const DataContainer &arg2);
DataContainer hypot(const DataContainer &arg1, const DataContainer &arg2);
DataContainer linInterp(const DataContainer &arg1, const DataContainer &arg2);
DataContainer paramCurve(const DataContainer &arg1, const DataContainer &arg2);
DataContainer pow(const DataContainer &arg1, const DataContainer &arg2);

DataContainer getNBits(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3);
DataContainer clamp(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3);
DataContainer time(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3);

DataContainer geo_cc2fi (const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3, const DataContainer &arg4);
DataContainer geo_cc2lam(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3, const DataContainer &arg4);
DataContainer geo_cc2h  (const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3, const DataContainer &arg4);

DataContainer g1(const DataContainer & );
DataContainer g2(const DataContainer & );
DataContainer g3(const DataContainer & );
DataContainer g4(const DataContainer & );
DataContainer g5(const DataContainer & );
DataContainer g6(const DataContainer & );
DataContainer g7(const DataContainer & );
DataContainer g8(const DataContainer & );
DataContainer g9(const DataContainer & );
DataContainer g10(const DataContainer & );
DataContainer g11(const DataContainer & );
DataContainer g12(const DataContainer & );
DataContainer g13(const DataContainer & );
DataContainer g14(const DataContainer & );
DataContainer g15(const DataContainer & );
DataContainer g16(const DataContainer & );
DataContainer g17(const DataContainer & );
DataContainer g18(const DataContainer & );
DataContainer g19(const DataContainer & );
DataContainer g20(const DataContainer & );
DataContainer g21(const DataContainer & );
DataContainer g22(const DataContainer & );
DataContainer g23(const DataContainer & );
DataContainer g24(const DataContainer & );
DataContainer g25(const DataContainer & );
DataContainer g26(const DataContainer & );
DataContainer g27(const DataContainer & );
DataContainer g28(const DataContainer & );
DataContainer g29(const DataContainer & );
DataContainer g30(const DataContainer & );
DataContainer g31(const DataContainer & );
DataContainer g32(const DataContainer & );
DataContainer g33(const DataContainer & );
DataContainer g34(const DataContainer & );
DataContainer g35(const DataContainer & );
DataContainer g36(const DataContainer & );
DataContainer g37(const DataContainer & );
DataContainer g38(const DataContainer & );
DataContainer g39(const DataContainer & );
DataContainer g40(const DataContainer & );
DataContainer g41(const DataContainer & );
DataContainer g42(const DataContainer & );
DataContainer g43(const DataContainer & );
DataContainer g44(const DataContainer & );
DataContainer g45(const DataContainer & );
DataContainer g46(const DataContainer & );
DataContainer g47(const DataContainer & );
DataContainer g48(const DataContainer & );
DataContainer g49(const DataContainer & );
DataContainer g50(const DataContainer & );
//DataContainer &operator=(const DataContainer&arg1, const DataContainer&arg2);
}

DataContainer pow(const DataContainer &arg1, const DataContainer &arg2);

#endif // DATACONTAINER_H
