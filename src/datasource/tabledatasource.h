#ifndef TABLEDATASOURCE_H
#define TABLEDATASOURCE_H

#include "datasource/datasource.h"
#include "builder/builder.h"
#include "data/data_attribute.h"
#include "director/tabledirector.h"

/*!
 \brief TableDataSource - data source that can load data in table form.

*/

class TableDataSource : public DataSource, public Builder
{
    using DataVector = QVector<double>;
public:
    TableDataSource(QString fileName);
    virtual ~TableDataSource() { }

    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>
    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex) const;
    virtual void addDataDefinition(shared_ptr<DataAttribute>);
    virtual void addDataSample(std::unique_ptr<DataSample>);
    virtual QString getDescriptionByIndex(int index) const;

    virtual int getMaxNameLength() const;
    virtual int progress() const;


    void run();

private:
    QString mFileName;
    bool mIsDataDefinitionsReady;


    QVector<QString> mDataNamesVector;
    QVector<size_t> mIdVector;
    std::vector<std::shared_ptr<DataAttribute>> mDataAttributeVector;
    QVector<DataVector> mYDataVector;
    QVector<DataVector> mXDataVector;

    TableDirector *mDirector;


    template <typename typeOfRetrievingFunction>
    void extractDataFromDataSample(std::unique_ptr<DataSample> dataSample, typeOfRetrievingFunction  retrievingFunction, int index, int positionToInsert);
};

#endif // TABLEDATASOURCE_H







//class CSVDataSource : public DataSource, public Builder
//{
//    using DataVector = QVector<double>;
////    void setDataSourceName(const QString& name) = delete;
//public:

//    CSVDataSource(QString fileName);

//    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>
//    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex) const;
//    virtual void addDataDefinition(shared_ptr<DataAttribute>);
//    virtual void addDataSample(shared_ptr<DataSample>);
//    virtual QString getDescriptionByIndex(int index) const;

//    virtual int getMaxNameLength() const;
//    virtual int progress() const;


//    void run();

//private:
//    QString mFileName;
//    QVector<QString> mDataNamesVector;
//    QVector<size_t> mIdVector;
//    QVector<size_t> mDimVector;
//    std::vector<std::shared_ptr<DataAttribute>> mDataAttributeVector;
//    QVector<QVector<DataVector>> mYDataVector;
//    QVector<DataVector> mXDataVector;

//    bool mIsDataDefinitionsReady;
//    char mFieldSeparator;

//    template <typename typeOfRetrievingFunction>
//    void extractDataFromDataSample(shared_ptr<DataSample> dataSample, typeOfRetrievingFunction  retrievingFunction, int index, int positionToInsert);


//};
