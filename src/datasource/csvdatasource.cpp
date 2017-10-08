#include "datasource/csvdatasource.h"
#include "director/csv_director.h"
#include <memory>
#include "builder/proxybuilder.h"
#include "basic/basic.h"
#include <QDebug>
#include <QInputDialog>
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"
#include "datasource/textdatasourcedialog.h"



CSVDataSource::CSVDataSource(QString dataSourceFileName):
    mFileName(dataSourceFileName), mIsDataDefinitionsReady(false), mFieldSeparator(','),
    mParameterNameLineNumber(-1), mParameterUnitsLineNumber(-1), mParameterDescriptionLineNumber(-1)
{
    mHeaderMutex.lock();
    mDataMutex.lock();
    setDataSourceName(dataSourceFileName);
////    TTDirector ttDirector(mFileName.toStdString());   //crashes with non ASCII symbols
//    SimpleDirector csvDirector(mFileName.toLocal8Bit().constData());  // Not certain that this stuff will always work properly

//    //Using ProxyBuilder, because otherwise  CSVDataSource will be
//    // deleted during deletion of ttDirector (as  TTDirector needs shared_ptr)
//    csvDirector.setBuilder(std::make_shared<ProxyBuilder>(this));
//    csvDirector.process();

//    bool ok;
//    QString separator = QInputDialog::getText(global::gJagMainWindowPointer, "Choose separator", mFileName + " was determined as *.csv file. Chose character that should "
//                                               "be used as a field separator",
//                                               QLineEdit::Normal, QString(mFieldSeparator), &ok);
//    if (ok && separator != "") {
//        mFieldSeparator = separator.toStdString()[0];
//    }


    TextDataSourceDialog dialog;
    dialog.loadFile(mFileName);
    if (dialog.exec() == QDialog::Accepted) {
        mFieldSeparator = dialog.fieldSeparator();
        mParameterNameLineNumber = dialog.parameterNameLineNumber();
        mParameterUnitsLineNumber = dialog.parameterUnitsLineNumber();
        mParameterDescriptionLineNumber = dialog.parameterDescriptionLineNumber();
        mArgumentColumn = dialog.argumentPosition();
    } else {
        //TODO: don't know what to do in this case
        errorExit("Unexpected situation in %s", __FUNCTION__);
    }


    start();
}

QVector<std::tuple<QString, int> > CSVDataSource::getDataList() const
{
    mHeaderMutex.lock();
    QVector<std::tuple<QString, int> > ret;
    for (int i = 0; i < mDataNamesVector.size(); ++i) {
        ret.push_back(std::tuple<QString, int>(mDataNamesVector[i].trimmed(), mDimVector[i]));
    }
    mHeaderMutex.unlock();

    return ret;
}

std::tuple<QVector<double>, QVector<double> > CSVDataSource::getRawDataByIndex(int primaryIndex, int secondaryIndex) const
{
    mDataMutex.lock();
    std::tuple<QVector<double>, QVector<double> > ret;
    if (primaryIndex < mXDataVector.size() && secondaryIndex < mYDataVector[primaryIndex].size()) {
        ret = std::tuple<QVector<double>, QVector<double> >(mXDataVector[primaryIndex], mYDataVector[primaryIndex][secondaryIndex]);
    }
    mDataMutex.unlock();
    return ret;
}

void CSVDataSource::addDataDefinition(shared_ptr<DataAttribute> da)
{
    mDataAttributeVector.push_back(da);
    mIdVector.push_back(da->getId());
    size_t dimension = 1;
    for (auto dim : da->getDimension())
        dimension *= dim;
    mDimVector.push_back(dimension);
    mDescriptionVector.push_back(da->description());
    mUnitsVector.push_back(da->dimensionString());
    mDataNamesVector.push_back(QString::fromStdString(da->getName()));

}

