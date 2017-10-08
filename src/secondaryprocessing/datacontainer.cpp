#include "datacontainer.h"
#include "basic/basic.h"
#include <QtAlgorithms>
#include "global/global_definitions.h"
#include <stdexcept>
#include "cmath"
#include "basic/bassic.h"
#include "secondaryprocessing/fft/my_fft_api.h"
#include "secondaryprocessing/special_functions/us_stand_atm1976.h"
#include "secondaryprocessing/special_functions/svrg_coorder.h"

using namespace DataContainerNmspc;

double DataContainer::mLowLimit = -1.e20;
double DataContainer::mHighLimit = 1.e20;
void DataContainer::setConstraints(double low, double high)
{
    mLowLimit = low;
    mHighLimit = high;
}

void DataContainer::setDefaultConstraints()
{
    mLowLimit = -1.e20;
    mHighLimit = 1.e20;
}



void DataContainer::stripData()
{

    auto itLow = qLowerBound(mXData.begin(), mXData.end(), mLowLimit);
    if (itLow != mXData.begin()) {
        itLow--;
    }
    int lowIndex = itLow - mXData.begin();

    auto itHigh = qLowerBound(mXData.begin(), mXData.end(), mHighLimit);
    if (itHigh != mXData.end()) {
        itHigh++;
    }

//    int highIndex = itHigh - mXData.begin();
//    mXData.erase(mXData.begin(), itLow);
//    mXData.erase(itHigh, mXData.end());
//    mYData.erase(mYData.begin() , mYData.begin() + lowIndex);
//    mYData.erase(mYData.begin() + highIndex, mYData.end());

     auto dif = itHigh - itLow;
     mXData.erase(mXData.begin(), itLow);
     mXData.erase(mXData.begin()+dif, mXData.end());
     mYData.erase(mYData.begin() , mYData.begin() + lowIndex);
     mYData.erase(mYData.begin()+dif, mYData.end());
}

double DataContainer::getValue(double x, bool useLastFountIndexInSearch) const
{
    if (x < mXData[0] || x > mXData[mXData.size() - 1]) {
        mLastIndexFoundInGetValue = 0;
        return NAN;
    } else {
//        auto it = std::find_if(mXData.begin(), mXData.end(), [x](double elem) {return elem >= x;});
        //TODO: changed on the 24.08.16. Check if it is equal to the string above
        auto it = std::lower_bound(mXData.begin() + (useLastFountIndexInSearch ? mLastIndexFoundInGetValue : 0),
                                   mXData.end(), x);
        useLastFountIndexInSearch = it - mXData.begin();

        int right = it - mXData.begin();
        int left = right != 0 ? right - 1 : right;
        double div;
        if (left == right)
            div = 0;
        else
            div = (x - mXData[left]) / (mXData[right] - mXData[left]);

        switch (lineInterpolation) {
            case GraphModel::LineStyle::lsImpulse:
            case GraphModel::LineStyle::lsNone:
                if (x == mXData[left])
                    return mYData[left];
                else if (x == mXData[right])
                    return mYData[right];
                else
                    return 0;
                break;
            case GraphModel::LineStyle::lsLine:
            case GraphModel::LineStyle::lsSmooth:
                return mYData[right] * div + mYData[left] * (1. - div);
                break;
            case GraphModel::LineStyle::lsStepLeft:
                if (x == mXData[right])
                    return mYData[right];
                else
                    return mYData[left];
                break;
            case GraphModel::LineStyle::lsStepRight:
                if (x == mXData[left])
                    return mYData[left];
                else
                    return mYData[right];
                break;
            case GraphModel::LineStyle::lsStepCenter:
                if (x > 0.5*(mXData[left]+mXData[right]))
                    return mYData[right];
                else
                    return mYData[left];
                break;
            default:
                errorExit("Unexpected situation in %s", __FUNCTION__);
                return NAN;
                break;
        }
    }
    return NAN;
}

QVector<double> DataContainer::combineXVectors(const QVector<double> x1, const QVector<double> x2)
{
    //union of xdata
//    QVector<double> ret;
//    ret.resize(std::max(x1.size(), x2.size()));
//    int ind1 = 0;
//    int ind2 = 0;
//    while (ind1 < x1.size() || ind2 < x2.size()) {
//        if (ind1 == x1.size()) {
//            ret.push_back(x2[ind2++]);
//        } else if (ind2 == x2.size()) {
//            ret.push_back(x1[ind1++]);
//        } else {
//            if (x1[ind1] < x2[ind2])
//                ret.push_back(x1[ind1++]);
//            else
//                ret.push_back(x2[ind2++]);
//        }
//    }



    //TODO - make algorithm more optimal
    if (x1.isEmpty() | x2.isEmpty())
        return QVector<double>();

    if (x1.size() == x2.size()) {
        if (x1 == x2)
            return x1;
    }

    QVector<double> retBuf = x1;
    retBuf += x2;
    qSort(retBuf.begin(), retBuf.end());


//    double xlow = std::max(x1[0], x2[0]);
//    double xhigh = std::min(x1[x1.size() - 1], x2[x2.size() - 1]);

    double xlow = std::max(x1[0], x2[0]);
    double xhigh = std::min(x1.back(), x2.back());


    QVector<double> ret;
    ret.reserve(std::max(x1.size(), x2.size()) / 2);

    foreach (double x, retBuf) {
        if (x >= xlow && x <= xhigh) {
//            if (ret.size() == 0 || x != ret[ret.size() - 1])  //avoiding equal numbers
//                ret.push_back(x);
            if (ret.size() == 0 || x != ret.back())  //avoiding equal numbers
                ret.push_back(x);
        }
    }

    return ret;
}

