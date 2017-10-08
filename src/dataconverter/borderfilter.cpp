#include "borderfilter.h"
#include "basic/basic.h"
#include <cmath>
BorderFilter::BorderFilter(double xLow, double xHigh, double yLow, double yHigh):
    mXLowBorder(xLow), mXHighBorder(xHigh), mYLowBorder(yLow), mYHighBorder(yHigh),
    mXFilteringType(FilteringType::BandPass), mYFilteringType(FilteringType::BandPass),
    mEliminatingLogic(EliminatingLogic::Cutting)
{

}

void BorderFilter::setXFilteringType(BorderFilter::FilteringType fType)
{
    mXFilteringType = fType;
}

void BorderFilter::setYFilteringType(BorderFilter::FilteringType fType)
{
    mYFilteringType = fType;
}

BorderFilter::FilteringType BorderFilter::xFilteringType() const
{
    return mXFilteringType;
}

BorderFilter::FilteringType BorderFilter::yFilteringType() const
{
    return mYFilteringType;
}

BorderFilter::EliminatingLogic BorderFilter::eliminatingLogic() const
{
    return mEliminatingLogic;
}

double BorderFilter::xLowBorder() const
{
    return mXLowBorder;
}

double BorderFilter::xHighBorder() const
{
    return mXHighBorder;
}

double BorderFilter::yLowBorder() const
{
    return mYLowBorder;
}

double BorderFilter::yHighBorder() const
{
    return mYHighBorder;
}

void BorderFilter::setXLowBorder(double arg)
{
    if (mXLowBorder != arg) {
        mXLowBorder = arg;
    }
}

void BorderFilter::setXHighBorder(double arg)
{
    if (mXHighBorder != arg) {
        mXHighBorder = arg;
    }
}

void BorderFilter::setYLowBorder(double arg)
{
    if (mYLowBorder != arg) {
        mYLowBorder = arg;
    }
}

void BorderFilter::setYHighBorder(double arg)
{
    if (mYHighBorder != arg) {
        mYHighBorder = arg;
    }
}

void BorderFilter::setEliminatingLogic(BorderFilter::EliminatingLogic logic)
{
    if (mEliminatingLogic != logic) {
        mEliminatingLogic = logic;
    }
}

std::tuple<QVector<double>, QVector<double> > BorderFilter::operator()(const QVector<double> &x, const QVector<double> &y)
{
    QVector<double> xRet;
    QVector<double> yRet;
    if (x.size() != y.size())
        errorExit("Unexpected situation in %s", __FUNCTION__);
    for (int i = 0; i < x.size(); ++i) {
        if (isXValueValid(x[i]) && isYValueValid(y[i])) {
            xRet.push_back(x[i]);
            yRet.push_back(y[i]);
        } else if (mEliminatingLogic == EliminatingLogic::ToNaN) {
            xRet.push_back(x[i]);
            yRet.push_back(NAN);
        }
    }
    return std::tuple<QVector<double>, QVector<double> > (xRet, yRet);

}

bool BorderFilter::isXValueValid(double x) const
{
    if (x >= mXLowBorder && x <= mXHighBorder) {
        return mXFilteringType == FilteringType::BandPass ? true : false;
    }
    return mXFilteringType == FilteringType::BandPass ? false : true;
}

bool BorderFilter::isYValueValid(double y) const
{
    if (y >= mYLowBorder && y <= mYHighBorder) {
        return mYFilteringType == FilteringType::BandPass ? true : false;
    }
    return mYFilteringType == FilteringType::BandPass ? false : true;
}

