#include "dummycolormapevaluator.h"
#include <cmath>

DummyColorMapEvaluator::DummyColorMapEvaluator(QObject *parent)
    :ColorMapEvaluator(parent)
{

}

QColor DummyColorMapEvaluator::getColorForPoint(double x, double y)
{
//    if (y > 0.5)
//        return QColor(128., 128., 128.);
//    else if (y < 0)
//        return Qt::black;
//    else
//        return QColor(256.0*y, 256.0*y, 256.0*y);

        y = std::abs(y);
//        if (y > 1.)
//            return QColor::fromHsvF ( 1., 1., 1.);
//        else if (y < 0)
//            return QColor::fromHsvF ( 0., 1., 1.);
//        else
            return QColor::fromHsvF ( y, x/30, 1.,  x/30);

}

