#ifndef DATASOURCEFILTERCONTAINER_H
#define DATASOURCEFILTERCONTAINER_H

#include "dataconverter/abstractfiltercontainer.h"
#include "datasource/datasource.h"

class DataSourceFilterContainer : public AbstractFilterContainer
{
public:
    DataSourceFilterContainer();
    virtual ~DataSourceFilterContainer();

    virtual void addFilter(std::shared_ptr<DataConverter> newConverter);
    virtual QVector<std::shared_ptr<DataConverter>> getFilterVector() const;
    virtual void removeFilter(int index);
    void setDataSource(DataSource* dataSource);
    virtual void updateOutputData();

private:
    DataSource *mDataSource;
};

#endif // DATASOURCEFILTERCONTAINER_H


