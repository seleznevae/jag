#include "simpledatasource.h"
#include <QDebug>
#include "basic/basic.h"

SimpleDataSource::SimpleDataSource()
{
}

QVector<std::tuple<QString, int> > SimpleDataSource::getDataList() const
{
    QVector<std::tuple<QString, int> > ret;
    for (auto parameter : mDataVector) {
        ret.push_back(std::tuple<QString, int>(std::get<0>(parameter), 1));
    }
    return ret;
}

std::tuple<QVector<double>, QVector<double> > SimpleDataSource::getRawDataByIndex(int primaryIndex, int /*secondaryIndex*/) const
{
    if ((primaryIndex >= mDataVector.size()) || (primaryIndex < 0)) {
        qWarning() << "primary index out of range in function" << __FUNCTION__;
        return std::tuple<QVector<double>, QVector<double> >();
    }
    auto parameter = mDataVector[primaryIndex];
    return std::tuple<QVector<double>, QVector<double> >(std::get<1>(parameter), std::get<2>(parameter));
}

void SimpleDataSource::addData(QVector<double> x, QVector<double> y, QString parameterName)
{
    mDataVector.push_back(std::tuple<QString, QVector<double>, QVector<double>>(parameterName, x, y));
    emit simpleDataSourceChanged();
}

QString SimpleDataSource::getDescriptionByIndex(int /*index*/) const
{
    return "";
}

int SimpleDataSource::getMaxNameLength() const
{
    return 20;
}

SimpleDataSource::~SimpleDataSource()
{

}

int SimpleDataSource::progress() const
{
    return 75;
}

//void SimpleDataSource::saveAs(const QString &)
//{
//    showCriticalMessage("Not implemented");
//}

void SimpleDataSource::run()
{
    //pass
}

void SimpleDataSource::clearData()
{
    mDataVector.clear();
}
