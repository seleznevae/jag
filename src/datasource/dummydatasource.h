#ifndef DUMMYDATASOURCE_H
#define DUMMYDATASOURCE_H

#include "datasource.h"
class DummyDataSource : public DataSource
{
public:
    using DataType = std::tuple<QVector<double>, QVector<double>>;
    DummyDataSource();

    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>

    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex);

    virtual int progress() const;

private:
    QVector<QString> mDataNameVector;
    QVector<QVector<DataType>> mDataVector;
};

#endif // DUMMYDATASOURCE_H