DataContainer::DataContainer(QString str)
{
    if (str == "g1") {
        mXData << 0 << 100;
        mXData << 100 << 0;
    } else {
        mXData << 10 << 110;
        mXData << 110 << 20;
    }
    lineInterpolation = GraphModel::LineStyle::lsLine;
}

DataContainer &DataContainer::operator+=(const DataContainer &arg)
{
    QVector<double> xVector = combineXVectors(this->mXData, arg.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(getValue(x) + arg.getValue(x));
//    }
    if (xVector == this->mXData && xVector == arg.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(mYData[i] + arg.mYData[i]);
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(getValue(x) + arg.getValue(x));
        }
    }

    mXData = xVector;
    mYData = yVector;
    mLastIndexFoundInGetValue = 0;
    return *this;
}

DataContainer &DataContainer::operator-=(const DataContainer &arg)
{
    QVector<double> xVector = combineXVectors(this->mXData, arg.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(getValue(x) - arg.getValue(x));
//    }
    if (xVector == this->mXData && xVector == arg.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(mYData[i] - arg.mYData[i]);
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(getValue(x) - arg.getValue(x));
        }
    }

    mXData = xVector;
    mYData = yVector;
    mLastIndexFoundInGetValue = 0;
    return *this;
}

DataContainer &DataContainer::operator*=(const DataContainer &arg)
{
    QVector<double> xVector = combineXVectors(this->mXData, arg.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(getValue(x) * arg.getValue(x));
//    }
    if (xVector == this->mXData && xVector == arg.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(mYData[i] * arg.mYData[i]);
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(getValue(x) * arg.getValue(x));
        }
    }

    mXData = xVector;
    mYData = yVector;
    mLastIndexFoundInGetValue = 0;
    return *this;
}

DataContainer &DataContainer::operator/=(const DataContainer &arg)
{
    QVector<double> xVector = combineXVectors(this->mXData, arg.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(getValue(x) / arg.getValue(x));
//    }
    if (xVector == this->mXData && xVector == arg.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(mYData[i] / arg.mYData[i]);
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(getValue(x) / arg.getValue(x));
        }
    }

    mXData = xVector;
    mYData = yVector;
    mLastIndexFoundInGetValue = 0;
    return *this;
}





DataContainer operator-(DataContainer arg)
{
    for (int i = 0; i < arg.mYData.size(); ++i)
        arg.mYData[i] = -arg.mYData[i];
    return arg;
}


DataContainer operator+(DataContainer arg)
{
    return arg;
}


bool operator==(const DataContainer &arg1, const DataContainer &arg2)
{
    if (arg1.mXData == arg2.mXData
            && arg1.mYData == arg2.mYData
            && arg1.lineInterpolation == arg2.lineInterpolation)
                return true;
    else
        return false;
}

DataContainer operator<(const DataContainer& arg1, const DataContainer& arg2)
{

    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(double(arg1.getValue(x) < arg2.getValue(x)));
//    }
    if (xVector == arg1.mXData && xVector == arg2.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(double(arg1.mYData[i] < arg2.mYData[i]));
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(double(arg1.getValue(x) < arg2.getValue(x)));
        }
    }

    return DataContainer(xVector, yVector);
}

DataContainer operator>(const DataContainer& arg1, const DataContainer& arg2)
{

    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(double(arg1.getValue(x) > arg2.getValue(x)));
//    }
    if (xVector == arg1.mXData && xVector == arg2.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(double(arg1.mYData[i] > arg2.mYData[i]));
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(double(arg1.getValue(x) > arg2.getValue(x)));
        }
    }

    return DataContainer(xVector, yVector);
}

DataContainer operator<=(const DataContainer& arg1, const DataContainer& arg2)
{

    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(double(arg1.getValue(x) <= arg2.getValue(x)));
//    }
    if (xVector == arg1.mXData && xVector == arg2.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(double(arg1.mYData[i] <= arg2.mYData[i]));
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(double(arg1.getValue(x) <= arg2.getValue(x)));
        }
    }
    return DataContainer(xVector, yVector);
}

DataContainer operator>=(const DataContainer& arg1, const DataContainer& arg2)
{

    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(double(arg1.getValue(x) >= arg2.getValue(x)));
//    }
    if (xVector == arg1.mXData && xVector == arg2.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(double(arg1.mYData[i] >= arg2.mYData[i]));
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(double(arg1.getValue(x) >= arg2.getValue(x)));
        }
    }

    return DataContainer(xVector, yVector);
}

