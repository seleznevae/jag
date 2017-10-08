#ifndef SIMPLEDATASOURCE_H
#define SIMPLEDATASOURCE_H

#include "datasource/datasource.h"
#include <QString>
#include <QVector>
#include <tuple>


class SimpleDataSource : public DataSource
{
    Q_OBJECT
public:
    SimpleDataSource();


    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>


    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex) const;
    void addData(QVector<double> x, QVector<double> y, QString parameterName);

    virtual QString getDescriptionByIndex(int index) const;

    virtual int getMaxNameLength() const;

    virtual ~SimpleDataSource();
    virtual int progress() const;
//    virtual void saveAs(const QString&);

    virtual void run();
    void clearData();

signals:
    void simpleDataSourceChanged();

private:
    QVector<std::tuple<QString, QVector<double>, QVector<double>> > mDataVector;

};

#endif // SIMPLEDATASOURCE_H
