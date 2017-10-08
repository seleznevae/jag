#include "datasourcefiltercontainer.h"

DataSourceFilterContainer::DataSourceFilterContainer()
    :mDataSource(NULL)
{
}

DataSourceFilterContainer::~DataSourceFilterContainer()
{

}



void DataSourceFilterContainer::addFilter(std::shared_ptr<DataConverter> newConverter)
{
    if (mDataSource) {
        mDataSource->addFilter(newConverter);
    }
}

QVector<std::shared_ptr<DataConverter> > DataSourceFilterContainer::getFilterVector() const
{
    if (mDataSource) {
        return mDataSource->getFilterVector();
    } else
        return QVector<std::shared_ptr<DataConverter> >();
}

void DataSourceFilterContainer::removeFilter(int index)
{
    if (mDataSource) {
        mDataSource->removeFilter(index);;
    }
}




void DataSourceFilterContainer::setDataSource(DataSource *dataSource)
{
    mDataSource = dataSource;
}


void DataSourceFilterContainer::updateOutputData()
{
    ;//do nothing
}
