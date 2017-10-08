#include "secondaryprocessing/fft/my_fft_api.h"
#include <algorithm>
#include <QDebug>
#include "basic/basic.h"
#include <complex>
#include "secondaryprocessing/fft/ffft/FFTReal.h"



namespace {
QVector<double> makeUniformGrid(const QVector<double>&);
double interp1(const QVector<double> &xData, const QVector<double> &yData, double x);
void makeUniformData(const QVector<double> &xVector, const QVector<double> &yVector,
                     QVector<double> &uniformXVector, QVector<double> &uniformYVector);
void expandToPowerOf2(QVector<double> &vector);
}



std::tuple<QVector<double>, QVector<double>, QVector<double>>
qCountFreqSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector)
{
//    if (timeVector.size() != dataVector.size())
//       errorExit("Unexpected situation in %s\n", __FUNCTION__);
//    QVector<double> uniformTimeVector;
//    uniformTimeVector.reserve(timeVector.size());
//    QVector<double> uniformDataVector;
//    uniformDataVector.reserve(timeVector.size());
//    makeUniformData(timeVector, dataVector, uniformTimeVector, uniformDataVector);
//    expandToPowerOf2(uniformDataVector);

//    std::vector<double> complexSpectr(uniformDataVector.size(), 0);

//    ffft::FFTReal<double> fftReal(uniformDataVector.size());
//    fftReal.do_fft(complexSpectr.data(), uniformDataVector.data());


//    int spectrSize = complexSpectr.size();

////    f [0]          : Real (bin 0)
////    f [...]        : Real (bin ...)
////    f [length/2]   : Real (bin length/2)
////    f [length/2+1] : Imag (bin 1)
////    f [...]        : Imag (bin ...)
////    f [length-1]   : Imag (bin length/2-1)

//// And FFT bins are distributed in f [] as above:

////    Bin         | Real part      | Imaginary part
////    ------------+----------------+----------------
////    0           | f [0]          | 0
////    1           | f [1]          | f [length/2+1]
////    ...         | f [...],       | f [...]
////    length/2-1  | f [length/2-1] | f [length-1]
////    length/2    | f [length/2]   | 0
////    length/2+1  | f [length/2-1] | - f [length-1]
////    ...         | f [...]        | - f [...]
////    length-1    | f [1]          | - f [length/2+1]

//    QVector<double> realSpectr;
//    QVector<double> freqVector;
//    realSpectr.reserve(spectrSize);
//    freqVector.reserve(spectrSize);
//    for (int i = 0; i < spectrSize; ++i) {
//        if (i == 0 || i == spectrSize/2) {
//            realSpectr.push_back(complexSpectr[i]);
//        } else if (i < spectrSize/2) {
//            realSpectr.push_back(std::sqrt(complexSpectr[i] * complexSpectr[i]
//                                           + complexSpectr[spectrSize/2 + i] * complexSpectr[spectrSize/2 + i]));
//        } else {
//            realSpectr.push_back(realSpectr[spectrSize - i]);
//        }
//    }

//    double step = (1./(uniformTimeVector[1] - uniformTimeVector[0])) / (realSpectr.size() - 1);
//    for (size_t i = 0; i < complexSpectr.size(); ++i) {
//         freqVector.push_back(step * i);
//    }

//    return std::tuple<QVector<double>, QVector<double>>(freqVector, realSpectr);



    if (timeVector.size() != dataVector.size())
       errorExit("Unexpected situation in %s\n", __FUNCTION__);
    QVector<double> uniformTimeVector;
    uniformTimeVector.reserve(timeVector.size());
    QVector<double> uniformDataVector;
    uniformDataVector.reserve(timeVector.size());
    makeUniformData(timeVector, dataVector, uniformTimeVector, uniformDataVector);
    expandToPowerOf2(uniformDataVector);

    std::vector<double> complexSpectr(uniformDataVector.size(), 0);

    ffft::FFTReal<double> fftReal(uniformDataVector.size());
    fftReal.do_fft(complexSpectr.data(), uniformDataVector.data());


    int spectrSize = complexSpectr.size();

//    f [0]          : Real (bin 0)
//    f [...]        : Real (bin ...)
//    f [length/2]   : Real (bin length/2)
//    f [length/2+1] : Imag (bin 1)
//    f [...]        : Imag (bin ...)
//    f [length-1]   : Imag (bin length/2-1)

// And FFT bins are distributed in f [] as above:

//    Bin         | Real part      | Imaginary part
//    ------------+----------------+----------------
//    0           | f [0]          | 0
//    1           | f [1]          | f [length/2+1]
//    ...         | f [...],       | f [...]
//    length/2-1  | f [length/2-1] | f [length-1]
//    length/2    | f [length/2]   | 0
//    length/2+1  | f [length/2-1] | - f [length-1]
//    ...         | f [...]        | - f [...]
//    length-1    | f [1]          | - f [length/2+1]

    QVector<double> realSpectr;
    QVector<double> imageSpectr;
    QVector<double> freqVector;
    realSpectr.reserve(spectrSize);
    imageSpectr.reserve(spectrSize);
    freqVector.reserve(spectrSize);
    for (int i = 0; i < spectrSize; ++i) {
        if (i == 0 || i == spectrSize/2) {
            realSpectr.push_back(complexSpectr[i]);
            imageSpectr.push_back(0.);
        } else if (i < spectrSize/2) {
            realSpectr.push_back(complexSpectr[i]);
            imageSpectr.push_back(complexSpectr[spectrSize/2 + i]);
        } else {
            realSpectr.push_back(complexSpectr[spectrSize - i]);
            imageSpectr.push_back(-complexSpectr[3*spectrSize/2 - i]);
        }
    }

    double step = (1./(uniformTimeVector[1] - uniformTimeVector[0])) / (realSpectr.size() - 1);
    for (size_t i = 0; i < complexSpectr.size(); ++i) {
         freqVector.push_back(step * i);
    }
    return std::tuple<QVector<double>, QVector<double>, QVector<double>>(freqVector, realSpectr, imageSpectr);
}



