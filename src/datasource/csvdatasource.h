#ifndef CSVDATASOURCE_H
#define CSVDATASOURCE_H



#include "datasource/datasource.h"
#include "builder/builder.h"
#include "data/data_attribute.h"
#include "director/csv_director.h"

class CSVDataSource : public DataSource, public Builder
{
    using DataVector = QVector<double>;
//    void setDataSourceName(const QString& name) = delete;
public:

    CSVDataSource(QString fileName);

    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>
    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex) const;
    virtual void addDataDefinition(shared_ptr<DataAttribute>);
    virtual void addDataSample(std::unique_ptr<DataSample>);
    virtual QString getDescriptionByIndex(int index) const;
    virtual QString getDimensionByIndex(int index) const;

    virtual int getMaxNameLength() const;
    virtual int progress() const;


    void run();

private:
    QString mFileName;
    QVector<QString> mDataNamesVector;
    QVector<size_t> mIdVector;
    QVector<size_t> mDimVector;
    std::vector<std::shared_ptr<DataAttribute>> mDataAttributeVector;
    QVector<QVector<DataVector>> mYDataVector;
    QVector<DataVector> mXDataVector;
    QVector<QString> mDescriptionVector;
    QVector<QString> mUnitsVector;


    bool mIsDataDefinitionsReady;
    char mFieldSeparator;
    int mParameterNameLineNumber;
    int mParameterUnitsLineNumber;
    int mParameterDescriptionLineNumber;
    int mArgumentColumn; // -1 - use line number
    CSVDirector *mCSVDirector;

    template <typename typeOfRetrievingFunction>
    void extractDataFromDataSample(std::unique_ptr<DataSample> dataSample, typeOfRetrievingFunction  retrievingFunction, int index, int positionToInsert);


    mutable QMutex mProgressMutex;
};



#endif // CSVDATASOURCE_H
