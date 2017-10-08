#ifndef PLUGINDATASOURCE_H
#define PLUGINDATASOURCE_H

#include <memory>
#include "../plugins/plugins_api.h"
#include "datasource.h"

class DataAttribute;
class DataSample;

class PluginDataSource: public DataSource
{
public:
    PluginDataSource(jag_data_source_api *loader, const QString &fileName);

    ~PluginDataSource();


    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>
    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex) const;
    virtual void addDataDefinition(std::shared_ptr<DataAttribute>);
    virtual void addDataSample(std::unique_ptr<DataSample>);
    virtual QString getDescriptionByIndex(int index) const;
    virtual QString getDimensionByIndex(int index) const;

    virtual int getMaxNameLength() const;
    virtual int progress() const;

    void run();
private:
    jag_data_source_api *mLoader;

    QVector<std::tuple<QString, int>> mDataList;
};

#endif // PLUGINDATASOURCE_H