DataContainer dataEqualOperator(const DataContainer& arg1, const DataContainer& arg2)
{

    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(double(arg1.getValue(x) == arg2.getValue(x)));
//    }
    if (xVector == arg1.mXData && xVector == arg2.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(double(arg1.mYData[i] == arg2.mYData[i]));
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(double(arg1.getValue(x) == arg2.getValue(x)));
        }
    }
    return DataContainer(xVector, yVector);
}

DataContainer dataNotEqualOperator(const DataContainer& arg1, const DataContainer& arg2)
{

    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
//    foreach (double x, xVector) {
//        yVector.push_back(double(arg1.getValue(x) != arg2.getValue(x)));
//    }
    if (xVector == arg1.mXData && xVector == arg2.mXData) {
        for (int i = 0; i < xVector.size(); ++i) {
            yVector.push_back(double(arg1.mYData[i] != arg2.mYData[i]));
        }
    } else {
        foreach (double x, xVector) {
            yVector.push_back(double(arg1.getValue(x) != arg2.getValue(x)));
        }
    }
    return DataContainer(xVector, yVector);
}


template <typename Func>
DataContainer templUnaryFunc(Func func, const DataContainer &arg1)
{
    QVector<double> yVector = arg1.mYData;
    for (int i = 0; i < yVector.size(); ++i)
        yVector[i] = (*func)(yVector[i]);
    return DataContainer(arg1.mXData, yVector);
}


//DataContainer sqrt(const DataContainer &arg1)
//{
//    QVector<double> yVector = arg1.mYData;
//    for (int i = 0; i < yVector.size(); ++i)
//        yVector[i] = std::sqrt(yVector[i]);
//    return DataContainer(arg1.mXData, yVector);
//}

//DataContainer abs(const DataContainer &arg1)
//{
//    QVector<double> yVector = arg1.mYData;
//    for (int i = 0; i < yVector.size(); ++i)
//        yVector[i] = std::abs(yVector[i]);
//    return DataContainer(arg1.mXData, yVector);
//}


