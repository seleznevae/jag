#include "dummydatasource.h"
#include "basic/bassic.h"

DummyDataSource::DummyDataSource()
{
    int nTochek = 200000;
    QVector<DataType> buffer;

    QVector<double> xData;
    QVector<double> yData;
    for (int i = 0; i < nTochek; ++i) {
        xData.push_back(1+(i*1+0.5)/(double)nTochek);
        yData.push_back(sin(18*(i*1+1.)/nTochek));
    }
    mDataNameVector.push_back("someThing");
    buffer.clear();
    buffer.push_back(DataType(xData, yData));
    mDataVector.push_back(buffer);

    QVector<double> xData2;
    QVector<double> yData2;
    for (int i = 0; i < nTochek; ++i) {
        xData2.push_back(-1+(i*1-1.5)/(double)nTochek);
        yData2.push_back(cos(9*(i*2-1.5)/nTochek/4.));
    }
    mDataNameVector.push_back("someThing2");
    buffer.clear();
    buffer.push_back(DataType(xData2, yData2));
    mDataVector.push_back(buffer);

    QVector<double> xData3;
    QVector<double> yData3;
    for (int i = 0; i < nTochek; ++i) {
        xData3.push_back(1.5+i/double(nTochek));
        yData3.push_back(cos(6*(i*1+0.1)/(double)nTochek));
    }
    mDataNameVector.push_back("someThing3");
    buffer.clear();
    buffer.push_back(DataType(xData3, yData3));
    mDataVector.push_back(buffer);

    QVector<double> xData4;
    QVector<double> yData4;
    for (int i = 0; i < nTochek; ++i) {
        xData4.push_back(-0.75+i/(double)nTochek);
        yData4.push_back(1);
    }
    mDataNameVector.push_back("someThing4");
    buffer.clear();
    buffer.push_back(DataType(xData4, yData4));
    mDataVector.push_back(buffer);

    QVector<double> xData5;
    QVector<double> yData5;
    for (int i = 0; i < nTochek; ++i) {
        xData5.push_back(i/(double)nTochek);
        yData5.push_back(0.5+sin(30*(9-i)/nTochek));
    }
    mDataNameVector.push_back("someThing5");
    buffer.clear();
    buffer.push_back(DataType(xData5, yData5));
    mDataVector.push_back(buffer);
}

QVector<std::tuple<QString, int> > DummyDataSource::getDataList() const
{
    QVector<std::tuple<QString, int> > ret;
    for (int i = 0; i < mDataNameVector.size(); i++) {
//        if (i != 2)  //making complex data for testing purposes
            ret.push_back(std::tuple<QString, int>(mDataNameVector[i], mDataVector[i].size()));
//        else
//            ret.push_back(std::tuple<QString, int>(mDataNameVector[i], 5));
    }
    return ret;
}

std::tuple<QVector<double>, QVector<double> > DummyDataSource::getRawDataByIndex(int primaryIndex, int secondaryIndex)
{
    if (primaryIndex >= 0 && primaryIndex < mDataVector.size()) {
        if (secondaryIndex >= 0 && secondaryIndex < mDataVector[primaryIndex].size()) {
            return mDataVector[primaryIndex][secondaryIndex];
        }
    }
    doErrorExit("Unexpexted situation in %s\n", __FUNCTION__);

    //never evaluated
    return mDataVector[primaryIndex][secondaryIndex];
}

int DummyDataSource::progress() const
{
    return 75;
}
