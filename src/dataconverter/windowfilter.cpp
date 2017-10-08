#include "windowfilter.h"
#include <QDebug>

namespace {
double meanValue(const QVector<double> &x, int begin, int end);
}

WindowFilter::WindowFilter()
    :mBeginTime(std::numeric_limits<double>::lowest()), mEndTime(std::numeric_limits<double>::max()), mWindowWidth(1)
{

}

int WindowFilter::windowWidth() const
{
    return mWindowWidth;
}

std::tuple<QVector<double>, QVector<double> > WindowFilter::operator()(const QVector<double> &x, const QVector<double> &y)
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
        retY.push_back(meanValue(y, i - leftWidth, i + rightWidth + 1));
    }
    return std::tuple<QVector<double>, QVector<double> >(retX, retY);
}

void WindowFilter::setWindowWidth(int width)
{
    if (width <= 0) {
        qWarning() << "Invalid window width for window filter was detected";
        return;
    }
    if (mWindowWidth != width) {
        mWindowWidth = width;
    }
}



void WindowFilter::setBeginTime(double time)
{
    if (mBeginTime != time) {
        mBeginTime = time;
    }
}

void WindowFilter::setEndTime(double time)
{
    if (mEndTime != time) {
        mEndTime = time;
    }

}



namespace {
double meanValue(const QVector<double> &x, int begin, int end)
{
    if (begin < 0) {
        begin = 0;
    }
    if (end > x.size()) {
        end = x.size();
    }
    double sum = 0;
    for (int i = begin; i < end; ++i)
        sum += x[i];
    return sum / (end - begin);
}
}
