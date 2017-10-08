#ifndef MY_FFT_API_H
#define MY_FFT_API_H

#include <tuple>
#include <vector>
#include <QVector>

namespace jag_fft {

std::tuple<QVector<double>, QVector<double>>
qCountFreqAmplSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector);

std::tuple<QVector<double>, QVector<double>>
qCountFreqRealSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector);

std::tuple<QVector<double>, QVector<double>>
qCountFreqImageSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector);

}
#endif // MY_FFT_API_H
