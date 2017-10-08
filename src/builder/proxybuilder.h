#ifndef PROXYBUILDER_H
#define PROXYBUILDER_H

#include "builder/builder.h"

class ProxyBuilder : public Builder
{
public:
    ProxyBuilder(Builder *builder);

    virtual void addDataDefinition(shared_ptr<DataAttribute>);

    virtual void addDataSample(std::unique_ptr<DataSample>);

private:
    Builder *mBuilder;
};

#endif // PROXYBUILDER_H
