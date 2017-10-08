#include "tabledatasource.h"
#include "global/global_definitions.h"
#include "builder/proxybuilder.h"
#include "basic/basic.h"
#include <QDebug>

TableDataSource::TableDataSource(QString fileName)
    :mFileName(fileName), mIsDataDefinitionsReady(false), mDirector(NULL)
{
    mHeaderMutex.lock();
    mDataMutex.lock();
    setDataSourceName(fileName);

    start();
}

QVector<std::tuple<QString, int> > TableDataSource::getDataList() const
{

    mHeaderMutex.lock();
    QVector<std::tuple<QString, int> > ret;
    for (int i = 0; i < mDataNamesVector.size(); ++i) {
        ret.push_back(std::tuple<QString, int>(mDataNamesVector[i], 1));
    }
    mHeaderMutex.unlock();
    return ret;
}

std::tuple<QVector<double>, QVector<double> > TableDataSource::getRawDataByIndex(int primaryIndex, int /*secondaryIndex*/) const
{
//    return std::tuple<QVector<double>, QVector<double> > (
//                                                          mXDataVector[primaryIndex],
//                                                          mYDataVector[primaryIndex]
//                                                          );
    mDataMutex.lock();
    std::tuple<QVector<double>, QVector<double> > ret;
    if (primaryIndex < mXDataVector.size() ) {
        ret = std::tuple<QVector<double>, QVector<double> > (mXDataVector[primaryIndex], mYDataVector[primaryIndex]);
    }
    mDataMutex.unlock();
    return ret;
}

void TableDataSource::addDataDefinition(shared_ptr<DataAttribute> da)
{
    mDataAttributeVector.push_back(da);
    mIdVector.push_back(da->getId());
    mDataNamesVector.push_back(QString::fromStdString(da->getName()));
}

void TableDataSource::addDataSample(std::unique_ptr<DataSample> dataSample)
{
    if (mIsDataDefinitionsReady == false) {
        mIsDataDefinitionsReady = true;
        for (int i = 0; i < mIdVector.size(); ++i) {
            mXDataVector.push_back(QVector<double>());
            mYDataVector.push_back(QVector<double>());
        }
        mHeaderMutex.unlock();
    }

    auto dataAttribute = dataSample->getDataAttribute();
    size_t id = dataAttribute->getId();
    int index = mIdVector.indexOf(id);
    if (index != -1) {
        int indexToInsert = -1;
        double time = dataSample->getTime();
        if (!mXDataVector[index].isEmpty() && time < mXDataVector[index].back()) {
            indexToInsert = qLowerBound(mXDataVector[index].begin(), mXDataVector[index].end(), time)
                    - mXDataVector[index].begin();
        }
        if (indexToInsert == -1) {
            mXDataVector[index].push_back(dataSample->getTime());
        } else {
            mXDataVector[index].insert(indexToInsert, 1, time);
        }
        switch (dataAttribute->getDataType()) {
            case DataAttribute::DataType::DOUBLE:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getDoubleValue, index, indexToInsert);
                break;
            case DataAttribute::DataType::FLOAT:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getFloatValue, index, indexToInsert);
                break;
            case DataAttribute::DataType::INT8:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getInt8Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::INT16:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getInt16Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::INT32:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getInt32Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::INT64:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getInt64Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::UINT8:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getUInt8Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::UINT16:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getUInt16Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::UINT32:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getUInt32Value, index, indexToInsert);
                break;
            case DataAttribute::DataType::UINT64:
                extractDataFromDataSample(std::move(dataSample), &DataSample::getUInt64Value, index, indexToInsert);
                break;
            default:
//                errorExit("Unexpected situation in %s\n", __FUNCTION__);
                throwRuntimeError("Unexpected situation in %s\n", __FUNCTION__);
                break;
        }
    }

}

QString TableDataSource::getDescriptionByIndex(int /*index*/) const
{
    return "";
}

int TableDataSource::getMaxNameLength() const
{
    return 20;
}

int TableDataSource::progress() const
{
    return 75;
}

void TableDataSource::run()
{


    try {
        TableDirector tableDirector(mFileName);
        tableDirector.setStrictChecking(global::gIsDataSourceCheckingStrict);

        //Using ProxyBuilder, because otherwise  CSVDataSource will be
        // deleted during deletion of ttDirector (as  TTDirector needs shared_ptr)
        tableDirector.setBuilder(std::make_shared<ProxyBuilder>(this));
        tableDirector.process();
        mHeaderMutex.unlock();
        mDataMutex.unlock();
        mIsDataCorrect = tableDirector.isCorrect();
        if (!mIsDataCorrect) {
            addErrorsToErrorsList(tableDirector.errorsList());
            emit nonCriticalErrorsDuringLoading();
        }
    } catch (std::exception &except) {
        mIsStateValid = false;
        emit errorDuringLoading(except.what());
    }

}







template <typename typeOfRetrievingFunction>
void TableDataSource::extractDataFromDataSample(std::unique_ptr<DataSample> dataSample, typeOfRetrievingFunction  retrievingFunction, int index, int positionToInsert)
{

    auto pointer = dataSample.get();
    auto dataVector = (pointer->*retrievingFunction)();
    if (positionToInsert == -1) {
        mYDataVector[index].push_back(double(dataVector[0]));   //all data are converted to double
    } else {
        mYDataVector[index].insert(positionToInsert, 1, double(dataVector[0]));   //all data are converted to double
    }
}
