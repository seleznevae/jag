#ifndef INFOBUILER_H
#define INFOBUILER_H

#include "builder.h"
#include <string>
#include <fstream>
#include <map>

class InfoBuilder : public Builder
{
public:
    InfoBuilder(std::string fileName_);
    virtual void addDataDefinition(shared_ptr<DataAttribute>);
    virtual void addDataSample(std::unique_ptr<DataSample>);
    ~InfoBuilder();
private:
    std::string fileName;
    std::ofstream fOut; /**< TODO */
    std::vector< std::shared_ptr<DataAttribute> > dataAttributeVector;
    std::map< unsigned, unsigned > idToDataNumberMap;
};

#endif // INFOBUILER_H
