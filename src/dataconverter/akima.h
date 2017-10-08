#ifndef AKIMAFILTER_H
#define AKIMAFILTER_H

#include <QVector>
#include <tuple>

std::tuple<QVector<double>, QVector<double>> akima_interp(
        const QVector<double> &xData,
        const QVector<double> &yData,
        const int n_pts = 2);


#endif // AKIMAFILTER_H