std::tuple<QVector<double>, QVector<double>>
jag_fft::qCountFreqAmplSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector)
{

    auto spectr = qCountFreqSpectr(timeVector, dataVector);
    int length = std::get<1>(spectr).size();
    QVector<double> ampl(length);
    for (int i = 0; i < length; ++i) {
        double real = std::get<1>(spectr)[i];
        double image = std::get<2>(spectr)[i];
        ampl[i] = std::sqrt(real*real + image*image);
    }
    return std::tuple<QVector<double>, QVector<double>>(std::get<0>(spectr), ampl);

}


std::tuple<QVector<double>, QVector<double>>
jag_fft::qCountFreqRealSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector)
{
    auto spectr = qCountFreqSpectr(timeVector, dataVector);
    return std::tuple<QVector<double>, QVector<double>>(std::get<0>(spectr), std::get<1>(spectr));
}

std::tuple<QVector<double>, QVector<double>>
jag_fft::qCountFreqImageSpectr(const QVector<double> &timeVector, const QVector<double> &dataVector)
{
    auto spectr = qCountFreqSpectr(timeVector, dataVector);
    return std::tuple<QVector<double>, QVector<double>>(std::get<0>(spectr), std::get<2>(spectr));
}




namespace {

void expandToPowerOf2(QVector<double> &vector)
{
    int size = vector.size();
    int new_size = 1;
    while (new_size < size) {
        new_size *= 2;
    }
    if (new_size > size) {
        vector.resize(new_size);  //NOTE: Qt documentation says that the new elements are initialized with a default-constructed value.
    }
}





QVector<double> makeUniformGrid(const QVector<double>&gridVector)
{
    if (gridVector.size() == 1)
         return gridVector;
    QVector<double> deltaVector;
    deltaVector.reserve(gridVector.size() - 1);
    for (int i = 1; i < gridVector.size(); ++i)
        deltaVector.push_back(gridVector[i] - gridVector[i-1]);
    double meanDelta = std::accumulate(deltaVector.begin(), deltaVector.end(), 0.) / deltaVector.size();
    double minDelta = *std::min_element(deltaVector.begin(), deltaVector.end());
    double uniformDelta = 0.;
    if (minDelta >= meanDelta*0.4)
        uniformDelta = minDelta;
    else
        uniformDelta = meanDelta;

    double x = gridVector[0];
    double lastElement = gridVector[gridVector.size() - 1];
    QVector<double> uniformGridVector;
    uniformGridVector.reserve(size_t((lastElement - x) / uniformDelta));
    while (x <=  lastElement) {
        uniformGridVector.push_back(x);
        x += uniformDelta;
    }
    return uniformGridVector;
}



double interp1(const QVector<double> &xData, const QVector<double> &yData, double x)
{
    if (x < xData[0] || x > xData[xData.size() - 1]) {
        return NAN;  // may be margin value will be better
    } else {
        auto it = std::find_if(xData.begin(), xData.end(), [x](double elem) {return elem >= x;});
        int right = it - xData.begin();
        int left = right != 0 ? right - 1 : right;
        double div;
        if (left == right)
            div = 0;
        else
            div = (x - xData[left]) / (xData[right] - xData[left]);
        return yData[right] * div + yData[left] * (1. - div);
    }
}




void makeUniformData(const QVector<double> &xVector, const QVector<double> &yVector,
                     QVector<double> &uniformXVector, QVector<double> &uniformYVector)
{
    uniformXVector.clear();
    uniformYVector.clear();
    uniformXVector = makeUniformGrid(xVector);
    for (auto x : uniformXVector) {
        uniformYVector.push_back(interp1(xVector, yVector, x));
    }
}

}



