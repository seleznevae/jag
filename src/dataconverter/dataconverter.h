#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <tuple>
#include <QVector>
#include <memory>

class DataConverter
{
public:
    DataConverter();

    virtual
    ~DataConverter();

    virtual
    std::shared_ptr<DataConverter>
    clone() const = 0;


    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &) = 0;
};

#endif // DATACONVERTER_H
