#include "affineconverter.h"


AffineConverter::AffineConverter(double x0, double kX, double dX,
                                 double y0, double kY, double dY):
    mX0(x0), mKX(kX), mDX(dX),
    mY0(y0), mKY(kY), mDY(dY)
{

}

std::tuple<QVector<double>, QVector<double> >
AffineConverter::operator()(const QVector<double> &x, const QVector<double> &y)
{
    QVector<double> retX(x.size());
    QVector<double> retY(y.size());
    if (mX0 == 0. && mKX == 1.0 && mDX == 0.) {
        retX = x;
    } else {
        for (int i = 0; i < x.size(); ++i)
            retX[i] = (mX0 + mKX * (x[i] + mDX));
    }
    if (mY0 == 0. && mKY == 1.0 && mDY == 0.) {
        retY = y;
    } else {
        for (int i = 0; i < y.size(); ++i)
            retY[i] = (mY0 + mKY * (y[i] + mDY));
    }

    return std::tuple<QVector<double>, QVector<double> > (retX, retY);
}

void AffineConverter::setX0(double newValue)
{
    if (mX0 != newValue) {
        mX0 = newValue;
    }
}

void AffineConverter::setKX(double newValue)
{
    if (mKX != newValue) {
        mKX = newValue;
    }
}

void AffineConverter::setDX(double newValue)
{
    if (mDX != newValue) {
        mDX = newValue;
    }
}

void AffineConverter::setY0(double newValue)
{
    if (mY0 != newValue) {
        mY0 = newValue;
    }
}

void AffineConverter::setKY(double newValue)
{
    if (mKY != newValue) {
        mKY = newValue;
    }
}

void AffineConverter::setDY(double newValue)
{
    if (mDY != newValue) {
        mDY = newValue;
    }
}


//double AffineConverter::a0X() const
//{
//    return mA0x;
//}

//double AffineConverter::a1X() const
//{
//    return mA1x;
//}

//double AffineConverter::b0Y() const
//{
//    return mB0y;
//}

//double AffineConverter::b1Y() const
//{
//    return mB1y;
//}

//void AffineConverter::setA0X(double newValue)
//{
//    if (mA0x != newValue) {
//        mA0x = newValue;
//    }
//}

//void AffineConverter::setA1X(double newValue)
//{
//    if (mA1x != newValue) {
//        mA1x = newValue;
//    }
//}

//void AffineConverter::setB0Y(double newValue)
//{
//    if (mB0y != newValue) {
//        mB0y = newValue;
//    }
//}

//void AffineConverter::setB1Y(double newValue)
//{
//    if (mB1y != newValue) {
//        mB1y = newValue;
//    }
//}

bool AffineConverter::operator==(const AffineConverter &arg) const
{
//    return (mA0x == arg.mA0x
//            && mA1x == arg.mA1x
//            && mB0y == arg.mB0y
//            && mB1y == arg.mB1y);

    return (mX0 == arg.mX0
            && mKX == arg.mKX
            && mDX == arg.mDX
            && mY0 == arg.mY0
            && mKY == arg.mKY
            && mDY == arg.mDY);

}
