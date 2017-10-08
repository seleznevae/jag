#include "identityconverter.h"

IdentityConverter::IdentityConverter()
{
}

std::tuple<QVector<double>, QVector<double> > IdentityConverter::operator()(const QVector<double> &x, const QVector<double> &y)
{
    return std::tuple<QVector<double>, QVector<double>>(x, y);
}