namespace DataContainerNmspc {

DataContainer sqrt(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::sqrt(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer cbrt(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::cbrt(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer log(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::log(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer log10(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::log10(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer log2(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::log2(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer exp(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::exp(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer abs(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::abs(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer sin(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::sin(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer cos(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::cos(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer tan(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::tan(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer asin(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::asin(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer acos(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::acos(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer atan(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::atan(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer sinh(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::sinh(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer cosh(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::cosh(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer tanh(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::tanh(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer asinh(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::asinh(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer acosh(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::acosh(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer atanh(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::atanh(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer ceil(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::ceil(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer floor(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::floor(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer round(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::round(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer trunc(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::trunc(x);};
    return templUnaryFunc(lambda, arg1);
}

DataContainer deriv(const DataContainer &arg1)
{
    if (arg1.mXData.size() == 0)
        return DataContainer();
    if (arg1.mXData.size() == 1)
        return DataContainer();

    QVector<double> yVector;
    yVector.reserve(arg1.mYData.size());
    yVector << 0.;
    for (int i = 1; i < arg1.mYData.size(); ++i)
        yVector.push_back((arg1.mYData[i] - arg1.mYData[i - 1]) / (arg1.mXData[i] - arg1.mXData[i - 1]));
    return DataContainer(arg1.mXData, yVector);
}

DataContainer integr(const DataContainer &arg1)
{
    if (arg1.mXData.size() == 0)
        return DataContainer();
    if (arg1.mXData.size() == 1)
        return DataContainer();

    QVector<double> yVector;
    yVector.reserve(arg1.mYData.size());
    yVector << 0.;
    double intergral = 0;
    for (int i = 1; i < arg1.mYData.size(); ++i)
        if (std::isnan(arg1.mYData[i]) || std::isnan(arg1.mYData[i - 1])) {
            yVector.push_back(NAN);
        } else {
            intergral += 0.5 * (arg1.mYData[i] + arg1.mYData[i - 1]) * (arg1.mXData[i] - arg1.mXData[i - 1]);
            yVector.push_back(intergral);
        }
    return DataContainer(arg1.mXData, yVector);
}


DataContainer fftA(const DataContainer &container)
{
    if (container.mXData.size() == 0)
        return DataContainer();
    if (container.mXData.size() == 1)
        return DataContainer();
    DataContainer result;
    std::tie(result.mXData, result.mYData) = jag_fft::qCountFreqAmplSpectr(container.mXData, container.mYData);
    return result;
}

DataContainer fftAN(const DataContainer &container)
{
    int sz = container.mXData.size();
    if (sz == 0)
        return DataContainer();
    if (sz == 1)
        return DataContainer();
    DataContainer result = fftA(container);
    int length = result.mXData.size();
    result.mXData.erase(result.mXData.begin() + length/2, result.mXData.end());
    result.mYData.erase(result.mYData.begin() + length/2, result.mYData.end());
    for (auto &i : result.mYData) {
        i *= 2./sz;
    }
    result.mYData[0] /= 2.;
    return result;
}




DataContainer fftR(const DataContainer &container)
{
    if (container.mXData.size() == 0)
        return DataContainer();
    if (container.mXData.size() == 1)
        return DataContainer();
    DataContainer result;
    std::tie(result.mXData, result.mYData) = jag_fft::qCountFreqRealSpectr(container.mXData, container.mYData);
    return result;
}



DataContainer fftI(const DataContainer &container)
{
    if (container.mXData.size() == 0)
        return DataContainer();
    if (container.mXData.size() == 1)
        return DataContainer();
    DataContainer result;
    std::tie(result.mXData, result.mYData) = jag_fft::qCountFreqImageSpectr(container.mXData, container.mYData);
    return result;
}



DataContainer fftES(const DataContainer &container)
{
    int sz = container.mXData.size();
    if (sz == 0)
        return DataContainer();
    if (sz == 1)
        return DataContainer();
    double dt = container.mXData[1] - container.mXData[0];
    double dt2 = 2*dt*dt;
    DataContainer result = fftA(container);
    int length = result.mXData.size();
    result.mXData.erase(result.mXData.begin() + length/2, result.mXData.end());
    result.mYData.erase(result.mYData.begin() + length/2, result.mYData.end());
    for (auto &i : result.mYData) {
        i *= i*dt2;
    }
    result.mYData[0] /= 2.;
    return result;
}

DataContainer fftPS(const DataContainer &container)
{
    int sz = container.mXData.size();
    if (sz == 0)
        return DataContainer();
    if (sz == 1)
        return DataContainer();
    double T = container.mXData.back() - container.mXData.front();
    double dt = container.mXData[1] - container.mXData[0];
    double dt2 = 2*dt*dt/T;
    DataContainer result = fftA(container);
    int length = result.mXData.size();
    result.mXData.erase(result.mXData.begin() + length/2, result.mXData.end());
    result.mYData.erase(result.mYData.begin() + length/2, result.mYData.end());
    for (auto &i : result.mYData) {
        i *= i*dt2;
    }
    result.mYData[0] /= 2.;
    return result;
}

DataContainer nanTo0(const DataContainer &arg1)
{

    QVector<double> yVector = arg1.mYData;
    for (int i = 0; i < yVector.size(); ++i)
        if (std::isnan(yVector[i]))
            yVector[i] = 0.;
    return DataContainer(arg1.mXData, yVector);
}

DataContainer negToNan(const DataContainer &arg1)
{
    QVector<double> yVector = arg1.mYData;
    for (int i = 0; i < yVector.size(); ++i)
        if (yVector[i] < 0)
            yVector[i] = NAN;
    return DataContainer(arg1.mXData, yVector);
}

DataContainer negToNanInterp(const DataContainer &arg1)
{
    DataContainer ret;
    ret.mXData.reserve(arg1.mXData.size());
    ret.mYData.reserve(arg1.mXData.size());

    for (int i = 0; i < arg1.mYData.size(); ++i) {
        if (arg1.mYData[i] < 0) {
            if (i != 0 && !std::isnan(ret.mYData.back())) {
                double x = arg1.mXData[i - 1]
                        - arg1.mYData[i - 1]*(arg1.mXData[i] - arg1.mXData[i - 1]) / (arg1.mYData[i] - arg1.mYData[i - 1]);
                ret.mXData.push_back(x);
                ret.mYData.push_back(0);
            }
            ret.mYData.push_back(NAN);
        } else {
            if (i != 0 && std::isnan(ret.mYData.back())) {
                double x = arg1.mXData[i - 1]
                        - arg1.mYData[i - 1]*(arg1.mXData[i] - arg1.mXData[i - 1]) / (arg1.mYData[i] - arg1.mYData[i - 1]);
                ret.mXData.push_back(x);
                ret.mYData.push_back(0);
            }
            ret.mYData.push_back(arg1.mYData[i]);
        }
        ret.mXData.push_back(arg1.mXData[i]);
    }
    return ret;
}

DataContainer nonnegToNan(const DataContainer &arg1)
{
    QVector<double> yVector = arg1.mYData;
    for (int i = 0; i < yVector.size(); ++i)
        if (yVector[i] >= 0)
            yVector[i] = NAN;
    return DataContainer(arg1.mXData, yVector);
}

DataContainer nonposToNan(const DataContainer &arg1)
{
    QVector<double> yVector = arg1.mYData;
    for (int i = 0; i < yVector.size(); ++i)
        if (yVector[i] <= 0)
            yVector[i] = NAN;
    return DataContainer(arg1.mXData, yVector);
}

DataContainer posToNan(const DataContainer &arg1)
{
    QVector<double> yVector = arg1.mYData;
    for (int i = 0; i < yVector.size(); ++i)
        if (yVector[i] > 0)
            yVector[i] = NAN;
    return DataContainer(arg1.mXData, yVector);
}

DataContainer maxElemInCont(const DataContainer &arg1)
{
    //to limit x range for analysis
    DataContainer input(1.);
    input *= arg1;

    if (arg1.mXData.size() == 0)
        return DataContainer();
    if (arg1.mXData.size() == 1) {
        return DataContainer(arg1.mXData, arg1.mYData);
    }

    QVector<double> xVector = {input.mXData.front(), input.mXData.back()};
    auto maxElem = findMaxInQVector(input.mYData);
    QVector<double> yVector = {maxElem, maxElem};

    return DataContainer(xVector, yVector);
}

DataContainer mean(const DataContainer &arg1)
{
    if (arg1.mXData.size() == 0)
        return DataContainer();
    if (arg1.mXData.size() == 1)
        return DataContainer();

    DataContainer ret = DataContainerNmspc::integr(arg1);
    double mean = ret.mYData.back() / (ret.mXData.back() - ret.mXData.front());

    for (auto &y : ret.mYData)
        y = mean;

    return ret;

//    QVector<double> yVector;
//    yVector.reserve(arg1.mYData.size());
//    yVector << 0.;
//    double intergral = 0;
//    for (int i = 1; i < arg1.mYData.size(); ++i)
//        if (std::isnan(arg1.mYData[i]) || std::isnan(arg1.mYData[i - 1])) {
//            yVector.push_back(NAN);
//        } else {
//            intergral += 0.5 * (arg1.mYData[i] + arg1.mYData[i - 1]) * (arg1.mXData[i] - arg1.mXData[i - 1]);
//            yVector.push_back(intergral);
//        }
//    return DataContainer(arg1.mXData, yVector);
}

DataContainer minElemInCont(const DataContainer &arg1)
{
    //to limit x range for analysis
    DataContainer input(1.);
    input *= arg1;

    if (arg1.mXData.size() == 0)
        return DataContainer();
    if (arg1.mXData.size() == 1) {
        return DataContainer(arg1.mXData, arg1.mYData);
    }

    QVector<double> xVector = {input.mXData.front(), input.mXData.back()};
    auto maxElem = findMinInQVector(input.mYData);
    QVector<double> yVector = {maxElem, maxElem};

    return DataContainer(xVector, yVector);

}


DataContainer sign(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return std::isfinite(x) ? (x > 0. ? 1. : (x < 0. ? -1. : 0.)) : x ;};
    return templUnaryFunc(lambda, arg1);
}



DataContainer heavis(const DataContainer &arg1)
{
    if (arg1.mXData.size() == 0)
        return DataContainer(arg1.mXData, arg1.mYData);

//    static double mLowLimit;
//    static double mHighLimit;
    double time = arg1.mYData[0];

    if (time >= DataContainer::mHighLimit) {
        return DataContainer(QVector<double>({DataContainer::mLowLimit, DataContainer::mHighLimit}), QVector<double>({0., 0.}));
    }
    if (time <= DataContainer::mLowLimit) {
        return DataContainer(QVector<double>({DataContainer::mLowLimit, DataContainer::mHighLimit}), QVector<double>({1., 1.}));
    }

    double lessThenTime = DataContainer::mLowLimit;
    double delta = time - DataContainer::mLowLimit;
    while ((time - delta) != time) {
        lessThenTime = time - delta;
        delta /= 2.;
    }

    QVector<double> xRet = {DataContainer::mLowLimit, lessThenTime, time, DataContainer::mHighLimit};
    QVector<double> yRet = {0., 0., 1., 1.};
    return DataContainer(xRet, yRet);

//    QVector<double> xRet;
//    QVector<double> yRet;
}

DataContainer heavis_t(const DataContainer &arg1)
{
    auto ret = heavis(arg1);
    for (auto &y : ret.mYData) {
        y = static_cast<double>(!(static_cast<int>(y)));
    }
    return ret;
}








DataContainer bartlett(const DataContainer &arg1)
{
    int sz = arg1.mXData.size();
    if (sz < 3)
        return DataContainer(arg1.mXData, arg1.mYData);

    double x_0 = arg1.mXData[0];
    double x_k = arg1.mXData.back();
    double dx = (x_k - x_0)/2;
    double x_m = (x_0 + x_k)/2;

    QVector<double> yVector(arg1.mYData.size());
    for (int i = 0; i < sz; ++i) {
        yVector[i] = arg1.mYData[i] * (1. - std::abs(arg1.mXData[i]-x_m)/dx);
    }
    return DataContainer(arg1.mXData, yVector);
}

DataContainer hamming(const DataContainer &arg1)
{
    const double alpha = 0.54;
    const double beta = 0.46;

    int sz = arg1.mXData.size();
    if (sz < 3)
        return DataContainer(arg1.mXData, arg1.mYData);

    double x_0 = arg1.mXData[0];
    double x_k = arg1.mXData.back();
    double dx = (x_k - x_0)/2;
    double x_m = (x_0 + x_k)/2;

    QVector<double> yVector(arg1.mYData.size());
    for (int i = 0; i < sz; ++i) {
        double c = std::cos(M_PI*(arg1.mXData[i]-x_m)/dx);
        yVector[i] = arg1.mYData[i] * (alpha + beta*c);
    }
    return DataContainer(arg1.mXData, yVector);
}

DataContainer hann(const DataContainer &arg1)
{
    int sz = arg1.mXData.size();
    if (sz < 3)
        return DataContainer(arg1.mXData, arg1.mYData);

    double x_0 = arg1.mXData[0];
    double x_k = arg1.mXData.back();
    double dx = (x_k - x_0)/2;
    double x_m = (x_0 + x_k)/2;

    QVector<double> yVector(arg1.mYData.size());
    for (int i = 0; i < sz; ++i) {
        double c = 0.5 *(1. + std::cos(M_PI*(arg1.mXData[i]-x_m)/dx));
        yVector[i] = arg1.mYData[i] * c * c;
    }
    return DataContainer(arg1.mXData, yVector);
}

DataContainer welch(const DataContainer &arg1)
{
    int sz = arg1.mXData.size();
    if (sz < 3)
        return DataContainer(arg1.mXData, arg1.mYData);

    double x_0 = arg1.mXData[0];
    double x_k = arg1.mXData.back();
    double dx = (x_k - x_0)/2;
    double dx2 = dx*dx;
    double x_m = (x_0 + x_k)/2;

    QVector<double> yVector(arg1.mYData.size());
    for (int i = 0; i < sz; ++i) {
        double ddx = arg1.mXData[i]-x_m;
        yVector[i] = arg1.mYData[i] * (1. - ddx*ddx/dx2);
    }
    return DataContainer(arg1.mXData, yVector);
}


/////////////////////////////////////////////////////////////////////////////////
DataContainer pow(const DataContainer &arg1, const DataContainer &arg2)
{
    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(std::pow(arg1.getValue(x), arg2.getValue(x)));
    }
    return DataContainer(xVector, yVector);
}


DataContainer atan2(const DataContainer &arg1, const DataContainer &arg2)
{
    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(std::atan2(arg1.getValue(x), arg2.getValue(x)));
    }
    return DataContainer(xVector, yVector);
}

DataContainer decimation(const DataContainer &arg1, const DataContainer &arg2)
{
    int sz = arg1.mXData.size();
    if (sz < 2 || arg2.mYData.size() == 0)
        return arg1;
    unsigned long n = std::max(static_cast<unsigned>(arg2.mYData[0]), 1U);
    int new_size = sz / n;

    QVector<double> xVector(new_size);
    QVector<double> yVector(new_size);
    for (int i = 0; i < new_size; ++i) {
        xVector[i] = arg1.mXData[i*n];
        yVector[i] = arg1.mYData[i*n];
    }
    return DataContainer(xVector, yVector);
}

DataContainer delay(const DataContainer &arg1, const DataContainer &arg2)
{
    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
    for (double &x : xVector) {
        yVector.push_back(arg1.getValue(x));
        x +=  arg2.getValue(x);
    }
    DataContainer ret(xVector, yVector);
    qSort(xVector);
    if (ret.mXData != xVector) {
        ret.mType = DataContainer::GraphType::Curve;
    }
    return ret;
}

DataContainer getBit(const DataContainer &arg1, const DataContainer &arg2)
{
    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(GET_BIT(static_cast<unsigned long>(arg1.getValue(x)), static_cast<unsigned>(arg2.getValue(x))));
    }
    return DataContainer(xVector, yVector);
}


DataContainer getNBits(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3)
{
    QVector<double> x1Vector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> xVector  = DataContainer::combineXVectors(x1Vector, arg3.mXData);

    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        if ((arg3.getValue(x) < 0.) || ((unsigned)arg3.getValue(x) == 0U)) {
            yVector.push_back(0.);
            continue;
        }
        yVector.push_back(GET_K_BITS(static_cast<unsigned long>(arg1.getValue(x)), static_cast<unsigned>(arg2.getValue(x)), static_cast<unsigned>(arg3.getValue(x))));
    }
    return DataContainer(xVector, yVector);
}



DataContainer clamp(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3)
{
    QVector<double> x1Vector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> xVector  = DataContainer::combineXVectors(x1Vector, arg3.mXData);

    QVector<double> yVector;
    yVector.reserve(xVector.size());
    auto clampLambda = [] (double x, double lo, double hi) { return (x < lo ? lo : ( x > hi ? hi : x)); };
    for (auto x : xVector) {
        yVector.push_back(clampLambda(arg1.getValue(x), arg2.getValue(x), arg3.getValue(x)));
    }
    return DataContainer(xVector, yVector);
}



DataContainer hypot(const DataContainer &arg1, const DataContainer &arg2)
{
    QVector<double> xVector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(std::hypot(arg1.getValue(x), arg2.getValue(x)));
    }
    return DataContainer(xVector, yVector);
}

DataContainer linInterp(const DataContainer &arg1, const DataContainer &arg2)
{
    int sz = arg1.mXData.size();
    if (sz < 2 || arg2.mYData.size() == 0)
        return arg1;
    unsigned long n = arg2.mYData[0];

    QVector<double> xData(sz + (sz - 1)*n);
    QVector<double> yData(sz + (sz - 1)*n);

    xData[0] = arg1.mXData[0];
    yData[0] = arg1.mYData[0];
    int k = 1;
    for (int i = 1; i < sz; ++i) {
        double dx = (arg1.mXData[i] - arg1.mXData[i-1])/(n+1);
        for (int j = 0; j < (int)n; ++j) {
            xData[k] = arg1.mXData[i-1] + (j+1)*dx;
            yData[k] = arg1.mYData[i-1] + (j+1)*dx*(arg1.mYData[i]-arg1.mYData[i-1])/(arg1.mXData[i]-arg1.mXData[i-1]);
            k++;
        }
        xData[k] = arg1.mXData[i];
        yData[k] = arg1.mYData[i];
        k++;
    }
    return DataContainer(xData, yData);

}

DataContainer time(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3)
{
    if (arg1.mYData.size() == 0 || arg2.mYData.size() == 0 || arg3.mYData.size() == 0)
        return DataContainer();
    double x_start = arg1.mYData[0], x_finish = arg2.mYData[0];
    int n = static_cast<int>(std::abs(arg3.mYData[0]));
    double dx = (x_finish - x_start) / (n - 1);
    QVector<double> xVector(n, 0);
    QVector<double> yVector(n, 0);
    for (int i = 0; i < n; ++i) {
        xVector[i] = yVector[i] = x_start + i * dx;
    }
    return DataContainer(xVector, yVector);
}

}



////////////////////////////////////////////////////////////////////
namespace {
DataContainer getDataContainerByIndex(int index) {
    if (index > global::gCurrGraphVector.size()) {
        throw std::exception();
    }

    DataContainer ret(global::gCurrGraphVector[index-1]->getXData(), global::gCurrGraphVector[index-1]->getYData());
    ret.lineInterpolation = global::gCurrGraphVector[index-1]->getLineInterpolation();

    //determine if we need to strip data
    const auto &dataLow  = ret.mXData.front();
    const auto &dataHigh = ret.mXData.back();
    //may be I should use another criteria ??
//    if ((dataHigh - dataLow) > (2.* (DataContainer::mHighLimit - DataContainer::mLowLimit))){
//        ret.stripData();
//    }
    //decided to strip always
    if (!(dataHigh <= DataContainer::mHighLimit  && dataLow >= DataContainer::mLowLimit)){
        ret.stripData();
    }

    return ret;
}
}


DataContainer pow(const DataContainer &arg1, const DataContainer &arg2) {

    return DataContainerNmspc::pow(arg1, arg2);
}

namespace DataContainerNmspc {

DataContainer g1 (const DataContainer &) { return getDataContainerByIndex(1) ; }
DataContainer g2 (const DataContainer &) { return getDataContainerByIndex(2) ; }
DataContainer g3 (const DataContainer &) { return getDataContainerByIndex(3) ; }
DataContainer g4 (const DataContainer &) { return getDataContainerByIndex(4) ; }
DataContainer g5 (const DataContainer &) { return getDataContainerByIndex(5) ; }
DataContainer g6 (const DataContainer &) { return getDataContainerByIndex(6) ; }
DataContainer g7 (const DataContainer &) { return getDataContainerByIndex(7) ; }
DataContainer g8 (const DataContainer &) { return getDataContainerByIndex(8) ; }
DataContainer g9 (const DataContainer &) { return getDataContainerByIndex(9) ; }
DataContainer g10(const DataContainer &) { return getDataContainerByIndex(10); }
DataContainer g11(const DataContainer &) { return getDataContainerByIndex(11); }
DataContainer g12(const DataContainer &) { return getDataContainerByIndex(12); }
DataContainer g13(const DataContainer &) { return getDataContainerByIndex(13); }
DataContainer g14(const DataContainer &) { return getDataContainerByIndex(14); }
DataContainer g15(const DataContainer &) { return getDataContainerByIndex(15); }
DataContainer g16(const DataContainer &) { return getDataContainerByIndex(16); }
DataContainer g17(const DataContainer &) { return getDataContainerByIndex(17); }
DataContainer g18(const DataContainer &) { return getDataContainerByIndex(18); }
DataContainer g19(const DataContainer &) { return getDataContainerByIndex(19); }
DataContainer g20(const DataContainer &) { return getDataContainerByIndex(20); }
DataContainer g21(const DataContainer &) { return getDataContainerByIndex(21); }
DataContainer g22(const DataContainer &) { return getDataContainerByIndex(22); }
DataContainer g23(const DataContainer &) { return getDataContainerByIndex(23); }
DataContainer g24(const DataContainer &) { return getDataContainerByIndex(24); }
DataContainer g25(const DataContainer &) { return getDataContainerByIndex(25); }
DataContainer g26(const DataContainer &) { return getDataContainerByIndex(26); }
DataContainer g27(const DataContainer &) { return getDataContainerByIndex(27); }
DataContainer g28(const DataContainer &) { return getDataContainerByIndex(28); }
DataContainer g29(const DataContainer &) { return getDataContainerByIndex(29); }
DataContainer g30(const DataContainer &) { return getDataContainerByIndex(30); }
DataContainer g31(const DataContainer &) { return getDataContainerByIndex(31); }
DataContainer g32(const DataContainer &) { return getDataContainerByIndex(32); }
DataContainer g33(const DataContainer &) { return getDataContainerByIndex(33); }
DataContainer g34(const DataContainer &) { return getDataContainerByIndex(34); }
DataContainer g35(const DataContainer &) { return getDataContainerByIndex(35); }
DataContainer g36(const DataContainer &) { return getDataContainerByIndex(36); }
DataContainer g37(const DataContainer &) { return getDataContainerByIndex(37); }
DataContainer g38(const DataContainer &) { return getDataContainerByIndex(38); }
DataContainer g39(const DataContainer &) { return getDataContainerByIndex(39); }
DataContainer g40(const DataContainer &) { return getDataContainerByIndex(40); }
DataContainer g41(const DataContainer &) { return getDataContainerByIndex(41); }
DataContainer g42(const DataContainer &) { return getDataContainerByIndex(42); }
DataContainer g43(const DataContainer &) { return getDataContainerByIndex(43); }
DataContainer g44(const DataContainer &) { return getDataContainerByIndex(44); }
DataContainer g45(const DataContainer &) { return getDataContainerByIndex(45); }
DataContainer g46(const DataContainer &) { return getDataContainerByIndex(46); }
DataContainer g47(const DataContainer &) { return getDataContainerByIndex(47); }
DataContainer g48(const DataContainer &) { return getDataContainerByIndex(48); }
DataContainer g49(const DataContainer &) { return getDataContainerByIndex(49); }
DataContainer g50(const DataContainer &) { return getDataContainerByIndex(50); }







DataContainer USAStAtm76_HToRho(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return usStandAtm76::usStandAtm76_HToRho(x);};
    return templUnaryFunc(lambda, arg1);
}


DataContainer USAStAtm76_HToA(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return usStandAtm76::usStandAtm76_HToA(x);};
    return templUnaryFunc(lambda, arg1);
}


DataContainer USAStAtm76_HToG(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return usStandAtm76::usStandAtm76_HToG(x);};
    return templUnaryFunc(lambda, arg1);
}


DataContainer USAStAtm76_HToT(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return usStandAtm76::usStandAtm76_HToT(x);};
    return templUnaryFunc(lambda, arg1);
}


DataContainer USAStAtm76_HToP(const DataContainer &arg1)
{
    auto lambda = [] (double x) { return usStandAtm76::usStandAtm76_HToP(x);};
    return templUnaryFunc(lambda, arg1);
}



DataContainer geo_cc2fi(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3, const DataContainer &arg4)
{

    QVector<double> x1Vector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> x2Vector = DataContainer::combineXVectors(arg3.mXData, x1Vector);
    QVector<double> xVector = DataContainer::combineXVectors(arg4.mXData, x2Vector);

    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(geo_conv::geo_cc2fi(arg1.getValue(x), arg2.getValue(x), arg3.getValue(x), arg4.getValue(x)));
    }
    return DataContainer(xVector, yVector);

}





DataContainer geo_cc2lam(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3, const DataContainer &arg4)
{
    QVector<double> x1Vector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> x2Vector = DataContainer::combineXVectors(arg3.mXData, x1Vector);
    QVector<double> xVector = DataContainer::combineXVectors(arg4.mXData, x2Vector);

    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(geo_conv::geo_cc2lam(arg1.getValue(x), arg2.getValue(x), arg3.getValue(x), arg4.getValue(x)));
    }
    return DataContainer(xVector, yVector);
}


DataContainer geo_cc2h(const DataContainer &arg1, const DataContainer &arg2, const DataContainer &arg3, const DataContainer &arg4)
{
    QVector<double> x1Vector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);
    QVector<double> x2Vector = DataContainer::combineXVectors(arg3.mXData, x1Vector);
    QVector<double> xVector = DataContainer::combineXVectors(arg4.mXData, x2Vector);

    QVector<double> yVector;
    yVector.reserve(xVector.size());
    foreach (double x, xVector) {
        yVector.push_back(geo_conv::geo_cc2h(arg1.getValue(x), arg2.getValue(x), arg3.getValue(x), arg4.getValue(x)));
    }
    return DataContainer(xVector, yVector);
}


DataContainer paramCurve(const DataContainer &arg1, const DataContainer &arg2)
{
    QVector<double> x1Vector = DataContainer::combineXVectors(arg1.mXData, arg2.mXData);


    QVector<double> xVector;
    QVector<double> yVector;
    xVector.reserve(x1Vector.size());
    yVector.reserve(x1Vector.size());
    double x_pred = -DBL_MAX;
    foreach (double x, x1Vector) {
        double newX = arg2.getValue(x);
        if (newX >= x_pred) {
            xVector.push_back(newX);
            yVector.push_back(arg1.getValue(x));
        }
    }
    DataContainer ret(xVector, yVector);
    ret.mType = DataContainer::GraphType::Curve;
    return ret;
}


}

