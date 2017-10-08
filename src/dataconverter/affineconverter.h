#ifndef AFFINECONVERTER_H
#define AFFINECONVERTER_H

#include <dataconverter/dataconverter.h>

class AffineConverter : public DataConverter
{
public:
    AffineConverter(double x0 = 0., double kX = 1., double dX = 0.,
                    double y0 = 0., double kY = 1., double dY = 0.);



    virtual
    std::shared_ptr<DataConverter>
    clone() const { return std::make_shared<AffineConverter>(mX0, mKX, mDX, mY0, mKY, mDY); }

    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &);



    double x0() const { return mX0; }
    double kX() const { return mKX; }
    double dX() const { return mDX; }
    double y0() const { return mY0; }
    double kY() const { return mKY; }
    double dY() const { return mDY; }

//    double a0X() const;
//    double a1X() const;
//    double b0Y() const;
//    double b1Y() const;


    void setX0(double);
    void setKX(double);
    void setDX(double);
    void setY0(double);
    void setKY(double);
    void setDY(double);

//    void setA0X(double);
//    void setA1X(double);
//    void setB0Y(double);
//    void setB1Y(double);

    bool operator==(const AffineConverter &arg) const;
    bool operator!=(const AffineConverter &arg) const { return !(*this == arg);}

private:
    double mX0;
    double mKX;
    double mDX;
    double mY0;
    double mKY;
    double mDY;

//    double mA0x;
//    double mA1x;
//    double mB0y;
//    double mB1y;

};

#endif // AFFINECONVERTER_H