void CSVDataSource::addDataSample(std::unique_ptr<DataSample> dataSample)
{
    if (mIsDataDefinitionsReady == false) {
        mIsDataDefinitionsReady = true;
        foreach (int size, mDimVector) {
            mXDataVector.push_back(QVector<double>());
            mYDataVector.push_back(QVector<DataVector>(size, QVector<double>()));
        }
        mHeaderMutex.unlock();

//                reserving space
//                bool isTTDataSourceLoadingOptimActive = true;
//                size_t dataSizeAssumption = 2000000;
//                if (isTTDataSourceLoadingOptimActive) {
//                    mXDataVector.back().reserve(dataSizeAssumption);
//                    for (auto & vector : mYDataVector.back()) {
//                        vector.reserve(dataSizeAssumption);
//                    }
//                }
    }
    auto dataAttribute = dataSample->getDataAttribute();
    size_t id = dataAttribute->getId();
    int index = mIdVector.indexOf(id);
    if (index != -1) {
        int indexToInsert = -1;
        double time = dataSample->getTime();
//        if (!mXDataVector.isEmpty() || time < mXDataVector[index].back()) {
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

QString CSVDataSource::getDescriptionByIndex(int index) const
{
   if (index >= 0 && index < mDescriptionVector.size()) {
       return mDescriptionVector[index].trimmed();
   }
   return "";
}

QString CSVDataSource::getDimensionByIndex(int index) const
{
    if (index >= 0 && index < mUnitsVector.size()) {
        return mUnitsVector[index].trimmed();
    }
    return "";
}

int CSVDataSource::getMaxNameLength() const
{
    return 20;
}

int CSVDataSource::progress() const
{
    int ret;
    mProgressMutex.lock();
    if (mCSVDirector == NULL) {
        ret = 75;
    } else {
        ret = mCSVDirector->progress();
    }
    mProgressMutex.unlock();
    return ret;
}



void CSVDataSource::run()
{
    try {
    //    TTDirector ttDirector(mFileName.toStdString());   //crashes with non ASCII symbols
        mCSVDirector = new CSVDirector(mFileName.toLocal8Bit().constData(), mFieldSeparator);
        mCSVDirector->setStrictChecking(global::gIsDataSourceCheckingStrict);
        mCSVDirector->setParameterNameLineNumber(mParameterNameLineNumber);
        mCSVDirector->setParameterUnitsLineNumber(mParameterUnitsLineNumber);
        mCSVDirector->setParameterDescriptionLineNumber(mParameterDescriptionLineNumber);
        mCSVDirector->setArgumentColumn(mArgumentColumn);


        //Using ProxyBuilder, because otherwise  CSVDataSource will be
        // deleted during deletion of ttDirector (as  TTDirector needs shared_ptr)
        mCSVDirector->setBuilder(std::make_shared<ProxyBuilder>(this));
        mCSVDirector->process();
        mHeaderMutex.unlock();
        mDataMutex.unlock();
        mProgressMutex.lock();
        mIsDataCorrect = mCSVDirector->isCorrect();
        if (!mIsDataCorrect) {
            addErrorsToErrorsList(mCSVDirector->errorsList());
            emit nonCriticalErrorsDuringLoading();
        }
        delete mCSVDirector;
        mCSVDirector = NULL;
        mProgressMutex.unlock();
    } catch (std::exception &except) {
        mIsStateValid = false;
        emit errorDuringLoading(except.what());
    }






//    try {
//        mDirector = new TTDirector(mFileName.toLocal8Bit().constData());  // Not certain that this stuff will always work properly
//        mDirector->setStrictChecking(gIsDataSourceCheckingStrict);
//        //Using ProxyBuilder, because otherwise  TTDataSource will be
//        // deleted during deletion of ttDirector (as  TTDirector needs shared_ptr)

//        mDirector->setBuilder(std::make_shared<ProxyBuilder>(this));
//        mDirector->process();
//        mDataMutex.unlock();
//        mProgressMutex.lock();
//        mIsDataCorrect = mDirector->isCorrect();
//        if (!mIsDataCorrect) {
//            addErrorsToErrorsList(mDirector->errorsList());
////            mErrorsList = mDirector->errorsList();
//            emit nonCriticalErrorsDuringLoading();
//        }
//        delete mDirector;
//        mDirector = NULL;
//        mProgressMutex.unlock();
//    } catch (std::runtime_error &rtError) {
//        mIsStateValid = false;
//        emit errorDuringLoading(rtError.what());
//    }

}





template <typename typeOfRetrievingFunction>
void CSVDataSource::extractDataFromDataSample(std::unique_ptr<DataSample> dataSample, typeOfRetrievingFunction  retrievingFunction, int index, int positionToInsert)
{

    auto pointer = dataSample.get();

    auto dataVector = (pointer->*retrievingFunction)();
    if (dataVector.size() != mDimVector[index]) {
//        errorExit("Unexpected situation in %s\n", __FUNCTION__);
        throwRuntimeError("Unexpected situation in %s\n", __FUNCTION__);
    }
    if (positionToInsert == -1) {
        for (int i = 0; i < (int)(dataVector.size()); ++i) {
            mYDataVector[index][i].push_back(double(dataVector[i]));   //all data are converted to double
        }
    } else {
        for (int i = 0; i < (int)(dataVector.size()); ++i) {
            mYDataVector[index][i].insert(positionToInsert, 1, double(dataVector[i]));   //all data are converted to double
        }
    }
}







