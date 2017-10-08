#ifndef _BASIC_H
#define _BASIC_H

#define GET_BIT(arg,N) (((static_cast<unsigned long>(arg))>>(unsigned(N))) & 0x1UL)

#define GET_K_BITS(arg,N,K) (((static_cast<unsigned long>(arg))>>(unsigned(N))) &    (~((~0x0UL)<<(unsigned(K)))))

#include <QString>
#include <QVector>
#include <stdexcept>
#include "core/graphmodel.h"

void sendLogMessage(const QString& logMessage);
void doErrorExit(const char*, const char *);


template <typename T>
T findMinInQVector(const QVector<T>& vector)
{
    auto stdVector = vector.toStdVector();
    return *(std::min_element(stdVector.begin(), stdVector.end()));
}

template <>
double findMinInQVector(const QVector<double>& vector);


template <typename T>
T findMinInQVectorInRange(const QVector<T>& vector, const QVector<T>& rangeVector, T rangeBegin, T rangeEnd)
{
    //rangeVector must be sorted
    auto stdVector = vector.toStdVector();
    auto stdRangeVector = rangeVector.toStdVector();
    auto lowerBound = lower_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeBegin);
    size_t low_index = lowerBound - stdRangeVector.begin();
    auto upperBound = upper_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeEnd);
    size_t high_index = upperBound - stdRangeVector.begin();
    if (low_index >= high_index) {
        throw std::exception("Not implemented case in findMinInQVectorInRange");
    }
    return *(std::min_element(stdVector.begin()+low_index, stdVector.begin()+high_index));
}

template <>
double findMinInQVectorInRange(const QVector<double>& vector, const QVector<double>& rangeVector, double rangeBegin, double rangeEnd);


template <typename T>
T findMaxInQVectorInRange(const QVector<T>& vector, const QVector<T>& rangeVector, T rangeBegin, T rangeEnd)
{
    //rangeVector must be sorted
    auto stdVector = vector.toStdVector();
    auto stdRangeVector = rangeVector.toStdVector();
    auto lowerBound = lower_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeBegin);
    size_t low_index = lowerBound - stdRangeVector.begin();
    auto upperBound = upper_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeEnd);
    size_t high_index = upperBound - stdRangeVector.begin();
    if (low_index >= high_index) {
        throw std::exception("Not implemented case in findMaxInQVectorInRange");
    }
    return *(std::max_element(stdVector.begin()+low_index, stdVector.begin()+high_index));
}

template <>
double findMaxInQVectorInRange(const QVector<double>& vector, const QVector<double>& rangeVector, double rangeBegin, double rangeEnd);


template <typename T>
T findMaxInQVector(const QVector<T>& vector)
{
    auto stdVector = vector.toStdVector();
    return *(std::max_element(stdVector.begin(), stdVector.end()));
}

template <>
double findMaxInQVector(const QVector<double>& vector);


bool lockUniqueResource();
bool unlockUniqueResource();


QString stripDirectoryFromFileName(const QString& fileName);
QString createIndexedFileName(const QString& fileName, int index);

QString valueToString(double value, GraphModel::StringRepresentation format);

#endif // BASIC_H
