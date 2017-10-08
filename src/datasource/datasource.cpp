#include "datasource.h"
#include "basic/bassic.h"
#include "basic/basic.h"
#include <QStringList>
#include <QDebug>
#include "data/data_attribute.h"
#include "data/data_sample_types.h"
#include "global/global_definitions.h"
#include <stdexcept>

#ifndef GPLv3
#include "bin_data_formats/bin_data_formats.h"
#else
#include "builder/csv_builder.h"
#endif



QStringList DataSource::sAliasesList;

DataSource::DataSource(): QThread(NULL), mIsStateValid(true), mGraphStyle(std::make_shared<GraphStyle>()),
    mTimeDeviation(0.), mIsDataCorrect(true)
{
}

QString DataSource::getDataSourceName() const
{
    return mName;
}

QString DataSource::getDataSourceAlias() const
{
    return mAlias;
}

QString DataSource::getDimensionByIndex(int /*index*/) const
{
    return QString();
}

std::tuple<QVector<double>, QVector<double> > DataSource::getDataByIndex(int primaryIndex, int secondaryIndex) const
{
    if (mTimeDeviation == 0. && mFilterVector.size() == 0)
        return getRawDataByIndex(primaryIndex, secondaryIndex);
    else {
        auto ret = getRawDataByIndex(primaryIndex, secondaryIndex);
        if (mTimeDeviation != 0.) {
            for (auto &x : std::get<0>(ret)) {
                x += mTimeDeviation;
            }
        }
        for (const auto& filter : mFilterVector) {
            auto newData = (*filter)(std::get<0>(ret), std::get<1>(ret));
            std::get<0>(ret) = std::get<0>(newData);
            std::get<1>(ret) = std::get<1>(newData);
        }
        return ret;
    }
}

void DataSource::setDataSourceName(const QString &name)
{
    mName = name;
    mAlias = stripDirectoryFromFileName(mName);
    //ensuring unique alias
    if (sAliasesList.indexOf(mAlias) != -1) {
        int index = 1;
        while (sAliasesList.indexOf(createIndexedFileName(mAlias, index)) != -1) {
            ++index;
        }
        mAlias = createIndexedFileName(mAlias, index);
    }
    sAliasesList << mAlias;
}

void DataSource::setDataSourceAlias(const QString &alias)
{
    mAlias = alias;
}

bool DataSource::isValid() const
{
    return mIsStateValid;
}

DataSource::~DataSource()
{
    qDebug() << "Deleting data source " << mName;
}

void DataSource::saveAs(const QString &outputFileName)
{

    Builder *builder = NULL;
    #ifndef GPLv3
    builder = getBinBuilderForDataSaving(outputFileName.toLocal8Bit().constData());
    #else
    builder = new CSVBuilder(outputFileName.toLocal8Bit().constData());
    #endif


    QVector<std::tuple<QString, int>> dataList = getDataList();
    QVector<std::shared_ptr<DataAttribute>> dataAttributeVector;
    for (int i = 0; i < dataList.size(); ++i) {
        auto attribute = std::make_shared<DataAttribute> ();
        attribute->setId(i);
        attribute->setName(std::get<0>(dataList[i]).toStdString());
        attribute->setSize(sizeof(double));  //maybe it will be better to write in float?
        attribute->setDataType(DataAttribute::DataType::DOUBLE);
        attribute->setShowType(DataAttribute::ShowType::DECIMAL);
        attribute->setMultiplier(1.0);
        attribute->setDimension(vector<size_t> ({static_cast<size_t>(std::get<1>(dataList[i]))}));
        dataAttributeVector.push_back(attribute);
        builder->addDataDefinition(attribute);
    }
    for (int i = 0; i < dataAttributeVector.size(); ++i) {
        size_t dim = dataAttributeVector[i]->getDimension()[0];

        QVector<double> xData;
        QVector<QVector<double>> yData;
        for (size_t j = 0; j < dim; ++j) {
            std::tuple<QVector<double>, QVector<double>> dataByIndex = getDataByIndex(i, j);
            if (j == 0) {
                xData = std::get<0>(dataByIndex);
            }
            yData.push_back(std::get<1>(dataByIndex));
        }

        for (int j = 0; j < xData.size(); ++j) {
            std::vector<double> valueVector;
            for (size_t k = 0; k < dim; ++k)
                valueVector.push_back(yData[k][j]);
            builder->addDataSample(std::make_unique<DoubleDataSample>(dataAttributeVector[i], xData[j], valueVector));
        }
    }
    delete builder;
}



bool DataSource::isDataReady() const
{
    if (mDataMutex.tryLock()) {
        mDataMutex.unlock();
        return true;
    }
    return false;
}

std::shared_ptr<GraphStyle> DataSource::graphStyle() const
{
    return mGraphStyle;
}

double DataSource::timeDeviation() const
{
    return mTimeDeviation;
}

void DataSource::setTimeDeviation(double deviation)
{
    mTimeDeviation = deviation;
}

void DataSource::addFilter(std::shared_ptr<DataConverter> newConverter)
{
    if (mFilterVector.indexOf(newConverter) == -1) {
        mFilterVector.push_back(newConverter);
    }
}

QVector<std::shared_ptr<DataConverter> > DataSource::getFilterVector() const
{
    return mFilterVector;
}

void DataSource::removeFilter(int index)
{
    if (index >= 0 && index < mFilterVector.size()) {
        mFilterVector.remove(index);
    }
}

void DataSource::addErrorToErrorsList(const QString &errorString)
{
    if (mErrorsList.size() <= global::gErrorsListCapacity) {
        if (!mErrorsList.contains(errorString)) {
            mErrorsList << errorString;
        }
    } else {
        throw std::runtime_error("Too many errors in " + mName.toStdString());
    }
}

void DataSource::addErrorsToErrorsList(const QStringList &errorStringList)
{
    mErrorsList << errorStringList;
    if (mErrorsList.size() > global::gErrorsListCapacity) {
        throw std::runtime_error("Too many errors in " + mName.toStdString());
    }
}

