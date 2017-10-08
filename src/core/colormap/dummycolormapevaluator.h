#ifndef DUMMYCOLORMAPEVALUATOR_H
#define DUMMYCOLORMAPEVALUATOR_H

#include <QObject>
#include "colormapevaluator.h"

class DummyColorMapEvaluator : public ColorMapEvaluator
{
public:
    explicit DummyColorMapEvaluator(QObject *parent = 0);

    virtual QColor getColorForPoint(double x, double y);
};

#endif // DUMMYCOLORMAPEVALUATOR_H
