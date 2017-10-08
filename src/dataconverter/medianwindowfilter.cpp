#include "medianwindowfilter.h"
#include <QtAlgorithms>
#include <QDebug>

namespace {
double medianValue(const QVector<double> &x, int begin, int end);
}


MedianWindowFilter::MedianWindowFilter()
    :mBeginTime(std::numeric_limits<double>::lowest()), mEndTime(std::numeric_limits<double>::max()), mWindowWidth(1)
{
}






int MedianWindowFilter::windowWidth() const
{
    return mWindowWidth;
}

std::tuple<QVector<double>, QVector<double> > MedianWindowFilter::operator()(const QVector<double> &x, const QVector<double> &y)
{
    QVector<double> retX = x;
    QVector<double> retY;
    int leftWidth = (mWindowWidth - 1) / 2;
    int rightWidth = mWindowWidth - leftWidth - 1;
    for (int i = 0; i < y.size(); ++i) {
        if (x[i] < mBeginTime || x[i] > mEndTime) {
            retY.push_back(y[i]);
            continue;
        }
        retY.push_back(medianValue(y, i - leftWidth, i + rightWidth + 1));
    }
    return std::tuple<QVector<double>, QVector<double> >(retX, retY);
}

void MedianWindowFilter::setWindowWidth(int width)
{
    if (width <= 0) {
        qDebug() << "invalid window width for window filter";
        return;
    }
    if (mWindowWidth != width) {
        mWindowWidth = width;
    }
}


void MedianWindowFilter::setBeginTime(double time)
{
    if (mBeginTime != time) {
        mBeginTime = time;
    }
}

void MedianWindowFilter::setEndTime(double time)
{
    if (mEndTime != time) {
        mEndTime = time;
    }

}

namespace {
double medianValue(const QVector<double> &x, int begin, int end)
{
    if (begin < 0) {
        begin = 0;
    }
    if (end > x.size()) {
        end = x.size();
    }
    QVector<double> dataVector = x.mid(begin, end - begin);
    qSort(dataVector);
    return dataVector.size() % 2 == 0 ? dataVector[(dataVector.size() / 2) - 1]
                                      : dataVector[(dataVector.size() / 2)];

}
}
