#include "akima.h"
#include "../utility/akima/GteIntpAkimaNonuniform1.h"

std::tuple<QVector<double>, QVector<double>> akima_interp(
        const QVector<double> &xData,
        const QVector<double> &yData,
        const int n_pts)
{
    using namespace gte;
    IntpAkimaNonuniform1<double>  interpolator(xData.size() , xData.data(), yData.data());
    QVector<double> newX;
    QVector<double> newY;

    const int data_size_1 = xData.size() - 1;
    for (int i = 0; i < data_size_1; ++i) {
        const double dx = (xData[i + 1] - xData[i]) / n_pts;
        for (int j = 0; j < n_pts; ++j) {
            newX.push_back(xData[i] + dx * j);
        }
    }
    newX.push_back(xData.back());
    for (int i = 0; i < newX.size(); ++i) {
        newY.push_back(interpolator(newX[i]));
    }
    return std::make_tuple(newX, newY);
}
