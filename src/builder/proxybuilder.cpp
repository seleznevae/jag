#include "proxybuilder.h"

ProxyBuilder::ProxyBuilder(Builder *builder):
    mBuilder(builder)
{
}

void ProxyBuilder::addDataDefinition(shared_ptr<DataAttribute> da)
{
    mBuilder->addDataDefinition(da);
}

void ProxyBuilder::addDataSample(std::unique_ptr<DataSample> ds)
{
    mBuilder->addDataSample(std::move(ds));
}

