#include "plugindatasource.h"
#include <assert.h>

PluginDataSource::PluginDataSource(jag_data_source_api *loader, const QString & fileName)
    :mLoader(loader)
{
    assert(mLoader);
    mHeaderMutex.lock();
    mDataMutex.lock();
    mIsDataCorrect = true; //?
    mIsStateValid = true;
    setDataSourceName(fileName);
    start();
}

PluginDataSource::~PluginDataSource()
{
    (mLoader->delete_data_source)(mLoader);
}



QVector<std::tuple<QString, int> > PluginDataSource::getDataList() const
{
    uint32_t paramNumber = (mLoader->get_number_of_parameters)(mLoader);
    QVector<std::tuple<QString, int>> result;
    for (uint32_t i = 0; i < paramNumber; ++i) {
        struct jag_param_description description;
        (mLoader->get_parameter_description)(mLoader, i, &description);
        result.push_back(std::tuple<QString, int>(QString::fromStdWString(description.name), description.dimension));
    }
    return result;
}

std::tuple<QVector<double>, QVector<double> > PluginDataSource::getRawDataByIndex(int primaryIndex, int secondaryIndex) const
{
    uint64_t paramSize = (mLoader->get_parameter_size)(mLoader, primaryIndex, secondaryIndex);
    std::tuple<QVector<double>, QVector<double>> result{QVector<double>(paramSize), QVector<double>(paramSize)};

    uint64_t n = (mLoader->get_parameter)(mLoader,
                             primaryIndex,
                             secondaryIndex,
                             std::get<0>(result).data(),
                             std::get<1>(result).data(),
                             paramSize);
    if (n != paramSize) {
        qWarning() << "Less bytes then expected were written to the parameter";
    }
    return result;
}

void PluginDataSource::addDataDefinition(std::shared_ptr<DataAttribute>)
{

}

void PluginDataSource::addDataSample(std::unique_ptr<DataSample>)
{

}

QString PluginDataSource::getDescriptionByIndex(int index) const
{
    struct jag_param_description description;
    (mLoader->get_parameter_description)(mLoader, index, &description);
    return QString::fromStdWString(description.description);
}

QString PluginDataSource::getDimensionByIndex(int index) const
{
    struct jag_param_description description;
    (mLoader->get_parameter_description)(mLoader, index, &description);
    return QString::fromStdWString(description.units);
}

int PluginDataSource::getMaxNameLength() const
{
    // TODO
    return 50;
}

int PluginDataSource::progress() const
{
    return (mLoader->loading_progress)(mLoader);
}

void PluginDataSource::run()
{
    uint32_t paramNumber = (mLoader->get_number_of_parameters)(mLoader);
    QVector<std::tuple<QString, int>> mDataList;
    for (uint32_t i = 0; i < paramNumber; ++i) {
        struct jag_param_description description;
        (mLoader->get_parameter_description)(mLoader, i, &description);
        mDataList.push_back(std::tuple<QString, int>(QString::fromStdWString(description.name), description.dimension));
    }
    mHeaderMutex.unlock();

    int progress = 0;
    while (true) {
        progress = (mLoader->loading_progress)(mLoader);
        if (progress < 100)
            usleep(100000);
        else
            break;
    }
    mIsStateValid = (mLoader->is_valid)(mLoader);
    mDataMutex.unlock();

}
