#include "clusterfilter.h"
#include <QDebug>
#include <cmath>

ClusterFilter::ClusterFilter():
    mBeginTime(std::numeric_limits<double>::lowest()), mEndTime(std::numeric_limits<double>::max()),
    mWindowWidth(5), mNumberOfConsecErrors(1), mRelativeThreshold(10.), mPolicy(Policy::CutOff)
{
}

ClusterFilter::~ClusterFilter()
{

}

std::shared_ptr<DataConverter> ClusterFilter::clone() const
{
    auto ret = std::make_shared <ClusterFilter>();
    ret->setBeginTime(mBeginTime);
    ret->setEndTime(mEndTime);
    return ret;
}

std::tuple<QVector<double>, QVector<double> > ClusterFilter::operator()(const QVector<double> &xData, const QVector<double> &yData)
{
//    QVector<double> xReturn;
//    xReturn.reserve(xData.size());
//    QVector<double> yReturn;
//    xReturn.reserve(yData.size());

//    const int sideWidth = (mWindowWidth - 1) / 2;
//    int dataSize = xData.size();
//    for (int i = 0; i < dataSize; ++i) {
//        if ((i - sideWidth < 0) || (i + sideWidth >= dataSize)
//                || (xData[i] < mBeginTime) || (xData[i] > mEndTime)) {
//            xReturn.push_back(xData[i]);
//            yReturn.push_back(yData[i]);
//            continue;
//        }

//        std::vector<double> dataInWindow(yData.begin() + i - sideWidth, yData.begin() + i + sideWidth + 1);
//        std::sort(dataInWindow.begin(), dataInWindow.end());
//        const double &data = yData[i];
//        if (data == dataInWindow.back()) {
//            dataInWindow.erase(dataInWindow.end() - 1);
//        } else if (data == dataInWindow.front()) {
//            dataInWindow.erase(dataInWindow.begin());
//        } else {
//            xReturn.push_back(xData[i]);
//            yReturn.push_back(yData[i]);
//            continue;
//        }
//        double clusterCenter = (dataInWindow.back() + dataInWindow.front()) / 2.;
//        double clusterRange = (dataInWindow.back() - dataInWindow.front()) / 2.;
//        //todo: think about another condition
//        if (std::abs(data - clusterCenter) <= mRelativeThreshold*clusterRange) {
//            xReturn.push_back(xData[i]);
//            yReturn.push_back(yData[i]);
//            continue;
//        }
//    }
//    return std::tuple<QVector<double>, QVector<double> >(xReturn, yReturn);




    QVector<double> xReturn;
    xReturn.reserve(xData.size());
    QVector<double> yReturn;
    xReturn.reserve(yData.size());

    const int sideWidth = (mWindowWidth - 1) / 2;
    int dataSize = xData.size();
    for (int i = 0; i < dataSize; ++i) {
        if ((i - sideWidth < 0) || (i + sideWidth >= dataSize)
                || (xData[i] < mBeginTime) || (xData[i] > mEndTime)) {
            xReturn.push_back(xData[i]);
            yReturn.push_back(yData[i]);
            continue;
        }

        std::vector<double> dataInWindow(yData.begin() + i - sideWidth, yData.begin() + i + sideWidth + 1);
        std::sort(dataInWindow.begin(), dataInWindow.end());
        std::vector<double> lowGroup(dataInWindow.begin(), dataInWindow.begin() + mNumberOfConsecErrors);
        std::vector<double> highGroup(dataInWindow.end() - mNumberOfConsecErrors, dataInWindow.end());
        const double &data = yData[i];
        if (std::find(lowGroup.begin(), lowGroup.end(), data) != lowGroup.end()) {
            dataInWindow.erase(dataInWindow.begin(), dataInWindow.begin() + mNumberOfConsecErrors);
        } else if (std::find(highGroup.begin(), highGroup.end(), data) != highGroup.end()) {
            dataInWindow.erase(dataInWindow.end() - mNumberOfConsecErrors, dataInWindow.end());
        } else {
            xReturn.push_back(xData[i]);
            yReturn.push_back(yData[i]);
            continue;
        }
        double clusterCenter = (dataInWindow.back() + dataInWindow.front()) / 2.;
        double clusterRange = (dataInWindow.back() - dataInWindow.front()) / 2.;
        //todo: think about another condition
        if (std::abs(data - clusterCenter) <= mRelativeThreshold*clusterRange) {
            xReturn.push_back(xData[i]);
            yReturn.push_back(yData[i]);
            continue;
        }

        switch (mPolicy) {
            case Policy::Replace:
                 xReturn.push_back(xData[i]);
                 yReturn.push_back(std::accumulate(dataInWindow.begin(), dataInWindow.end(), 0.)
                                   / dataInWindow.size());
                 break;
            case Policy::CutOff:
                //nothing
                 break;
        }
    }
    return std::tuple<QVector<double>, QVector<double> >(xReturn, yReturn);
}

void ClusterFilter::setPolicy(ClusterFilter::Policy policy)
{
    if (mPolicy != policy) {
        mPolicy = policy;
    }

}

void ClusterFilter::setNumberOfConsecErrors(int number)
{
    if (number < 1)
        number = 1;
    if (number > (mWindowWidth / 3))
        number = mWindowWidth / 3;
    if (mNumberOfConsecErrors != number) {
        mNumberOfConsecErrors = number;
    }
}

void ClusterFilter::setRelativeThreshold(double threshold)
{
    if (threshold < 3)
        threshold = 3;
    if (mRelativeThreshold != threshold) {
        mRelativeThreshold = threshold;
    }
}

void ClusterFilter::setWindowWidth(int width)
{
    if (width < 3)
        width = 3;
    if (mWindowWidth != width) {
        mWindowWidth = width;
        if (mNumberOfConsecErrors > mWindowWidth / 3) {
            mNumberOfConsecErrors = mWindowWidth / 3;
        }
    }

}

void ClusterFilter::setBeginTime(double time)
{
    if (mBeginTime != time) {
        mBeginTime = time;
    }
}

void ClusterFilter::setEndTime(double time)
{
    if (mEndTime != time) {
        mEndTime = time;
    }

}

